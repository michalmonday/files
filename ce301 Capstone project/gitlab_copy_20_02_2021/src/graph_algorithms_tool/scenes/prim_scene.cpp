#include "scenes/prim_scene.h"
#include "graph/graph.h"
#include "graph/node.h"
#include "graph/edge.h"
#include "algorithms/controlled_prims_algorithm.h"
#include "legend_graphics_item.h"
#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <dock.h>
#include "code_items/prims_code_graphics_item.h"

#include "colors.h"

#include "adjacency_list_graphics_item.h"
#include "select_algorithm_widget.h"


PrimScene::PrimScene(QObject *parent, Graph *graph)
    : AbstractScene(parent),
      prims_algorithm(nullptr),
      vtable_graphics_item(nullptr)
{
    initMainGraph("Prims", graph);
    main_graph->weightsShown(true);
    // create legend item but don't add it to scene yet
    legend = new LegendGraphicsItem("Legend - Prims");
    legend->addRect(COLOR_MCST.lighter(), "Minimum cost spanning tree");
    legend->addRect(COLOR_CONSIDERED.lighter(), "Considered edges/nodes");
    legend->addRect(COLOR_RETIRED.lighter(), "Edges not considered anymore");
    legend->addDonut(COLOR_ADDED_NODE, "Node just added to MCST");

    // setting position has to be done after scene was initialized (so singleShot is used)
    // if by any chance the scene won't be initialized within 100ms then position won't be set
    //if (!use_docks) {
    //    QTimer::singleShot(100, this, [this](){
    //        legend->setPos(sceneRect().bottomRight() - legend->boundingRect().bottomRight() - QPointF(10,10));
    //    });
    //}



    prims_algorithm = new ControlledPrimsAlgorithm(this->main_graph);

    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = prims_algorithm;

    code_graphics_item = new PrimsCodeGraphicsItem(prims_algorithm);

    vtable_graphics_item = new VtableGraphicsItem(prims_algorithm, "V table - Prims");
    vtable_graphics_item->indicateMinEntry();
}

PrimScene::~PrimScene()
{
    if (!is_exiting) {
        if (vtable_graphics_item)
            delete vtable_graphics_item;
    }
}

void PrimScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::AltModifier) {

    }


    AbstractScene::mousePressEvent(event);
}

void PrimScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    AbstractScene::mouseReleaseEvent(event);
}


void PrimScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}

void PrimScene::onAlgorithmStart()
{
    vtable_graphics_item->reset();
    vtable_graphics_item->updateValues(prims_algorithm);
    vtable_graphics_item->showDock();
    vtable_graphics_item->updateStaticBoundingRect();
}

void PrimScene::onAlgorithmStep()
{
    /*
      I have to implement nextStep in controlled_algorithm
      There's no need to call all these explicit "includeNode", "updateAdjacentNodesSurro..."
      It could be handled in controlled_algorithm or controlled_prims_algorithm.

      On the other side:
      If I implement more "steps" (triggered by mouse press) that are not causing any change
      in the prims_algorithm, (but do something on prim_scene), then having explicit function calls
      could be handy (?). I'm not sure about this...
     */

    //PRIMS_STEP_NEW_NODE_ASSIGNED
    //PRIMS_STEP_NEW_NODE_INCLUDED
    //PRIMS_STEP_ADJACENT_NODES_UPDATED

    int previous_state = prims_algorithm->getPreviousState();
    int state = prims_algorithm->getState();

    // this state is caught here only at the begining (bad code, but it's the way it is...)
    if (state == PRIMS_STATE_NEW_NODE_INCLUDED) {
        Node *included_node = prims_algorithm->includedNode();
        included_node->setColor(COLOR_MCST);
        included_node->setHighlighted(COLOR_ADDED_NODE);
    }

    if (state == PRIMS_STATE_ADJACENT_NODES_UPDATED) {
        for (Node *e : prims_algorithm->potentialSurroundingNodes()) {
            e->setColor(COLOR_CONSIDERED);
            e->setHighlighted(true);
        }

        for (Edge *e : prims_algorithm->potentialSurroundingEdges()) {
            e->setColor(COLOR_CONSIDERED);
            e->setPenWidth(5);
            e->setHighlighted(true);
        }

        for (Edge *e : prims_algorithm->retiredEdges())
            e->setHighlighted(COLOR_HIGHLIGHT_RETIRED);

    }


    if (state == PRIMS_STATE_NEW_NODE_ASSIGNED) {

        for (Node *e : prims_algorithm->previousPotentialSurroundingNodes())
            e->setHighlighted(false);

        for (Edge *e : prims_algorithm->previousPotentialSurroundingEdges())
            e->setHighlighted(false);

        for (Edge *e : prims_algorithm->previousRetiredEdges())
            e->setHighlighted(false);

        Node *included_node = prims_algorithm->includedNode();
        included_node->setColor(COLOR_MCST);

        Edge *included_edge = prims_algorithm->includedEdge();
        included_edge->setColor(COLOR_MCST);
        included_edge->setPenWidth(5);

        included_node->setHighlighted(COLOR_ADDED_NODE);
        prims_algorithm->mcstNodes().last()->setHighlighted(false);



        // includeNode changes state of prims_algorithm so currentStepDescription is called before it
        prims_algorithm->includeNode();



    }

    QList<Edge *> unused_edges = prims_algorithm->allRetiredEdges();
    for (Edge *e : unused_edges) {
        QColor clr(COLOR_RETIRED);
        //clr.setAlpha(40);
        e->setColor(clr);
        e->setPenWidth(5);
        e->getLabelItem()->setColor(COLOR_RETIRED);
    }



    vtable_graphics_item->updateValues();
    adjacency_list_graphics_item->updateColors();
}

void PrimScene::onAlgorithmEnd()
{
    prims_algorithm->mcstNodes().last()->setHighlighted(false);
    vtable_graphics_item->hideDock();
    adjacency_list_graphics_item->updateColors();
}

void PrimScene::onAlgorithmEvent()
{
}



