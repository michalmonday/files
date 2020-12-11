#include "vtable_entry_hover_text_item.h"

#include <QPainter>

VtableEntryHoverTextItem::VtableEntryHoverTextItem(QString text, QGraphicsItem * parent)
    : QGraphicsTextItem(parent)
{
    setHtml(text);
}


void VtableEntryHoverTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen({Qt::darkGray, 2});
    painter->setBrush(Qt::white);
    painter->drawRect(boundingRect().marginsAdded({2,2,2,2}));

    QGraphicsTextItem::paint(painter, option, widget);
}
