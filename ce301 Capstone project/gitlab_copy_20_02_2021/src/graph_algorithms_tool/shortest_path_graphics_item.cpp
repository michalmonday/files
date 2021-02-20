#include "shortest_path_graphics_item.h"
#include "algorithms/controlled_dijkstra_algorithm.h"
#include "graph/graph.h"
#include "graph/node.h"
#include "functions.h"
#include "colors.h"

#include <QGraphicsScene>

static const qreal Y_SPACING = 1.2; // same as vtable_graphics_item.cpp

ShortestPathGraphicsItem::ShortestPathGraphicsItem(ControlledDijkstraAlgorithm *algorithm, QGraphicsItem *parent)
    :
      MovableGraphicsItem(parent),
      dijkstra_algorithm(algorithm)
{
    updateEntries();
}

void ShortestPathGraphicsItem::updateEntries() {
    QMap<Node*, int> dist = dijkstra_algorithm->dist();
    QMap<Node*, Node*> pred = dijkstra_algorithm->pred();

    // delete entry items if they're not there anymore
    for (Node *n : entries.keys()) {
        if (!dist.keys().contains(n)) {
            delete entries[n];
            entries.remove(n);
        }
    }

    // SORT nodes (making output neat)
    QList<Node *> sorted_keys = dist.keys();
    std::sort(sorted_keys.begin(), sorted_keys.end(), [](Node *n, Node*n2){ return n->getLabel() < n2->getLabel();});


    int y_pos = 0;
    for (Node *n : sorted_keys) {
        if (!entries.keys().contains(n)) {
            entries[n] = new QGraphicsTextItem(this);
            entries[n]->setPos(0, y_pos);
        }
        QString text = QString("V[%1] = (%2, %3)")
                .arg(colouredHtml(n->getLabel(),n->getColor() == COLOR_NODE_DEFAULT ? Qt::black : n->getColor()))
                .arg(colouredHtml(QString::number(dist[n]), dist[n] == INT_MAX ? Qt::black : COLOR_DIJKSTRA_OPTIMAL))
                .arg(pred[n] ? colouredHtml(pred[n]->getLabel(), pred[n]->getColor()) : "-");
        entries[n]->setHtml(text);
        y_pos += entries[n]->boundingRect().height() * Y_SPACING;
    }

    //update();
    if (QGraphicsScene *scene = this->scene())
        scene->update();

}

void ShortestPathGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    MovableGraphicsItem::paint(painter, option, widget);
}
