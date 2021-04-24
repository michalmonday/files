#ifndef CONTROLLEDDIJKSTRAALGORITHM_H
#define CONTROLLEDDIJKSTRAALGORITHM_H

#include "controlled_algorithm.h"

#include <QMap>
#include <queue>
#include "typedefs.h"

class Graph;
class Node;

extern const int DIJKSTRA_STATE_INIT;
extern const int DIJKSTRA_STATE_NEW_NODE_SELECTED;
extern const int DIJKSTRA_STATE_ADJACENT_NODES_UPDATED;
extern const int DIJKSTRA_STATE_FINISHED;

class ControlledDijkstraAlgorithm : public ControlledAlgorithm
{
public:
    ControlledDijkstraAlgorithm(Graph *graph);

    // ControlledAlgorithm interface
    virtual void reset() override;
    virtual void step() override;
    virtual QString name() override { return "Dijkstra"; }
    virtual QString brief() override;
    virtual QString currentStepLabel() override;
    virtual QString currentStepDescription() override;

    Node * startNode() { return start_node; }
    Node * currentNode() { return current_node; }

    virtual bool finished() override;

    QList<Node*> previous_updatedNodes() { return previous_updated_nodes; }
    QList<Edge*> updatedEdges();
    QList<Edge*> previous_updatedEdges() { return previous_updated_edges; }
    Node *previousNode() { return previous_node; }

    // edges that are no longer used due to relaxation (due to updatedEdges taking their place at shortest path)
    QList<Edge*> retiredEdges() { return retired_edges; }
    QList<Edge*> previous_retiredEdges() { return previous_retired_edges; }

    QMap<Node*, int> dist() { return dist_; }
    QMap<Node*, Node*> pred() { return pred_; }
    QMultiMap<int, Node*> pq() { return pq_; }

    QList<int> pqKeysOfUpdatedNodes();

private:
    AdjacencyMap adjacency_map;
    Node *start_node;

    // Node::algo_weight (setAlgoWeight, getAlgoWeight) could be used instead of dist
    // (but idk if I should use it, I'll leave it for now the way it is)
    QMap<Node*, int> dist_;

    QMap<Node*, Node*> pred_;

    QMultiMap<int, Node*> pq_;
    //std::priority_queue<Node*> pq;

    Node *current_node;
    int current_dist;

    QList<Node*> expanded_nodes;

    QList<Node*> previous_updated_nodes;
    QList<Edge*> previous_updated_edges;
    QList<Edge*> retired_edges;
    QList<Edge*> previous_retired_edges;
    Node *previous_node;

    void clearLastIterationInfo();
};

#endif // CONTROLLEDDIJKSTRAALGORITHM_H
