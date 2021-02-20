#ifndef MOVABLEGRAPHICSITEM_H
#define MOVABLEGRAPHICSITEM_H

#include <QGraphicsItem>

class MovableGraphicsItem : public QGraphicsItem
{
public:
    MovableGraphicsItem(QGraphicsItem * parent = nullptr);

    bool hasBorder();
    void hasBorder(bool state);
    bool hasLabel();
    void setLabel(QString label);

    void keepCurrentBoundingRect(bool state = true);
    void updateStaticBoundingRect();
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    bool is_moved;
    QPointF grabbed_offset;
    bool has_border;
    QGraphicsSimpleTextItem *label;

    QRectF static_bounding_rect;
    bool use_static_bounding_rect;
};

#endif // MOVABLEGRAPHICSITEM_H
