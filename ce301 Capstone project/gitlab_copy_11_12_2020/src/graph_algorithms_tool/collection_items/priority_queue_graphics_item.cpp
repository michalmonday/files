#include "priority_queue_graphics_item.h"


PriorityQueueGraphicsItem::PriorityQueueGraphicsItem(QString dock_title, QGraphicsItem *parent)
    :CollectionGraphicsItem(parent)
{
    init(dock_title);
}

void PriorityQueueGraphicsItem::reset()
{
    CollectionGraphicsItem::reset();
    pq_nodes.clear();
    pq_edges.clear();
}

void PriorityQueueGraphicsItem::enqueue(int key, Node *n)
{
    nodes_to_highlight.clear();
    nodes_to_highlight << n;
    int i;
    if ((i = pq_nodes.values().indexOf(n)) != -1)
        pq_nodes.remove(pq_nodes.keys()[i], n);

    pq_nodes.insert(key, n);

    pq_weights = pq_nodes.keys();

    adjustToLeft();
}

void PriorityQueueGraphicsItem::enqueue(const QList<int> &keys, const QList<Node*> &nodes)
{
    for (int i = 0; i < keys.size(); i++) {
        int j;
        if ((j = pq_nodes.values().indexOf(nodes[i])) != -1)
            pq_nodes.remove(pq_nodes.keys()[j], nodes[i]);

        pq_nodes.insert(keys[i], nodes[i]);
    }

    pq_weights = pq_nodes.keys();

    nodes_to_highlight = nodes;
    adjustToLeft();
}

Node *PriorityQueueGraphicsItem::dequeueNode()
{
    if (pq_nodes.isEmpty())
        return nullptr;

    Node *n = pq_nodes.first();
    int key = pq_nodes.firstKey();
    pq_nodes.remove(key, n);

    pq_weights = pq_nodes.keys();

    nodes_to_highlight.clear();
    adjustToLeft();
    return n;
}


void PriorityQueueGraphicsItem::enqueue(Edge *e)
{
    edges_to_highlight.clear();
    edges_to_highlight << e;
    int key = e->getWeight();
    int i;
    if ((i = pq_edges.values().indexOf(e)) != -1)
        pq_edges.remove(pq_nodes.keys()[i], e);

    pq_edges.insert(key, e);
    pq_weights = pq_edges.keys();
    adjustToLeft();
}

void PriorityQueueGraphicsItem::enqueue(const QList<Edge*> &edges)
{
    for (Edge *e : edges) {
        int j;
        if ((j = pq_edges.values().indexOf(e)) != -1)
            pq_edges.remove(e->getWeight(), e);
        pq_edges.insert(e->getWeight(), e);
    }
    pq_weights = pq_edges.keys();
    edges_to_highlight = edges;
    adjustToLeft();
}

Edge *PriorityQueueGraphicsItem::dequeueEdge()
{
    if (pq_edges.isEmpty())
        return nullptr;

    Edge *e = pq_edges.first();
    int key = pq_edges.firstKey();
    pq_edges.remove(key, e);
    pq_weights = pq_edges.keys();

    edges_to_highlight.clear();
    adjustToLeft();
    return e;
}
