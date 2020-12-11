#include "graph/edge.h"
#include "graph/edge_text_item.h"
#include "graph/graph.h"
#include "graph/node.h"

#include <QKeyEvent>
#include <QDebug>
#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QtGui>

EdgeTextItem::EdgeTextItem(QString text, Edge *parent)
    : QGraphicsTextItem(text, dynamic_cast<QGraphicsItem *>(parent)),
      previous_value("1")
{
    document()->setDefaultStyleSheet("p { margin: 0; }");

    setFont(QFont());
    setTextInteractionFlags(Qt::TextEditorInteraction);
    edge = parent;
    resetColor();
}

void EdgeTextItem::setColor(QColor c)
{
    color_ = c;
    update();
}

void EdgeTextItem::resetColor()
{
    color_ = Qt::white;
    update();
}

void EdgeTextItem::focusInEvent(QFocusEvent *event)
{
    while (Node *selected_node = dynamic_cast<Graph*>(edge->parentItem())->selectedNode())
        selected_node->setSelected(false);

    previous_value = toPlainText();
    setPlainText("");
    edge->adjust();
    update();
    QGraphicsTextItem::focusInEvent(event);
}

void EdgeTextItem::focusOutEvent(QFocusEvent *event)
{
    QString digits_only = this->toPlainText().remove(QRegExp("[^0-9]"));
    if (digits_only.isEmpty())
        digits_only = previous_value;
    setPlainText(digits_only);
    edge->adjust();
    scene()->update();
    emit textEntered(digits_only);

    QGraphicsTextItem::focusOutEvent(event);
}


void EdgeTextItem::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (key == Qt::Key_Escape) {
        setPlainText("");
        clearFocus();
        return;
    }

    if (key == Qt::Key_Return) {
        clearFocus();
        return;
    }

    QGraphicsTextItem::keyPressEvent(event);
    if (edge)
        edge->adjust();
    scene()->update();
}

void EdgeTextItem::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyReleaseEvent(event);
}


void EdgeTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->fillRect(boundingRect(), color.lighter());
    QGraphicsTextItem::paint(painter, option, widget);
}
