#ifndef PRIORITYQUEUEGRAPHICSITEM_H
#define PRIORITYQUEUEGRAPHICSITEM_H

#include "collection_graphics_item.h"
#include "dock.h"

#include <QMultiMap>

class Node;
class PriorityQueueGraphicsItem : public CollectionGraphicsItem
{
public:
    PriorityQueueGraphicsItem(QString dock_title = "Priority queue", QGraphicsItem *parent = nullptr);


    void enqueue(int key, Node *n);
    void enqueue(const QList<int> &keys, const QList<Node*> &nodes);
    Node * dequeueNode();

    void enqueue(Edge *e);
    void enqueue(const QList<Edge*> &edges);
    Edge * dequeueEdge();

    // CollectionGraphicsItem interface
    virtual void reset() override;
    virtual QList<Node *> nodesCollection() const override { return pq_nodes.values(); }
    virtual QList<Edge *> edgesCollection() const override { return pq_edges.values(); }
    virtual QString collectionName() const override { return "Priority queue"; }
    virtual QString frontText() const override { return "PQ front (first to take)"; }
    virtual QString backText() const override { return "PQ back"; }


private:
    QMultiMap<int, Node *> pq_nodes;
    QMultiMap<int, Edge *> pq_edges;
};

#endif // PRIORITYQUEUEGRAPHICSITEM_H
