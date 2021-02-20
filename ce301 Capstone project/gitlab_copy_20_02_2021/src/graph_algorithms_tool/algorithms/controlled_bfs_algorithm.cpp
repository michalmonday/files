#include "controlled_bfs_algorithm.h"

#include "graph/graph.h"
#include "functions.h"
#include "colors.h"

#include <graph/node.h>
#include <graph/edge.h>

const int BFS_STATE_INIT = 1;
const int BFS_STATE_NEW_NODE_SELECTED      = 2;
const int BFS_STATE_ADJACENT_NODES_UPDATED = 3;

const int NODE_STATE_UNKNOWN = 0;
const int NODE_STATE_SEEN = 1;
const int NODE_STATE_EXPANDED = 2;

ControlledBfsAlgorithm::ControlledBfsAlgorithm(Graph *g)
    : ControlledAlgorithm(g),
      previous_node(nullptr),
      current_node(nullptr)
{
    reset();
}

void ControlledBfsAlgorithm::step()
{
    if (state == BFS_STATE_INIT || state == BFS_STATE_ADJACENT_NODES_UPDATED) {
        clearLastIterationInfo();
        current_node = queue_.dequeue();

        setState(BFS_STATE_NEW_NODE_SELECTED);
    } else if (state == BFS_STATE_NEW_NODE_SELECTED) {
        for (Node *v : adjacency_map[current_node]) {
            Edge *e = current_node->edgeWith(v);
            if (node_state[v] == NODE_STATE_UNKNOWN) {
                dist_[v] = dist_[current_node] + 1;
                pred_[v] = current_node;
                node_state[v] = NODE_STATE_SEEN;
                queue_.enqueue(v);
                updated_nodes.append(v);

                V[v] = {dist_[current_node] + 1, current_node}; // kind of redundant (until I decide to use pred+dist or V)

                EdgeTextItem *text_item = e->getLabelItem();
                text_item->setPlainText(QString::number(dist_[v]));
                text_item->show();
                // to repaint the weight label rectangle
                e->adjust();
            } else {
                if (node_state[v] != NODE_STATE_EXPANDED) {
                    retired_edges.append(e);
                }
            }
        }
        node_state[current_node] = NODE_STATE_EXPANDED;

        setState(BFS_STATE_ADJACENT_NODES_UPDATED);
    }

}

QString ControlledBfsAlgorithm::brief()
{
    return "Breadth first search (BFS) traverses the graph by prioritizing the closest nodes first.";
    //       "<br><br>"
    //       "BFS vs DFS<br>"
    //       "";
}

QString ControlledBfsAlgorithm::currentStepLabel()
{
    if (previous_state == BFS_STATE_INIT)
        return QString("Start node '%1' selected").arg(current_node->getLabel());


    if (state == BFS_STATE_ADJACENT_NODES_UPDATED)
        return QString("Adjacent nodes of '%1' updated").arg(current_node->getLabel());

    if (state == BFS_STATE_NEW_NODE_SELECTED)
        return QString("Node '%1' selected").arg(current_node->getLabel());

    return "unrecognized state";
}

QString ControlledBfsAlgorithm::currentStepDescription()
{
    QString description;
    if (previous_state == BFS_STATE_INIT) {
        return QString("Start node (%1) was selected. "
                       "The algorithm will search through all the nodes, prioritizing "
                       "the closest nodes first.")
                .arg(current_node->colouredLabel());
    }

    if (state == BFS_STATE_ADJACENT_NODES_UPDATED) {
        if (updated_nodes.empty()) {
            description = QString("'%1' node does not have any remaining unexpanded neighbors.")
                    .arg(current_node->colouredLabel());
        } else {

            description = QString("Adjacent nodes of selected node '%1' were updated. "
                                  "In other words, the node '%1' was expanded. "
                                  "All neighbors of node '%1' that were not expanded yet, "
                                  "were marked as 'seen'. <br><br>")
                        .arg(current_node->colouredLabel());

            description += "These nodes were:<br>";
            for (Node *n : updated_nodes)
                description += n->colouredLabel() + (n != updatedNodes().last() ? ", " : "");
        }
    }


    if (state == BFS_STATE_NEW_NODE_SELECTED) {
        description = QString("'%1' was selected to be expanded. That is because it was marked as 'seen' "
                              "the earliest from all 'seen' nodes.")
                              .arg(current_node->colouredLabel());
    }

    return description;
}

bool ControlledBfsAlgorithm::finished()
{
    return queue_.isEmpty() && state == BFS_STATE_ADJACENT_NODES_UPDATED;
}



QList<Edge *> ControlledBfsAlgorithm::updatedEdges()
{
    QList<Edge*> edges;
    for (Node *n : updated_nodes)
        edges.append(current_node->edgeWith(n));
    return edges;
}

void ControlledBfsAlgorithm::clearLastIterationInfo()
{
    previous_updated_nodes = updated_nodes;
    previous_updated_edges = updatedEdges();

    updated_nodes.clear();
    // updatedEdges() does not need clearing, it's generated dynamically upon request (based on updated_nodes and current_node)

    previous_retired_edges = retired_edges;
    retired_edges.clear();
    previous_node = current_node;
}


void ControlledBfsAlgorithm::reset()
{
    ControlledAlgorithm::reset();

    dist_.clear();
    pred_.clear();
    queue_.clear();
    state = BFS_STATE_INIT;
    previous_state = BFS_STATE_INIT;
    adjacency_map = graph_->adjacencyMap();

    for (Node *n : adjacency_map.keys()) {
        dist_[n] = INT_MAX;
        pred_[n] = nullptr;

        V[n] = {INT_MAX, nullptr}; // kind of redundant (until I decide to use pred+dist or V)
    }

    node_state.clear();
    for (Node *n : adjacency_map.keys())
        node_state[n] = NODE_STATE_UNKNOWN;



    start_node = graph_->selectedNodeOrRandom();
    dist_[start_node] = 0;
    queue_.enqueue(start_node);

    V[start_node].first = 0; // kind of redundant (until I decide to use pred+dist or V)

    updated_nodes.clear();
    previous_updated_edges.clear();
    previous_updated_nodes.clear();
    retired_edges.clear();
    previous_retired_edges.clear();
    previous_node = nullptr;
    current_node = nullptr;
    expanded_nodes.clear();
}
