#ifndef DFSSCENE_H
#define DFSSCENE_H

#include "abstract_scene.h"
#include "algorithms/controlled_dfs_algorithm.h"

class VtableGraphicsItem;
class DfsCodeGraphicsItem;

class DfsScene : public AbstractScene
{
public:
    DfsScene(QObject *parent);
    virtual ~DfsScene() override;

    // QGraphicsScene interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    ControlledDfsAlgorithm *dfs_algorithm;

    DfsCodeGraphicsItem * code_graphics_item;

    VtableGraphicsItem * vtable_graphics_item;
    Dock *dock_vtable;

    virtual void onAlgorithmStart() override;
    virtual void onAlgorithmStep() override;
    virtual void onAlgorithmEnd() override;
    virtual void onAlgorithmEvent() override;
};

#endif // DFSSCENE_H
