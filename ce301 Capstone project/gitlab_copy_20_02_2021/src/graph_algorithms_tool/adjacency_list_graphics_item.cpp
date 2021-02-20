#include "adjacency_list_graphics_item.h"

#include <QGraphicsScene>
#include <QPainter>
#include "graph/graph.h"
#include "graph/node.h"
#include "functions.h"
#include <QDebug>

#include "colors.h"

static const qreal Y_SPACING = 1.2; // same as vtable_graphics_item.cpp

AdjacencyListGraphicsItem::AdjacencyListGraphicsItem(Graph *g_, QGraphicsItem *parent)
    : MovableGraphicsItem(parent),
      Dockable(),
      graph(g_),
      colors_enabled(false)
{
    //setLabel("Adjacency list");
    setToolTip("Adjacency list is one way of storing/describing contents of a graph.\n");

    updateEntries();

    createDock(this, "Adjacency list");
    showDock();
}

void AdjacencyListGraphicsItem::updateEntries() {
    if (!graph)
        return;

    adjacency_map = graph->adjacencyMap();

    // delete entry items if they're not there anymore
    for (Node *key_node : entries.keys()) {
        if (!adjacency_map.keys().contains(key_node)) {
            delete entries[key_node];
            entries.remove(key_node);
        }
    }

    // SORT nodes (making output neat)
    QList<Node *> sorted_keys = adjacency_map.keys();
    std::sort(sorted_keys.begin(), sorted_keys.end(), [](Node *n, Node*n2){ return n->getLabel() < n2->getLabel();});

    int y_pos = 0;
    for (Node *node : sorted_keys) {
        if (!entries.keys().contains(node))
            entries[node] = new QGraphicsTextItem(this);

        updateEntryText(node);

        entries[node]->setPos(0, y_pos);
        y_pos += entries[node]->boundingRect().height() * Y_SPACING;
    }

    if (QGraphicsScene *scene = this->scene())
        scene->update();
}

void AdjacencyListGraphicsItem::updateColors() {
    for (Node *node : entries.keys()) {
        updateEntryText(node);
        //qDebug() << node->getLabel() << " = " << node->getColor();
        //qDebug() << colouredHtml(node->getLabel(), node->getColor());
    }
}

void AdjacencyListGraphicsItem::updateEntryText(Node *node)
{
    QColor clr = !colors_enabled || node->getColor() == COLOR_NODE_DEFAULT ? Qt::black : node->getColor();
    QString entry_text = colouredHtml(node->getLabel(), clr) + " = [";

    // SORT adjacent nodes (making output neat)
    QList<Node *> sorted_adjacent_nodes = adjacency_map[node];
    std::sort(sorted_adjacent_nodes.begin(), sorted_adjacent_nodes.end(), [](Node *n, Node*n2){ return n->getLabel() < n2->getLabel();});

    for (Node *adjacent_node : sorted_adjacent_nodes) {
        clr = !colors_enabled || adjacent_node->getColor() == COLOR_NODE_DEFAULT ? Qt::black : adjacent_node->getColor();
        entry_text += colouredHtml(adjacent_node->getLabel(), clr) + (adjacent_node != sorted_adjacent_nodes.last() ? ", " : "]");
    }

    entries[node]->setHtml(entry_text);
}

void AdjacencyListGraphicsItem::setGraph(Graph *g_)
{
    graph = g_;
    updateEntries();
    updateDock();
}

void AdjacencyListGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    MovableGraphicsItem::paint(painter, option, widget);
}
