#ifndef PRIMSCENE_H
#define PRIMSCENE_H

#include "scenes/abstract_scene.h"

#include "vtable_graphics_item.h"

class Dock;

class ControlledPrimsAlgorithm;
class LegendGraphicsItem;
class PrimsCodeGraphicsItem;

class PrimScene : public AbstractScene
{
public:
    PrimScene(QObject *parent = nullptr, Graph *g = nullptr);
    virtual ~PrimScene() override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    ControlledPrimsAlgorithm *prims_algorithm;
    QGraphicsTextItem *legend_text_item;
    VtableGraphicsItem * vtable_graphics_item;

    virtual void onAlgorithmStart() override;
    virtual void onAlgorithmStep() override;
    virtual void onAlgorithmEnd() override;
    virtual void onAlgorithmEvent() override;
};

#endif // PRIMSCENE_H



