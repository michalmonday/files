#ifndef STEPDESCRIPTIONGRAPHICSITEM_H
#define STEPDESCRIPTIONGRAPHICSITEM_H

#include "movable_graphics_item.h"

class StepLogGraphicsItem : public MovableGraphicsItem
{
public:
    StepLogGraphicsItem(QGraphicsItem *parent=nullptr);

    void setHtml(const QString &html);
private:
    QGraphicsTextItem *text_item;
};

#endif // STEPLOGGRAPHICSITEM_H
