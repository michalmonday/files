#include "dijkstra_scene.h"
#include "shortest_path_graphics_item.h"
#include "code_items/dijkstra_code_graphics_item.h"

#include "algorithms/controlled_dijkstra_algorithm.h"
#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "select_algorithm_widget.h"
#include "colors.h"
#include "legend_graphics_item.h"
#include "dock.h"
#include "vtable_graphics_item.h"
#include "functions.h"
#include "collection_items/priority_queue_graphics_item.h"

#include <QKeyEvent>





DijkstraScene::DijkstraScene(QObject *parent, Graph *graph)
    : AbstractScene(parent),
      dijkstra_algorithm(nullptr),
      //shortest_path_graphics_item(nullptr),
      //dock_shortest_paths(nullptr),
      vtable_graphics_item(nullptr),
      pq_graphics_item(nullptr)
{
    initMainGraph("Dijkstra", graph);
    main_graph->weightsShown(true);
    dijkstra_algorithm = new ControlledDijkstraAlgorithm(this->main_graph);
    // algorithm is a member of abstract parent class
    // it is responsible for calling:
    //     - onAlgorithmStart
    //     - onAlgorithmStep
    //     - onAlgorithmEnd
    algorithm = dijkstra_algorithm;

    legend = new LegendGraphicsItem("Legend - Dijkstra");
    legend->addCircle(COLOR_DIJKSTRA_START_NODE, "Start node");
    legend->addRect(COLOR_DIJKSTRA_EXPANDED.lighter(), "Expanded nodes");
    legend->addRect(COLOR_DIJKSTRA_OPTIMAL.lighter(), "Currently optimal");
    legend->addRect(COLOR_DIJKSTRA_SUBOPTIMAL.lighter(), "Definitely sub-optimal");
    legend->addDonut(COLOR_ADDED_NODE, "Recently expanded");
    //legend->addCircle(COLOR_NODE_DEFAULT, "Priority queue");

    code_graphics_item = new DijkstraCodeGraphicsItem(dijkstra_algorithm);

    //shortest_path_graphics_item = new ShortestPathGraphicsItem(dijkstra_algorithm);
    //dock_shortest_paths = new Dock(shortest_path_graphics_item, "Shortest paths (format: V[node] = dist, pred)", Qt::LeftDockWidgetArea);
    //dock_shortest_paths->hide();
    vtable_graphics_item = new VtableGraphicsItem(dijkstra_algorithm, "V table - Dijkstra");

    pq_graphics_item = new PriorityQueueGraphicsItem("Priority queue - Dijkstra");

    // restores original edges weights after algorithm change
    // it allows Dijkstra to modify weights (to display total path cost) without losing original ones
    enableRestorableEdgeWeights();
}

DijkstraScene::~DijkstraScene()
{

    /*if (shortest_path_graphics_item)
        delete shortest_path_graphics_item;

    if (dock_shortest_paths)
        delete dock_shortest_paths;*/
    if (!is_exiting) {
        if (vtable_graphics_item)
            delete vtable_graphics_item;

        if (pq_graphics_item)
            delete pq_graphics_item;
    }
}


void DijkstraScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}

void DijkstraScene::onAlgorithmStart()
{
    vtable_graphics_item->reset();
    vtable_graphics_item->updateValues(dijkstra_algorithm);
    vtable_graphics_item->showDock();
    vtable_graphics_item->updateStaticBoundingRect();

    /*
    dock_shortest_paths->show();
    shortest_path_graphics_item->updateEntries();
    dock_shortest_paths->update();*/

    pq_graphics_item->reset();
    pq_graphics_item->setExpectedMaxSize(main_graph->nodes().size()/2);
    pq_graphics_item->adjustToLeft();
    pq_graphics_item->updateDock();
}

void DijkstraScene::onAlgorithmStep()
{
    pq_graphics_item->clearHighlight();

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

        pq_graphics_item->dequeueNode();
    }

    if (state == DIJKSTRA_STATE_ADJACENT_NODES_UPDATED) {

        for (Node *n : dijkstra_algorithm->updatedNodes()) {
            //n->setColor(COLOR_DIJKSTRA_OPTIMAL);
            n->setHighlighted(true);
        }

        pq_graphics_item->enqueue(dijkstra_algorithm->pqKeysOfUpdatedNodes(),
                                  dijkstra_algorithm->updatedNodes());

        for (Edge *e : dijkstra_algorithm->updatedEdges()) {
            e->setColor(COLOR_DIJKSTRA_OPTIMAL);
            e->setPenWidth(5);
            e->setHighlighted(true);
            updateEdgeTotalCost(e, COLOR_DIJKSTRA_OPTIMAL);
        }

        for (Edge *e : dijkstra_algorithm->retiredEdges()) {
            e->setColor(COLOR_DIJKSTRA_SUBOPTIMAL);
            e->setHighlighted(COLOR_HIGHLIGHT_RETIRED);
            e->setPenWidth(5);
            e->getLabelItem()->setColor(COLOR_DIJKSTRA_SUBOPTIMAL);
            updateEdgeTotalCost(e, COLOR_DIJKSTRA_SUBOPTIMAL);
        }
    }

    // set optimal edges to different color at the end
    // indicating that these are certainly optimal paths
    // and that algorithm is finished
    if (state == DIJKSTRA_STATE_FINISHED) {
        Node *n = dijkstra_algorithm->currentNode();
        n->setColor(COLOR_DIJKSTRA_EXPANDED);
        n->setHighlighted(false);
        if (Node *pn = dijkstra_algorithm->previousNode())
            pn->setHighlighted(false);

        for (Edge *e : main_graph->edges()) {
            if (e->getColor() == COLOR_DIJKSTRA_OPTIMAL) {
                e->setColor(COLOR_DIJKSTRA_EXPANDED);
                updateEdgeTotalCost(e, COLOR_DIJKSTRA_EXPANDED);
            }
            e->setHighlighted(false);
        }
        pq_graphics_item->dequeueNode();
    }
    //shortest_path_graphics_item->updateEntries();
    vtable_graphics_item->updateValues();

    pq_graphics_item->adjustToLeft();
    pq_graphics_item->updateDock();
}

void DijkstraScene::onAlgorithmEnd()
{
    //dock_shortest_paths->hide();
    vtable_graphics_item->hideDock();
    pq_graphics_item->hideDock();
    removeEdgeTotalCosts();
}

void DijkstraScene::onAlgorithmEvent()
{

}

void DijkstraScene::updateEdgeTotalCost(Edge *e, QColor clr)
{
    const Vtable &V = dijkstra_algorithm->vTable();
    Node *n1 = e->sourceNode();
    Node *n2 = e->destNode();

    Node *dest_node = nullptr;
    Node *src_node = nullptr;

    // compare weights to determine which one is source and destination
    if (V[n1].first < V[n2].first) {
        src_node = n1;
        dest_node = n2;
    } else {
        src_node = n2;
        dest_node = n1;
    }

    QString total_weight = "";
    if (V[dest_node].second == src_node)
        // optimal total weight
        total_weight = QString::number( V[dest_node].first );
    else
        // what total weight would be if it went through this edge
        total_weight = QString::number( V[src_node].first + e->getWeight() );

    EdgeTextItem *text_item = e->getLabelItem();
    text_item->setHtml(QString("%1 (%2)")
                       .arg(e->getWeight())
                       .arg(colouredHtml(total_weight,
                                         clr,
                                         /*bold*/ false)));
    e->adjust();
}

// once algorithm is finished, the total costs should disappear
void DijkstraScene::removeEdgeTotalCosts()
{
    for (Edge *e : main_graph->edges()) {
        e->getLabelItem()->setHtml(QString::number(e->getWeight()));
        e->adjust();
    }
}

