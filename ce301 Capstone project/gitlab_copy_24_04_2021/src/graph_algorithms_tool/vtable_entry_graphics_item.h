#ifndef VTABLEENTRYGRAPHICSITEM_H
#define VTABLEENTRYGRAPHICSITEM_H

#include "vtable_entry_hover_text_item.h"

#include <QGraphicsItem>

#include <graph/graph.h>

class Node;
class Edge;
class LineDescriptionGraphicsItem;

class VtableEntryGraphicsItem : public QGraphicsTextItem
{
public:
    VtableEntryGraphicsItem(Node *key_node, int weight, Node *src_node, QGraphicsItem *parent = nullptr);
    virtual ~VtableEntryGraphicsItem() override;

    void set(Node *key_node, int weight, Node *src_node);

    Node *keyNode() { return key_node; }
    Node *srcNode() { return src_node; }
    int getWeight() { return weight; }

    /*
    void setKeyNodeColor(QString html_color);
    void setWeightColor(QString html_color);
    void setSrcNodeColor(QString html_color);
    */

    void drawDescriptionLine(QPainter *painter, Edge *edge, QString text);

    // QGraphicsItem interface
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //virtual QRectF boundingRect() const override;

    // QGraphicsItem interface
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    //bool is_hovered;

    Node *key_node;
    Node *src_node;
    int weight;

    Edge *previous_edge;

    VtableEntryHoverTextItem *description_item;

    // to ensure that node/edge still exists
    Graph *graph;

    void onHoverEnter();
    void onHoverLeave();

    //LineDescriptionGraphicsItem *line_description_item;
};

#endif // VTABLEENTRYGRAPHICSITEM_H
