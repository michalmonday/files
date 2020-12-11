#include "queue_graphics_item.h"


QueueGraphicsItem::QueueGraphicsItem(QString dock_title, QGraphicsItem *parent)
    :CollectionGraphicsItem(parent)
{
    init(dock_title);
}

void QueueGraphicsItem::reset()
{
    CollectionGraphicsItem::reset();
    queue_nodes.clear();
}

void QueueGraphicsItem::enqueue(Node *n)
{
    nodes_to_highlight.clear();
    nodes_to_highlight << n;
    queue_nodes.enqueue(n);
    adjustToLeft();
}

void QueueGraphicsItem::enqueue(const QList<Node *> &nodes)
{
    for (Node *n : nodes)
        queue_nodes.enqueue(n);
    nodes_to_highlight = nodes;
    adjustToLeft();
}

Node *QueueGraphicsItem::dequeue()
{
    if (queue_nodes.isEmpty())
        return nullptr;

    Node *n = queue_nodes.dequeue();
    nodes_to_highlight.clear();
    adjustToLeft();
    return n;
}
