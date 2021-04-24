#ifndef DFSCODEGRAPHICSITEM_H
#define DFSCODEGRAPHICSITEM_H

#include <QGraphicsItem>
#include "code_items/code_graphics_item.h"

class CodeGraphicsItem;
class ControlledDfsAlgorithm;

class DfsCodeGraphicsItem : public CodeGraphicsItem
{
public:
    DfsCodeGraphicsItem(ControlledDfsAlgorithm *algorithm, QGraphicsItem * parent = nullptr);
    virtual ~DfsCodeGraphicsItem();

    virtual void setLanguage(int language_type) override;

    virtual void setAlgorithm(ControlledAlgorithm *a_) override;
protected:
    virtual QList<QList<QGraphicsTextItem *>> getTextListItemsForPreviousState(int prims_algorithm_step) override;


private:
    ControlledDfsAlgorithm *dfs_algorithm;

    QGraphicsTextItem * text_init_1;
    QGraphicsTextItem * text_init_2;
    QGraphicsTextItem * text_init_3;
    QGraphicsTextItem * text_while;
    QGraphicsTextItem * text_pop;
    QGraphicsTextItem * text_expanded;
    QGraphicsTextItem * text_for_v;
    QGraphicsTextItem * text_if;
    QGraphicsTextItem * text_dist;
    QGraphicsTextItem * text_pred;
    QGraphicsTextItem * text_state;
    QGraphicsTextItem * text_push;
};


#endif // DFSCODEGRAPHICSITEM_H
