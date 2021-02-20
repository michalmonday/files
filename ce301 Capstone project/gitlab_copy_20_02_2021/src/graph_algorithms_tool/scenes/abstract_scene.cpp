#include "scenes/abstract_scene.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "graph/graph.h"
#include "graph/preset_graphs.h"

#include <QDebug>
#include "dock.h"
#include "step_description_graphics_item.h"

#include <QDockWidget>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "select_algorithm_widget.h"
#include "adjacency_list_graphics_item.h"
#include "mainwindow.h"
#include "algorithms/controlled_algorithm.h"
#include "legend_graphics_item.h"
#include "functions.h"
#include "code_items/code_graphics_item.h"


#include <QJsonObject>

AdjacencyListGraphicsItem *AbstractScene::adjacency_list_graphics_item = nullptr;

//Dock *AbstractScene::dock_adjacency_list = nullptr;
//Dock *AbstractScene::dock_step_description = nullptr;
//QDockWidget *AbstractScene::dock_step_log = nullptr;
Graph *AbstractScene::main_graph = nullptr;

AbstractScene::AbstractScene(QObject *parent, QRectF scene_rect)
    : QGraphicsScene(parent),
      algorithm(nullptr),
      algorithm_started(false),
      legend(nullptr),
      code_graphics_item(nullptr)
{
    static bool once = false;
    if (!once) {
        setSceneRect(scene_rect);
        adjacency_list_graphics_item = new AdjacencyListGraphicsItem();
        once = true;
    }
}

AbstractScene::~AbstractScene()
{
    removeItem(main_graph);

    if (!is_exiting) {
        if (legend)
            delete legend;

        if (code_graphics_item)
            delete code_graphics_item;
    }

    // restore previous
    main_graph->weightsShown(main_graph_had_weights_shown);

    restoreWeights();

    // select_algorithm_widget gets deleted before...
    if (use_suggested_graphs) {
        delete main_graph;
        main_graph = nullptr;
    } else {
        if (is_exiting) {
            QSettings settings("settings.ini", QSettings::IniFormat);
            settings.setValue("last_graph", main_graph->toJson().toVariantMap());
            settings.setValue("last_graph_saved", true);
        }
    }
}

// this can be used by algorithms like BFS (in bfs_scene.cpp) to hide and modify edge weights
// after changing the algorithm back to something like Dijkstra the original
// weights will be restored
void AbstractScene::enableRestorableEdgeWeights()
{
    restorable_main_graph_edge_weights = true;
    for (Edge *e : main_graph->edges()) {
        initial_edge_weights[e] = e->getLabelItem()->toPlainText();
    }
}

void AbstractScene::initMainGraph(QString algorithm_name, Graph *g)
{
    if (use_suggested_graphs || (!main_graph && !g)) {
        if (main_graph) {
            if (items().contains(main_graph)) {
                removeItem(main_graph);
                delete main_graph;
            }
        }
        main_graph = PresetGraphs::generateForAlgorithm(algorithm_name);
        main_graph->setPos(-main_graph->boundingRect().center());
    } else if (g) {
        if (items().contains(main_graph)) {
            removeItem(main_graph);
            delete main_graph;
        }
        main_graph = g;
        //main_graph->setPos(-main_graph->boundingRect().center());
    }


    addItem(main_graph);
    main_graph->resetColours();

    // save initial state (to restore it later, after specific algorithm is changed)
    main_graph_had_weights_shown = main_graph->weightsShown();
    if (select_algorithm_widget)
        select_algorithm_widget->reset();

    adjacency_list_graphics_item->setGraph(main_graph);
    adjacency_list_graphics_item->updateEntries();
}

void AbstractScene::restoreWeights()
{
    if (restorable_main_graph_edge_weights) {
        const QList<Edge*> &edges = main_graph->edges();
        for (Edge *e : initial_edge_weights.keys()) {
            if (edges.contains(e)) {
                e->getLabelItem()->setPlainText(initial_edge_weights[e]);

                // weight rect may change so it needs to be repainted
                // otherwise it looks ugly
                e->adjust();
            }
        }
    }
}

void AbstractScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!main_graph->editable()) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    if (Node *selected_node = main_graph->selectedNode()) {
        Graph *graph = dynamic_cast<Graph*>(selected_node->parentItem());
        if (!graph)
            qDebug() << "selected node was not part of any graph";

        if (event->modifiers() & Qt::ShiftModifier) {
            event->ignore();

            Edge *new_edge = nullptr;
            Node *dst_node = nullptr;

            // If clicked on another node:
            //   create new edge from 2 existing nodes
            if (QGraphicsItem *item = itemAt(event->scenePos(), views()[0]->transform()))
                if (Node *node = dynamic_cast<Node*>(item))
                    if (node != selected_node)
                        dst_node = node;

            // If clicked on empty space:
            //     create new edge from 1 existing node and 1 new node
            if (dst_node == nullptr) {
                dst_node = graph->addNode();
                dst_node->setPos(getClosestPoint(event->scenePos() - main_graph->pos()));
            }

            new_edge = graph->addEdge(selected_node->getLabel(), dst_node->getLabel(), 1 + rand() % 20);
            new_edge->adjust();

            // Update user interface (TODO: delete this, or improve, maybe create Graph class, and store pairs there, instead of using listWidget)
            //QListWidgetItem *list_item = new QListWidgetItem(new_edge->nodeLabels());
            //list_item->setTextAlignment(Qt::AlignCenter);
            //main_window->ui->listWidget->addItem(list_item);

            adjacency_list_graphics_item->updateEntries();
            adjacency_list_graphics_item->updateDock();
            views()[0]->updateGeometry();

            select_algorithm_widget->useSuggestedGraphs(false);
        }

        if (event->modifiers() & Qt::ControlModifier) {
            event->ignore();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void AbstractScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << main_graph->scenePos() << event->scenePos();
    //if (event->button() == Qt::RightButton) {
    //    main_graph->setPos(event->scenePos());
    //}
}


void AbstractScene::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    switch (key) {
        case Qt::Key_PageUp: return;
        case Qt::Key_PageDown: return;
        case Qt::Key_End: return;
        case Qt::Key_Home: return;
        case Qt::UpArrow: return;
        case Qt::DownArrow: return;
    }

    if (main_graph->editable()) {
        if (key == Qt::Key_Delete) {
            if (Node * selected_node = main_graph->selectedNode()) {
                main_graph->removeNode(selected_node);
                //update();
            }

            if (Edge * selected_edge = main_graph->selectedEdge()) {
                if (selected_edge->getLabelItem()->hasFocus() == false)
                    main_graph->removeEdge(selected_edge);
                //update();
            }

            adjacency_list_graphics_item->updateEntries();
            adjacency_list_graphics_item->updateDock();

            views()[0]->updateGeometry();

            select_algorithm_widget->useSuggestedGraphs(false);
        }
    }

    std::function<void()> onEnd = [&]() {
        algorithm_started = false;
        onAlgorithmEnd();
        algorithm->reset();
        main_graph->resetColours();
        main_graph->editable(true);
        select_algorithm_widget->reset();
        if (code_graphics_item)
            code_graphics_item->hideDock();
        legend->hideDock();

        update();
    };

    if (key == Qt::Key_Escape && algorithm_started)
        onEnd();
    else if (key == Qt::Key_Escape)
        main_graph->deselect();


    if (key == Qt::Key_Right) {
        if (!algorithm_started) {
            algorithm_started = true;
            select_algorithm_widget->reset();
            main_graph->editable(false);
            algorithm->updateGraph(main_graph);

            if (code_graphics_item)
                code_graphics_item->showDock();
            legend->showDock();
            adjacency_list_graphics_item->updateEntries();
            onAlgorithmStart();
        }
        if (algorithm->finished()) {
            onEnd();
        } else {
            algorithm->step();
            onAlgorithmStep();
            select_algorithm_widget->addStep(algorithm->currentStepLabel(), algorithm->currentStepDescription());
            if (code_graphics_item)
                code_graphics_item->updateDock();
        }
        onAlgorithmEvent();


        //event->setAccepted(true);
        return;
    }

    QGraphicsScene::keyPressEvent(event);
}



/*
void AbstractScene::focusInEvent(QFocusEvent *event)
{
    while (Node * selected_node = main_graph->selectedNode())
        selected_node->setSelected(false);
    QGraphicsScene::focusInEvent(event);
}*/
