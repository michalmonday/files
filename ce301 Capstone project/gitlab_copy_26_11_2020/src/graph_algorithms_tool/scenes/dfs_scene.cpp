#include "dfs_scene.h"

#include "algorithms/controlled_dfs_algorithm.h"
#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "step_log.h"
#include "colors.h"
#include "legend_graphics_item.h"
#include "dock.h"
#include "code_items/dfs_code_graphics_item.h"

#include <QKeyEvent>
#include "vtable_graphics_item.h"



DfsScene::DfsScene(QObject *parent)
    : AbstractScene(parent),
      code_graphics_item(nullptr),
      vtable_graphics_item(nullptr),
      dock_vtable(nullptr)
{
    dfs_algorithm = new ControlledDfsAlgorithm(this->main_graph);
    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = dfs_algorithm;

    legend = new LegendGraphicsItem();
    legend->addCircle(COLOR_DFS_START_NODE, "Start node");
    legend->addRect(COLOR_DFS_EXPANDED.lighter(), "Expanded nodes");
    legend->addRect(COLOR_DFS_SEEN.lighter(), "Seen nodes");
    legend->addRect(COLOR_DFS_NO_BACKTRACK.lighter(), "Unused for backtracking");
    legend->addDonut(COLOR_ADDED_NODE, "Recently expanded");
    //legend->addCircle(COLOR_NODE_DEFAULT, "Priority queue");

    dock_legend = new Dock(legend, "Legend", Qt::LeftDockWidgetArea);
    dock_legend->hide();

    code_graphics_item = new DfsCodeGraphicsItem(dfs_algorithm);
    dock_code = new Dock(code_graphics_item, "Code flow", Qt::LeftDockWidgetArea);
    dock_code->hide();

    // create vtable item but don't add it to scene yet
    vtable_graphics_item = new VtableGraphicsItem(dfs_algorithm);
    dock_vtable = new Dock(vtable_graphics_item, "V table", Qt::LeftDockWidgetArea);
    dock_vtable->hide();

    main_graph->weightsShown(false);

    // restores original edges weights after algorithm change
    // it allows DFS to modify weights (to display depth) without losing original ones
    enableRestorableEdgeWeights();
}

DfsScene::~DfsScene()
{
    if (vtable_graphics_item)
        delete vtable_graphics_item;

    if (dock_vtable)
        delete dock_vtable;
}


void DfsScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}

void DfsScene::onAlgorithmStart()
{
    vtable_graphics_item->reset();
    vtable_graphics_item->updateValues(dfs_algorithm);
    dock_vtable->update();
    dock_vtable->show();

    dock_legend->show();
}

void DfsScene::onAlgorithmStep()
{
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
    }

    if (state == DFS_STATE_ADJACENT_NODES_UPDATED) {

        for (Node *n : dfs_algorithm->updatedNodes()) {
            //n->setColor(COLOR_DIJKSTRA_OPTIMAL);
            n->setHighlighted(true);
            n->setColor(COLOR_DFS_SEEN);
        }

        for (Edge *e : dfs_algorithm->updatedEdges()) {
            e->setColor(COLOR_DFS_EXPANDED);
            e->setPenWidth(3);
            e->setHighlighted(true);
        }

        for (Edge *e : dfs_algorithm->retiredEdges()) {
            e->setColor(COLOR_DFS_NO_BACKTRACK);
            e->setHighlighted(true);
            e->setPenWidth(3);
            e->getLabelItem()->setColor(COLOR_DFS_NO_BACKTRACK);
        }
    }
    vtable_graphics_item->updateValues();
}

void DfsScene::onAlgorithmEnd()
{
    dock_vtable->hide();
    dock_legend->hide();
    main_graph->weightsShown(false);
}

void DfsScene::onAlgorithmEvent()
{

}
