#include "step_description_graphics_item.h"

StepLogGraphicsItem::StepLogGraphicsItem(QGraphicsItem * parent) :
    MovableGraphicsItem(parent),
    text_item(new QGraphicsTextItem(this))
{
    text_item->setHtml("Hello world");
}

void StepLogGraphicsItem::setHtml(const QString &html)
{
    text_item->setHtml(html);
}
