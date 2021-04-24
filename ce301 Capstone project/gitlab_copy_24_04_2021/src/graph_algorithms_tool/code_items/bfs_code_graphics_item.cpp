#include "code_items/bfs_code_graphics_item.h"
#include "code_items/code_graphics_item.h"
#include "algorithms/controlled_bfs_algorithm.h"
#include "functions.h"
#include "colors.h"

BfsCodeGraphicsItem::BfsCodeGraphicsItem(ControlledBfsAlgorithm *a_, QGraphicsItem * parent)
    : CodeGraphicsItem(a_, parent),
    bfs_algorithm(a_),
    text_init(new QGraphicsTextItem(this)),
    text_while(new QGraphicsTextItem(this)),
    text_dequeue(new QGraphicsTextItem(this)),
    text_expanded(new QGraphicsTextItem(this)),
    text_for_v(new QGraphicsTextItem(this)),
    text_if(new QGraphicsTextItem(this)),
    text_dist(new QGraphicsTextItem(this)),
    text_pred(new QGraphicsTextItem(this)),
    text_state(new QGraphicsTextItem(this)),
    text_enqueue(new QGraphicsTextItem(this))
{

  text_list << text_init
            << text_while
            << text_dequeue
            << text_expanded
            << text_for_v
            << text_if
            << text_dist
            << text_pred
            << text_state
            << text_enqueue;

  indents_list << 0 // text_init
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

    createDock(this, "Pseudocode - BFS");
}

BfsCodeGraphicsItem::~BfsCodeGraphicsItem()
{
}

void BfsCodeGraphicsItem::setAlgorithm(ControlledAlgorithm *a_)
{
  algorithm = a_;
  bfs_algorithm = dynamic_cast<ControlledBfsAlgorithm *>(a_);
}

void BfsCodeGraphicsItem::setLanguage(int language_type)
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
      text_init->setHtml(py::kw("for") + " v " + py::kw("in") + " all_nodes:<br><br>"
                         "pred[v] = -1 " + py::com("# keeps track of preceding node<br><br>") +
                         "dist[v] = INT_MAX " + py::com("# keeps track of depth<br><br>") +
                         "state[v] = UNKNOWN<br><br><br><br>"
                         "Q = queue()<br><br>"
                         "Q.enqueue(start_node)<br><br>");
      text_while->setHtml(py::kw("while not") + " Q.is_empty():");
      text_dequeue->setHtml("u = Q.dequeue() " + py::com("# node to expand"));
      text_expanded->setHtml("state[u] = EXPANDED");
      text_for_v->setHtml(py::kw("for") + " v " + py::kw("in") + " u.neighbors(): " + py::com("# surrounding nodes"));
      text_if->setHtml(py::kw("if") + " state[v] == UNKNOWN:");
      text_dist->setHtml("dist[v] = dist[u] + 1");
      text_pred->setHtml("pred[v] = u");
      text_state->setHtml("state[v] = SEEN " + py::com("# avoids adding same node twice"));
      text_enqueue->setHtml("Q.enqueue(v)" + py::com("# add to the end of queue"));

  }

  CodeGraphicsItem::setLanguage(language_type);
}

QList<QList<QGraphicsTextItem *>> BfsCodeGraphicsItem::getTextListItemsForPreviousState(int bfs_algorithm_state)
{
  QMap<int, QList<QList<QGraphicsTextItem *>>> state_items_map = {
      {BFS_STATE_INIT, {{text_init, text_expanded}}},
      {BFS_STATE_ADJACENT_NODES_UPDATED, {{text_dequeue, text_expanded}}},
      {BFS_STATE_NEW_NODE_SELECTED, {{text_for_v, text_if, text_dist, text_pred, text_state, text_enqueue}}},
  };

  if (state_items_map.keys().contains(bfs_algorithm_state))
      return state_items_map[bfs_algorithm_state];

  return QList<QList<QGraphicsTextItem *>>();
}
