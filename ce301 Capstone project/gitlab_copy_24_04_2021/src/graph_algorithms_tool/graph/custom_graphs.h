#ifndef CUSTOMGRAPHS_H
#define CUSTOMGRAPHS_H

#include <QList>
#include <QString>



class Graph;

class CustomGraphs
{
public:
    CustomGraphs();
    ~CustomGraphs();

    void load();
    void save();
    bool add(Graph *);
    bool removeByName(QString graph_name);
    QList<Graph *> all() { return graphs; }
private:
    QList<Graph *> graphs;
};

extern CustomGraphs *custom_graphs;

#endif // CUSTOMGRAPHS_H
