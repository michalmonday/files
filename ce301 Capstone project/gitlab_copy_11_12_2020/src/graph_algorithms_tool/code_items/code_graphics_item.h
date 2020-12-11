#ifndef CODEGRAPHICSITEM_H
#define CODEGRAPHICSITEM_H

#include "movable_graphics_item.h"

#include <dock.h>
class MovableGraphicsItem;

class ControlledAlgorithm;

class CodeGraphicsItem : public MovableGraphicsItem, virtual public Dockable
{
public:
    CodeGraphicsItem(ControlledAlgorithm *algorithm, QGraphicsItem * parent = nullptr);
    ~CodeGraphicsItem();

    virtual void setLanguage(int language_type);
    virtual void setAlgorithm(ControlledAlgorithm *a_) = 0;

protected:

    ControlledAlgorithm *algorithm;
    QList<QGraphicsTextItem *> text_list;
    QList<int> indents_list;
    int language;
    QGraphicsSimpleTextItem *language_name_item;

    // implementation of getTextListItemsForState in derived class should be based on the current state
    // of the language member
    // Why is it nested list?
    // For elegant highlighting groups of lines that may be apart of each other.
    virtual QList<QList<QGraphicsTextItem *>> getTextListItemsForPreviousState(int algorithm_state) = 0;

    // QGraphicsItem interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;
    // QGraphicsItem interface

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


 };

extern const int CODE_LANG_PSEUDOCODE;
extern const int CODE_LANG_PYTHON;

#endif // CODEGRAPHICSITEM_H
