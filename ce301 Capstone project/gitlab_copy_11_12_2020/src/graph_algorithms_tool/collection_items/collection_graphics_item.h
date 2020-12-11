#ifndef COLLECTIONGRAPHICSITEM_H
#define COLLECTIONGRAPHICSITEM_H

#include <QGraphicsItem>
#include "dock.h"

class Node;

class CollectionGraphicsItem :  public QGraphicsItem, virtual public Dockable, public QObject
{
public:
    CollectionGraphicsItem(QGraphicsItem * parent = nullptr);

    void init(QString dock_title = "Collection");

    virtual void reset();

    // subclasses may implement QQueue, QMultimap, QStack, but ultimately
    // they will all provide a QList, so their order can be recognized
    // and painted by this abstract class
    virtual QList<Node *> nodesCollection() const = 0;
    virtual QList<Edge *> edgesCollection() const = 0;

    // different subclasses will insert Node using differrent way
    // (need to think about it more)
    // (maybe this isn't a good idea)
    // maybe it abstract class should insert items into:
    //     - ellipse_items
    //     - text_items
    // and derived classes inser into their own collections
    //virtual void insert(Node *n, int weight = 0) = 0;

    // each subclass will implement a simple method that returns string like "PriorityQueue"
    virtual QString collectionName() const = 0;
    virtual QString frontText() const = 0;
    virtual QString backText() const = 0;

    // QGraphicsItem interface
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;

    // this will make the bounding rect bigger even if there are 0 elements
    // so the scale will not drastically "jump" as the queue gets filled
    void setExpectedMaxSize(int s) { expected_max_size = s; }

    void adjustToLeft();

    void clearHighlight() { nodes_to_highlight.clear(); }

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

    QList<Node *> nodes_to_highlight;
    QList<Edge *> edges_to_highlight;
    QList<int> pq_weights;

private:
    QMap<Node *, QGraphicsEllipseItem *> ellipse_items;
    QMap<Node *, QGraphicsTextItem *> text_items;

    QPointF front_text_size;
    QPointF back_text_size;

    int expected_max_size;

    QString front_text;
    QString back_text;


};

#endif // COLLECTIONGRAPHICSITEM_H
