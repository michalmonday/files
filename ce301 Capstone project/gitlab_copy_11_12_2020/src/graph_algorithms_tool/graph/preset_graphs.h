#ifndef PRESETGRAPHS_H
#define PRESETGRAPHS_H

#include <QString>

class Graph;
class PresetGraphs
{
public:
    PresetGraphs();
    static Graph *generateForAlgorithm(QString algorithm_name);
    static QList<Graph *> all();

private:
    static Graph *prims();
    static Graph *kruskals();
    static Graph *dijkstra();
    static Graph *bfs();
    static Graph *dfs();
    static Graph *circular();
    static Graph *tree();
    static Graph *treeLarge();
    static Graph *square5x5();
    static Graph *square4x4();
};

#endif // PRESETGRAPHS_H
