#include "custom_graphs.h"
#include "graph.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>

CustomGraphs *custom_graphs = nullptr;

CustomGraphs::CustomGraphs()
{
    load();
}

CustomGraphs::~CustomGraphs()
{
    save();
}

void CustomGraphs::load()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    if(settings.value("custom_graphs_saved").toBool())
        for (QJsonValue graph_value : settings.value("custom_graphs").toJsonArray())
            graphs << Graph::fromJson(graph_value.toObject());
}

void CustomGraphs::save()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    QJsonArray graphs_array;
    for (Graph *g : graphs)
        graphs_array.append(g->toJson());
    settings.setValue("custom_graphs", graphs_array);
    settings.setValue("custom_graphs_saved", true);
}

bool CustomGraphs::add(Graph *new_g)
{
    for (Graph *g : graphs)
        if (g->name() == new_g->name())
            return false;
    graphs << new_g;
    save();
    return true;
}

bool CustomGraphs::removeByName(QString graph_name)
{
    for (Graph *g : graphs) {
        if (g->name() == graph_name) {
            if (graphs.removeOne(g)) {
                save();
                //delete g;
                return true;
            }
        }
    }


    return false;
}


