#include "vtable_graphics_item.h"
#include <algorithm>
#include "graph/node.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>

#include "algorithms/controlled_algorithm.h"

#include "vtable_entry_graphics_item.h"
#include "colors.h"

static const qreal Y_SPACING = 1.2;

VtableGraphicsItem::VtableGraphicsItem(ControlledAlgorithm *a_, QString dock_title, QGraphicsItem *parent)
    : MovableGraphicsItem(parent),
      Dockable(),
      algorithm(a_),
      indicate_min_entry(false)
{
    updateValues();
    setToolTip("0 means that node is part of minimum spanning tree (MCST).\n"
               "INT_MAX (2147483647) means that the node is not considered yet.\n"
               "The smallest positive value from the table gets selected to be a new edge of the MCST.");
    //setLabel("V table");

    keepCurrentBoundingRect();
    setZValue(3.0);

    createDock(this, dock_title);
}


// Vtable is:  QMap<Node *, QPair<int, Node*>>
void VtableGraphicsItem::updateValues(ControlledAlgorithm *a_)
{
    if (a_)
        algorithm = a_;

    const Vtable &V = algorithm->vTable();
    updated_nodes = algorithm->updatedNodes();
    //last_iteration_nodes_with_replaced_optimal_edges = algorithm->lastIterationNodesWithReplacedOptimalEdges();

    // Vtable is a QMap of having keys of Node* type.
    // So is orders keys based on their address
    // sorted_keys will help to display "Vtable V" in alphabetical order
    QList<Node *> sorted_keys = V.keys();
    std::sort(sorted_keys.begin(), sorted_keys.end(), [](Node *n, Node*n2){ return n->getLabel() < n2->getLabel();});

    int y_pos = 0;
    for (Node *key_node : sorted_keys) {
        int weight = V[key_node].first;

        //QString v_entry_text = key_node->getLabel() + " = (" + QString::number(weight) + "," + src_label + ")";

        if (!entries.contains(key_node)) {
            entries[key_node] = new VtableEntryGraphicsItem(key_node, weight, V[key_node].second, this);
            entries[key_node]->setPos(0, y_pos);
            entries[key_node]->setToolTip("<span style='font-weight: bold;'>INT_MAX is initial value.</span><br>Meaning that V[node] is not considered yet.");
        } else {
            if (updated_nodes.contains(key_node))
                entries[key_node]->setToolTip("<span style='font-weight: bold;'>Previous value:</span>" +
                                                  entries[key_node]->toHtml());

            entries[key_node]->set(key_node, weight, V[key_node].second);
        }







        /*
        // format:
        // V[key_node_label] = (coloured_weight, src_label)
        QString key_node_label = key_node->getLabel();
        QString coloured_weight = QString::number(weight);
        QString src_label = "-";


        if (weight == 0) {
            //value_items[key_node]->setBrush(Qt::darkGreen);

            // set html instead
            // use V to get values again and rebuild the html string
            //;
            key_node_label = "<span style='color:darkGreen; font-weight: bold;'>" + key_node_label + "</span>";
            coloured_weight = "<span style='color:darkGreen; font-weight: bold;'>" + coloured_weight + "</span>";
            src_label = "<span style='color:darkGreen; font-weight: bold;'>" + src_label + "</span>";
        }
        else if (weight == INT_MAX) {
            // it's all black by default
        }
        else {
            key_node_label =  "<span style='color:blue; font-weight: bold;'>" + key_node_label + "</span>";
            coloured_weight = "<span style='color:blue; font-weight: bold;'>" + coloured_weight + "</span>";
            src_label =  "<span style='color:darkGreen; font-weight: bold;'>" + V[key_node].second->getLabel() + "</span>";
        }

        QString html = "V[" + key_node_label + "] = (" + coloured_weight + ", " + src_label + ")";

        entries[key_node]->setHtml(html);
        */

        y_pos += entries[key_node]->boundingRect().height() * Y_SPACING;
    }
    //updateStaticBoundingRect();

    this->update();
    if (QGraphicsScene *scene = this->scene())
        scene->update();

    //qDebug() << "value_items.size() = " << entries.size();

    //adjust();
    updateDock();
}

void VtableGraphicsItem::reset()
{
    updated_nodes.clear();
    //last_iteration_nodes_with_replaced_optimal_edges.clear();

    for (VtableEntryGraphicsItem * entry_item : entries.values())
        delete entry_item;

    entries.clear();

    //updateStaticBoundingRect();
}

void VtableGraphicsItem::setControlledAlgorithm(ControlledAlgorithm *a_)
{
    algorithm = a_;
}

/* // designed to keep right of the view (even after it's resized)
 * // but instead I'm using "keepCurrentBoundingRect()" (so it doesn't resize)
void VtableGraphicsItem::adjust()
{
    if (QGraphicsScene * scene = this->scene()) {
        QPointF new_pos = {scene->sceneRect().right() - boundingRect().width(), scenePos().y()};
        qDebug() << scenePos() << new_pos;
        setPos(new_pos);
    }
}*/

/*void VtableGraphicsItem::setSingleVvalue(Node *key_node, QPair<int, Node *> value_pair)
{
    V[key_node] = value_pair;

    QString src_label = value_pair.second ? value_pair.second->getLabel() : "-";
    QString v_entry_text = "(" + QString::number(value_pair.first) + "," + src_label + ")";

    value_items[key_node]->setText(v_entry_text);
}*/




void VtableGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    MovableGraphicsItem::paint(painter, option, widget);
    //painter->fillRect(boundingRect(),Qt::white);


    for (Node *updated_node : updated_nodes) {
        // This can be used to show when some edge becomes unused:
        /*if (last_iteration_nodes_with_replaced_optimal_edges.contains(updated_node)) {
            painter->setBrush(Qt::red);
        } else {
            painter->setBrush(QColor(252, 186, 3, 127));
        }*/
        //painter->setBrush(COLOR_VTABLE_AFFECTED_DURING_LAST_STEP);
        //painter->setPen(Qt::PenStyle::NoPen);
        painter->setPen({COLOR_VTABLE_AFFECTED_DURING_LAST_STEP, 3});

        const int margin = 2;
        QRectF v_entry_item_rect = entries[updated_node]->boundingRect();
        v_entry_item_rect.moveTo(entries[updated_node]->pos());// - QPointF(margin, margin));
        painter->drawRect(v_entry_item_rect);
    }

    // get minPositiveEntry();
    // colour it using COLOR_VTABLE_LOWEST_POSITIVE_VALUE
    if (indicate_min_entry) {
        if (VtableEntryGraphicsItem *min_item = minPositiveEntry()) {
            painter->setBrush(Qt::NoBrush);
            painter->setPen(COLOR_VTABLE_LOWEST_POSITIVE_VALUE);
            QRectF rect = min_item->boundingRect();
            rect.moveTo(min_item->pos());
            //painter->drawRect(rect);

            QFont font = painter->font();
            font.setBold(true);
            painter->setFont(font);
            QPointF p = rect.center() + QPointF(rect.width()/2, 4);// + QPointF(10, 0);
            painter->drawText(p, "  <- min");
        }
    }
}

VtableEntryGraphicsItem *VtableGraphicsItem::minPositiveEntry()
{
    VtableEntryGraphicsItem * min_entry = nullptr;
    int min_weight = INT_MAX;
    for (auto const &entry : entries) {
        int w_ = entry->getWeight();
        if (w_ && w_ < min_weight) {
            min_entry = entry;
            min_weight = w_;
        }
    }
    return min_entry;
}
