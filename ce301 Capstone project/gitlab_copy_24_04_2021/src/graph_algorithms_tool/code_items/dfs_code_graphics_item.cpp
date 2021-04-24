#include "code_items/dfs_code_graphics_item.h"
#include "code_items/code_graphics_item.h"
#include "algorithms/controlled_dfs_algorithm.h"
#include "functions.h"
#include "colors.h"

DfsCodeGraphicsItem::DfsCodeGraphicsItem(ControlledDfsAlgorithm *a_, QGraphicsItem * parent)
    : CodeGraphicsItem(a_, parent),
    dfs_algorithm(a_),
    text_init_1(new QGraphicsTextItem(this)),
    text_init_2(new QGraphicsTextItem(this)),
    text_init_3(new QGraphicsTextItem(this)),
    text_while(new QGraphicsTextItem(this)),
    text_pop(new QGraphicsTextItem(this)),
    text_expanded(new QGraphicsTextItem(this)),
    text_for_v(new QGraphicsTextItem(this)),
    text_if(new QGraphicsTextItem(this)),
    text_dist(new QGraphicsTextItem(this)),
    text_pred(new QGraphicsTextItem(this)),
    text_state(new QGraphicsTextItem(this)),
    text_push(new QGraphicsTextItem(this))
{

  text_list << text_init_1
                << text_init_2
            << text_init_3
            << text_while
                << text_pop
                << text_expanded
                << text_for_v
                    << text_if
                        << text_dist
                        << text_pred
                        << text_state
                        << text_push;

  indents_list << 0 // text_init_1
                  << 1 // text_init_2
               << 0 // text_init_3
               << 0 // text_while
                   << 1 // text_dequeue
                   << 1 // text_expanded
                   << 1 // text_for_v
                       << 2 // text_if
                           << 3 // text_dist
                           << 3 // text_pred
                           << 3 // text_state
                           << 3 // text_enqueue
                  ;

    setLanguage(CODE_LANG_PYTHON);

    createDock(this, "Pseudocode - DFS");
}

DfsCodeGraphicsItem::~DfsCodeGraphicsItem()
{
}

void DfsCodeGraphicsItem::setAlgorithm(ControlledAlgorithm *a_)
{
  algorithm = a_;
  dfs_algorithm = dynamic_cast<ControlledDfsAlgorithm *>(a_);
}

void DfsCodeGraphicsItem::setLanguage(int language_type)
{
  if (language_type == CODE_LANG_PSEUDOCODE) {

/*      text_pq_poll_min->setHtml("Select random node");
      text_for_v->setHtml("While MCST isn't completed:");
      text_new_len->setHtml(QString("Add <span style='color:%1'>selected node</span> to MCST (set V[<span style='color:%1'>selected_node</span>] to <span style='color:%1'>0</span>)").arg(COLOR_MCST.name()));
      text_if_new_len->setHtml(QString("For each edge between newly <span style='color:%1'>selected node</span> and <span style='color:%2'>adjacent nodes</span>:").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
      text_decrease_key->setHtml(QString("If edge weight &lt; V[<span style='color:%2'>adjacent_node</span>]:").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
      text_dist->setHtml(QString("Set V[<span style='color:%2'>adjacent_node</span>] "
                                                  "to '(<span style='color:%2;'>edge_weight</span>, <span style='color:%1'>selected_node</span>)'").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
      text_pred->setHtml(QString("The lowest positive value from V table becomes the new <span style='color:%1'>selected node.</span>").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));*/
  }

  if (language_type == CODE_LANG_PYTHON) {
      text_init_1->setHtml(py::kw("for") + " v " + py::kw("in") + " all_nodes:");
      text_init_2->setHtml("pred[v] = -1 " + py::com("# keeps track of preceding node<br><br>") +
                           "dist[v] = INT_MAX " + py::com("# keeps track of depth<br><br>") +
                           "state[v] = UNKNOWN<br>");
      text_init_3->setHtml("S = stack()<br><br>"
                           "S.push(start_node)<br>");
      text_while->setHtml(py::kw("while not") + " S.is_empty():");
      text_pop->setHtml("u = S.pop() " + py::com("# node to expand"));
      text_expanded->setHtml("state[u] = EXPANDED");
      text_for_v->setHtml(py::kw("for") + " v " + py::kw("in") + " u.neighbors(): " + py::com("# surrounding nodes"));
      text_if->setHtml(py::kw("if") + " state[v] == UNKNOWN:");
      text_dist->setHtml("dist[v] = dist[u] + 1");
      text_pred->setHtml("pred[v] = u");
      text_state->setHtml("state[v] = SEEN " + py::com("# avoids adding same node twice"));
      text_push->setHtml("S.push(v)" + py::com("# equivalent to appending to list"));

  }

  CodeGraphicsItem::setLanguage(language_type);
}

QList<QList<QGraphicsTextItem *>> DfsCodeGraphicsItem::getTextListItemsForPreviousState(int dfs_algorithm_state)
{
  QMap<int, QList<QList<QGraphicsTextItem *>>> state_items_map = {
      {DFS_STATE_INIT, {{text_init_1, text_init_2, text_init_3, text_expanded}}},
      {DFS_STATE_ADJACENT_NODES_UPDATED, {{text_pop, text_expanded}}},
      {DFS_STATE_NEW_NODE_SELECTED, {{text_for_v, text_if, text_dist, text_pred, text_state, text_push}}},
  };

  if (state_items_map.keys().contains(dfs_algorithm_state))
      return state_items_map[dfs_algorithm_state];

  return QList<QList<QGraphicsTextItem *>>();
}
