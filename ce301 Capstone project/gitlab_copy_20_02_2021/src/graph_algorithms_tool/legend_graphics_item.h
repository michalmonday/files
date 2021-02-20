#ifndef PRIMSLEGENDITEM_H
#define PRIMSLEGENDITEM_H

#include "movable_graphics_item.h"
#include "dock.h"

class LegendGraphicsItem : public MovableGraphicsItem, virtual public Dockable
{
public:
    LegendGraphicsItem(QString title, QGraphicsItem *parent = nullptr);

    void addRect(QColor, QString);
    void addCircle(QColor, QString, bool empty_inside=false);
    void addDonut(QColor, QString);

private:
    int y_pos;
    int line_length;
    int pen_width;
    int half_line_width;
    int half_pen_width;
    int text_x;

/*
    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool is_moved;
    bool is_hovered;
    QPointF grabbed_offset;*/
};

#endif // PRIMSLEGENDITEM_H
