#ifndef GRAPH_H
#define GRAPH_H

//#include "graph/edge.h"
//#include "graph/node.h"

#include "movable_graphics_item.h"
#include "typedefs.h"

class Node;
class Edge;

class Graph : public MovableGraphicsItem
{
public:
    Graph(QGraphicsItem *parent = nullptr);
    QList<Node *> nodes(bool sorted=false) const;
    QList<Edge *> edges() const;

    AdjacencyList adjacencyList();
    AdjacencyMap adjacencyMap();
    AdjacencyMatrix adjacencyMatrix();

    Node * addNode();
    Node * addNode(QString label);
    Edge * addEdge(QString src_label, QString dst_label, int weight = -1);

    void removeNode(Node *);
    void removeEdge(Edge *);

    Node * selectedNode();
    QList<Node *> selectedNodes();

    Edge * selectedEdge();
    QList<Edge *> selectedEdges();

    Node *selectedNodeOrRandom();



    void resetColours();

    void deselect();

    void editable(bool is_editable);
    bool editable();

    static QString IdToLabel(int id_);
    static int LabelToId(QString label);

    void weightsShown(bool state);
    bool weightsShown() { return weights_shown; }

    bool has(Edge *edge);
    bool has(Node *node);

private:
    int highest_node_id;

    bool is_editable;

    bool weights_shown;

    /*bool is_hovered;
    bool is_moved;
    QPointF grabbed_offset;*/

    Node * getNodeForLabel(QString label);


    // QGraphicsItem interface
public:
    virtual QPainterPath shape() const override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // MovaableGraphicsItem interface
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GRAPH_H
