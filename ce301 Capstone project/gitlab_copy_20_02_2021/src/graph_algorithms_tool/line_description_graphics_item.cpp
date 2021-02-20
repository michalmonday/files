/*

  I may completely delete this class

*/

#include "line_description_graphics_item.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QTextItem>

#include "functions.h"

LineDescriptionGraphicsItem::LineDescriptionGraphicsItem(QGraphicsItem *src, QGraphicsItem *dst, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      src_item(src),
      dst_item(dst),
      description_text_item(nullptr)
{
    setAcceptHoverEvents(true);
//    setPos((src_item->scenePos() + dst_item->scenePos()) / 2);
    description_text_item = new QGraphicsTextItem("description", this);
}


QRectF LineDescriptionGraphicsItem::boundingRect() const
{
    return produceLinePath().boundingRect();
    //return {mapFromItem(src_item, 0, 0), mapFromItem(dst_item, 0, 0)};
}

void LineDescriptionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::black);

    QPainterPath line_path = produceLinePath();

    painter->drawPath(line_path);
}

QPainterPath LineDescriptionGraphicsItem::produceLinePath() const
{
    // main line
    //QPainterPath line_path(mapFromItem(src_item, 0, 0) + src_item->boundingRect().center());
    //line_path.lineTo(mapFromItem(dst_item, 0, 0) + dst_item->boundingRect().center());
    //line_path = qt_graphicsItem_shapeFromPath(line_path, QPen(Qt::darkGray, 2, Qt::DashLine));


    QPainterPath line_path;
    QGraphicsLineItem line(QLineF(mapFromItem(src_item, 0, 0) + src_item->boundingRect().center(),mapFromItem(dst_item, 0, 0) + dst_item->boundingRect().center()));
    line.setPen({Qt::green, 2, Qt::DashLine});
    line_path.addPath(line.opaqueArea());

    // to avoid drawing over source and destination items themselves
    QPainterPath mask_path, src_path(src_item->shape()), dst_path(dst_item->shape());
    src_path.translate(mapFromItem(src_item,0,0));
    dst_path.translate(mapFromItem(dst_item,0,0));
    mask_path.addPath(src_path);
    mask_path.addPath(dst_path);

    line_path -= mask_path;
    return line_path;
}


void LineDescriptionGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    update();
}

void LineDescriptionGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    update();
}
