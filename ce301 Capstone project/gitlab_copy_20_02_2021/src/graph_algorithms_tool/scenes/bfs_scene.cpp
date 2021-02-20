#include "bfs_scene.h"

#include "algorithms/controlled_bfs_algorithm.h"
#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "select_algorithm_widget.h"
#include "colors.h"
#include "legend_graphics_item.h"
#include "dock.h"
#include "code_items/bfs_code_graphics_item.h"
#include "vtable_graphics_item.h"

#include <QKeyEvent>

#include <collection_items/queue_graphics_item.h>




BfsScene::BfsScene(QObject *parent, Graph *graph)
    : AbstractScene(parent),
      vtable_graphics_item(nullptr),
      queue_graphics_item(nullptr)
{
    initMainGraph("Breadth first search", graph);

    main_graph->weightsShown(false);
    bfs_algorithm = new ControlledBfsAlgorithm(this->main_graph);
    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = bfs_algorithm;

    legend = new LegendGraphicsItem("Legend - BFS");
    legend->addCircle(COLOR_BFS_START_NODE, "Start node");
    legend->addRect(COLOR_BFS_EXPANDED.lighter(), "Expanded nodes");
    legend->addRect(COLOR_BFS_SEEN.lighter(), "Seen nodes");
    legend->addRect(COLOR_BFS_NO_BACKTRACK.lighter(), "Unused for backtracking");
    legend->addDonut(COLOR_ADDED_NODE, "Recently expanded");
    //legend->addCircle(COLOR_NODE_DEFAULT, "Priority queue");

    code_graphics_item = new BfsCodeGraphicsItem(bfs_algorithm);

    vtable_graphics_item = new VtableGraphicsItem(bfs_algorithm, "V table - BFS");

    queue_graphics_item = new QueueGraphicsItem("Queue - BFS");

    main_graph->weightsShown(false);

    // restores original edges weights after algorithm change
    // it allows BFS to modify weights (to display depth) without losing original ones
    enableRestorableEdgeWeights();
}

BfsScene::~BfsScene()
{
    if (!is_exiting) {
        if (vtable_graphics_item)
            delete vtable_graphics_item;

        if (queue_graphics_item)
            delete queue_graphics_item;
    }
}


void BfsScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}

void BfsScene::onAlgorithmStart()
{
    vtable_graphics_item->reset();
    vtable_graphics_item->updateValues(bfs_algorithm);
    vtable_graphics_item->showDock();
    vtable_graphics_item->updateStaticBoundingRect();

    queue_graphics_item->reset();
    queue_graphics_item->setExpectedMaxSize(main_graph->nodes().size()/2);
    queue_graphics_item->adjustToLeft();
    queue_graphics_item->updateDock();


}


void BfsScene::onAlgorithmStep()
{
    queue_graphics_item->clearHighlight();

    int previous_state = bfs_algorithm->getPreviousState();
    int state = bfs_algorithm->getState();

    // this state is caught here only at the begining (bad code, but it's the way it is...)
    if (state == BFS_STATE_NEW_NODE_SELECTED) {
        for (Node *n : bfs_algorithm->previous_updatedNodes())
            n->setHighlighted(false);

        for (Edge *e : bfs_algorithm->previous_updatedEdges())
            e->setHighlighted(false);

        Node *n = bfs_algorithm->currentNode();

        n->setColor(n == bfs_algorithm->startNode() ? COLOR_BFS_START_NODE : COLOR_BFS_EXPANDED);
        n->setHighlighted(COLOR_ADDED_NODE);

        if (Node *pn = bfs_algorithm->previousNode())
            pn->setHighlighted(false);

        for (Edge *e : bfs_algorithm->previous_retiredEdges())
            e->setHighlighted(false);

        queue_graphics_item->dequeue();
    }

    if (state == BFS_STATE_ADJACENT_NODES_UPDATED) {

        for (Node *n : bfs_algorithm->updatedNodes()) {
            //n->setColor(COLOR_DIJKSTRA_OPTIMAL);
            n->setHighlighted(true);
            n->setColor(COLOR_BFS_SEEN);
        }
        queue_graphics_item->enqueue(bfs_algorithm->updatedNodes());

        for (Edge *e : bfs_algorithm->updatedEdges()) {
            e->setColor(COLOR_BFS_EXPANDED);
            e->setPenWidth(5);
            e->setHighlighted(true);
        }

        for (Edge *e : bfs_algorithm->retiredEdges()) {
            e->setColor(COLOR_BFS_NO_BACKTRACK);
            e->setHighlighted(COLOR_HIGHLIGHT_RETIRED);
            e->setPenWidth(5);
            e->getLabelItem()->setColor(COLOR_BFS_NO_BACKTRACK);
        }
    }
    vtable_graphics_item->updateValues();

    queue_graphics_item->adjustToLeft();
    queue_graphics_item->updateDock();
}

void BfsScene::onAlgorithmEnd()
{
    vtable_graphics_item->hideDock();
    queue_graphics_item->hideDock();
    main_graph->weightsShown(false);
}

void BfsScene::onAlgorithmEvent()
{

}
