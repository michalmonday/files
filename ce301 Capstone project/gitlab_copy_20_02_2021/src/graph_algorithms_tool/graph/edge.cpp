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

#include <math.h>

#include <QPainter>

#include <QObject>
#include <QFontMetrics>
#include <QStyleOption>

#include "functions.h"
#include "colors.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
//static const int WEIGHT_RECT_SIZE = 20;


/* There are 2 constructors:
        - 1st allows supplying nodes by pointers (so they definitely were created before).
        - 2nd allows supplying nodes by label names (string like "a"), if such node does not exist then it's created.
*/
Edge::Edge(Node *src_node, Node *dst_node, int weight_)
    : source(src_node),
      dest(dst_node),
      weight(weight_),
      label_item(new EdgeTextItem(QString().sprintf("%d", weight), this)),
      //graph(graph_),
      arrowSize(0),
      v_entry_is_hoovered(false),
      highlighted(false),
      highlight_color(COLOR_HIGHLIGHT)
{
    resetStyling();
    //setAcceptedMouseButtons(nullptr);
    setAcceptHoverEvents(true);
    setFlag(ItemIsSelectable);

    source->addEdge(this);
    dest->addEdge(this);

    setZValue(source->zValue() - 0.1);
    adjust();

    //graph->scene()->addItem(this);

    label_item->setTextInteractionFlags(Qt::TextEditable);

    //label_item->setPos(pos() + QPointF(WEIGHT_RECT_SIZE/2, WEIGHT_RECT_SIZE/2));

    connect(label_item, &EdgeTextItem::textEntered, this, &Edge::setWeight_s);
}

Edge::~Edge()
{
    source->removeEdge(this);
    dest->removeEdge(this);
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

bool Edge::hasNode(Node *n)
{
    return source == n || dest == n;
}

void Edge::setWeight_s(QString w_str) { setWeight(w_str.toInt()); }
void Edge::setWeight(int w)
{
    weight = w;
    qDebug() << "New weight = " << weight;
}

int Edge::getWeight()
{
    return weight;
}

EdgeTextItem *Edge::getLabelItem()
{
    return label_item;
}

void Edge::resetStyling()
{
    if (label_item)
        label_item->resetColor();

    setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setHighlighted(false);
    setVentryHovered(false);
    highlight_color = COLOR_HIGHLIGHT;
}

void Edge::setVentryHovered(bool is_hovered)
{
    v_entry_is_hoovered = is_hovered;
    update();
}

void Edge::setHighlighted(bool is_highlighted)
{
    highlighted = is_highlighted;
    update();
}

void Edge::setHighlighted(QColor clr)
{
    highlight_color = clr;
    setHighlighted();
}


void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }

    if (label_item) {
        //QFontMetrics fm(label_item->font());
        //int text_width = fm.width(label_item->toPlainText());
        //label_item->setPos(midlinePoint() - QPointF(text_width/2, text_width/2));

        QPointF half_size = label_item->boundingRect().center();
        label_item->setPos(midlinePoint() - half_size);
    }
}

QString Edge::nodeLabels() const
{
    return source->getLabel() + " - " + dest->getLabel();
}

void Edge::setColor(QColor c)
{
    color = c;
    pen.setColor(c);

    // make it light (not just lighter) regardless of original color
    // so the text is visible
    label_item->setColor(QColor::fromHsv(c.hsvHue(), 190, 255));
    update();
}

void Edge::setPen(QPen p)
{
    pen = p;
    color = p.color();
    update();
}

void Edge::setPenWidth(int w)
{
    pen.setWidth(w);
    update();
}

QPointF Edge::midlinePoint()
{
    return (sourcePoint + destPoint) / 2;
}


QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    QPainterPath path = shape();
    QRectF rect = QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                             destPoint.y() - sourcePoint.y()))
               .normalized()
               .adjusted(-extra, -extra, extra, extra);
    path.addRect(rect);

    return path.boundingRect();
}

QPainterPath Edge::shape() const
{
    QPainterPath path(sourcePoint);
    //painter.
    path.lineTo(destPoint);
    return qt_graphicsItem_shapeFromPath(path, QPen(Qt::black, 10));
}


void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (!source || !dest)
        return;

    bool is_hovered = option->state & QStyle::State_MouseOver;

    QLineF line(sourcePoint, destPoint);

    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    if (highlighted) {
        painter->fillPath(shape(), highlight_color);
    }

    // Draw the line itself
    pen.setColor(color.lighter());
    painter->setPen(pen);

    if (is_hovered || isSelected() || v_entry_is_hoovered) {
        painter->setBrush(pen.color());
        painter->drawPath(shape());
    }
    else
        painter->drawLine(line);




    // Draw the arrows
    if ((bool)arrowSize) {
        double angle = ::acos(line.dx() / line.length());
        if (line.dy() >= 0)
            angle = TwoPi - angle;

        QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                      cos(angle + Pi / 3) * arrowSize);
        QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                      cos(angle + Pi - Pi / 3) * arrowSize);
        QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                                  cos(angle - Pi / 3) * arrowSize);
        QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                                  cos(angle - Pi + Pi / 3) * arrowSize);

        painter->setBrush(color.lighter());
        painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }

    QPointF midline_point = midlinePoint();
    //int text_width = (int)label_item->textWidth();
    //int text_height = (int)label_item->font();

    /*QFontMetrics fm(label_item->font());
    int text_width = fm.width(label_item->toPlainText());
    int text_height = fm.height();*/

    if (label_item->isVisible()) {
        QPointF half_size = label_item->boundingRect().center();
        int x = half_size.x();
        int y = half_size.y() * 0.9;
        QRectF mid_line_rect(midline_point.x()-x, midline_point.y()- y, x*2, y*2);
        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(label_item->color().lighter());
        painter->drawRect(mid_line_rect);
    }

    //painter->setBrush(Qt::blue);
    //painter->drawText(mid_line_rect, Qt::AlignCenter, QString().sprintf("%d", weight));

}


/*
void Edge::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void Edge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    is_hovered = false;
    update();

    // sometimes (rarely) there are small leftovers after the line becomes thin again
    if (QGraphicsItem * parent = parentItem())
        parent->update();
    QGraphicsItem::hoverLeaveEvent(event);
}*/
