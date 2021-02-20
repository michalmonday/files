/*  This file implements QGraphicsItem.
    QGraphicsItem has "setFlag(ItemIsMovable)" by default, however it does not work well
    when item has child items (moving item is chaotic). For that reason, in "MovableGraphicsItem" class
    the "ItemIsMovable" flag is not set and moving is accomplished using "mousePressEvent, mouseReleaseEvent".

    Several classes inherit from MovableGraphicsItem class, for example:
        - Graph
        - Node
        - PrimsLegendItem
        - VtableGraphicsItem
        - CodeGraphicsItem
*/

#include "movable_graphics_item.h"
#include "mainwindow.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QStyleOption>

#include "functions.h"

MovableGraphicsItem::MovableGraphicsItem(QGraphicsItem *parent)
    : QGraphicsItem(parent),
      is_moved(false),
      has_border(false),
      label(nullptr),
      use_static_bounding_rect(false)
{
    setAcceptHoverEvents(true);
}

bool MovableGraphicsItem::hasBorder()
{
    return has_border;
}

void MovableGraphicsItem::hasBorder(bool state)
{
    has_border = state;
    update();
}

bool MovableGraphicsItem::hasLabel()
{
    return label;
}

void MovableGraphicsItem::setLabel(QString text)
{
    if (!label)
        label = new QGraphicsSimpleTextItem(this);
    label->setText(text + "\n");
    //top_label->setPen(QPen(Qt::black, 1));
    QFont font = label->font();
    font.setBold(true);
    font.setWeight(font.weight() + 4);
    label->setFont(font);
    label->setPos(0, -(label->boundingRect().height()));
}

void MovableGraphicsItem::keepCurrentBoundingRect(bool state)
{
    if (state)
        static_bounding_rect = boundingRect();
    use_static_bounding_rect = state;
}

void MovableGraphicsItem::updateStaticBoundingRect()
{
    prepareGeometryChange();
    use_static_bounding_rect = false;
    static_bounding_rect = boundingRect();
    use_static_bounding_rect = true;
}

QRectF MovableGraphicsItem::boundingRect() const
{
    //QRectF original = childrenBoundingRect().marginsAdded({10,10,10,10});
    if (use_static_bounding_rect)
        return static_bounding_rect;
    return childrenBoundingRect().marginsAdded({10,10,10,10});
}

void MovableGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    bool is_hovered = option->state & QStyle::State_MouseOver;
    if (is_hovered)
        painter->fillRect(boundingRect(), QColor(0,0,0, 10));
    if (has_border) {
        painter->setPen(QPen(Qt::black, 1));
        painter->drawRect(boundingRect());
    }

}


void MovableGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    is_moved = true;
    grabbed_offset = event->scenePos() - scenePos();


    if (parentItem())
        QGraphicsItem::mousePressEvent(event);
}

void MovableGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (is_moved) {
        QPointF new_pos = event->scenePos() - grabbed_offset;
        if (QGraphicsItem *parent_item = parentItem())
            new_pos -= parent_item->scenePos();

        if (~event->modifiers() & Qt::ControlModifier)
            new_pos = getClosestPoint(new_pos);
        if (pos() != new_pos) {
            setPos(new_pos);
            if (QGraphicsScene * scene = this->scene())
                scene->update();
        }
    }
    else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void MovableGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    is_moved = false;
    if (parentItem())
        QGraphicsItem::mouseReleaseEvent(event);
}

