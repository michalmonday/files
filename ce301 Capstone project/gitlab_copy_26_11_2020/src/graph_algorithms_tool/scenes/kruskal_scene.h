#ifndef KRUSKAL_SCENE_H
#define KRUSKAL_SCENE_H

#include "scenes/abstract_scene.h"

class Graph;
class Node;

class KruskalScene : public AbstractScene
{
public:
    KruskalScene(QObject *parent = nullptr);

    // QGraphicsScene interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // KRUSKAL_SCENE_H
