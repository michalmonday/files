#include "code_items/dijkstra_code_graphics_item.h"
#include "code_items/code_graphics_item.h"
#include "algorithms/controlled_dijkstra_algorithm.h"
#include "functions.h"
#include "colors.h"

DijkstraCodeGraphicsItem::DijkstraCodeGraphicsItem(ControlledDijkstraAlgorithm *a_, QGraphicsItem * parent)
    : CodeGraphicsItem(a_, parent),
    dijkstra_algorithm(a_),
    text_init(new QGraphicsTextItem(this)),
    text_while(new QGraphicsTextItem(this)),
    text_pq_poll_min(new QGraphicsTextItem(this)),
    text_for_v(new QGraphicsTextItem(this)),
    text_new_len(new QGraphicsTextItem(this)),
    text_if_new_len(new QGraphicsTextItem(this)),
    text_decrease_key(new QGraphicsTextItem(this)),
    text_dist(new QGraphicsTextItem(this)),
    text_pred(new QGraphicsTextItem(this))
{
  text_list << text_init
            << text_while
            << text_pq_poll_min
            << text_for_v
            << text_new_len
            << text_if_new_len
            << text_decrease_key
            << text_dist
            << text_pred;

  indents_list << 0 // text_init
               << 0 // text_while
               << 1 // text_pq_poll_min
               << 1 // text_for_v
               << 2 // text_new_len
               << 2 // text_if_new_len
               << 3 // text_decrease_key
               << 3 // text_dist
               << 3 // text_pred
                  ;

    setLanguage(CODE_LANG_PYTHON);

    createDock(this, "Pseudocode - Dijkstra");
}

DijkstraCodeGraphicsItem::~DijkstraCodeGraphicsItem()
{
}

void DijkstraCodeGraphicsItem::setAlgorithm(ControlledAlgorithm *a_)
{
    algorithm = a_;
    dijkstra_algorithm = dynamic_cast<ControlledDijkstraAlgorithm *>(a_);
}

void DijkstraCodeGraphicsItem::setLanguage(int language_type)
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
      text_init->setHtml("PQ.insert(all_nodes) " + py::com(" # with INT_MAX") + "<br><br>PQ[start_node] = 0 " + py::com(" # put start node at PQ front"));
      text_while->setHtml(py::kw("while not") + " PQ.is_empty():");
      text_pq_poll_min->setHtml("u = PQ.pop() " + py::com("# node to expand"));
      text_for_v->setHtml(py::kw("for") + " v " + py::kw("in") + " u.neighbors(): " + py::com("# surrounding nodes"));
      text_new_len->setHtml("newLen = dist[u] + edge_weight(v,u)");
      text_if_new_len->setHtml(py::kw("if") + " newLen < dist[v]:");
      text_decrease_key->setHtml("PQ[v] = newLen");
      text_dist->setHtml("dist[v] = newLen");
      text_pred->setHtml("pred[v] = u");
  }

  CodeGraphicsItem::setLanguage(language_type);
}

QList<QList<QGraphicsTextItem *>> DijkstraCodeGraphicsItem::getTextListItemsForPreviousState(int dijkstra_algorithm_state)
{
  QMap<int, QList<QList<QGraphicsTextItem *>>> state_items_map = {
      {DIJKSTRA_STATE_INIT, {{text_init, text_pq_poll_min}}},
      {DIJKSTRA_STATE_ADJACENT_NODES_UPDATED, {{text_pq_poll_min}}},
      {DIJKSTRA_STATE_NEW_NODE_SELECTED, {{text_for_v, text_new_len, text_if_new_len, text_decrease_key, text_dist, text_pred}}},
  };

  if (state_items_map.keys().contains(dijkstra_algorithm_state))
      return state_items_map[dijkstra_algorithm_state];

  return QList<QList<QGraphicsTextItem *>>();
}
