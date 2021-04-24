#include "line_description_graphics_item.h"
#include "vtable_entry_graphics_item.h"
#include "vtable_entry_hover_text_item.h"

#include <QPainter>
#include <QStyleOption>

#include "graph/node.h"

#include "graph/edge.h"

#include <QDebug>

#include "colors.h"
#include "functions.h"
#include "mainwindow.h"

VtableEntryGraphicsItem::VtableEntryGraphicsItem(Node *key_node_, int weight_, Node *src_node_, QGraphicsItem *parent)
    : QGraphicsTextItem(parent),
      key_node(nullptr),
      src_node(nullptr),
      previous_edge(nullptr),
      description_item(nullptr)
      //line_description_item(nullptr)
{
    graph = key_node_->parentGraph();

    setAcceptHoverEvents(true);
    set(key_node_, weight_, src_node_);
    setZValue(3.0);
}

VtableEntryGraphicsItem::~VtableEntryGraphicsItem()
{
    if (!is_exiting) {
        if (graph->has(key_node)) {
            key_node->setVentryHovered(false);
            if (src_node && graph->has(src_node)) {
                Edge *e = key_node->edgeWith(src_node);
                if (graph->has(e))
                    e->setVentryHovered(false);
            }
        }
    }
}

void VtableEntryGraphicsItem::set(Node *key_node_, int weight_, Node *src_node_)
{
    onHoverLeave();

    if (key_node && src_node && src_node_ && src_node != src_node_) {
        previous_edge = key_node->edgeWith(src_node);
    } else {
        previous_edge = nullptr;
    }

    key_node = key_node_;
    src_node = src_node_;

    QString key_node_html = key_node->getLabel();
    QString weight_html = QString::number(weight_);
    QString src_html = src_node ? colouredHtml(src_node->getLabel(), COLOR_MCST) : "-";

    if (weight_ == 0) {
        key_node_html = colouredHtml(key_node_html, COLOR_MCST);
        weight_html = colouredHtml(weight_html, COLOR_MCST);
    } else if (weight_ != INT_MAX) {
        key_node_html = colouredHtml(key_node_html, COLOR_CONSIDERED);
        weight_html = colouredHtml(weight_html, COLOR_CONSIDERED);
    }

    setHtml(QString("V[%1] = (%2, %3)").arg(
                key_node_html,
                weight_html,
                src_html)
            );

    /*
    if (src_node) {
        if (line_description_item) {
            delete line_description_item;
            line_description_item = nullptr; // just in case if destructor is called meanwhile
        }
        line_description_item = new LineDescriptionGraphicsItem(this, key_node);
        if (QGraphicsScene *scene = this->scene()) {
            scene->addItem(line_description_item);
            scene->update();
        }
    }
    */




    if(isUnderMouse()) {
        onHoverEnter();
    }

    weight = weight_;
}




/*void VtableEntryGraphicsItem::setKeyNodeColor(QString html_color)
{
    key_node_coloured_html = QString("<span style='%1'>%2</span>").arg(html_color).arg(key_node->getLabel());
}

void VtableEntryGraphicsItem::setWeightColor(QString html_color)
{
    weight_coloured_html = QString("<span style='%1'>%2</span>").arg(html_color).arg(weight);
}

void VtableEntryGraphicsItem::setSrcNodeColor(QString html_color)
{
    QString node_label = src_node ? src_node->getLabel() : "-";
    src_node_coloured_html = QString("<span style='%1'>%2</span>").arg(html_color).arg(node_label);
}*/


void VtableEntryGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    bool is_hovered = option->state & QStyle::State_MouseOver;

    if (is_hovered) {
        painter->fillRect(boundingRect(), QColor(0,0,0, 10));

        if (src_node) {
            if (Edge *edge = key_node->edgeWith(src_node)) {
                QString description = "This edge is currently considered to be optimal.";
                if (previous_edge)
                    description += "\nPrevious edge is not considered anymore";

                //drawDescriptionLine(painter, edge, description);
            }
        }
    }

    // this needs to be simplified
    // also, I may modify this whole class and make separate QGraphicsTextObjects for:
    // - V[key_node]
    // - weight
    // - src_node

    // This way a different connection will be drawn when hovering over each of them



    QGraphicsTextItem::paint(painter, option, widget);
}

// this isn't used anymore (after deciding to use docks)
void VtableEntryGraphicsItem::drawDescriptionLine(QPainter *painter, Edge *edge, QString text)
{
    const int margin = 4;
    QPointF edge_middle = edge->scenePos() - scenePos()  + edge->midlinePoint();
    QPointF line_start_point(0, boundingRect().height()/2);

    // draw line
    painter->setPen(QPen(Qt::gray, 2, Qt::DashLine));
    painter->drawLine(line_start_point, edge_middle);

    /*
    // draw description box
    painter->setBrush(Qt::white);
    QRectF text_rect(QPointF({(edge_middle + line_start_point) / 2}), QSizeF(1,1));

    QGraphicsSimpleTextItem item(text);

    qreal text_width = item.boundingRect().width();
    qreal text_height = item.boundingRect().height();
    //QRectF text_rect = text_rect.marginsAdded({0,0, text_width, text_height});
    text_rect = text_rect.marginsAdded({text_width/2, text_height/2, text_width/2, text_height/2});

    QPen pen = painter->pen();
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);

    painter->drawRect(text_rect.marginsAdded({margin,margin,margin,margin}));
    painter->drawText(text_rect.bottomLeft(), item.text());
    */
}

/*
QRectF VtableEntryGraphicsItem::boundingRect() const
{
    const int margin = 2;
    QRectF centered_rect = QGraphicsTextItem::boundingRect();
    centered_rect.translate(QGraphicsTextItem::boundingRect().center());
    return centered_rect.marginsAdded({margin,margin,margin,margin});
}*/

void VtableEntryGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    onHoverEnter();
    QGraphicsTextItem::hoverEnterEvent(event);
    scene()->update();
}


void VtableEntryGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{    
    onHoverLeave();
    QGraphicsTextItem::hoverLeaveEvent(event);
    scene()->update();
}

void VtableEntryGraphicsItem::onHoverEnter()
{
    if (src_node && key_node) {
        if (Edge *e = key_node->edgeWith(src_node)) {
            e->setVentryHovered(true);

            QString description = "This edge is currently considered to be optimal.";
            if (previous_edge)
                description += QString("<br>Previous edge (<span style='color:gray; font-weight:bold;'>%1</span>) is not considered anymore (due to last iteration).").arg(previous_edge->nodeLabels());

            if (description_item)
                delete description_item;

            description_item = new VtableEntryHoverTextItem(description,this);

            QPointF relative_edge_pos = mapFromItem(key_node->edgeWith(src_node)->getLabelItem(),0,0) / 2.0;
            description_item->setPos(mapFromItem(key_node->edgeWith(src_node)->getLabelItem(), -relative_edge_pos-description_item->boundingRect().center()));
        }
    }

    if (key_node)
        key_node->setVentryHovered(true);

    update();
}

void VtableEntryGraphicsItem::onHoverLeave()
{
    if (src_node && key_node)
        if (Edge *e = key_node->edgeWith(src_node))
            e->setVentryHovered(false);

    if (key_node)
        key_node->setVentryHovered(false);

    update();

    if (description_item) {
        delete description_item;
        description_item = nullptr;
    }
}


