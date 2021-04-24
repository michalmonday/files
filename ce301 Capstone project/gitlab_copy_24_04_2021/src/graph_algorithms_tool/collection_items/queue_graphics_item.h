#ifndef QUEUEGRAPHICSITEM_H
#define QUEUEGRAPHICSITEM_H

#include "collection_graphics_item.h"

#include "dock.h"
#include <QQueue>

class Node;
class QueueGraphicsItem : public CollectionGraphicsItem
{
public:
    QueueGraphicsItem(QString dock_title = "Queue", QGraphicsItem *parent = nullptr);


    void enqueue(Node *n);
    void enqueue(const QList<Node*> &nodes);
    Node * dequeue();

    // CollectionGraphicsItem interface
    virtual void reset() override;
    virtual QList<Node *> nodesCollection() const override { return queue_nodes.toVector().toList(); }

    // not implemented
    virtual QList<Edge *> edgesCollection() const override { return QList<Edge*>(); }

    virtual QString collectionName() const override { return "Queue"; }
    virtual QString frontText() const override { return "queue front (first to take)"; }
    virtual QString backText() const override { return "queue back"; }

private:
    QQueue<Node *> queue_nodes;

};

#endif // QUEUEGRAPHICSITEM_H
