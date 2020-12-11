#ifndef STACKGRAPHICSITEM_H
#define STACKGRAPHICSITEM_H

#include "collection_graphics_item.h"

class Node;
class StackGraphicsItem : public CollectionGraphicsItem
{
public:
    StackGraphicsItem(QString dock_title = "Stack", QGraphicsItem *parent = nullptr);

    void push(Node *n);
    void push(const QList<Node*> &nodes);
    Node * pop();

    // CollectionGraphicsItem interface
    virtual void reset() override;
    virtual QList<Node *> nodesCollection() const override { return stack_nodes; }

    // not implemented
    virtual QList<Edge *> edgesCollection() const override { return QList<Edge*>(); }

    virtual QString collectionName() const override { return "Stack"; }
    virtual QString frontText() const override { return "stack bottom"; }
    virtual QString backText() const override { return "stack top (first to take)"; }

private:
    QList<Node *> stack_nodes;

};

#endif // STACKGRAPHICSITEM_H
