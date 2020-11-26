#ifndef BFSCODEGRAPHICSITEM_H
#define BFSCODEGRAPHICSITEM_H

#include <QGraphicsItem>
#include "code_items/code_graphics_item.h"

class CodeGraphicsItem;
class ControlledBfsAlgorithm;

class BfsCodeGraphicsItem : public CodeGraphicsItem
{
public:
    BfsCodeGraphicsItem(ControlledBfsAlgorithm *algorithm, QGraphicsItem * parent = nullptr);
    virtual ~BfsCodeGraphicsItem();

    virtual void setLanguage(int language_type) override;

    virtual void setAlgorithm(ControlledAlgorithm *a_) override;
protected:
    virtual QList<QList<QGraphicsTextItem *>> getTextListItemsForPreviousState(int prims_algorithm_step) override;


private:
    ControlledBfsAlgorithm *bfs_algorithm;

    QGraphicsTextItem * text_init;
    QGraphicsTextItem * text_while;
    QGraphicsTextItem * text_dequeue;
    QGraphicsTextItem * text_expanded;
    QGraphicsTextItem * text_for_v;
    QGraphicsTextItem * text_if;
    QGraphicsTextItem * text_dist;
    QGraphicsTextItem * text_pred;
    QGraphicsTextItem * text_state;
    QGraphicsTextItem * text_enqueue;
};


#endif // BFSCODEGRAPHICSITEM_H
