#ifndef ADJACENCYLISTGRAPHICSITEM_H
#define ADJACENCYLISTGRAPHICSITEM_H

#include "dock.h"
#include "movable_graphics_item.h"
#include "typedefs.h"

class MovableGraphicsItem;
class Graph;
class Node;

class AdjacencyListGraphicsItem : public MovableGraphicsItem, virtual public Dockable
{
public:
    AdjacencyListGraphicsItem(Graph *graph = nullptr, QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void updateEntries();
    void updateColors();
    void updateEntryText(Node *node);
    void setGraph(Graph *graph);
private:
    Graph *graph;
    AdjacencyMap adjacency_map;
    bool colors_enabled;

    QMap<Node*, QGraphicsTextItem *> entries;
};

#endif // ADJACENCYLISTGRAPHICSITEM_H
