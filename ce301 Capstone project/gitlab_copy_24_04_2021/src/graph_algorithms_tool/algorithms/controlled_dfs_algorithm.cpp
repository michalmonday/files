#include "controlled_dfs_algorithm.h"

#include "graph/graph.h"
#include "functions.h"
#include "colors.h"

#include <graph/node.h>
#include <graph/edge.h>

const int DFS_STATE_INIT = 1;
const int DFS_STATE_NEW_NODE_SELECTED      = 2;
const int DFS_STATE_ADJACENT_NODES_UPDATED = 3;

const int NODE_STATE_UNKNOWN = 0;
const int NODE_STATE_SEEN = 1;
const int NODE_STATE_EXPANDED = 2;

ControlledDfsAlgorithm::ControlledDfsAlgorithm(Graph *g)
    : ControlledAlgorithm(g),
      previous_node(nullptr),
      current_node(nullptr)
{
    reset();
}

void ControlledDfsAlgorithm::step()
{
    if (state == DFS_STATE_INIT || state == DFS_STATE_ADJACENT_NODES_UPDATED) {
        clearLastIterationInfo();
        current_node = stack_.pop();

        setState(DFS_STATE_NEW_NODE_SELECTED);
    } else if (state == DFS_STATE_NEW_NODE_SELECTED) {
        for (Node *v : adjacency_map[current_node]) {
            Edge *e = current_node->edgeWith(v);
            if (node_state[v] == NODE_STATE_UNKNOWN) {
                dist_[v] = dist_[current_node] + 1;
                pred_[v] = current_node;
                node_state[v] = NODE_STATE_SEEN;
                stack_.push(v);
                updated_nodes.append(v);

                V[v] = {dist_[current_node] + 1, current_node}; // kind of redundant (until I decide to use pred+dist or V)

                EdgeTextItem *text_item = e->getLabelItem();
                text_item->setPlainText(QString::number(dist_[v]));
                text_item->show();
                // to repaint the weight label rectangle
                e->adjust();
            } else {
                if (node_state[v] != NODE_STATE_EXPANDED)
                    retired_edges.append(e);
            }
        }
        node_state[current_node] = NODE_STATE_EXPANDED;

        setState(DFS_STATE_ADJACENT_NODES_UPDATED);
    }

}

QString ControlledDfsAlgorithm::brief()
{
    return "Depth first search traverses the graph by prioritizing the furthest nodes first.";
    //       "<br><br>"
    //       "";
}

QString ControlledDfsAlgorithm::currentStepLabel()
{
    if (previous_state == DFS_STATE_INIT)
        return QString("Start node '%1' selected").arg(current_node->getLabel());


    if (state == DFS_STATE_ADJACENT_NODES_UPDATED)
        return QString("Adjacent nodes of '%1' pushed to stack").arg(current_node->getLabel());

    if (state == DFS_STATE_NEW_NODE_SELECTED)
        return QString("Node '%1' selected").arg(current_node->getLabel());

    return "unrecognized state";
}

QString ControlledDfsAlgorithm::currentStepDescription()
{
    QString description;
    if (previous_state == DFS_STATE_INIT) {
        return QString("Start node (%1) was selected. "
                       "The algorithm will search through all the nodes, prioritizing "
                       "the furthest nodes first.")
                .arg(current_node->colouredLabel());
    }

    if (state == DFS_STATE_ADJACENT_NODES_UPDATED) {
        if (updated_nodes.empty()) {
            description = QString("'%1' node does not have any remaining unexpanded neighbors.")
                    .arg(current_node->colouredLabel());
        } else {

            description = QString("Adjacent nodes of selected node '%1' were added to stack. "
                                  "In other words, the node '%1' was expanded. "
                                  "All neighbors of node '%1' that were not expanded yet, "
                                  "were marked as '%2' and pushed to stack. <br><br>")
                        .arg(current_node->colouredLabel())
                        .arg(colouredHtml("seen", COLOR_DFS_SEEN));

            description += "These nodes were:<br>";
            for (Node *n : updated_nodes)
                description += n->colouredLabel() + (n != updatedNodes().last() ? ", " : "");
        }
    }


    if (state == DFS_STATE_NEW_NODE_SELECTED) {
        description = QString("'%1' was selected to be expanded. That is because it is on top "
                              "of the stack. In other words, it was marked as '%2' the most recently.")
                              .arg(current_node->colouredLabel())
                              .arg(colouredHtml("seen", COLOR_DFS_SEEN));
    }

    return description;
}

bool ControlledDfsAlgorithm::finished()
{
    return stack_.isEmpty() && state == DFS_STATE_ADJACENT_NODES_UPDATED;
}



QList<Edge *> ControlledDfsAlgorithm::updatedEdges()
{
    QList<Edge*> edges;
    for (Node *n : updated_nodes)
        edges.append(current_node->edgeWith(n));
    return edges;
}

void ControlledDfsAlgorithm::clearLastIterationInfo()
{
    previous_updated_nodes = updated_nodes;
    previous_updated_edges = updatedEdges();

    updated_nodes.clear();
    // updatedEdges() does not need clearing, it's generated dynamically upon request (based on updated_nodes and current_node)

    previous_retired_edges = retired_edges;
    retired_edges.clear();
    previous_node = current_node;
}


void ControlledDfsAlgorithm::reset()
{
    ControlledAlgorithm::reset();
    dist_.clear();
    pred_.clear();
    stack_.clear();
    state = DFS_STATE_INIT;
    previous_state = DFS_STATE_INIT;
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
    stack_.push(start_node);

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
