#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QColor>
#include <QGraphicsItem>
#include <QPointF>
#include <QString>


class QPainterPath;
class QPen;
/*  qt_graphicsItem_shapeFromPath was copied from:  https://code.woboq.org/qt5/qtbase/src/widgets/graphicsview/qgraphicsitem.cpp.html
    The whole point of me using that function is to generate shape of line with certain width.
    Normally, when adding line to QPainterPath we can use:
        path->addLine(QLine(0,0))

    The problem is that there's no way to specify width of the line.
    So I'm using the function below to add width to a line shape.

    Why I want the line to have wider shape than default 1?
    To make it easily selectable, even if the mouse pointer is not directly over it.
    */
extern QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

extern QString colouredHtml(const QString &str, const char *html_color, bool bold = true);
extern QString colouredHtml(const QString &str, const QColor &color, bool bold = true);
extern QString setHtmlBold(const QString &str);
extern QString indent(int how_many = 1);
extern QPointF getClosestPoint(QPointF pos, int granularity = 10);

namespace py {
    extern QString com(QString comment_text);
    extern QString kw(QString keyword_text);
    extern QString fn(QString function_text);
    extern QString call(QString function_call);
    extern QString str(QString function_call);
};

//template <class K, class V>
//extern void decreaseKey(QMultiMap<K,V> &map, V value, K new_key);

extern QPixmap QPixmapFromItem(QGraphicsItem *item);

#endif // FUNCTIONS_H
