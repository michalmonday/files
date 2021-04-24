#include "stack_graphics_item.h"

StackGraphicsItem::StackGraphicsItem(QString dock_title, QGraphicsItem *parent)
    :CollectionGraphicsItem(parent)
{
    init(dock_title);
}

void StackGraphicsItem::reset()
{
    CollectionGraphicsItem::reset();
    stack_nodes.clear();
}

void StackGraphicsItem::push(Node *n)
{
    nodes_to_highlight.clear();
    nodes_to_highlight << n;
    stack_nodes.append(n);
    adjustToLeft();
}

void StackGraphicsItem::push(const QList<Node *> &nodes)
{
    nodes_to_highlight.clear();
    stack_nodes += nodes;
    nodes_to_highlight = nodes;
    adjustToLeft();
}

Node *StackGraphicsItem::pop()
{
    if (stack_nodes.isEmpty())
        return nullptr;

    Node *n = stack_nodes.takeLast();
    nodes_to_highlight.clear();
    adjustToLeft();
    return n;
}
