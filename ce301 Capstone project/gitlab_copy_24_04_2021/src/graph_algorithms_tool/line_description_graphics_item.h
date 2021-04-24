#ifndef LINEDESCRIPTIONGRAPHICSITEM_H
#define LINEDESCRIPTIONGRAPHICSITEM_H

#include <QGraphicsItem>

class LineDescriptionGraphicsItem : public QGraphicsItem
{
public:
    LineDescriptionGraphicsItem(QGraphicsItem *src, QGraphicsItem *dst, QGraphicsItem *parent = nullptr);

    // QGraphicsItem interface
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    QGraphicsItem *src_item;
    QGraphicsItem *dst_item;

    QGraphicsTextItem *description_text_item;

    QPainterPath produceLinePath() const;

    // QGraphicsItem interface
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // LINEDESCRIPTIONGRAPHICSITEM_H
