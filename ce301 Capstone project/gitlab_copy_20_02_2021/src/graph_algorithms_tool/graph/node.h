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

#ifndef NODE_H
#define NODE_H

#include <QList>
#include "movable_graphics_item.h"

class Edge;
class GraphicsView;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Graph;

class Node : public MovableGraphicsItem
{
public:
    Node(QString label);
    virtual ~Node() override;

    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);

    QList<Edge *> edges() const;
    QList<Node *> adjacentNodes () const;
    QString adjacentNodesAsString() const;

    Edge *edgeWith(Node *) const;

    //enum { Type = UserType + 1 };
    //int type() const override { return Type; }

    //void calculateForces();
    //bool advance();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString getLabel() { return label; }

    QString colouredLabel();

    Graph *parentGraph();

    void setColor(QColor c);
    QColor getColor();
    void resetColor();
    void resetStyling();

    void setVentryHovered(bool is_hovered);
    void setHighlighted(bool is_highlighted);
    void setHighlighted(QColor highlight_color);

    // for simpler implementation of algorithms like Dijstra (where weight/cost is assoiated with nodes)
    void cost(int w) { cost_ = w; }
    int cost() { return cost_; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

private:
    QList<Edge *> edge_list;
    QPointF newPos;
    //GraphicsView *graph;

    QString label;

    int radius;

    QColor color;

    bool v_entry_is_hoovered;
    bool highlighted;
    QColor highlight_color;


    // some algorithms like Dijkstra, associate weight with nodes
    // having this variable here will simplify implementation of
    // some algorithms
    // e.g: https://stackoverflow.com/questions/38492612/how-do-you-order-objects-in-a-priority-queue-in-c/38492624
    int cost_;
    bool operator < (const Node &other) const { return cost_ < other.cost_; }
};

/*
// need to verify if it actually has any impact on priority queue storing Node pointers
bool operator < (const std::unique_ptr<Node>& left_node, const std::unique_ptr<Node>& right_node) {
    return left_node->cost() < right_node->cost();
}*/

#endif // NODE_H
