#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QMap>
#include <QList>

class Node;
typedef QMap<Node *, QPair<int, Node*>> Vtable;
typedef QList<QPair<Node *, QList<Node *>>> AdjacencyList;
typedef QMap<Node *, QList<Node *>> AdjacencyMap;
typedef QList<QList<int>> AdjacencyMatrix;

#endif // TYPEDEFS_H
