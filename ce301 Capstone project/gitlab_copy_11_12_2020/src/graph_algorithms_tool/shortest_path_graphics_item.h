#ifndef SHORTESTPATHGRAPHICSITEM_H
#define SHORTESTPATHGRAPHICSITEM_H

#include "movable_graphics_item.h"

class ControlledDijkstraAlgorithm;
class Node;

class ShortestPathGraphicsItem : public MovableGraphicsItem
{
public:
    ShortestPathGraphicsItem(ControlledDijkstraAlgorithm *algorithm, QGraphicsItem * parent = nullptr);

    void updateEntries();

private:
    ControlledDijkstraAlgorithm *dijkstra_algorithm;
    QMap<Node *, QGraphicsTextItem *> entries;

    // QGraphicsItem interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // SHORTESTPATHGRAPHICSITEM_H
