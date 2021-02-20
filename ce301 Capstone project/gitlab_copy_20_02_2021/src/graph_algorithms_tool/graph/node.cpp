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

#include "graph/edge.h"
#include "graph/node.h"
#include "graphics_view.h"
#include "graph/graph.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <QDebug>

#include "colors.h"
#include "functions.h"


static const qreal radius_multiplier = 1; //1.3;
//static const QColor DEFAULT_COLOR = Qt::yellow;

Node::Node(QString label_)
    :
      //graph(graphWidget),
      label(label_),
      radius(10),
      v_entry_is_hoovered(false),
      highlighted(false),
      highlight_color(COLOR_HIGHLIGHT)
{
    /* Default "ItemIsMovable" won't work properly when it's child of Graph
       So "is_moving" member is implemented instead and moving is handled within the following callbacks:
       - onMousePress
       - onMouseMove
       - onMouseRelease */
    //setFlag(ItemIsMovable);
    setFlag(ItemIsMovable,false);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    setAcceptHoverEvents(true);

    //    qDebug() << "label = " << label_ << ", id = " << id << " , highest_id = " << highest_id ;
    resetColor();
}

Node::~Node()
{
    for (Edge *edge : edge_list) {
        Node *potentially_lone_node = this == edge->sourceNode() ? edge->destNode() : edge->sourceNode();
        delete edge;
        if (potentially_lone_node->edges().isEmpty())
            delete potentially_lone_node;
    }
}

void Node::addEdge(Edge *edge)
{
    edge_list << edge;
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    if (edge_list.contains(edge))
        edge_list.removeOne(edge);
}

QList<Edge *> Node::edges() const
{
    return edge_list;
}

QList<Node *> Node::adjacentNodes() const
{
    QList<Node *> adjacent_nodes;
    for (Edge *edge : edge_list)
        adjacent_nodes << (edge->sourceNode() == this ? edge->destNode() : edge->sourceNode());

    return adjacent_nodes;
}

QString Node::adjacentNodesAsString() const
{
    QString ret;
    QList<Node *> adjacent_nodes = adjacentNodes();
    for (Node *n : adjacent_nodes)
        ret += n->getLabel() + (n == adjacent_nodes.last() ? "" : ", ");
    return ret;
}

Edge *Node::edgeWith(Node *node) const
{
    for (Edge *edge : edge_list)
        if (edge->sourceNode() == node || edge->destNode() == node)
            return edge;
    return nullptr;
}

/*
void Node::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        Node *node = dynamic_cast<Node *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // Now subtract all forces pulling items together
    double weight = (edge_list.size() + 1) * 10;
    foreach (Edge *edge, edge_list) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}


bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
*/


QRectF Node::boundingRect() const
{
    qreal adjust = 4;
    return QRectF (-radius - adjust, -radius - adjust, (radius + adjust)*2, (radius + adjust)*2);

    /*int effective_radius = (is_hovered || isSelected()) ? radius*radius_multiplier : radius;
    return QRectF( -effective_radius - adjust, -effective_radius - adjust, effective_radius*2 + adjust*2, effective_radius*2 + adjust*2);*/
}


QPainterPath Node::shape() const
{
    QPainterPath path;

    //int effective_radius = (is_hovered || isSelected()) ? radius*radius_multiplier : radius;
    //path.addEllipse(-effective_radius, -effective_radius, effective_radius*2, effective_radius*2);
    path.addEllipse(-radius, -radius, radius*2, radius*2);
    return path;
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{

    bool is_hovered = option->state & QStyle::State_MouseOver;
    // shadow
    /*painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);*/


    //QRadialGradient gradient(-3, -3, 10);

    // sunken means it's dragged/clicked
    /*if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);*/

    if (highlighted) {
        painter->setBrush(highlight_color);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(boundingRect());
    }

    painter->setBrush(color.lighter());

    QPen pen(Qt::black, 1);

    if (isSelected() || is_hovered || v_entry_is_hoovered)
        pen.setWidth(3);

    painter->setPen(pen);

    //int effective_radius = (is_hovered || isSelected()) ? radius*radius_multiplier : radius;
    //QRectF rect(-effective_radius, -effective_radius, effective_radius*2, effective_radius*2);
    QRectF rect(-radius, -radius, radius*2, radius*2);
    painter->drawEllipse(rect);
    painter->drawText(rect, Qt::AlignCenter, label);

    //painter->fillRect(QRectF(boundingRect().bottomRight(), QSizeF(15.0,15.0)), Qt::black);
}

QString Node::colouredLabel()
{
    return colouredHtml(label, color == COLOR_NODE_DEFAULT ? Qt::black : color);
}

Graph * Node::parentGraph()
{
    if (Graph *graph = dynamic_cast<Graph *>(parentItem()))
        return graph;
    return nullptr;
}

void Node::setColor(QColor c)
{
    color = c;
    update();
}

QColor Node::getColor()
{
    return color;
}

void Node::resetColor()
{
    color = COLOR_NODE_DEFAULT;
    update();
}

void Node::resetStyling()
{
    resetColor();
    setHighlighted(false);
}

void Node::setVentryHovered(bool is_hovered)
{
    v_entry_is_hoovered = is_hovered;
    update();
}

void Node::setHighlighted(bool is_highlighted)
{
    highlighted = is_highlighted;
    highlight_color = COLOR_HIGHLIGHT;
    update();
}

void Node::setHighlighted(QColor clr)
{
    highlighted = true;
    highlight_color = clr;
    update();
}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edge_list)
            edge->adjust();

        if (QGraphicsScene *scene = this->scene())
            scene->update();
        break;
    default:
        break;
    };

    return MovableGraphicsItem::itemChange(change, value);
}

/*
void Node::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    prepareGeometryChange();
    scene()->update();
    MovableGraphicsItem::hoverEnterEvent(event);
}

void Node::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    prepareGeometryChange();
    scene()->update();
    MovableGraphicsItem::hoverLeaveEvent(event);
}*/


void Node::focusOutEvent(QFocusEvent *event)
{
    prepareGeometryChange();
    scene()->update();
    MovableGraphicsItem::focusOutEvent(event);
}

/*void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}*/

/*void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_moved)
        setPos(event->scenePos() - parentGraph()->scenePos() - grabbed_offset);
    return QGraphicsItem::mouseMoveEvent(event);
}*/



/*void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    return MovableGraphicsItem::mouseReleaseEvent(event);
}*/
