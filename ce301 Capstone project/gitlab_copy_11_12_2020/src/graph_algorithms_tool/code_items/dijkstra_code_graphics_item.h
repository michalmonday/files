#ifndef DIJKSTRACODEGRAPHICSITEM_H
#define DIJKSTRACODEGRAPHICSITEM_H

#include <QGraphicsItem>
#include "code_items/code_graphics_item.h"

class CodeGraphicsItem;
class ControlledDijkstraAlgorithm;

class DijkstraCodeGraphicsItem : public CodeGraphicsItem
{
public:
    DijkstraCodeGraphicsItem(ControlledDijkstraAlgorithm *algorithm, QGraphicsItem * parent = nullptr);
    virtual ~DijkstraCodeGraphicsItem();

    virtual void setLanguage(int language_type) override;

    virtual void setAlgorithm(ControlledAlgorithm *a_) override;
protected:
    virtual QList<QList<QGraphicsTextItem *>> getTextListItemsForPreviousState(int dijkstra_algorithm_step) override;


private:
    ControlledDijkstraAlgorithm *dijkstra_algorithm;

    QGraphicsTextItem * text_init;
    QGraphicsTextItem * text_while;
    QGraphicsTextItem * text_pq_poll_min;
    QGraphicsTextItem * text_for_v;
    QGraphicsTextItem * text_new_len;
    QGraphicsTextItem * text_if_new_len;
    QGraphicsTextItem * text_decrease_key;
    QGraphicsTextItem * text_dist;
    QGraphicsTextItem * text_pred;
};

#endif // DIJKSTRACODEGRAPHICSITEM_H
