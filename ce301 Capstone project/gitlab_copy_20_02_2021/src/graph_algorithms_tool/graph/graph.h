#ifndef GRAPH_H
#define GRAPH_H

//#include "graph/edge.h"
//#include "graph/node.h"

#include "movable_graphics_item.h"
#include "typedefs.h"

class Node;
class Edge;
class Graph;

class NodeLabel;

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
    Node * addNode(const NodeLabel &label);
    Edge * addEdge(QString src_label, QString dst_label, int weight = -1);
    Edge * addEdgeByCloning(Edge *e);

    void removeNode(Node *);
    void removeEdge(Edge *);

    Node * selectedNode();
    QList<Node *> selectedNodes();

    Edge * selectedEdge();
    QList<Edge *> selectedEdges();

    Node *selectedNodeOrRandom();
    Edge *selectedEdgeOrRandom();



    void resetColours();

    void deselect();

    void editable(bool is_editable);
    bool editable();

    QMap<NodeLabel, bool> node_labels_map;

    static QString IdToLabel(int id_);
    static int LabelToId(QString label);
    static QString IncLabel(QString label);

    void weightsShown(bool state);
    bool weightsShown() { return weights_shown; }

    bool has(Edge *edge);
    bool has(Node *node);

    void printExportCode();

    Node * getNodeForLabel(QString label);

    QPixmap generatePixmap();

    Graph *clone(bool centered = false);

    QJsonObject toJson();
    static Graph * fromJson(const QJsonObject &obj);

    QString name() { return name_; }
    void name(const QString &n) { name_ = n; }

    void centerNodes();
    QPoint nodesOffsetFromCenter();

private:
    int highest_node_id;

    bool is_editable;

    bool weights_shown;

    QString name_;


    /*bool is_hovered;
    bool is_moved;
    QPointF grabbed_offset;*/




    // QGraphicsItem interface
public:
    virtual QPainterPath shape() const override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    // MovaableGraphicsItem interface
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

/* NodeLabel class was created for the sake of making QString sortable in QMap where:
a - 1st
b - 2nd
...
z - 25th
aa - 26th
ab - 27th
etc. */
class NodeLabel {
public:
    NodeLabel(QString str, int id_=-1) {
        q_string = str;
        if (id_==-1)
            id = Graph::LabelToId(q_string);
        else
            id = id_;
    }
    NodeLabel(int id_) {
        id = id_;
        q_string = Graph::IdToLabel(id_);
    }
    // copy constructor to avoid using "LabelToId" which is not efficient
    NodeLabel(const NodeLabel &nl) {
        q_string = nl.q_string;
        id = nl.id;
    }
    bool operator < (const NodeLabel &nl) const {
        return id < nl.id;
    }
    QString q_string;
    int id;
};

#endif // GRAPH_H
