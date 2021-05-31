#include "graph/edge.h"
#include "scenes/kruskal_scene.h"
#include "graph/node.h"
#include "graph/graph.h"
#include "algorithms/controlled_prims_algorithm.h"


#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

KruskalScene::KruskalScene(QObject *parent)
    : AbstractScene(parent)
{
    main_graph = new Graph();

    Edge *e1 = main_graph->addEdge("a", "b", 10);
    Edge *e2 = main_graph->addEdge("a", "c", 6);
    Edge *e3 = main_graph->addEdge("a", "d", 5);
    Edge *e4 = main_graph->addEdge("b", "d", 15);
    Edge *e5 = main_graph->addEdge("c", "d", 4);

    Node *node_a = e1->sourceNode();
    Node *node_b = e1->destNode();
    Node *node_c = e2->destNode();
    Node *node_d = e3->destNode();

    // position is relative to graph itself (not the scene)
    // this way I can use graph->setPos() to move whole graph to specific place
    node_a->setPos(-50, -50);
    node_b->setPos(50, -50);
    node_c->setPos(-50, 50);
    node_d->setPos(50, 50);

    addItem(main_graph);

    qDebug() << "main_graph bounding rect = " << main_graph->boundingRect();
}


void KruskalScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    AbstractScene::mousePressEvent(event);
}

void KruskalScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    AbstractScene::mouseReleaseEvent(event);
}


void KruskalScene::keyPressEvent(QKeyEvent *event)
{
    AbstractScene::keyPressEvent(event);
}


