#include "code_items/prims_code_graphics_item.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>

#include "algorithms/controlled_prims_algorithm.h"

#include "colors.h"
#include "functions.h"



/*
TODO:
There are 2 problems:
MCST stores edges, not nodes
Adjacency list stores nodes, not edges

*/

PrimsCodeGraphicsItem::PrimsCodeGraphicsItem(ControlledPrimsAlgorithm *a_, QGraphicsItem * parent)
    : CodeGraphicsItem(a_, parent),
      prims_algorithm(a_),
      text_select_random_node(new QGraphicsTextItem(this)),
      text_while_mcst_not_completed(new QGraphicsTextItem(this)),
      text_add_to_mcst(new QGraphicsTextItem(this)),
      text_update_nodes_loop(new QGraphicsTextItem(this)),
      text_update_nodes_loop_if(new QGraphicsTextItem(this)),
      text_update_nodes_loop_then(new QGraphicsTextItem(this)),
      text_get_lowest_positive_v(new QGraphicsTextItem(this))
{
    text_list << text_select_random_node
              << text_while_mcst_not_completed
              << text_add_to_mcst
              << text_update_nodes_loop
              << text_update_nodes_loop_if
              << text_update_nodes_loop_then
              << text_get_lowest_positive_v;

    indents_list << 0 // text_select_random_node
                 << 0 // text_while_mcst_not_completed
                 << 1 // text_add_to_mcst
                 << 1 // text_update_nodes_loop
                 << 2 // text_update_nodes_loop_if
                 << 3 // text_update_nodes_loop_then
                 << 1 // text_get_lowest_positive_v
                    ;

    setLanguage(CODE_LANG_PYTHON);

    createDock(this, "Pseudocode - Prims");
}

PrimsCodeGraphicsItem::~PrimsCodeGraphicsItem()
{
}

void PrimsCodeGraphicsItem::setAlgorithm(ControlledAlgorithm *a_)
{
    algorithm = a_;
    prims_algorithm = dynamic_cast<ControlledPrimsAlgorithm *>(a_);
}

void PrimsCodeGraphicsItem::setLanguage(int language_type)
{
    if (language_type == CODE_LANG_PSEUDOCODE) {
        text_select_random_node->setHtml("Select random node");
        text_while_mcst_not_completed->setHtml("While MCST isn't completed:");
        text_add_to_mcst->setHtml(QString("Add <span style='color:%1'>selected node</span> to MCST (set V[<span style='color:%1'>selected_node</span>] to <span style='color:%1'>0</span>)").arg(COLOR_MCST.name()));
        text_update_nodes_loop->setHtml(QString("For each edge between newly <span style='color:%1'>selected node</span> and <span style='color:%2'>adjacent nodes</span>:").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
        text_update_nodes_loop_if->setHtml(QString("If edge weight &lt; V[<span style='color:%2'>adjacent_node</span>]:").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
        text_update_nodes_loop_then->setHtml(QString("Set V[<span style='color:%2'>adjacent_node</span>] "
                                                    "to '(<span style='color:%2;'>edge_weight</span>, <span style='color:%1'>selected_node</span>)'").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
        text_get_lowest_positive_v->setHtml(QString("The lowest positive value from V table becomes the new <span style='color:%1'>selected node.</span>").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
    }

    if (language_type == CODE_LANG_PYTHON) {
        text_select_random_node->setHtml(QString("<span style='color:%1'>selected_node</span> = random.choice(nodes)").arg(COLOR_MCST.name()));//, COLOR_CONSIDERED.name()));
        text_while_mcst_not_completed->setHtml(QString("<span style='color:%1'>for</span> _<span style='color:%1'> in</span> nodes:").arg(COLOR_PYTHON_ORANGE.name()));
        text_add_to_mcst->setHtml(QString("V[<span style='color:%1'>selected_node</span>][0] = <span style='color:%1'>0</span> <span style='color:%2'>#includes node in MCST</span>").arg(COLOR_MCST.name(), COLOR_PYTHON_RED.name()));
        text_update_nodes_loop->setHtml(QString("<span style='color:%1'>for</span> <span style='color:%3'>edge</span> <span style='color:%1'>in</span> adjacency_list[<span style='color:%2'>selected_node</span>]:").arg(COLOR_PYTHON_ORANGE.name(), COLOR_MCST.name(), COLOR_CONSIDERED.name()));
        text_update_nodes_loop_if->setHtml(QString("<span style='color:%1'>if</span> <span style='color:%2'>edge.weight</span> &lt; V[<span style='color:%2'>adjacent_node</span>][0]:").arg(COLOR_PYTHON_ORANGE.name(), COLOR_CONSIDERED.name()));
        text_update_nodes_loop_then->setHtml(QString("V[<span style='color:%2'>adjacent_node</span>] = (<span style='color:%2;'>edge.weight</span>, <span style='color:%1;'>selected_node</span>)").arg(COLOR_MCST.name(), COLOR_CONSIDERED.name()));
        text_get_lowest_positive_v->setHtml(QString("<span style='color:%1'>selected_node</span> = minimalEntry(V)").arg(COLOR_MCST.name()));//, COLOR_CONSIDERED.name()));
    }

    CodeGraphicsItem::setLanguage(language_type);
}

QList<QList<QGraphicsTextItem *>> PrimsCodeGraphicsItem::getTextListItemsForPreviousState(int prims_algorithm_step)
{
    QMap<int, QList<QList<QGraphicsTextItem *>>> state_items_map = {
        {PRIMS_STATE_INIT, {{text_select_random_node, text_add_to_mcst}}},
        {PRIMS_STATE_NEW_NODE_ASSIGNED, {{text_add_to_mcst}, {text_get_lowest_positive_v}}},
        {PRIMS_STATE_NEW_NODE_INCLUDED, {{text_update_nodes_loop, text_update_nodes_loop_if, text_update_nodes_loop_then}}},
        //{PRIMS_STATE_ADJACENT_NODES_UPDATED, {text_get_lowest_positive_v}}
    };

    if (state_items_map.keys().contains(prims_algorithm_step))
        return state_items_map[prims_algorithm_step];

    return QList<QList<QGraphicsTextItem *>>();
}
