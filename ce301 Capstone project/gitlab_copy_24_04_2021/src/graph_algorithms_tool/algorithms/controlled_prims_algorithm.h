#ifndef CONTROLLEDPRIMSALGORITHM_H
#define CONTROLLEDPRIMSALGORITHM_H

#include "algorithms/controlled_algorithm.h"
#include <QList>

#include "graph/graph.h"
#include "typedefs.h"

class Graph;
class Node;


extern const int PRIMS_STATE_INIT;
extern const int PRIMS_STATE_NEW_NODE_ASSIGNED;
extern const int PRIMS_STATE_NEW_NODE_INCLUDED;
extern const int PRIMS_STATE_ADJACENT_NODES_UPDATED;

const int PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_ALREADY_INCLUDED = 1; // node with 0 in V table
const int PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_OPTIMAL = 2;
const int PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_BAD = 3;


/*
struct PrimsPqEntry {
    Node * node;
    int weight;

    bool operator < (const PrimsPqEntry &other) { return weight < other.weight; }
};*/

class ControlledPrimsAlgorithm : public ControlledAlgorithm
{
public:
    ControlledPrimsAlgorithm(Graph *graph);

    //void updateGraph(Graph *graph = nullptr);

    // methods to operate algorithm (to process it)
    void includeNode();
    void updateSingleAdjacentNodeSurroundingIncludedOne();
    void updateAdjacentNodesSurroundingIncludedOne();
    void assignNewNodeToInclude();

    //there's also "finished()" method in "controlled_algorithm.h/.cpp"

    // methods to gain insight into what's currently going on
    QList<Node *> potentialSurroundingNodes();
    QList<Edge *> potentialSurroundingEdges();
    QList<Node *> previousPotentialSurroundingNodes() { return previous_potential_surrounding_nodes; }
    QList<Edge *> previousPotentialSurroundingEdges() { return previous_potential_surrounding_edges; }

    Node *includedNode();
    Edge *includedEdge();
    QList<Edge *> allRetiredEdges();
    QList<Edge *> retiredEdges();
    QList<Edge *> previousRetiredEdges();
    //QList<Node *> lastIterationNodesWithReplacedOptimalEdges();



    //there's also getMCST in "controlled_algorithm.h/.cpp"
    int lastUpdatedAdjacentNodeType();
    Node *lastUpdatedAdjacentNode();
    Edge *lastUpdatedAdjacentEdge();


    // ControlledAlgorithm interface
    virtual void reset() override;
    virtual void step() override;
    virtual QString name() override { return "Prims"; }
    virtual QString brief() override;
    virtual QString currentStepLabel() override;
    virtual QString currentStepDescription() override;

    virtual bool finished() override;
    const QList<Edge *> & mcstEdges();
    const QList<Node *> & mcstNodes();

protected:
    virtual void setState(int state) override;


private:
    QList<Edge *> MCST_edges;
    QList<Node *> MCST_nodes;

    /*
    Why there's so many members and methods of this class?

    Because I'm not sure if design of the program should show the user:
        1. All adjacent nodes being updated all at once in V table (following a single "step"/key press).
        2. Only a single node getting updated at once in V table (following a single "step"/key press).

    I think that using 2nd option may be too much and could cause more confusion than understanding.
    (I'm not sure about it)
    But some members and methods of this class allow to gain insight into single node being updated,
    these members are:
        - QList<Node *> adjacent_nodes_needed_updating; (first item of it, is removed from it after each updateSingleAdjacentNodeSurroundingIncludedOne() call )
        - Node *last_updated_adjacent_node;
        - Edge *last_updated_adjacent_edge;

    And methods used to implement 2nd option are:
        - int lastUpdatedAdjacentNodeType();
        - Node *lastUpdatedAdjacentNode();
        - Edge *lastUpdatedAdjacentEdge();

    If I choose later that I won'd implement that 2nd option then possibly all of these methods/members could be removed.
    */


    AdjacencyMap adjacency_map;
    QList<Node *> sorted_nodes;

    Vtable previousV;

    // Node that supposed to be (or just was) included in MCST
    Node *included_node;

    /* example states:
            PRIMS_STEP_INIT;
            PRIMS_STEP_NEW_NODE_ASSIGNED;
            PRIMS_STEP_NEW_NODE_INCLUDED;
            PRIMS_STEP_ADJACENT_NODES_UPDATED; */


    // Nodes surrounding the node that was just added to MCST
    QList<Node *> adjacent_nodes_needed_updating;

    Node *last_updated_adjacent_node;
    Edge *last_updated_adjacent_edge;

    /* Example types:
            PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_ALREADY_INCLUDED
            PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_OPTIMAL
            RIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_BAD */
    int last_updated_adjacent_node_type;

    // Retired edges are edges not considered anymore
    QList<Edge *> all_retired_edges;

    // Keeping a list of edges that became not considered anymore after last iteration will help to
    // highlight when that happens and why
    QList<Edge*> retired_edges;
    QList<Edge*> previous_retired_edges;

    // This can be used to highlight when V table entry value previously set to some weight
    // (that isn't INT_MAX) got replaced. Highlighting when it occurs will help to understand algorithm.
    //QList<Node*> last_iteration_nodes_with_replaced_optimal_edges;

    // these 2 lists are saved for the sake of distinguishing nodes supposed to be updated in current iteration
    // (using violet color)
    QList<Node *> previous_potential_surrounding_nodes;
    QList<Edge *> previous_potential_surrounding_edges;

    int included_node_previous_weight;


    Node *minVentry(const Vtable &V);


};

#endif // CONTROLLEDPRIMSALGORITHM_H
