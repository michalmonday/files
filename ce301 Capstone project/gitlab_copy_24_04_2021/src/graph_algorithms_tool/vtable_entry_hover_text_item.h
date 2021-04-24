#ifndef VTABLEENTRYHOVERTEXTITEM_H
#define VTABLEENTRYHOVERTEXTITEM_H

#include <QGraphicsItem>

class VtableEntryHoverTextItem : public QGraphicsTextItem
{
public:
    VtableEntryHoverTextItem(QString text, QGraphicsItem * parent = nullptr);

    // QGraphicsItem interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // VTABLEENTRYHOVERTEXTITEM_H
