#include "controlled_kruskals_algorithm.h"
#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "colors.h"
#include "functions.h"

const int KRUSKALS_STATE_INIT = 1;
const int KRUSKALS_STATE_EDGE_ADDED = 2;
const int KRUSKALS_STATE_EDGE_DROPPED = 3;

/*const int KRUSKALS_STATE_EDGE_SELECTED = 2;
const int KRUSKALS_STATE_EDGE_ADDED_OR_DROPPED = 3;
const int KRUSKALS_STATE_FINISHED = 4;*/

ControlledKruskalsAlgorithm::ControlledKruskalsAlgorithm(Graph *g)
    : ControlledAlgorithm(g)
{
    reset();

}

void ControlledKruskalsAlgorithm::reset()
{
    ControlledAlgorithm::reset();

    adjacency_map = graph_->adjacencyMap();
    edges.clear();
    ranks.clear();

    for (QSet<Edge*> &list : sub_graphs.values())
        list.clear();
    sub_graphs.clear();
    color_ids.clear();
    available_color_ids.clear();

    int i=0;
    for (Node *n : graph_->nodes()) {
        V[n] = {0, n};
        ranks[n] = 0;
        sub_graphs[n] = QSet<Edge*>();
        color_ids[n] = -1;
        available_color_ids.enqueue(i++);
    }

    for (Edge *e : graph_->edges())
        edges.insert(e->getWeight(), e);

    previous_edge = nullptr;
    current_edge = nullptr;

    state = KRUSKALS_STATE_INIT;
    previous_state = KRUSKALS_STATE_INIT;

    new_root = nullptr;
    MCST.clear();
    expected_MCST_size = graph()->nodes().count() - 1;
}

void ControlledKruskalsAlgorithm::step()
{
    previous_edge = current_edge;
    current_edge = edges.first();
    edges.remove(edges.firstKey(), current_edge);

    n1 = current_edge->sourceNode();
    n2 = current_edge->destNode();

    root_1 = findSet(n1);
    root_2 = findSet(n2);

    if (root_1 != root_2) {
        // this conditional isn't required for algorithm to work
        // but it makes it visually more pleasing and therefore
        // easier to understand (because nodes that were not
        // part of any subgraph before will not become new roots
        // and possibly confuse the student)
        if (ranks[root_1] < ranks[root_2]) {
            unionSets(root_2, root_1);
        } else if (ranks[root_1] > ranks[root_2]) {
            unionSets(root_1, root_2);
        } else {
            unionSets(root_1, root_2);
            ranks[root_1] += 1;
            V[root_1].first += 1;
        }

        was_edge_added = true;
        MCST << current_edge;
        setState(KRUSKALS_STATE_EDGE_ADDED);
    } else {
        was_edge_added = false;
        new_root = nullptr;
        retired_root = nullptr;
        setState(KRUSKALS_STATE_EDGE_DROPPED);
    }
}

QString ControlledKruskalsAlgorithm::brief()
{
    return "Kruskals algorithm finds minimum cost spanning tree (MST). "
           "It traverses the graph focusing on edges. First it sorts the edges "
           "(e.g. by inserting them to priority queue). Then it considers adding "
           "the lowest cost edge from that priority queue to MST, it only does so "
           "if adding the node does not create a cycle within sub-graph."
           "<br><br>"
           "Nodes could be treated as buildings/households within neighborhood, "
           "edge weights could be treated as distances between these households, "
           "in such scenario MST can be used to find minimum length of hydraulic pipes, "
           "or electronic wires, to supply all households with water or electricity using "
           "minimum total length of wires/pipes. "
            ;
}

QString ControlledKruskalsAlgorithm::currentStepLabel()
{
    if (state == KRUSKALS_STATE_EDGE_ADDED)
        return QString("Edge '%1' (%2) was added.")
                .arg(current_edge->nodeLabels())
                .arg(current_edge->getWeight());

    if (state == KRUSKALS_STATE_EDGE_DROPPED)
        return QString("Edge '%1' (%2) was dropped.")
                .arg(current_edge->nodeLabels())
                .arg(current_edge->getWeight());

    return "this shouldn't happen";
}

QString ControlledKruskalsAlgorithm::currentStepDescription()
{
    QString description = "";
    if (state == KRUSKALS_STATE_EDGE_ADDED)
        description = QString("Edge '%1' (weight=%2) was added to minimum cost spanning tree. "
                              "It was chosen becaues it had the lowest weight of all remaining edges.<br><br>"
                              "Using findSet method on both nodes and comparing the result allowed to "
                              "verify that adding this edge wouldn't result in a cycle, because the "
                              "root of node '%3' (root='%5') and '%4' (root='%6') differs.<br><br>"
                              "Joining of 2 graphs was performed by simply overwriting root "
                              "node of '%7'.")
                .arg(current_edge->nodeLabels())
                .arg(current_edge->getWeight())
                .arg(n1->getLabel())
                .arg(n2->getLabel())
                .arg(root_1 == n1 ? n1->getLabel() + " itself" : root_1->colouredLabel())
                .arg(root_2 == n2 ? n2->getLabel() + " itself" : root_2->colouredLabel())
                .arg(retired_root->getLabel());

    if (finished())
        description += "<br><br>" + colouredHtml("The minimum cost spanning tree is completed.", COLOR_MCST);

    if (state == KRUSKALS_STATE_EDGE_DROPPED)
        description = QString("Edge '%1' (weight=%2) was dropped. That is because adding it would create a cycle."
                              " Node '%3' and node '%4' happen to have the same root ('%5').")
                .arg(current_edge->nodeLabels())
                .arg(current_edge->getWeight())
                .arg(n1->getLabel())
                .arg(n2->getLabel())
                .arg(root_1->colouredLabel());

    return description;
}

bool ControlledKruskalsAlgorithm::finished()
{
    return MCST.count() == expected_MCST_size;
}

// it's like "getRootNodeOf(Node*)"
Node *ControlledKruskalsAlgorithm::findSet(Node *n)
{
    while (V[n].second != n)
        n = V[n].second;
    return n;
}

void ControlledKruskalsAlgorithm::unionSets(Node *base, Node *added)
{
    sub_graphs[base].insert(current_edge);
    if (color_ids[base] == -1)
        color_ids[base] = available_color_ids.dequeue();

    if (color_ids[added] != -1) {
        // reuse the colour id
        available_color_ids.prepend(color_ids[added]);
        color_ids[added] = -1;
    }

    V[added].second = base;
    new_root = base;
    retired_root = added;
    sub_graphs[base] += sub_graphs[added];
    sub_graphs[added].clear();
    qDebug() << "sub_graphs size = " << subGraphs().size();
}
