#include "mainwindow.h"
#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>



/* NOTES */

/*

Edge should not be a parent of source and destination Nodes, because deleting edge would result in deletion
of both Nodes, but these Nodes could have multiple Edges, so it's a bad idea.



IMPORTANT
qgraphics_cast requires implementing "type()" method within subclass of QGraphicsItem

IMPORTANT
Passing Vtable object as const reference may result in a crash or weird behaviour if parent object gets deleted meanwhile.


Create AdjacencyList, AdjacencyMap and AdjacencyMatrix graphics items


Hover state can be checked directly from event (no need to override onHoverEnter/onHoverLeave methods)
Example:   bool hover = option.state & QStyle::State_MouseOver;



Add "Current algorithm" dock with:
- algorithm name
- current implementation (with its time complexity)

Add "Previous/current step description" dock
*/

#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
