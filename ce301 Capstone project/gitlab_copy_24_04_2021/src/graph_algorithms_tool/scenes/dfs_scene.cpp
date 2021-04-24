#include "dfs_scene.h"

#include "algorithms/controlled_dfs_algorithm.h"
#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "select_algorithm_widget.h"
#include "colors.h"
#include "legend_graphics_item.h"
#include "dock.h"
#include "code_items/dfs_code_graphics_item.h"
#include "collection_items/stack_graphics_item.h"
#include "vtable_graphics_item.h"

#include <QKeyEvent>





DfsScene::DfsScene(QObject *parent, Graph *graph)
    : AbstractScene(parent),
      vtable_graphics_item(nullptr)
      //stack_graphics_item(nullptr),
      //dock_stack(nullptr)
{
    initMainGraph("Depth first search", graph);
    main_graph->weightsShown(false);
    dfs_algorithm = new ControlledDfsAlgorithm(this->main_graph);
    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = dfs_algorithm;

    legend = new LegendGraphicsItem("Legend - DFS");
    legend->addCircle(COLOR_DFS_START_NODE, "Start node");
    legend->addRect(COLOR_DFS_EXPANDED.lighter(), "Expanded nodes");
    legend->addRect(COLOR_DFS_SEEN.lighter(), "Seen nodes");
    legend->addRect(COLOR_DFS_NO_BACKTRACK.lighter(), "Unused for backtracking");
    legend->addDonut(COLOR_ADDED_NODE, "Recently expanded");
    //legend->addCircle(COLOR_NODE_DEFAULT, "Priority queue");

    code_graphics_item = new DfsCodeGraphicsItem(dfs_algorithm);

    vtable_graphics_item = new VtableGraphicsItem(dfs_algorithm, "V table - DFS");

    stack_graphics_item = new StackGraphicsItem("Stack - DFS");

    main_graph->weightsShown(false);

    // restores original edges weights after algorithm change
    // it allows DFS to modify weights (to display depth) without losing original ones
    enableRestorableEdgeWeights();
}

DfsScene::~DfsScene()
{
    if (!is_exiting) {
        if (vtable_graphics_item)
            delete vtable_graphics_item;

        if (stack_graphics_item)
                delete stack_graphics_item;
    }
}


void DfsScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}

void DfsScene::onAlgorithmStart()
{
    vtable_graphics_item->reset();
    vtable_graphics_item->updateValues(dfs_algorithm);
    vtable_graphics_item->showDock();
    //stack_graphics_item->setExpectedMaxSize(main_graph->nodes().size()/2);
    //dock_stack->show();
    vtable_graphics_item->updateStaticBoundingRect();

    stack_graphics_item->reset();
    stack_graphics_item->setExpectedMaxSize(main_graph->nodes().size()/2);
    stack_graphics_item->adjustToLeft();
    stack_graphics_item->updateDock();
}

void DfsScene::onAlgorithmStep()
{
    stack_graphics_item->clearHighlight();

    int previous_state = dfs_algorithm->getPreviousState();
    int state = dfs_algorithm->getState();

    // this state is caught here only at the begining (bad code, but it's the way it is...)
    if (state == DFS_STATE_NEW_NODE_SELECTED) {
        for (Node *n : dfs_algorithm->previous_updatedNodes())
            n->setHighlighted(false);

        for (Edge *e : dfs_algorithm->previous_updatedEdges())
            e->setHighlighted(false);

        Node *n = dfs_algorithm->currentNode();

        n->setColor(n == dfs_algorithm->startNode() ? COLOR_DFS_START_NODE : COLOR_DFS_EXPANDED);
        n->setHighlighted(COLOR_ADDED_NODE);

        if (Node *pn = dfs_algorithm->previousNode())
            pn->setHighlighted(false);

        for (Edge *e : dfs_algorithm->previous_retiredEdges())
            e->setHighlighted(false);

        stack_graphics_item->pop();
    }

    if (state == DFS_STATE_ADJACENT_NODES_UPDATED) {

        for (Node *n : dfs_algorithm->updatedNodes()) {
            //n->setColor(COLOR_DIJKSTRA_OPTIMAL);
            n->setHighlighted(true);
            n->setColor(COLOR_DFS_SEEN);
        }
        stack_graphics_item->push(dfs_algorithm->updatedNodes());

        for (Edge *e : dfs_algorithm->updatedEdges()) {
            e->setColor(COLOR_DFS_EXPANDED);
            e->setPenWidth(5);
            e->setHighlighted(true);
        }

        for (Edge *e : dfs_algorithm->retiredEdges()) {
            e->setColor(COLOR_DFS_NO_BACKTRACK);
            e->setHighlighted(COLOR_HIGHLIGHT_RETIRED);
            e->setPenWidth(5);
            e->getLabelItem()->setColor(COLOR_DFS_NO_BACKTRACK);
        }
    }
    vtable_graphics_item->updateValues();

    stack_graphics_item->adjustToLeft();
    stack_graphics_item->updateDock();
}

void DfsScene::onAlgorithmEnd()
{
    vtable_graphics_item->hideDock();
    stack_graphics_item->hideDock();
    main_graph->weightsShown(false);
}

void DfsScene::onAlgorithmEvent()
{

}
