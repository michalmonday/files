#include "controlled_dijkstra_algorithm.h"
#include "graph/graph.h"
#include "functions.h"
#include "colors.h"

#include <graph/node.h>
#include <graph/edge.h>

const int DIJKSTRA_STATE_INIT = 1;
const int DIJKSTRA_STATE_NEW_NODE_SELECTED      = 2;
const int DIJKSTRA_STATE_ADJACENT_NODES_UPDATED = 3;

ControlledDijkstraAlgorithm::ControlledDijkstraAlgorithm(Graph *g)
    : ControlledAlgorithm(g),
      previous_node(nullptr),
      current_node(nullptr)
{
    reset();


}

void ControlledDijkstraAlgorithm::step()
{
    if (state == DIJKSTRA_STATE_INIT || state == DIJKSTRA_STATE_ADJACENT_NODES_UPDATED) {
        //for insight info:
        clearLastIterationInfo();

        // select min_node from priority queue
        current_node = pq_.first();
        current_dist = pq_.firstKey();
        pq_.remove(current_dist, current_node);
        setState(DIJKSTRA_STATE_NEW_NODE_SELECTED);
        qDebug() << "DIJKSTRA_STATE_NEW_NODE_SELECTED";
        expanded_nodes.append(current_node);
    } else if (state == DIJKSTRA_STATE_NEW_NODE_SELECTED) {
        // relax surrounding nodes

        for (Node *v : adjacency_map[current_node]) {
            Edge * edge = current_node->edgeWith(v);
            int w = edge->getWeight();
            int newLen = dist_[current_node] + w;
            if (newLen < dist_[v]) {
                // for insight info
                updated_nodes.append(v);
                if (pred_[v] != nullptr)
                    retired_edges.append(v->edgeWith(pred_[v]));

                // 2 lines below are equivalent to: decreaseKey(pq, v, newLen)
                pq_.remove(dist_[v], v);
                pq_.insert(newLen, v);
                dist_[v] = newLen;
                pred_[v] = current_node;
            } else {
                if (pred_[v] != nullptr && !expanded_nodes.contains(v))
                    retired_edges.append(current_node->edgeWith(v));
            }

        }

        setState(DIJKSTRA_STATE_ADJACENT_NODES_UPDATED);
        qDebug() << "DIJKSTRA_STATE_ADJACENT_NODES_UPDATED";
    }


    /*
    qDebug() << "State:";
    for (Node *n : adjacency_map.keys()) {
        qDebug() << "dist[" << n->getLabel() <<  "] = " << dist_[n];
        qDebug() << "pred[" << n->getLabel() <<  "] = " << pred_[n];
    }

    qDebug() << "pq:";


    for (Node *n : pq_.values()) {
        qDebug() << n->getLabel();
    }

    qDebug() << "\n";*/
}

QString ControlledDijkstraAlgorithm::currentStepLabel()
{
    if (previous_state == DIJKSTRA_STATE_INIT)
        return QString("Start node '%1' selected").arg(current_node->getLabel());


    if (state == DIJKSTRA_STATE_ADJACENT_NODES_UPDATED)
        return QString("Adjacent nodes of '%1' updated").arg(current_node->getLabel());

    if (state == DIJKSTRA_STATE_NEW_NODE_SELECTED)
        return QString("Node '%1' selected").arg(current_node->getLabel());

    return "unrecognized state";
}

QString ControlledDijkstraAlgorithm::currentStepDescription()
{
    QString description;
    if (previous_state == DIJKSTRA_STATE_INIT) {
        return QString("Start node (%1) was selected. "
                       "The algorithm will compute optimal paths to all remaining nodes. "
                       "In other words, if I wanted to visit every place (but had to return \"home\" "
                       "after every trip), this algorithm would allow me to cover minimum distance")
                .arg(current_node->colouredLabel());
    }

    if (state == DIJKSTRA_STATE_ADJACENT_NODES_UPDATED) {
        if (updated_nodes.empty()) {
            description = QString("'%1' node does not have any remaining unexpanded nodes.")
                    .arg(current_node->colouredLabel());
        } else {

            description = QString("Adjacent nodes of selected node '%1' were updated. "
                                  "In other words, the node '%1' was expanded. "
                                  "All neighbours of node '%1' that were not expanded yet, "
                                  "were updated with the new shorter paths (if path through node '%1' happened to be shorted. <br><br>")
                        .arg(current_node->colouredLabel());

            description += "These nodes were:<br>";
            for (Node *n : updated_nodes)
                description += n->colouredLabel() + (n != updatedNodes().last() ? ", " : "");
        }
    }


    if (state == DIJKSTRA_STATE_NEW_NODE_SELECTED) {
        description = QString("'%1' was selected to be expanded. That is because it is the closest "
                              "to the starting node among all unexpanded nodes.")
                              .arg(current_node->colouredLabel());
    }

    return description;
}

bool ControlledDijkstraAlgorithm::finished()
{
    return pq_.isEmpty();
}



QList<Edge *> ControlledDijkstraAlgorithm::updatedEdges()
{
    QList<Edge*> edges;
    for (Node *n : updated_nodes)
        edges.append(current_node->edgeWith(n));
    return edges;
}

void ControlledDijkstraAlgorithm::clearLastIterationInfo()
{
    previous_updated_nodes = updated_nodes;
    previous_updated_edges = updatedEdges();

    updated_nodes.clear();
    // updatedEdges() does not need clearing, it's generated dynamically upon request (based on updated_nodes and current_node)

    previous_retired_edges = retired_edges;
    retired_edges.clear();
    previous_node = current_node;
}


void ControlledDijkstraAlgorithm::reset()
{
    dist_.clear();
    pred_.clear();
    pq_.clear();
    state = DIJKSTRA_STATE_INIT;
    previous_state = DIJKSTRA_STATE_INIT;
    adjacency_map = graph_->adjacencyMap();

    for (Node *n : adjacency_map.keys()) {
        dist_[n] = INT_MAX;
        pred_[n] = nullptr;
    }

    start_node = graph_->selectedNodeOrRandom();
    dist_[start_node] = 0;

    for (Node *n : adjacency_map.keys())
        pq_.insert(dist_[n], n);

    updated_nodes.clear();
    previous_updated_edges.clear();
    previous_updated_nodes.clear();
    retired_edges.clear();
    previous_retired_edges.clear();
    previous_node = nullptr;
    current_node = nullptr;
    expanded_nodes.clear();
}
