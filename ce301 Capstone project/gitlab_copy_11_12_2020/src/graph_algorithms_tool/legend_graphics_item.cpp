#include "legend_graphics_item.h"

#include <QPen>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>

#include "colors.h"

LegendGraphicsItem::LegendGraphicsItem(QString title, QGraphicsItem *parent)
    : MovableGraphicsItem(parent),
      Dockable()
     // is_moved(false),
     // is_hovered(false)
{    
    y_pos = 0;
    line_length = 25;
    pen_width = 10;
    half_line_width = line_length / 2;
    half_pen_width = pen_width / 2;
    text_x = line_length + pen_width/2 + 12;


    /*
    // triangle (to make half-blue/half-violet)
    QGraphicsPolygonItem *polygon_item = new QGraphicsPolygonItem(this);
    polygon_item->setPolygon(QPolygonF(QVector<QPointF>{
                                           considered_line->line().p1() - QPointF(half_pen_width, half_pen_width),
                                           considered_line->line().p1() - QPointF(half_pen_width, -half_pen_width),
                                           considered_line->line().p2() + QPointF(half_pen_width, half_pen_width)
                                       }));
    polygon_item->setPen(Qt::NoPen);
    polygon_item->setBrush(COLOR_NEW_CONSIDERED.lighter());*/

    /*
    // NEW CONSIDERED NODES
    QGraphicsSimpleTextItem *new_considered_text = new QGraphicsSimpleTextItem("Nodes updated in V table", this);
    new_considered_text->setPos(text_x, y-half_font_size);
    QGraphicsLineItem * new_considered_line = new QGraphicsLineItem(this);
    new_considered_line->setPen(QPen(COLOR_NEW_CONSIDERED.lighter(), pen_width));
    new_considered_line->setLine(QLine(0, y, line_length, y));
    y += y_space;*/    


    //setLabel("Legend");
    createDock(this, title);
}

void LegendGraphicsItem::addRect(QColor clr, QString text)
{
    QGraphicsSimpleTextItem *mcst_text = new QGraphicsSimpleTextItem(text, this);
    int half_font_size = mcst_text->boundingRect().height() / 2;
    int y_space = half_font_size * 4;
    mcst_text->setPos(text_x, y_pos-half_font_size);
    QGraphicsLineItem * mcst_line = new QGraphicsLineItem(this);
    mcst_line->setPen(QPen(clr, pen_width));
    mcst_line->setLine(QLine(0, y_pos, line_length, y_pos));
    y_pos += y_space;
}


void LegendGraphicsItem::addCircle(QColor clr, QString text, bool empty_inside)
{
    QGraphicsSimpleTextItem *text_item = new QGraphicsSimpleTextItem(text, this);
    int half_font_size = text_item->boundingRect().height() / 2;
    int y_space = half_font_size * 4;

    text_item->setPos(text_x, y_pos-half_font_size);
    QGraphicsPathItem * path_item = new QGraphicsPathItem(this);

    // dirty hack
    if (!empty_inside) {
        path_item->setPen(QPen(Qt::black, 1));
        clr = clr.lighter();
    }
    else
        path_item->setPen(Qt::NoPen);

    path_item->setBrush(clr);
    QPainterPath ellipse_path, mask;
    int ellipse_size = half_font_size*3;
    ellipse_path.addEllipse(QRectF(0, 0, ellipse_size, ellipse_size));

    if(empty_inside) {
        int mask_size = ellipse_size * 0.7;
        int mask_pos = ellipse_path.boundingRect().width()/2 - mask_size / 2;
        mask.addEllipse(QRectF(mask_pos, mask_pos, mask_size, mask_size));
        ellipse_path -= mask;
    }

    path_item->setPath(ellipse_path);
    path_item->setPos(line_length/2 - ellipse_size/2, y_pos - ellipse_size/2);
    y_pos += y_space;
}

void LegendGraphicsItem::addDonut(QColor clr, QString text)
{
    addCircle(clr, text, /*empty_inside*/ true);
}

/*
QRectF PrimsLegendItem::boundingRect() const
{
    return childrenBoundingRect().marginsAdded({10,10,10,10});
}

void PrimsLegendItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, 1));
    if (is_hovered)
        painter->fillRect(boundingRect(), QColor(0,0,0, 10));
    painter->drawRect(boundingRect());

}


void PrimsLegendItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    is_moved = true;
    grabbed_offset = event->scenePos() - scenePos();
}

void PrimsLegendItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (is_moved) {
        setPos(event->scenePos() - grabbed_offset);
        scene()->update();
    }
}

void PrimsLegendItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    is_moved = false;
}


void PrimsLegendItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    is_hovered = true;
    update();
}

void PrimsLegendItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    is_hovered = false;
    update();
}*/
