#ifndef KRUSKALSCODEGRAPHICSITEM_H
#define KRUSKALSCODEGRAPHICSITEM_H

#include "code_graphics_item.h"

class ControlledKruskalsAlgorithm;
class KruskalsCodeGraphicsItem : public CodeGraphicsItem
{
public:
    KruskalsCodeGraphicsItem(ControlledKruskalsAlgorithm *algorithm, QGraphicsItem * parent = nullptr);
    virtual ~KruskalsCodeGraphicsItem();

    virtual void setLanguage(int language_type) override;

    virtual void setAlgorithm(ControlledAlgorithm *a_) override;
protected:
    virtual QList<QList<QGraphicsTextItem *>> getTextListItemsForPreviousState(int kruskals_algorithm_step) override;


private:
    ControlledKruskalsAlgorithm *kruskals_algorithm;

    QGraphicsTextItem * text_findset_def;
    QGraphicsTextItem * text_findset_comwhile;
    QGraphicsTextItem * text_findset_n;
    QGraphicsTextItem * text_findset_return;
    QGraphicsTextItem * text_union_def;
    QGraphicsTextItem * text_union_compred;
    QGraphicsTextItem * text_initpredfor;
    QGraphicsTextItem * text_pred;
    QGraphicsTextItem * text_PQfor;
    QGraphicsTextItem * text_enqueue;
    QGraphicsTextItem * text_MCST;
    QGraphicsTextItem * text_mainworkfor;
    QGraphicsTextItem * text_edge_if;
    QGraphicsTextItem * text_appendunion;
    QGraphicsTextItem * text_else;
    QGraphicsTextItem * text_ignorepass;
};

#endif // KRUSKALSCODEGRAPHICSITEM_H
