#ifndef DIJKSTRASCENE_H
#define DIJKSTRASCENE_H

#include "abstract_scene.h"

#include "algorithms/controlled_dijkstra_algorithm.h"

class DijkstraCodeGraphicsItem;
class ShortestPathGraphicsItem;
class DijkstraScene : public AbstractScene
{
public:
    DijkstraScene(QObject *parent);
    virtual ~DijkstraScene() override;

    // QGraphicsScene interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    ControlledDijkstraAlgorithm *dijkstra_algorithm;

    ShortestPathGraphicsItem *shortest_path_graphics_item;
    Dock *dock_shortest_paths;
    DijkstraCodeGraphicsItem * code_graphics_item;

    virtual void onAlgorithmStart() override;
    virtual void onAlgorithmStep() override;
    virtual void onAlgorithmEnd() override;
    virtual void onAlgorithmEvent() override;
};

#endif // DIJKSTRASCENE_H
