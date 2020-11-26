#include "dijkstra_scene.h"
#include "shortest_path_graphics_item.h"
#include "code_items/dijkstra_code_graphics_item.h"

#include "algorithms/controlled_dijkstra_algorithm.h"
#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "step_log.h"
#include "colors.h"
#include "legend_graphics_item.h"
#include "dock.h"

#include <QKeyEvent>


DijkstraScene::DijkstraScene(QObject *parent)
    : AbstractScene(parent),
      dijkstra_algorithm(nullptr),
      shortest_path_graphics_item(nullptr),
      dock_shortest_paths(nullptr),
      code_graphics_item(nullptr)
{
    dijkstra_algorithm = new ControlledDijkstraAlgorithm(this->main_graph);
    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = dijkstra_algorithm;

    legend = new LegendGraphicsItem();
    legend->addCircle(COLOR_DIJKSTRA_START_NODE, "Start node");
    legend->addRect(COLOR_DIJKSTRA_EXPANDED.lighter(), "Expanded nodes");
    legend->addRect(COLOR_DIJKSTRA_OPTIMAL.lighter(), "Currently optimal");
    legend->addRect(COLOR_DIJKSTRA_SUBOPTIMAL.lighter(), "Definitely sub-optimal");
    legend->addDonut(COLOR_ADDED_NODE, "Recently expanded");
    //legend->addCircle(COLOR_NODE_DEFAULT, "Priority queue");

    dock_legend = new Dock(legend, "Legend", Qt::LeftDockWidgetArea);
    dock_legend->hide();

    code_graphics_item = new DijkstraCodeGraphicsItem(dijkstra_algorithm);
    dock_code = new Dock(code_graphics_item, "Code flow", Qt::LeftDockWidgetArea);
    dock_code->hide();

    shortest_path_graphics_item = new ShortestPathGraphicsItem(dijkstra_algorithm);
    dock_shortest_paths = new Dock(shortest_path_graphics_item, "Shortest paths (format: V[node] = dist, pred)", Qt::LeftDockWidgetArea);
    dock_shortest_paths->hide();
}

DijkstraScene::~DijkstraScene()
{
    if (code_graphics_item)
        delete code_graphics_item;

    if (shortest_path_graphics_item)
        delete shortest_path_graphics_item;

    if (dock_shortest_paths)
        delete dock_shortest_paths;
}


void DijkstraScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}

void DijkstraScene::onAlgorithmStart()
{
    dock_legend->show();
    dock_shortest_paths->show();

    shortest_path_graphics_item->updateEntries();
    dock_shortest_paths->update();
}

void DijkstraScene::onAlgorithmStep()
{
    int previous_state = dijkstra_algorithm->getPreviousState();
    int state = dijkstra_algorithm->getState();

    // this state is caught here only at the begining (bad code, but it's the way it is...)
    if (state == DIJKSTRA_STATE_NEW_NODE_SELECTED) {
        for (Node *n : dijkstra_algorithm->previous_updatedNodes())
            n->setHighlighted(false);

        for (Edge *e : dijkstra_algorithm->previous_updatedEdges())
            e->setHighlighted(false);

        Node *n = dijkstra_algorithm->currentNode();

        n->setColor(n == dijkstra_algorithm->startNode() ? COLOR_DIJKSTRA_START_NODE : COLOR_DIJKSTRA_EXPANDED);
        n->setHighlighted(COLOR_ADDED_NODE);

        if (Node *pn = dijkstra_algorithm->previousNode())
            pn->setHighlighted(false);

        for (Edge *e : dijkstra_algorithm->previous_retiredEdges())
            e->setHighlighted(false);
    }

    if (state == DIJKSTRA_STATE_ADJACENT_NODES_UPDATED) {

        for (Node *n : dijkstra_algorithm->updatedNodes()) {
            //n->setColor(COLOR_DIJKSTRA_OPTIMAL);
            n->setHighlighted(true);
        }

        for (Edge *e : dijkstra_algorithm->updatedEdges()) {
            e->setColor(COLOR_DIJKSTRA_OPTIMAL);
            e->setPenWidth(3);
            e->setHighlighted(true);
        }

        for (Edge *e : dijkstra_algorithm->retiredEdges()) {
            e->setColor(COLOR_DIJKSTRA_SUBOPTIMAL);
            e->setHighlighted(true);
            e->setPenWidth(3);
            e->getLabelItem()->setColor(COLOR_DIJKSTRA_SUBOPTIMAL);
        }
    }
    shortest_path_graphics_item->updateEntries();
}

void DijkstraScene::onAlgorithmEnd()
{
    dock_legend->hide();
    dock_shortest_paths->hide();
}

void DijkstraScene::onAlgorithmEvent()
{

}
