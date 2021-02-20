#ifndef KRUSKAL_SCENE_H
#define KRUSKAL_SCENE_H

#include "scenes/abstract_scene.h"

class PriorityQueueGraphicsItem;
class ControlledKruskalsAlgorithm;
class VtableGraphicsItem;
class Graph;
class Node;

class KruskalScene : public AbstractScene
{
public:
    KruskalScene(QObject *parent = nullptr, Graph *g = nullptr);
    virtual ~KruskalScene() override;

private:
    ControlledKruskalsAlgorithm *kruskals_algorithm;
    QGraphicsTextItem *legend_text_item;
    VtableGraphicsItem * vtable_graphics_item;
    PriorityQueueGraphicsItem *pq_graphics_item;

    virtual void onAlgorithmStart() override;
    virtual void onAlgorithmStep() override;
    virtual void onAlgorithmEnd() override;
    virtual void onAlgorithmEvent() override;
};

#endif // KRUSKAL_SCENE_H
