#include "code_items/code_graphics_item.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QList>

#include "algorithms/controlled_algorithm.h"

#include "colors.h"
#include "functions.h"
#include "kruskals_code_graphics_item.h"


/*
TODO:
There are 2 problems:
MCST stores edges, not nodes
Adjacency list stores nodes, not edges

*/



const int CODE_LANG_PSEUDOCODE = 1;
const int CODE_LANG_PYTHON = 2;

CodeGraphicsItem::CodeGraphicsItem(ControlledAlgorithm *a_, QGraphicsItem * parent)
    : MovableGraphicsItem(parent),
      Dockable(),
      algorithm(a_)
{
    //setLabel("Code flow");
    if (label) {
        language_name_item = new QGraphicsSimpleTextItem(this);
        QPointF label_top_right = label->boundingRect().topRight() + label->pos();
        language_name_item->setPos(label_top_right + QPointF(0,0));
        language_name_item->setFont(label->font());
    }
}

CodeGraphicsItem::~CodeGraphicsItem()
{
}

void CodeGraphicsItem::setLanguage(int language_type)
{
    language = language_type;

    /*switch (language) {
        case CODE_LANG_PSEUDOCODE: language_name_item->setText(" (pseudocode)"); break;
        case CODE_LANG_PYTHON:     language_name_item->setText(" (python)");     break;
    }*/

    int y_pos = 0;
    const int indent = 30;
    for (QGraphicsTextItem *text_item : text_list) {
        text_item->setPos(0, y_pos);
        y_pos += text_item->boundingRect().height() + 5;
    }

    // adjust x position
    for (int i = 0; i < indents_list.size(); i++) {
        QGraphicsTextItem *item = text_list[i];
        item->setPos(item->pos() + QPointF(indents_list[i] * indent, 0));
    }

    update();

    // bounding rect may change and be smaller, so scene must be updated to avoid ugly leftover lines
    if (QGraphicsScene *scene = this->scene())
        scene->update();
}



void CodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    MovableGraphicsItem::paint(painter, option, widget);
    QList<QList<QGraphicsTextItem *>> text_item_groups;

    // pls don't judge
    text_item_groups = getTextListItemsForPreviousState(dynamic_cast<KruskalsCodeGraphicsItem*>(this) ?
                                                            algorithm->getState() :
                                                            algorithm->getPreviousState());
    const int margin = 2;

    /* This should be done in overriden pain method in PrimsCodeGraphicsItem
     *
     if (algorithm && algorithm->getPreviousState() == PRIMS_STATE_NEW_NODE_INCLUDED) {
        QRectF items_rect;
        for (QGraphicsTextItem *item : text_items) {
            QRectF item_rect = item->boundingRect().marginsAdded({margin,margin,margin,margin});
            item_rect.moveTo(item->pos() - QPointF(margin/2, margin/2));
            items_rect = items_rect.united(item_rect);
        }
        painter->drawRect(items_rect);
        return;
    }*/


    for (QList<QGraphicsTextItem *> text_items : text_item_groups) {
        QRectF items_rect;
        for (QGraphicsTextItem * item : text_items) {
            QRectF item_rect = item->boundingRect().marginsAdded({margin,margin,margin,margin});
            item_rect.moveTo(item->pos() - QPointF(margin/2, margin/2));
            items_rect = items_rect.united(item_rect);
        }
        painter->drawRect(items_rect);
    }
}


void CodeGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::RightButton) {
        if (language == CODE_LANG_PYTHON)
            setLanguage(CODE_LANG_PSEUDOCODE);
        else if (language == CODE_LANG_PSEUDOCODE)
            setLanguage(CODE_LANG_PYTHON);
    }
    MovableGraphicsItem::mousePressEvent(event);
}



QRectF CodeGraphicsItem::boundingRect() const
{
    QRectF all_text_items_rect;
    for (QGraphicsTextItem *item : text_list)
        all_text_items_rect = all_text_items_rect.united(item->boundingRect());
    return MovableGraphicsItem::boundingRect().united(all_text_items_rect);
}

