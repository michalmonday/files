#ifndef CONTROLLEDKRUSKALSALGORITHM_H
#define CONTROLLEDKRUSKALSALGORITHM_H

#include "controlled_algorithm.h"

#include <QMultiMap>
#include <QQueue>
#include <QSet>

extern const int KRUSKALS_STATE_INIT;
extern const int KRUSKALS_STATE_EDGE_ADDED;
extern const int KRUSKALS_STATE_EDGE_DROPPED;

class ControlledKruskalsAlgorithm : public ControlledAlgorithm
{
public:
    ControlledKruskalsAlgorithm(Graph *graph);

    // ControlledAlgorithm interface
    virtual void reset() override;
    virtual void step() override;
    virtual QString name() override { return "Kruskals"; }
    virtual QString brief() override;
    virtual QString currentStepLabel() override;
    virtual QString currentStepDescription() override;
    virtual bool finished() override;

    bool wasEdgeAdded() { return was_edge_added; }
    bool wasEdgeDiscarded() { return !was_edge_added; }

    Edge *currentEdge() { return current_edge; }
    Edge *previousEdge() { return previous_edge; }

    Node *newRoot() { return new_root; }
    Node *retiredRoot() { return retired_root; }


    QList<QList<Edge*>> subGraphs() {
        QList<QList<Edge*>> list;
        for (Node *n : sub_graphs.keys())
            if (!sub_graphs[n].isEmpty())
                list.append(sub_graphs[n].values());
        return list;
    }

    QList<int> subgraphColorIds() {
        QList<int> ids;
        for (int i : color_ids.values())
            if (i != -1)
                ids << i;
        return ids;
    }

    QList<Edge*> mcst() { return MCST; }

    QList<Edge*> edgeQueue() { return edges.values(); }
private:
    AdjacencyMap adjacency_map;

    QMultiMap<int, Edge*> edges; // priority queue
    QMap<Node*, int> ranks;

    Edge *current_edge;
    Edge *previous_edge;

    bool was_edge_added;

    // for gaining insight:
    Node *new_root;
    Node *retired_root;

    QList<Edge*> MCST;
    int expected_MCST_size;

    // members that could really be local variables
    // but being members allows to generate descriptions
    // that include their values (to gain insight what is
    // going on)
    Node *n1; // these are 2 nodes at the end of current_edge
    Node *n2;
    Node *root_1; // and their corresponding root nodes
    Node *root_2;

    // it's like "getRootNodeOf(Node*)"
    Node *findSet(Node*);
    void unionSets(Node *base, Node *added);

    QMap<Node*, QSet<Edge*>> sub_graphs;
    QMap<Node*, int> color_ids;
    QQueue<int> available_color_ids;
};

#endif // CONTROLLEDKRUSKALSALGORITHM_H
