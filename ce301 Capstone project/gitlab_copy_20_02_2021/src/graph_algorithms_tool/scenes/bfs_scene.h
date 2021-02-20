#ifndef BFSSCENE_H
#define BFSSCENE_H

#include "abstract_scene.h"
#include "algorithms/controlled_bfs_algorithm.h"


class VtableGraphicsItem;
class BfsCodeGraphicsItem;
class QueueGraphicsItem;

class BfsScene : public AbstractScene
{
public:
    BfsScene(QObject *parent, Graph *g = nullptr);
    virtual ~BfsScene() override;

    // QGraphicsScene interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    ControlledBfsAlgorithm *bfs_algorithm;
    //BfsCodeGraphicsItem * code_graphics_item;
    VtableGraphicsItem * vtable_graphics_item;
    QueueGraphicsItem *queue_graphics_item;

    virtual void onAlgorithmStart() override;
    virtual void onAlgorithmStep() override;
    virtual void onAlgorithmEnd() override;
    virtual void onAlgorithmEvent() override;
};

#endif // BFSSCENE_H
