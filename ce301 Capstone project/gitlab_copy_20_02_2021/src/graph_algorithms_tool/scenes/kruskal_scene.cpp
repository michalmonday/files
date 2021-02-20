#include "graph/edge.h"
#include "scenes/kruskal_scene.h"
#include "graph/node.h"
#include "graph/graph.h"
#include "algorithms/controlled_prims_algorithm.h"
#include "mainwindow.h"
#include "legend_graphics_item.h"
#include "colors.h"
#include "algorithms/controlled_kruskals_algorithm.h"
#include "vtable_graphics_item.h"
#include "code_items/kruskals_code_graphics_item.h"
#include "collection_items/priority_queue_graphics_item.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>



KruskalScene::KruskalScene(QObject *parent, Graph *graph)
    : AbstractScene(parent),
    kruskals_algorithm(nullptr),
    pq_graphics_item(nullptr)
{
    initMainGraph("Kruskals", graph);
    main_graph->weightsShown(true);
    legend = new LegendGraphicsItem("Legend - Kruskals");
    legend->addRect(COLOR_MCST.lighter(), "Minimum cost spanning tree");
    legend->addRect(COLOR_RETIRED.lighter(), "Edges not considered anymore");
    legend->addCircle(COLOR_KRUSKALS_ROOT, "Sub-graph roots");

    kruskals_algorithm = new ControlledKruskalsAlgorithm(this->main_graph);

    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = kruskals_algorithm;

    code_graphics_item = new KruskalsCodeGraphicsItem(kruskals_algorithm);

    vtable_graphics_item = new VtableGraphicsItem(kruskals_algorithm, "V table - Kruskals");
    //vtable_graphics_item->indicateMinEntry();

    pq_graphics_item = new PriorityQueueGraphicsItem("Priority queue - Kruskals");
}

KruskalScene::~KruskalScene()
{
    if (!is_exiting) {
        if (vtable_graphics_item)
            delete vtable_graphics_item;

        if (pq_graphics_item)
            delete pq_graphics_item;
    }
}

void KruskalScene::onAlgorithmStart()
{
    vtable_graphics_item->reset();
    vtable_graphics_item->updateValues(kruskals_algorithm);
    vtable_graphics_item->showDock();
    vtable_graphics_item->updateStaticBoundingRect();

    pq_graphics_item->reset();
    pq_graphics_item->setExpectedMaxSize(main_graph->nodes().size()/2);
    pq_graphics_item->adjustToLeft();
    pq_graphics_item->updateDock();

    pq_graphics_item->enqueue(main_graph->edges());
}

void KruskalScene::onAlgorithmStep()
{
    pq_graphics_item->clearHighlight();

    //int previous_state = kruskals_algorithm->getPreviousState();
    //int state = kruskals_algorithm->getState();

    Edge *e = kruskals_algorithm->currentEdge();

    if (Node *new_root = kruskals_algorithm->newRoot()) {
        new_root->setColor(COLOR_KRUSKALS_ROOT);
    }

    if (kruskals_algorithm->wasEdgeAdded()) {
        /*e->setColor(COLOR_MCST);
        e->setHighlighted(COLOR_HIGHLIGHT);
        Node *n1 = e->sourceNode();
        Node *n2 = e->destNode();
        if (n1->getColor() != COLOR_KRUSKALS_ROOT)
            n1->setColor(COLOR_MCST);
        if (n2->getColor() != COLOR_KRUSKALS_ROOT)
            n2->setColor(COLOR_MCST);*/
    } else if (kruskals_algorithm->wasEdgeDiscarded()) {
        e->setColor(COLOR_RETIRED);
        e->setHighlighted(COLOR_HIGHLIGHT_RETIRED);
        e->getLabelItem()->setColor(COLOR_RETIRED);
    }
    e->setPenWidth(5);

    if (Edge *pe = kruskals_algorithm->previousEdge())
        pe->setHighlighted(false);

    if (Node *retired_root = kruskals_algorithm->retiredRoot()) {
        //retired_root->setHighlighted(false);
        retired_root->setColor(COLOR_NODE_DEFAULT);
    }

    if (kruskals_algorithm->finished()) {
        for (Edge *e : kruskals_algorithm->mcst()) {
            e->setColor(COLOR_MCST);
            Node *n1 = e->sourceNode();
            Node *n2 = e->destNode();
            if (n1->getColor() != COLOR_KRUSKALS_ROOT)
                n1->setColor(COLOR_MCST);
            if (n2->getColor() != COLOR_KRUSKALS_ROOT)
                n2->setColor(COLOR_MCST);
        }
        for (Edge *e : kruskals_algorithm->edgeQueue()) {
            e->setColor(COLOR_RETIRED);
            e->getLabelItem()->setColor(COLOR_RETIRED);
            e->setPenWidth(5);
        }
    } else {
        const QList<QColor> colors = {"#0027e8", "#064d00", "#cc2500", "#451600"};
        QList<int> color_ids = kruskals_algorithm->subgraphColorIds();
        int i = 0;
        for (QList<Edge*> sub_graph : kruskals_algorithm->subGraphs()) {
            for (Edge *e : sub_graph) {
                // It's ugly, but the resulting behaviour is neat.
                // Colours are kept by the higher ranked sets after
                // union operation. Also, if the number of subgraphs
                // is higher than the number of available colors,
                // colors are reused in their original order.
                int color_id = color_ids[i % color_ids.size()];
                QColor color = colors[color_id % colors.size()];
                e->setColor(color);
            }
            i++;
        }
    }
    pq_graphics_item->dequeueEdge();
    pq_graphics_item->adjustToLeft();
    pq_graphics_item->updateDock();

    vtable_graphics_item->updateValues();
}

void KruskalScene::onAlgorithmEnd()
{
    vtable_graphics_item->hideDock();
    pq_graphics_item->hideDock();
}

void KruskalScene::onAlgorithmEvent()
{

}

