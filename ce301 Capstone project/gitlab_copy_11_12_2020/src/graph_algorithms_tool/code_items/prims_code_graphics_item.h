#ifndef PRIMSCODEGRAPHICSITEM_H
#define PRIMSCODEGRAPHICSITEM_H

#include <QGraphicsItem>
#include "code_items/code_graphics_item.h"

class CodeGraphicsItem;
class ControlledPrimsAlgorithm;



class PrimsCodeGraphicsItem : public CodeGraphicsItem
{
public:
    PrimsCodeGraphicsItem(ControlledPrimsAlgorithm *algorithm, QGraphicsItem * parent = nullptr);
    ~PrimsCodeGraphicsItem();

    virtual void setLanguage(int language_type) override;

    virtual void setAlgorithm(ControlledAlgorithm *a_) override;
protected:
    virtual QList<QList<QGraphicsTextItem *>> getTextListItemsForPreviousState(int prims_algorithm_step) override;


private:
    ControlledAlgorithm *prims_algorithm;

    QGraphicsTextItem * text_select_random_node;
    QGraphicsTextItem * text_while_mcst_not_completed;
    QGraphicsTextItem * text_add_to_mcst;
    QGraphicsTextItem * text_update_nodes_loop;
    QGraphicsTextItem * text_update_nodes_loop_if;
    QGraphicsTextItem * text_update_nodes_loop_then;
    QGraphicsTextItem * text_get_lowest_positive_v;

};

#endif // CODEGRAPHICSITEM_H
