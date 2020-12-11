#ifndef DIJKSTRASCENE_H
#define DIJKSTRASCENE_H

#include "abstract_scene.h"

#include "algorithms/controlled_dijkstra_algorithm.h"

class PriorityQueueGraphicsItem;
class VtableGraphicsItem;
class DijkstraCodeGraphicsItem;
class ShortestPathGraphicsItem;

class DijkstraScene : public AbstractScene
{
public:
    DijkstraScene(QObject *parent, Graph *g = nullptr);
    virtual ~DijkstraScene() override;

    // QGraphicsScene interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    ControlledDijkstraAlgorithm *dijkstra_algorithm;

    //DijkstraCodeGraphicsItem * code_graphics_item;


    //ShortestPathGraphicsItem *shortest_path_graphics_item;
    //Dock *dock_shortest_paths;
    VtableGraphicsItem * vtable_graphics_item;

    PriorityQueueGraphicsItem *pq_graphics_item;

    virtual void onAlgorithmStart() override;
    virtual void onAlgorithmStep() override;
    virtual void onAlgorithmEnd() override;
    virtual void onAlgorithmEvent() override;

    // adds total cost to a node weight label
    void updateEdgeTotalCost(Edge *e, QColor clr);

    // once algorithm is finished, the total costs should disappear
    void removeEdgeTotalCosts();
};

#endif // DIJKSTRASCENE_H
