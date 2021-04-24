#include "kruskals_code_graphics_item.h"
#include "code_items/code_graphics_item.h"
#include "algorithms/controlled_kruskals_algorithm.h"
#include "functions.h"
#include "colors.h"

KruskalsCodeGraphicsItem::KruskalsCodeGraphicsItem(ControlledKruskalsAlgorithm *a_, QGraphicsItem * parent)
    : CodeGraphicsItem(a_, parent),
      kruskals_algorithm(a_),
      text_findset_def(new QGraphicsTextItem(this)),
      text_findset_comwhile(new QGraphicsTextItem(this)),
      text_findset_n(new QGraphicsTextItem(this)),
      text_findset_return(new QGraphicsTextItem(this)),
      text_union_def(new QGraphicsTextItem(this)),
      text_union_compred(new QGraphicsTextItem(this)),
      text_initpredfor(new QGraphicsTextItem(this)),
      text_pred(new QGraphicsTextItem(this)),
      text_PQfor(new QGraphicsTextItem(this)),
      text_enqueue(new QGraphicsTextItem(this)),
      text_MCST(new QGraphicsTextItem(this)),
      text_mainworkfor(new QGraphicsTextItem(this)),
      text_edge_if(new QGraphicsTextItem(this)),
      text_appendunion(new QGraphicsTextItem(this)),
      text_else(new QGraphicsTextItem(this)),
      text_ignorepass(new QGraphicsTextItem(this))
{
    text_list << text_findset_def
              << text_findset_comwhile
              << text_findset_n
              << text_findset_return
              << text_union_def
              << text_union_compred
              << text_initpredfor
              << text_pred
              << text_PQfor
              << text_enqueue
              << text_MCST
              << text_mainworkfor
              << text_edge_if
              << text_appendunion
              << text_else
              << text_ignorepass;

    indents_list << 0 // text_findset_def
                 << 1 // text_findset_comwhile
                 << 2 // text_findset_n
                 << 1 // text_findset_return
                 << 0 // text_union_def
                 << 1 // text_union_compred
                 << 0 // text_initpredfor
                 << 1 // text_pred
                 << 0 // text_PQfor
                 << 1 // text_enqueue
                 << 0 // text_MCST
                 << 0 // text_mainwordkfor
                 << 1 // text_edge_if
                 << 2 // text_appendunion
                 << 1 // text_else
                 << 2 // text_ignorepass
                    ;

      setLanguage(CODE_LANG_PYTHON);

      createDock(this, "Pseudocode - Kruskals");
}


KruskalsCodeGraphicsItem::~KruskalsCodeGraphicsItem()
{
}

void KruskalsCodeGraphicsItem::setAlgorithm(ControlledAlgorithm *a_)
{
    algorithm = a_;
    kruskals_algorithm = dynamic_cast<ControlledKruskalsAlgorithm *>(a_);
}

void KruskalsCodeGraphicsItem::setLanguage(int language_type)
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
      text_findset_def->setHtml(py::kw("def") + py::fn(" findSet") + "(n):");
      text_findset_comwhile->setHtml(py::str("''' Finds sub-graph root node. '''") + "<br><br>" +
                                     py::kw("while") + " n != pred[n]:");
      text_findset_n->setHtml("n = pred[n]");
      text_findset_return->setHtml(py::kw("return") + " n");
      text_union_def->setHtml(py::kw("def") + py::fn(" union") + "(root_1, root_2):");
      text_union_compred->setHtml(py::str("''' See \"union by rank\" for better efficiency<br><br>"
                                          "due to faster \"findSet\" lookup. '''") + "<br><br>" +
                                          "pred[root_1] = root_2");
      text_initpredfor->setHtml("<br>" + py::com("#initialization") + "<br>"
                                "pred = {}<br><br>" +
                                py::kw("for") + " n " + py::kw("in") + " nodes:");
      text_pred->setHtml("pred[n] = n " + py::com("# each node becomes a sub-graph with 1 node"));
      text_PQfor->setHtml("PQ = PriorityQueue()<br><br>" +
                          py::kw("for") + " e " + py::kw("in") + " edges:");
      text_enqueue->setHtml("PQ.enqueue(e)");
      text_MCST->setHtml("MCST = []");
      text_mainworkfor->setHtml("<br>" + py::com("# main work") + "<br>" +
                                py::kw("for") + " MCST.size() &lt; node_count - 1:");
      text_edge_if->setHtml("edge = PQ.dequeue()<br><br>"
                            "root_1 = findSet(edge.node_1)<br><br>"
                            "root_2 = findSet(edge.node_2)<br><br>" +
                            py::kw("if") + " root_1 != root_2:");
      text_appendunion->setHtml("MCST.append(edge)<br><br>"
                                "union(root_1, root_2) " + py::com("# merge 2 sets into 1"));
      text_else->setHtml(py::kw("else") + ":");
      text_ignorepass->setHtml(py::com("# ignore this edge because it would create a cycle") + "<br>" +
                               py::kw("pass"));
  }

  CodeGraphicsItem::setLanguage(language_type);
}

QList<QList<QGraphicsTextItem *>> KruskalsCodeGraphicsItem::getTextListItemsForPreviousState(int kruskals_algorithm_state)
{
  QMap<int, QList<QList<QGraphicsTextItem *>>> state_items_map = {
      {KRUSKALS_STATE_INIT, {{text_initpredfor, text_pred, text_PQfor, text_enqueue, text_MCST}}},
      {KRUSKALS_STATE_EDGE_ADDED, {{text_findset_comwhile, text_findset_n, text_findset_return}, {text_union_compred}, {text_edge_if, text_appendunion}}},
      {KRUSKALS_STATE_EDGE_DROPPED, {{text_else, text_ignorepass}}},
  };

  if (state_items_map.keys().contains(kruskals_algorithm_state))
      return state_items_map[kruskals_algorithm_state];

  return QList<QList<QGraphicsTextItem *>>();
}
