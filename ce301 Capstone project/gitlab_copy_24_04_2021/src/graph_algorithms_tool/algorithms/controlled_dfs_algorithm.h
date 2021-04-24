#ifndef CONTROLLEDDFSALGORITHM_H
#define CONTROLLEDDFSALGORITHM_H

#include "controlled_algorithm.h"
#include <QMap>
#include <QQueue>
#include <QStack>
#include <queue>
#include "typedefs.h"

class Graph;
class Node;

extern const int DFS_STATE_INIT;
extern const int DFS_STATE_NEW_NODE_SELECTED;
extern const int DFS_STATE_ADJACENT_NODES_UPDATED;

class ControlledDfsAlgorithm : public ControlledAlgorithm
{
public:
    ControlledDfsAlgorithm(Graph *graph);

    // ControlledAlgorithm interface
    virtual void reset() override;
    virtual void step() override;
    virtual QString name() override { return "Depth first search"; }
    virtual QString brief() override;
    virtual QString currentStepLabel() override;
    virtual QString currentStepDescription() override;

    Node * startNode() { return start_node; }
    Node * currentNode() { return current_node; }

    virtual bool finished() override;

    QList<Node*> updatedNodes() { return updated_nodes; }
    QList<Node*> previous_updatedNodes() { return previous_updated_nodes; }
    QList<Edge*> updatedEdges();
    QList<Edge*> previous_updatedEdges() { return previous_updated_edges; }
    Node *previousNode() { return previous_node; }

    // edges that are no longer used due to relaxation (due to updatedEdges taking their place at shortest path)
    QList<Edge*> retiredEdges() { return retired_edges; }
    QList<Edge*> previous_retiredEdges() { return previous_retired_edges; }

    QMap<Node*, int> dist() { return dist_; }
    QMap<Node*, Node*> pred() { return pred_; }
    QMap<Node*, int> nodeState() { return node_state; }
    QStack<Node *> stack() { return stack_; }

private:
    AdjacencyMap adjacency_map;
    Node *start_node;

    QMap<Node*, int> dist_;
    QMap<Node*, Node*> pred_;
    QMap<Node*, int> node_state;
    QStack<Node*> stack_;

    Node *current_node;
    int current_dist;

    QList<Node*> expanded_nodes;

    // insight data:
    //QList<Node*> updated_nodes;
    QList<Node*> previous_updated_nodes;
    QList<Edge*> previous_updated_edges;
    QList<Edge*> retired_edges;
    QList<Edge*> previous_retired_edges;
    Node *previous_node;

    void clearLastIterationInfo();
};

#endif // CONTROLLEDDFSALGORITHM_H
