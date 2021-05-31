#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"

#include <QGraphicsSceneMouseEvent>
#include <QtMath>

#include <algorithm>

Graph::Graph(QGraphicsItem *parent)
    : MovableGraphicsItem (parent),
      highest_node_id(0),
      is_editable(true),
      weights_shown(true)
{
    setZValue(3);
    setFlag(ItemIsFocusable);
    hasBorder(false);

    //setHandlesChildEvents(false);
    //setFlag(ItemSendsGeometryChanges);
}

QList<Node *> Graph::nodes(bool sorted) const
{
    QList<Node *> nodes;
    for (QGraphicsItem *item : childItems())
        if (Node *node = dynamic_cast<Node *>(item))
            nodes << node;
    if (sorted)
        std::sort(nodes.begin(), nodes.end(), [](Node*n, Node*n2){ return n->getLabel() < n2->getLabel(); });

    return nodes;
}

QList<Edge *> Graph::edges() const
{
    QList<Edge *> edges;
    for (QGraphicsItem *item : childItems())
        if (Edge *edge = dynamic_cast<Edge *>(item))
            edges << edge;
    return edges;
}

/*
adjacency_list = [
    (1st node, [all nodes connected to 1st node])
    (2nd node, [all nodes connected to 2nd node])
    (3rd node, [all nodes connected to 3rd node])
    ...
    ]

It doesn't contain weights (it contains pointers to nodes, so getting weights can be done easily anyway).
*/
//QList<QPair<Node *, QList<Node *>>> Graph::adjacencyList()
AdjacencyList Graph::adjacencyList()
{
    QList<QPair<Node *, QList<Node *>>> adjacency_list;
    for (Node *node : nodes())
        adjacency_list.append({node, node->adjacentNodes()});

    qDebug() << adjacency_list;
    return adjacency_list;
}

AdjacencyMap Graph::adjacencyMap()
{
    QMap<Node *, QList<Node *>> adjacency_map;
    for (Node *node : nodes())
        adjacency_map[node] = node->adjacentNodes();
    return adjacency_map;
}

//QList<QList<int>> Graph::adjacencyMatrix()
AdjacencyMatrix Graph::adjacencyMatrix()
{
    QList<QList<int>> adjacency_matrix;

    const QList<Node *> all_nodes = nodes();
    const int nodes_size = all_nodes.size();

    for (int i = 0; i < nodes_size; i++) {
        Node *node = all_nodes[i];
        QList<int> row;
        for (Node *adjacent_node : all_nodes) {
            Edge *edge = node->edgeWith(adjacent_node);
            row << ((adjacent_node == node || edge == nullptr) ? 0 : edge->getWeight() );
        }
        adjacency_matrix << row;
    }

    qDebug() << adjacency_matrix;
    return adjacency_matrix;
}

Node *Graph::addNode()
{
    return addNode(IdToLabel(++highest_node_id));
}

Node * Graph::addNode(QString label)
{
    return getNodeForLabel(label);
}

Edge * Graph::addEdge(QString src_label, QString dst_label, int weight)
{
    Edge *edge = new Edge(getNodeForLabel(src_label), getNodeForLabel(dst_label), weight);
    edge->setParentItem(this);
    edge->weightVisible(weights_shown);
    return edge;
}

void Graph::removeNode(Node *node)
{
    // destructor of Node handles deleting appropriate Edges
    // destructor of Edge handles deleting its' reference in both Nodes (source and destination)
    if (childItems().contains(node)) {
        QList<Edge *> edges = node->edges();
        node->setParentItem(nullptr);
        delete node;
    }
    prepareGeometryChange();
}

void Graph::removeEdge(Edge *edge)
{
    Node * src_node = edge->sourceNode();
    Node * dst_node = edge->destNode();

    if (childItems().contains(edge)) {
        edge->setParentItem(nullptr);
        delete edge;

        // destructor of Edge already removed it from edge list of src_node and dst_node

        if (src_node->edges().isEmpty())
            removeNode(src_node);

        if (dst_node->edges().isEmpty())
            removeNode(dst_node);
    }
}

Node *Graph::selectedNode()
{
    for (Node *n : nodes())
        if (n->isSelected())
            return n;
    return nullptr;
}

QList<Node *> Graph::selectedNodes()
{
    QList<Node*> nodes_;
    for (Node *n : nodes())
        if (n->isSelected())
            nodes_.append(n);
    return nodes_;
}

Edge *Graph::selectedEdge()
{
    for (Edge *e : edges())
        if (e->isSelected())
            return e;
    return nullptr;
}

QList<Edge *> Graph::selectedEdges()
{
    QList<Edge *> edges_;
    for (Edge *e : edges())
        if (e->isSelected())
            edges_.append(e);
    return edges_;
}

Node *Graph::selectedNodeOrRandom()
{
    if (Node * n = selectedNode())
        return n;

    QList<Node*> nodes_ = nodes();
    return  nodes_[rand() % nodes_.size()];
}

void Graph::resetColours()
{
    for (Node *n : nodes())
        n->resetStyling();

    for (Edge *e : edges())
        e->resetStyling();
}

void Graph::deselect()
{
    setSelected(false);
    for (Node *n : selectedNodes())
        n->setSelected(false);

    for (Edge *e : selectedEdges())
        e->setSelected(false);
}

void Graph::editable(bool is_editable_)
{
    is_editable = is_editable_;

    for (Edge *edge : edges()) {
        Qt::TextInteractionFlags flags = edge->getLabelItem()->textInteractionFlags();

        if (is_editable)
            flags |= Qt::TextInteractionFlag::TextEditable;
        else
            flags &= ~Qt::TextInteractionFlag::TextEditable;

        edge->getLabelItem()->setTextInteractionFlags(flags);
    }

}

bool Graph::editable()
{
    return is_editable;
}


// based on this answer: https://stackoverflow.com/a/55655445/4620679
// from this question:   https://stackoverflow.com/questions/55655137/how-do-i-create-a-dynamic-alphabet-id-system
QString Graph::IdToLabel(int id_)
{
    static const char * alphabet = "abcdefghijklmnopqrstuvwxyz";
    static const int alphabet_sz = 26;
    if (id_ < alphabet_sz)
        return QString().sprintf("%c", alphabet[id_]);
    return IdToLabel(qFloor(id_ / alphabet_sz) - 1) + IdToLabel(id_ % alphabet_sz);
}

int Graph::LabelToId(QString label_)
{
    int sum = 0;
    for (QChar c : label_) {
        sum += c.toLatin1() - 'a';
    }
    return sum;
}

void Graph::weightsShown(bool state)
{
    weights_shown = state;
    for (Edge *e : edges()) {
        e->weightVisible(weights_shown);
    }
}

bool Graph::has(Edge *e)
{
    if (!e)
        return false;
    for (QGraphicsItem *item : childItems())
        if (e == dynamic_cast<Edge *>(item))
            return true;
    return false;
}

bool Graph::has(Node *n)
{
    if (!n)
        return false;
    for (QGraphicsItem *item : childItems())
        if (n == dynamic_cast<Node *>(item))
            return true;
    return false;
}



Node * Graph::getNodeForLabel(QString label) {
    foreach (QGraphicsItem *item, childItems()) {
        if (Node * node = dynamic_cast<Node*>(item)) {
        //if (Node * node = dynamic_cast<Node*>(item)) {
            if (node->getLabel().compare(label) == 0) {
                return node;
            }
        }
    }
    Node *new_node = new Node(label);
    new_node->setParentItem(this);

    int node_id = LabelToId(label);
    if (node_id > highest_node_id)
        highest_node_id = node_id;
    return new_node;
}


void Graph::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    while (Node *selected_node = selectedNode())
        selected_node->setSelected(false);

    while (Edge *selected_edge = selectedEdge())
        selectedEdge()->setSelected(false);

    MovableGraphicsItem::mousePressEvent(event);
}



QPainterPath Graph::shape() const
{
    QPainterPath path;
    //for (QGraphicsItem *item : childItems())
    //    path.addPath(item->shape());
    path.addRect(boundingRect());
    return path;
}




void Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    MovableGraphicsItem::paint(painter, option, widget);
}


