
#include <QPainterPath>
#include <QPen>
#include "functions.h"
#include "colors.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QMap>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

/*  qt_graphicsItem_shapeFromPath was copied from:  https://code.woboq.org/qt5/qtbase/src/widgets/graphicsview/qgraphicsitem.cpp.html
    The whole point of me using that function is to generate shape of line with certain width.
    Normally, when adding line to QPainterPath we can use:
        path->addLine(QLine(0,0))

    The problem is that there's no way to specify width of the line.
    So I'm using the function below to add width to a line shape.

    Why I want the line to have wider shape than default 1?
    To make it easily selectable, even if the mouse pointer is not directly over it.
    */
QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);
    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}


QString colouredHtml(const QString &str, const char *html_color, bool bold)
{
    if (!strcmp(html_color,"black"))
        bold = false;

    return QString("<span style='color:%1; %2'>%3</span>").arg(html_color, (bold ? "font-weight:bold;" : ""), str);
}

QString colouredHtml(const QString &str, const QColor &clr, bool bold)
{
    if (clr == Qt::black)
        bold = false;
    return QString("<span style='color:rgba(%1,%2,%3,%4); %5'>%6</span>")
            .arg(clr.red())
            .arg(clr.green())
            .arg(clr.blue())
            .arg((qreal)(clr.alpha()/255.0))
            .arg((bold ? "font-weight:bold;" : ""))
            .arg(str);
}

QString setHtmlBold(const QString &str)
{
    return QString("<span style='font-weight:bold;'>%1</span>").arg(str);
}

QString indent(int how_many)
{
    QString html;
    while(how_many--)
        html += "&nbsp;&nbsp;&nbsp;&nbsp;";
    return html;
}


/* Function below is based on the following post:
   https://stackoverflow.com/questions/29557459/round-to-nearest-multiple-of-a-number

    It's modified slightly because the original answer did not handle well the negative values.
*/
QPointF getClosestPoint(QPointF pos, int granularity) {
    int g = granularity;

    int x = abs(pos.x()) + g/2;
    x -= x % g;
    x *= pos.x() > 0 ? 1 : -1;

    int y = abs(pos.y()) + g/2;
    y -= y % g;
    y *= pos.y() > 0 ? 1 : -1;

    //qDebug() << "GetClosestPoint orig = " << pos << ", new = " << QPointF(x,y);
    return QPointF(x, y);
}

namespace py {
    QString com(QString comment_text) {
        return colouredHtml(comment_text, COLOR_PYTHON_RED, false);
    }

    QString kw(QString keyword_text) {
        return colouredHtml(keyword_text, COLOR_PYTHON_ORANGE, false);
    }

    QString fn(QString function_text) {
        return colouredHtml(function_text, COLOR_PYTHON_BLUE, false);
    }

    QString call(QString function_call) {
        return colouredHtml(function_call, COLOR_PYTHON_VIOLET, false);
    }

    QString str(QString function_call) {
        return colouredHtml(function_call, COLOR_PYTHON_GREEN, false);
    }
};

/*
template <class K, class V>
void decreaseKey(QMultiMap<K,V> &map, V value, K new_key) {
    for (K key : map.keys()) {
        if (map[key] == value) {
            map.remove(key, value);
            map[new_key] = value;
            return;
        }
    }
}*/

QPixmap QPixmapFromItem(QGraphicsItem *item) {
    QPixmap pixmap(item->boundingRect().size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QStyleOptionGraphicsItem opt;


    std::function<void(QGraphicsItem *, QPainter &, QStyleOptionGraphicsItem &)> paint_recursive = [&](QGraphicsItem *gi, QPainter &p, QStyleOptionGraphicsItem &op){
        QTransform transform = p.transform();
        p.translate(-gi->boundingRect().x() - gi->pos().x(),-gi->boundingRect().y() - gi->pos().y() );
        gi->paint(&p, &op);
        p.setTransform(transform);
        for (auto *ci : gi->childItems()) {
            paint_recursive(ci, p, op);
        }
    };

    QGraphicsItem *copy(item);
    paint_recursive(copy, painter, opt);
    return pixmap;
}
