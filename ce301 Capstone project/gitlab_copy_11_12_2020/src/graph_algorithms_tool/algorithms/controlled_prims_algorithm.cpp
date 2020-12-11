#include "algorithms/controlled_prims_algorithm.h"

#include "graph/node.h"
#include "graph/edge.h"

#include "colors.h"
#include "functions.h"


const int PRIMS_STATE_INIT = 1;
const int PRIMS_STATE_NEW_NODE_ASSIGNED      = 2;
const int PRIMS_STATE_NEW_NODE_INCLUDED      = 3;
const int PRIMS_STATE_ADJACENT_NODES_UPDATED = 4;


//void ControlledPrimsAlgorithm::includedNodes();
//void ControlledPrimsAlgorithm::notIncludedNodes() {}


/* Operational methods (that process the data) */

ControlledPrimsAlgorithm::ControlledPrimsAlgorithm(Graph *g)
    : ControlledAlgorithm(g)
{
    // map implementation isn't as efficient but doesn't require nodes to be labelled with numbers used as indices
    // It may be better for understanding the principle of Prims algorithm.
    // It may be worse for understanding how to implement it efficiently.


    reset();

    /*
    qDebug() << "Adjacency map:";
    for (Node *node : sorted_nodes) {
        QString adjacent_nodes = "[";
        for (Node *adj_node : adjacency_map[node]) {
            adjacent_nodes += adj_node->getLabel() + (adj_node == adjacency_map[node].last() ? "" : ", ");
        }
        adjacent_nodes += "]";
        qDebug() << node->getLabel() << " : " << adjacent_nodes;
    }*/

    /*while(!finished()) {
        includeNode();
        updateAdjacentNodesSurroundingIncludedOne();
        assignNewNodeToInclude();
    }*/


    /*
    qDebug() << "<br><br><br>MCST =";
    for (Edge *edge : graph->edges())
        edge->setColor(Qt::black);

    for (Edge *edge : MCST) {
        qDebug() << "\t" << edge->sourceNode()->getLabel() << " - " << edge->getWeight() << " - " << edge->destNode()->getLabel();
        edge->setColor(Qt::green);
    }
    if (QGraphicsScene *scene = graph->scene())
        scene->update();
    */
}

void ControlledPrimsAlgorithm::includeNode() {
    qDebug() << "ControlledPrimsAlgorithm::includeNode()";

    MCST_nodes << included_node;
    included_node_previous_weight = V[included_node].first;
    V[included_node].first = 0;

    /*
    adjacent_nodes_needed_updating.clear();
    for (Node *adj_node : adjacency_map[included_node]) {
        if (V[adj_node].first == 0)
            continue;
        adjacent_nodes_needed_updating << adj_node;
    }*/
    adjacent_nodes_needed_updating = adjacency_map[included_node];

    // nodes_affected_during_last_iteration is useful for highlighting changes in V table
    updated_nodes.clear();
    updated_nodes << included_node;

    setState(PRIMS_STATE_NEW_NODE_INCLUDED);
}

void ControlledPrimsAlgorithm::updateSingleAdjacentNodeSurroundingIncludedOne() {
    Node *adj_node = adjacent_nodes_needed_updating.first();
    adjacent_nodes_needed_updating.removeFirst();

    if (V[adj_node].first == 0) {
        // do nothing because that adjacent node was added to MCST already
        last_updated_adjacent_node_type = PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_ALREADY_INCLUDED;
    } else {
        int weight = included_node->edgeWith(adj_node)->getWeight();
        int current_min_weight = V[adj_node].first;
        if (weight < current_min_weight) {
            // edge between the adjacent node and previously included node (to MCST) seems optimal at this point
            V[adj_node] = {weight, included_node};
            updated_nodes << adj_node;
            last_updated_adjacent_node_type = PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_OPTIMAL;
        } else {
            // edge between the adjacent node and previously included node (to MCST) seems not worth making
            last_updated_adjacent_node_type = PRIMS_LAST_UPDATED_ADJACENT_NODE_TYPE_BAD;
        }
    }

    last_updated_adjacent_node = adj_node;
    last_updated_adjacent_edge = adj_node->edgeWith(included_node);

    if (adjacent_nodes_needed_updating.isEmpty())
        setState(PRIMS_STATE_ADJACENT_NODES_UPDATED);
}

void ControlledPrimsAlgorithm::updateAdjacentNodesSurroundingIncludedOne() {
    qDebug() << "ControlledPrimsAlgorithm::updateAdjacentNodesSurroundingIncludedOne()";

    updated_nodes.clear();
    previous_retired_edges = retired_edges;
    retired_edges.clear();
    //last_iteration_nodes_with_replaced_optimal_edges.clear();

    for (Node *adj_node : adjacency_map[included_node]) {
        if (V[adj_node].first == 0)
            continue;
        int weight = included_node->edgeWith(adj_node)->getWeight();
        int current_min_weight = V[adj_node].first;
        if (weight < current_min_weight) {
            // add to unused_edges if there was already value lower than INT_MAX
            if (current_min_weight != INT_MAX) {
                Edge *unused_edge = adj_node->edgeWith(V[adj_node].second);
                all_retired_edges << unused_edge;
                retired_edges << unused_edge;
                unused_edge->setToolTip("This edge became retired because the new considered '" + included_node->getLabel() + "' and '" + adj_node->getLabel() + "' edge has lower weight (" + QString::number(weight) + ")");

                //last_iteration_nodes_with_replaced_optimal_edges << adj_node;
            }

            V[adj_node] = {weight, included_node};
            updated_nodes << adj_node;


        } else {
            Edge *unused_edge = included_node->edgeWith(adj_node);
            all_retired_edges << unused_edge;
            unused_edge->setToolTip("This edge became retired because it has higher weight than already considered '" + adj_node->getLabel() + "' and '" + V[adj_node].second->getLabel() + "' edge weight (" + QString::number(current_min_weight) + ")");

            retired_edges << unused_edge;
        }
    }
    setState(PRIMS_STATE_ADJACENT_NODES_UPDATED);
}

void ControlledPrimsAlgorithm::assignNewNodeToInclude() {
    qDebug() << "ControlledPrimsAlgorithm::assignNewNodeToInclude()";

    previous_potential_surrounding_edges = potentialSurroundingEdges();
    previous_potential_surrounding_nodes = potentialSurroundingNodes();

    previous_retired_edges = retired_edges;
    retired_edges.clear();
    //last_iteration_nodes_with_replaced_optimal_edges.clear();

    Node *best_node = minVentry(V);
    Edge *new_mcst_edge = V[best_node].second->edgeWith(best_node);

    MCST_edges << new_mcst_edge;
    //V[best_node] = {0, nullptr};
    included_node = best_node;

    // update unused_edges
    for (Node *adj_node : adjacency_map[included_node]) {
        Edge *edge = included_node->edgeWith(adj_node);
        if (MCST_nodes.contains(adj_node) && !MCST_edges.contains(edge))
            if (!all_retired_edges.contains(edge))
                all_retired_edges << edge;
    }

    setState(PRIMS_STATE_NEW_NODE_ASSIGNED);
}


/*  Methods that allow getting insight into what's going on:  */

QList<Node *> ControlledPrimsAlgorithm::potentialSurroundingNodes() {
    QList<Node *> potential_nodes;
    for (Node *node : adjacency_map[included_node])
        if (V[node].first != 0)
            potential_nodes << node;
    return potential_nodes;
}

/* Potential nodes include even those nodes that already have lowest possible V table entry. */
QList<Edge *> ControlledPrimsAlgorithm::potentialSurroundingEdges() {
    QList<Edge *> potential_edges;
    for (Node *node : adjacency_map[included_node])
        if (V[node].first != 0)
            potential_edges << included_node->edgeWith(node);
    return potential_edges;
}

Node *ControlledPrimsAlgorithm::includedNode()
{
    return included_node;
}

Edge *ControlledPrimsAlgorithm::includedEdge()
{
    return MCST_edges.last();
}

QList<Edge *> ControlledPrimsAlgorithm::allRetiredEdges() {
    return all_retired_edges;
    /*
    QList<Edge *> edges;
    for (Node *n : MCST_nodes)
        for (Node *n2 : MCST_nodes)
            if (Edge *edge = n->edgeWith(n2))
                if (!MCST_edges.contains(edge))
                    edges << edge;
        //if (!MCST_edges.contains(edge) && MCST_nodes.contains(edge->sourceNode()) && MCST_nodes.contains(edge->sourceNode()))
          //  edges << edge;
    return edges;
    */
}

QList<Edge *> ControlledPrimsAlgorithm::retiredEdges()
{
    return retired_edges;
}

QList<Edge *> ControlledPrimsAlgorithm::previousRetiredEdges()
{
    return previous_retired_edges;
}

/*
QList<Node *> ControlledPrimsAlgorithm::lastIterationNodesWithReplacedOptimalEdges()
{
    return last_iteration_nodes_with_replaced_optimal_edges;
}*/

int ControlledPrimsAlgorithm::lastUpdatedAdjacentNodeType()
{
    return last_updated_adjacent_node_type;
}

Node *ControlledPrimsAlgorithm::lastUpdatedAdjacentNode()
{
    return last_updated_adjacent_node;
}

Edge *ControlledPrimsAlgorithm::lastUpdatedAdjacentEdge()
{
    return last_updated_adjacent_edge;
}

/*QList<Node *> ControlledPrimsAlgorithm::updatedNodes()
{
    return updated_nodes;
}*/


Node * ControlledPrimsAlgorithm::minVentry(const Vtable &V) {
    int min = INT_MAX;
    Node *returned_node = nullptr;
    for (Node *node : V.keys()) {
        int weight = V[node].first;
        if (weight > 0 && weight < min) {
            min = weight;
            returned_node = node;
        }
    }
    return returned_node;
}


/*void ControlledPrimsAlgorithm::update()
{

}*/


void ControlledPrimsAlgorithm::reset()
{
    ControlledAlgorithm::reset();

    state = PRIMS_STATE_INIT;
    previous_state = PRIMS_STATE_INIT;
    adjacency_map = graph_->adjacencyMap();

    // sorted nodes will be useful for neatly displaying explanations and state of adjacency map
    sorted_nodes = graph_->nodes(/*sorted*/ true);

    /*  Vtable V contains 1 entry for each node.  */
    for (Node *node : sorted_nodes)
        V[node] = {INT_MAX,nullptr};

    // first node can be chosen randomly (because all vertices will be connected in MCST)
    // use selected node if there's one (else use random node)
    included_node = graph_->selectedNode() ? graph_->selectedNode() : sorted_nodes[rand() % sorted_nodes.size()];

    adjacent_nodes_needed_updating.clear();
    last_updated_adjacent_node = nullptr;
    last_updated_adjacent_edge = nullptr;
    all_retired_edges.clear();
    retired_edges.clear();
    //last_iteration_nodes_with_replaced_optimal_edges.clear();
    updated_nodes.clear();

    previous_potential_surrounding_edges.clear();
    previous_potential_surrounding_nodes.clear();

    MCST_edges.clear();
    MCST_nodes.clear();
}


void ControlledPrimsAlgorithm::step()
{

    if (state == PRIMS_STATE_NEW_NODE_ASSIGNED || state == PRIMS_STATE_INIT)
        includeNode();
    else if (state == PRIMS_STATE_NEW_NODE_INCLUDED)
        updateAdjacentNodesSurroundingIncludedOne();
    else if (state == PRIMS_STATE_ADJACENT_NODES_UPDATED)
        assignNewNodeToInclude();
}

QString ControlledPrimsAlgorithm::brief()
{
    return "Prims algorithm finds minimum cost spanning tree (MST). "
           "It traverses the graph focusing on nodes. It selects a random one first, "
           "and then adds nodes based on closest distance to any previously added node."
           "<br><br>"
           "Nodes could be treated as buildings/households within neighborhood, "
           "edge weights could be treated as distances between these households, "
           "in such scenario MST can be used to find minimum length of hydraulic pipes, "
           "or electronic wires, to supply all households with water or electricity using "
           "minimum total length of wires/pipes. "
            ;
}

QString ControlledPrimsAlgorithm::currentStepLabel()
{
    if (previous_state == PRIMS_STATE_INIT)
        return QString("Random node '%1' added to MST").arg(included_node->getLabel());

    if (state == PRIMS_STATE_ADJACENT_NODES_UPDATED)
        return QString("Adjacent nodes of '%1' updated").arg(included_node->getLabel());

    if (state == PRIMS_STATE_NEW_NODE_INCLUDED)
        return QString("'%1' added to MST").arg(included_node->getLabel());

    return "unrecognized state";
}

QString ControlledPrimsAlgorithm::currentStepDescription()
{
    QString description;
    if (previous_state == PRIMS_STATE_INIT) {
        return QString("Random node (%1) was selected.<br><br>"
                              "It doesn't really matter which node is "
                              "selected first because in the end every "
                              "node will be connected together.").arg(included_node->colouredLabel());
    }

    if (state == PRIMS_STATE_ADJACENT_NODES_UPDATED) {
        QList<Node *> potential_surrounding_nodes = potentialSurroundingNodes();
        QList<Edge *> potential_surrounding_edges = potentialSurroundingEdges();

        Node *included_node = includedNode();
        QString potential_surrounding_nodes_str;
        QString potential_surrounding_edges_str;

        bool better_edges_found = false;
        bool not_better_edges_found = false;
        for (Node *node : potential_surrounding_nodes) {
            potential_surrounding_nodes_str += node->colouredLabel() + (node == potential_surrounding_nodes.last() ? "" : ", ");

            Edge *edge = node->edgeWith(included_node);
            if (edge->getWeight() < previousV[node].first) {
                better_edges_found = true;
                potential_surrounding_edges_str += colouredHtml(
                            QString("Edge '%1' has weight '%2' which is lower than the current V table value of '%3' node ('%4'), so V[%3] entry is updated.<br>")
                                    .arg(edge->nodeLabels())     // %1
                                    .arg(edge->getWeight())      // %2
                                    .arg(node->getLabel())       // %3
                                    .arg(previousV[node].first)  // %4
                            , COLOR_CONSIDERED);
            } else {
                not_better_edges_found = true;
                potential_surrounding_edges_str += colouredHtml(
                            QString("Edge '%1' has weight '%2' which is higher (or the same) as the current V table value of '%3' node ('%4'), V[%3] entry is not updated.<br>")
                                    .arg(edge->nodeLabels())    // %1
                                    .arg(edge->getWeight())     // %2
                                    .arg(node->getLabel())      // %3
                                    .arg(previousV[node].first) // %4
                            , COLOR_RETIRED);
            }
        }

        description = QString("In the previous step, node '%1' was included in MST. "
                                      "If we take a look at the adjacency list, "
                                      "we can notice that adjacent nodes of '%1' are:<br>'%2'<br><br>")
                              .arg( included_node->colouredLabel(),
                                    included_node->adjacentNodesAsString());

        if (potential_surrounding_nodes_str.isEmpty()) {
            description += QString("All nodes surrounding node '%1' are already part of MST.<br>"
                                   "So their values are not going to be updated in V table.<br><br>")
                                    .arg( included_node->colouredLabel() );
        } else {
            description += QString("The following of above nodes are not yet included in MST:<br>'%2'<br><br>"
                                   "So their optimal edges possibly will be updated in the 'V table', "
                                   "however that will happen only if the edge weight between node '%1' and adjacent node "
                                   "is lower than the current weight in 'V table'.<br><br>")
                                    .arg( included_node->colouredLabel() ,
                                         potential_surrounding_nodes_str);
        }

        /*
        if (better_edges_found) {
            description += "The following adjacent nodes will have their V table values updated:<br>" + potential_surrounding_edges_str;
        } else {
            //description += "None of the adjacent nodes will be updated.<br>";
        }

        if (not_better_edges_found) {
            description += "<br><br>The following adjacent nodes will not have their V table values updated:<br>" + ;
        }*/

        description += potential_surrounding_edges_str;
    }

    if (state == PRIMS_STATE_NEW_NODE_INCLUDED) {
        description = QString("'%1' was added to MST because "
                              "its value in the V table was the lowest positive value ('%2').")
                              .arg( included_node->colouredLabel() )
                              .arg(colouredHtml( QString::number(included_node_previous_weight), COLOR_CONSIDERED) ); // %1
    }

    return description;
}



bool ControlledPrimsAlgorithm::finished()
{
    return MCST_edges.size() == graph_->nodes().size() - 1;
}

const QList<Edge *> & ControlledPrimsAlgorithm::mcstEdges()
{
    return MCST_edges;
}

const QList<Node *> & ControlledPrimsAlgorithm::mcstNodes()
{
    return MCST_nodes;
}

void ControlledPrimsAlgorithm::setState(int state)
{
    ControlledAlgorithm::setState(state);

    if (state == PRIMS_STATE_NEW_NODE_INCLUDED)
        previousV = V;
}
