#ifndef VTABLEGRAPHICSITEM_H
#define VTABLEGRAPHICSITEM_H

#include "dock.h"
#include "movable_graphics_item.h"
#include "typedefs.h"

class MovableGraphicsItem;
class ControlledAlgorithm;
class Edge;
class VtableEntryGraphicsItem;

class VtableGraphicsItem : virtual public Dockable, public MovableGraphicsItem
{
public:
    VtableGraphicsItem(ControlledAlgorithm *algorithm, QString dock_title = "V table", QGraphicsItem * parent = nullptr);
    void setControlledAlgorithm(ControlledAlgorithm *algorithm);

    void updateValues(ControlledAlgorithm *algorithm = nullptr);

    //void setSingleVvalue(Node *key_node, QPair<int, Node*>value_pair);

    //void adjust();

    void reset();

    // useful for Prims algorithm
    void indicateMinEntry() { indicate_min_entry = true; }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    ControlledAlgorithm *algorithm;
    QList<Node*> updated_nodes;
    QList<Node*> previous_updated_nodes;
    //QList<Node*> last_iteration_nodes_with_replaced_optimal_edges;

    QMap<Node *, VtableEntryGraphicsItem *> entries;

    bool indicate_min_entry;
    VtableEntryGraphicsItem *minPositiveEntry();
};

#endif // VTABLEGRAPHICSITEM_H
