#include "collection_graphics_item.h"
#include "graph/node.h"
#include "colors.h"
#include "dock.h"
#include "priority_queue_graphics_item.h"

#include <QEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>


const int half_margin = 3;
const int margin = half_margin * 2;

const int radius = 10;
const int diameter = radius * 2;

CollectionGraphicsItem::CollectionGraphicsItem(QGraphicsItem *parent)
    : QGraphicsItem(parent),
      Dockable(),
      expected_max_size(0)
{
    // MUST CALL init() FROM DERIVED CONSTRUCTORS
}

void CollectionGraphicsItem::init(QString dock_title)
{
    front_text = "  " + frontText() + " ->  ";
    back_text = "  <- " + backText() + "  ";
    QGraphicsSimpleTextItem * front_text_item = new QGraphicsSimpleTextItem(front_text);
    QGraphicsSimpleTextItem * back_text_item = new QGraphicsSimpleTextItem(back_text);
    front_text_size = front_text_item->boundingRect().bottomRight();
    back_text_size = back_text_item->boundingRect().bottomRight();

    createDock(this, dock_title);
    dock()->installEventFilter(this);
}

void CollectionGraphicsItem::reset()
{
    nodes_to_highlight.clear();
    edges_to_highlight.clear();
    pq_weights.clear();
}

bool CollectionGraphicsItem::eventFilter(QObject *object, QEvent *event)
{
    if (object == dock() && event->type() == QEvent::Resize)
        adjustToLeft();
    return false;
}

void CollectionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const int y = margin;

    std::function<void(Node*, QPoint)> draw_node = [&](Node *n, QPoint center) {
        painter->setBrush(n->getColor().lighter());
        painter->setPen({Qt::black, 1});
        painter->drawEllipse(center.x(), center.y(), diameter, diameter);
        painter->drawText(QRectF(center.x(), center.y(), diameter, diameter), Qt::AlignCenter, n->getLabel());
    };

    std::function<void(QRect)> draw_highlight = [&](QRectF rect_) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen({COLOR_HIGHLIGHT, 2});
        painter->drawRect(rect_.marginsAdded({half_margin,half_margin,half_margin,half_margin}));
    };

    int x = 0;
    painter->drawText(QPointF(x, y + diameter  - front_text_size.y()/2), front_text);
    x += front_text_size.x();
    int i = 0;
    for (Node *n : nodesCollection()) {

        if (nodes_to_highlight.contains(n))
            draw_highlight({x,y,diameter, diameter});

        draw_node(n, {x,y});

        // draw PQ weights underneath nodes
        if (!pq_weights.isEmpty()) {
            int weight = pq_weights[i++];
            QPen prev_pen = painter->pen();
            painter->setPen(COLOR_DIJKSTRA_OPTIMAL);
            painter->drawText(QRectF(x, y + diameter + margin, diameter, diameter), Qt::AlignCenter, QString::number(weight));
            painter->setPen(prev_pen);
        }

        // next ellipse left side
        x += margin + diameter;
    }

    for (Edge *e : edgesCollection()) {

        if (edges_to_highlight.contains(e)) {
            // should be more than *2 (should be containing both nodes of the edge + some margin)
            draw_highlight({x,y,diameter, diameter*3});
        }


        int line_x = x + diameter * 0.5;
        int y2 = y + diameter * 2;

        painter->setPen({Qt::black, 1});
        painter->drawLine(line_x, y, line_x, y2);
        draw_node(e->sourceNode(), {x, y});
        draw_node(e->destNode(), {x, y2});

        painter->setBrush(Qt::white);
        const int a = 2; // adjust
        painter->drawRect(QRectF(x+a, y+diameter+a, diameter-a*2, diameter-a*2));
        painter->drawText(QRectF(x, y, diameter, diameter*3), Qt::AlignCenter, QString::number(e->getWeight()));

        // next ellipse left side
        x += margin + diameter;
    }
    painter->drawText(QPointF(x, y + diameter - back_text_size.y()/2), back_text);

    // could draw "this is first item", "this is last item" indicators
}


QRectF CollectionGraphicsItem::boundingRect() const
{
    //static const std::function<QRectF(int)> calculate_rect = [&](int size){ return QRectF(0, 0, front_text_size.x() + back_text_size.x() + (radius * 2 + margin) * size, (radius + margin) * 2); };
    std::function<QRectF(int)> calculate_rect = [&](int size) {
        int x = front_text_size.x() + back_text_size.x() + (radius * 2 + margin) * size;
        int y = (radius + margin) * 2;

        if (dynamic_cast<PriorityQueueGraphicsItem *>((CollectionGraphicsItem*)this)) {
            y += diameter;

            if (!edgesCollection().isEmpty())
                y += diameter;
        }

        return QRectF(0, 0, x, y);
    };

    int x_units = std::max(nodesCollection().size(), edgesCollection().size());
    x_units = std::max(x_units, expected_max_size);
    return calculate_rect(x_units);
}

void CollectionGraphicsItem::adjustToLeft()
{
    if (Dock *d = dock())
        d->adjustSize();

    update();
    if (QGraphicsScene *s = scene()) {
        s->setSceneRect(boundingRect());
        s->update();
        QGraphicsView *view = s->views()[0];
        setPos(view->mapToScene(0,0).x(), scenePos().y());
    }
}
