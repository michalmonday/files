#include "bfs_scene.h"

#include "algorithms/controlled_bfs_algorithm.h"
#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "step_log.h"
#include "colors.h"
#include "legend_graphics_item.h"
#include "dock.h"
#include "code_items/bfs_code_graphics_item.h"

#include <QKeyEvent>



BfsScene::BfsScene(QObject *parent)
    : AbstractScene(parent),
      code_graphics_item(nullptr)
{
    bfs_algorithm = new ControlledBfsAlgorithm(this->main_graph);
    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = bfs_algorithm;

    legend = new LegendGraphicsItem();
    legend->addCircle(COLOR_BFS_START_NODE, "Start node");
    legend->addRect(COLOR_BFS_EXPANDED.lighter(), "Expanded nodes");
    legend->addRect(COLOR_BFS_SEEN.lighter(), "Seen nodes");
    legend->addRect(COLOR_BFS_NO_BACKTRACK.lighter(), "Unused for backtracking");
    legend->addDonut(COLOR_ADDED_NODE, "Recently expanded");
    //legend->addCircle(COLOR_NODE_DEFAULT, "Priority queue");

    dock_legend = new Dock(legend, "Legend", Qt::LeftDockWidgetArea);
    dock_legend->hide();

    code_graphics_item = new BfsCodeGraphicsItem(bfs_algorithm);
    dock_code = new Dock(code_graphics_item, "Code flow", Qt::LeftDockWidgetArea);
    dock_code->hide();

    main_graph->weightsShown(false);

    // restores original edges weights after algorithm change
    // it allows BFS to modify weights (to display depth) without losing original ones
    enableRestorableEdgeWeights();
}

BfsScene::~BfsScene()
{
    //if (code_graphics_item)
    //    delete code_graphics_item;
}


void BfsScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}

void BfsScene::onAlgorithmStart()
{
    dock_legend->show();
}

void BfsScene::onAlgorithmStep()
{
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
    }

    if (state == BFS_STATE_ADJACENT_NODES_UPDATED) {

        for (Node *n : bfs_algorithm->updatedNodes()) {
            //n->setColor(COLOR_DIJKSTRA_OPTIMAL);
            n->setHighlighted(true);
            n->setColor(COLOR_BFS_SEEN);
        }

        for (Edge *e : bfs_algorithm->updatedEdges()) {
            e->setColor(COLOR_BFS_EXPANDED);
            e->setPenWidth(3);
            e->setHighlighted(true);
        }

        for (Edge *e : bfs_algorithm->retiredEdges()) {
            e->setColor(COLOR_BFS_NO_BACKTRACK);
            e->setHighlighted(true);
            e->setPenWidth(3);
            e->getLabelItem()->setColor(COLOR_BFS_NO_BACKTRACK);
        }
    }
}

void BfsScene::onAlgorithmEnd()
{
    dock_legend->hide();
    main_graph->weightsShown(false);
}

void BfsScene::onAlgorithmEvent()
{

}
