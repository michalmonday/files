#include "scenes/abstract_scene.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "graph/graph.h"

#include <QDebug>
#include "dock.h"
#include "step_description_graphics_item.h"

#include <QDockWidget>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "step_log.h"
#include "adjacency_list_graphics_item.h"
#include "mainwindow.h"
#include "algorithms/controlled_algorithm.h"
#include "legend_graphics_item.h"
#include "functions.h"

AdjacencyListGraphicsItem *AbstractScene::adjacency_list_graphics_item = nullptr;
StepLog *AbstractScene::step_log = nullptr;
Dock *AbstractScene::dock_adjacency_list = nullptr;
//Dock *AbstractScene::dock_step_description = nullptr;
//QDockWidget *AbstractScene::dock_step_log = nullptr;
Graph *AbstractScene::main_graph = nullptr;

AbstractScene::AbstractScene(QObject *parent, QRectF scene_rect)
    : QGraphicsScene(parent),
      algorithm(nullptr),
      algorithm_started(false),
      legend(nullptr),
      dock_legend(nullptr),
      dock_code(nullptr)
{
    static bool initialized = false;
    if (!initialized) {
        main_graph = new Graph();
        Edge *e1 = main_graph->addEdge("a", "b", 10);
        Edge *e2 = main_graph->addEdge("a", "c", 6);
        Edge *e3 = main_graph->addEdge("a", "d", 5);
        Edge *e4 = main_graph->addEdge("b", "d", 15);
        Edge *e5 = main_graph->addEdge("c", "d", 4);

        Node *node_a = e1->sourceNode();
        Node *node_b = e1->destNode();
        Node *node_c = e2->destNode();
        Node *node_d = e3->destNode();

        // position is relative to graph itself (not the scene)
        // this way I can use graph->setPos() to move whole graph to specific place
        node_a->setPos(-50, -50);
        node_b->setPos(50, -50);
        node_c->setPos(-50, 50);
        node_d->setPos(50, 50);

        setSceneRect(scene_rect);

        //main_graph->setPos(-200, -150);
        main_graph->setPos(-main_graph->boundingRect().center());

        adjacency_list_graphics_item = new AdjacencyListGraphicsItem(main_graph);
        adjacency_list_graphics_item->setGraph(main_graph);
        adjacency_list_graphics_item->updateEntries();

        dock_adjacency_list = new Dock(adjacency_list_graphics_item, "Adjacency list", Qt::DockWidgetArea::RightDockWidgetArea);
        //dock_step_description = new Dock(new StepLogGraphicsItem(), "Step description", Qt::RightDockWidgetArea);
        //dock_step_log = new QDockWidget("Step log");
        step_log = new StepLog();
        step_log->show();
        initialized = true;

    }



    addItem(main_graph);
    main_graph->resetColours();

    //qDebug() << "After: main_graph->scenePos() = " << main_graph->scenePos() << ", main_graph->pos()" << main_graph->pos();

/*
    QDockWidget *dock = new QDockWidget(tr("Adjacency list"));
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QGraphicsView *view = new QGraphicsView(dock);
    QGraphicsScene *scene_ = new QGraphicsScene(this);

    adjacency_list_graphics_item->setPos(-adjacency_list_graphics_item->boundingRect().center());
    scene_->addItem(adjacency_list_graphics_item);
    view->setScene(scene_);

    //dock->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    dock->setWidget(view);
    dock->setFloating(true);
    dock->setWindowFlag(Qt::WindowContextHelpButtonHint, true);
    main_window->addDockWidget(Qt::RightDockWidgetArea, dock);

    dock->setFloating(true);
    //dock->setVisible(false);
    QSizeF diff = (adjacency_list_graphics_item->boundingRect().size() - dock->widget()->size()) / 2;
    dock->setGeometry(dock->geometry().adjusted(-diff.width(), -diff.height(), diff.width(), diff.height()));
    */

    // save initial state (to restore it later, after specific algorithm is changed)
    main_graph_had_weights_shown = main_graph->weightsShown();
    step_log->reset();
}

AbstractScene::~AbstractScene()
{
    removeItem(main_graph);

    if (legend)
        delete legend;

    if (dock_legend)
        delete dock_legend;

    if (dock_code)
        delete dock_code;

  //  dock_adjacency_list
    /*if (dock_adjacency_list)
        delete dock_adjacency_list;

    if (step_log)
        delete step_log;*/

    // restore previous
    main_graph->weightsShown(main_graph_had_weights_shown);

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
            if (use_docks)
                dock_adjacency_list->update();
            views()[0]->updateGeometry();
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

    if (key == Qt::Key_Escape)
        main_graph->deselect();

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
            dock_adjacency_list->update();

            views()[0]->updateGeometry();
        }
    }

    std::function<void()> onEnd = [&]() {
        algorithm_started = false;
        onAlgorithmEnd();
        algorithm->reset();
        main_graph->resetColours();
        main_graph->editable(true);
        step_log->reset();
        dock_code->hide();
        dock_legend->hide();

        update();
    };

    if (event->key() == Qt::Key_Escape && algorithm_started)
        onEnd();

    if (event->key() == Qt::Key_Right) {
        if (!algorithm_started) {
            algorithm_started = true;
            step_log->reset();
            main_graph->editable(false);
            algorithm->updateGraph(main_graph);

            dock_code->show();
            dock_legend->show();
            adjacency_list_graphics_item->updateEntries();
            onAlgorithmStart();
        }
        if (algorithm->finished()) {
            onEnd();
        } else {
            algorithm->step();
            onAlgorithmStep();
            step_log->addStep(algorithm->currentStepLabel(), algorithm->currentStepDescription());
        }
        onAlgorithmEvent();
        dock_code->update();

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
