/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graphics_view.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "graph/graph.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scenes/abstract_scene.h"
#include "scenes/kruskal_scene.h"
#include "scenes/prim_scene.h"
#include "scenes/dijkstra_scene.h"
#include "scenes/bfs_scene.h"
#include "scenes/dfs_scene.h"

#include <math.h>

#include <QDebug>
#include <QKeyEvent>
#include <QMainWindow>
#include <QTimer>
#include <QWindow>

#include "colors.h"
#include "functions.h"
#include "select_algorithm_widget.h"

#include <qmath.h>





GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    AbstractScene *scene = new PrimScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    //setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHints(renderHints() | QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    //scale(qreal(0.8), qreal(0.8));
    //setMinimumSize(400, 400);

    setUpdatesEnabled(true);

    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    //setFocus();
    //scene->setFocus();
    hide();

    //QTimer::singleShot(200, [&](){

    //});
}


void GraphicsView::setAlgorithm(const QString &algorithm_name, Graph *new_graph)
{
    QRectF previous_rect;

    //QRect g(geometry());
    //QTransform t(transform());

    QPointF previous_scene_center = mapToScene( viewport()->rect().center() );
    QPointF previous_main_graph_pos = QPointF(0,0);

    Graph *main_graph = nullptr;
    if (AbstractScene *previous_scene = dynamic_cast<AbstractScene*>(this->scene())) {
        main_graph = previous_scene->mainGraph();
        previous_main_graph_pos = main_graph->scenePos();
        previous_rect = previous_scene->sceneRect();

        //qDebug() << "Before: main_graph->scenePos() = " << main_graph->scenePos() << ", main_graph->pos()" << main_graph->pos();
        //qDebug() << "Before: previous_scene->sceneRect() = " << previous_scene->sceneRect();
        delete previous_scene;
    }
    //qDebug() << "After deletion: main_graph->scenePos() = " << main_graph->scenePos() << ", main_graph->pos()" << main_graph->pos();

    AbstractScene *scene = nullptr;

    if (algorithm_name == "Prims")
        scene = new PrimScene(this, new_graph);
    else if (algorithm_name == "Kruskals")
        scene = new KruskalScene(this, new_graph);
    else if (algorithm_name == "Dijkstra")
        scene = new DijkstraScene(this, new_graph);
    else if (algorithm_name == "Breadth first search")
        scene = new BfsScene(this, new_graph);
    else if (algorithm_name == "Depth first search")
        scene = new DfsScene(this, new_graph);

    if (select_algorithm_widget) {
        select_algorithm_widget->setAlgorithmBrief(scene->algorithm->brief());
        select_algorithm_widget->show();
    }

        //qDebug() << "GraphicsView::setAlgorithm unrecognized algorithm name, this will probably crash.";

    scene->setSceneRect(previous_rect);
    setScene(scene);
    //qDebug() << "After: scene->sceneRect() = " << scene->sceneRect();


    centerOn(previous_scene_center);
    //setGeometry(g);
   // setTransform(t);

    main_graph = scene->mainGraph();
    main_graph->editable(true);
    main_graph->setPos(previous_main_graph_pos);

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("last_selected_algorithm", algorithm_name);

    //qDebug() << "After setting new scene: main_graph->scenePos() = " << main_graph->scenePos() << ", main_graph->pos()" << main_graph->pos();
}

void GraphicsView::setGraph(Graph *g)
{
    setAlgorithm(select_algorithm_widget->selectedAlgorithm(), g);
    /*
    if (AbstractScene *scene = dynamic_cast<AbstractScene*>(this->scene())) {
        Graph *main_graph = scene->mainGraph();
        QPointF previous_main_graph_pos = main_graph->scenePos();
        scene->removeItem(main_graph);
    }*/
}


void GraphicsView::init()
{
    show();

    qDebug() << geometry();
    if (settings->value("view/was_saved").toBool()) {

        // restore zoom (works well only if no rotation/shear is applied)
        // I could find solution online but it's not needed for this project (at least so far)
        qreal m11 = settings->value("view/m11").toReal();
        QTransform t = transform();
        t.setMatrix(m11, t.m12(),t.m13(), t.m21(), m11, t.m23(), t.m31(), t.m32(), t.m33());
        setTransform(t);

        // if window is maximized and geometry is restored then:
        //      scrollbars position isn't loaded correctly (it's offset to one side or another)
        // if window is not maximized and geometry is restored then:
        //      QGraphicsView is not working well until resized
        // The conditional below is solving these 2 weird bugs.
        if (main_window->isMaximized())
            restoreGeometry(settings->value("view/geometry").toByteArray());

        centerOn(0,0);
        if (AbstractScene *scene = dynamic_cast<AbstractScene*>(this->scene()))
            scene->mainGraph()->setPos(settings->value("view/main_graph_viewport_offset").toPoint());

    } else {
        scale(2,2);
        centerOnMainGraph();
    }
}

GraphicsView::~GraphicsView()
{
    // PLAN:
    // don't save position of main_graph in scene
    // save position relative to viewport
    // this way upon next program start, the viewport can be centered
    // otherwise the main_graph may gradually move out of the scene (over multiple program launches)

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("view/was_saved", true);
    settings.setValue("view/m11", transform().m11());
    settings.setValue("view/geometry", saveGeometry());

    // save position of main_graph relative to center of viewport
    if (AbstractScene *scene = dynamic_cast<AbstractScene*>(this->scene()))
        settings.setValue("view/main_graph_viewport_offset", scene->mainGraph()->scenePos() - mapToScene(viewport()->rect()).boundingRect().center());
}


void GraphicsView::itemMoved()
{
    //if (!timerId)
    //    timerId = startTimer(1000 / 25);
}

void GraphicsView::adjustScene()
{
    qDebug() << mapToScene(viewport()->rect());
    QRectF extended_scene_rect = mapToScene(viewport()->rect()).boundingRect();
    setSceneRect(extended_scene_rect.united(sceneRect()));
    scene()->update();
    update();
}

void GraphicsView::centerOnMainGraph()
{
    if (AbstractScene *scene = dynamic_cast<AbstractScene*>(this->scene())) {
        Graph *graph = scene->mainGraph();
        centerOn(graph->scenePos());
    }

    // this solves the problem for some reason... (because centerOn doesn't work for some reason, probably because at this point the main window geometry isn't restored)
    /*QTimer::singleShot(1000, [&](){
        if (AbstractScene *scene = dynamic_cast<AbstractScene*>(this->scene())) {
            Graph *graph = scene->mainGraph();
            //centerOn(graph->boundingRect().center());
            centerOn(graph->scenePos());
            qDebug() << main_window->geometry();
        }
    });*/
}

void GraphicsView::ensureMainGraphVisible()
{
    if (AbstractScene *scene = dynamic_cast<AbstractScene*>(this->scene())) {
        const int margin = 50;
        Graph *graph = scene->mainGraph();
        ensureVisible(graph->boundingRect().translated(graph->scenePos()), margin, margin);

        /*
        int vw = viewport()->size().width();
        int vh = viewport()->size().height();
        int w = graph->boundingRect().size().width() + margin;
        int h = graph->boundingRect().size().height() + margin;
        if (w > vw || h > vh)
            fitInView(graph->boundingRect(), Qt::AspectRatioMode::KeepAspectRatio);
            */

    }
}


void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
}

/*
void GraphicsView::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = dynamic_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}
*/


#if QT_CONFIG(wheelevent)

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    int angle = event->angleDelta().y();
    qreal factor = qPow(1.0015, angle); // requires #include <qmath.h>

    // limit zoom-in
    if (transform().m11() > 10.0 && factor > 1.0)
        return;

    // limit zoom-out
    if (transform().m11() < 0.4 && factor < 1.0)
        return;

    scale(factor, factor);



    //qDebug() << "scale factor = " << factor << ", transform = " << transform();


    //ensureMainGraphVisible();

    //} else {
        //QGraphicsView::wheelEvent(event);
    //}
}

#endif

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    painter->fillRect(rect, COLOR_BACKGROUND);
    painter->drawRect(sceneRect);

    /* // draw point under cursor (requires updating scene at "mouseMoveEvent")
    painter->setBrush(Qt::black);
    QPointF cursor_pos = mapToScene(mapFromGlobal(QCursor::pos()));
    cursor_pos = GetClosestPoint(cursor_pos, 10);
    painter->drawEllipse(cursor_pos, 1,1);
    */
}


void GraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    ensureMainGraphVisible();
}


void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    //scene()->update();
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}


