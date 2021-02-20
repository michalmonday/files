#include "graph/graph.h"
#include "graph/edge.h"
#include "graph/node.h"
#include "graph/custom_graphs.h"

#include "edge.h"
#include "node.h"
#include "select_algorithm_widget.h"
#include "select_graph_widget.h"
#include "mainwindow.h"


#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonObject>
#include <QtMath>
#include <QInputDialog>
#include <QDir>
#include "select_graph_widget.h"

#include <QJsonDocument>
#include <algorithm>


Graph::Graph(QGraphicsItem *parent)
    : MovableGraphicsItem (parent),
      highest_node_id(0),
      is_editable(true),
      weights_shown(true),
      name_("graph_" + QString::number((long long)this))
{   
    setZValue(3);
    setFlag(ItemIsFocusable);
    hasBorder(false);

    //setHandlesChildEvents(false);
    //setFlag(ItemSendsGeometryChanges);
}

QList<Node *> Graph::nodes(bool sorted) const
{
    QList<Node *> nodes;
    for (QGraphicsItem *item : childItems())
        if (Node *node = dynamic_cast<Node *>(item))
            nodes << node;
    if (sorted)
        std::sort(nodes.begin(), nodes.end(), [](Node*n, Node*n2){ return n->getLabel() < n2->getLabel(); });

    return nodes;
}

QList<Edge *> Graph::edges() const
{
    QList<Edge *> edges;
    for (QGraphicsItem *item : childItems())
        if (Edge *edge = dynamic_cast<Edge *>(item))
            edges << edge;
    return edges;
}

/*
adjacency_list = [
    (1st node, [all nodes connected to 1st node])
    (2nd node, [all nodes connected to 2nd node])
    (3rd node, [all nodes connected to 3rd node])
    ...
    ]

It doesn't contain weights (it contains pointers to nodes, so getting weights can be done easily anyway).
*/
//QList<QPair<Node *, QList<Node *>>> Graph::adjacencyList()
AdjacencyList Graph::adjacencyList()
{
    QList<QPair<Node *, QList<Node *>>> adjacency_list;
    for (Node *node : nodes())
        adjacency_list.append({node, node->adjacentNodes()});

    qDebug() << adjacency_list;
    return adjacency_list;
}

AdjacencyMap Graph::adjacencyMap()
{
    QMap<Node *, QList<Node *>> adjacency_map;
    for (Node *node : nodes())
        adjacency_map[node] = node->adjacentNodes();
    return adjacency_map;
}

//QList<QList<int>> Graph::adjacencyMatrix()
AdjacencyMatrix Graph::adjacencyMatrix()
{
    QList<QList<int>> adjacency_matrix;

    const QList<Node *> all_nodes = nodes();
    const int nodes_size = all_nodes.size();

    for (int i = 0; i < nodes_size; i++) {
        Node *node = all_nodes[i];
        QList<int> row;
        for (Node *adjacent_node : all_nodes) {
            Edge *edge = node->edgeWith(adjacent_node);
            row << ((adjacent_node == node || edge == nullptr) ? 0 : edge->getWeight() );
        }
        adjacency_matrix << row;
    }

    qDebug() << adjacency_matrix;
    return adjacency_matrix;
}

Node *Graph::addNode()
{
    for (const NodeLabel &label : node_labels_map.keys())
        if (!node_labels_map[label])
            return addNode(label);
    if (!node_labels_map.isEmpty())
        return addNode({node_labels_map.lastKey().id + 1});
    return addNode({"a",0});
}

Node * Graph::addNode(const NodeLabel &label)
{
    node_labels_map[label] = true;
    return getNodeForLabel(label.q_string);
}

Edge * Graph::addEdge(QString src_label, QString dst_label, int weight)
{
    Edge *edge = new Edge(getNodeForLabel(src_label), getNodeForLabel(dst_label), weight);
    edge->setParentItem(this);
    edge->weightVisible(weights_shown);
    return edge;
}

Edge *Graph::addEdgeByCloning(Edge *e)
{
    Edge *new_e = addEdge(e->sourceNode()->getLabel(), e->destNode()->getLabel(), e->getWeight());
    new_e->weightVisible(e->weightVisible());
    new_e->sourceNode()->setPos(e->sourceNode()->pos());
    new_e->destNode()->setPos(e->destNode()->pos());
    return new_e;
}

void Graph::removeNode(Node *node)
{
    // destructor of Node handles deleting appropriate Edges
    // destructor of Edge handles deleting its' reference in both Nodes (source and destination)
    if (childItems().contains(node)) {
        QList<Edge *> edges = node->edges();
        node->setParentItem(nullptr);
        node_labels_map[node->getLabel()] = false;
        delete node;
    }
    prepareGeometryChange();
}

void Graph::removeEdge(Edge *edge)
{
    Node * src_node = edge->sourceNode();
    Node * dst_node = edge->destNode();

    if (edge->parentItem() == this) {
        edge->setParentItem(nullptr);
        delete edge;

        // destructor of Edge already removed it from edge list of src_node and dst_node

        if (src_node->edges().isEmpty())
            removeNode(src_node);

        if (dst_node->edges().isEmpty())
            removeNode(dst_node);
    }
}

Node *Graph::selectedNode()
{
    for (Node *n : nodes())
        if (n->isSelected())
            return n;
    return nullptr;
}

QList<Node *> Graph::selectedNodes()
{
    QList<Node*> nodes_;
    for (Node *n : nodes())
        if (n->isSelected())
            nodes_.append(n);
    return nodes_;
}

Edge *Graph::selectedEdge()
{
    for (Edge *e : edges())
        if (e->isSelected())
            return e;
    return nullptr;
}

QList<Edge *> Graph::selectedEdges()
{
    QList<Edge *> edges_;
    for (Edge *e : edges())
        if (e->isSelected())
            edges_.append(e);
    return edges_;
}

Node *Graph::selectedNodeOrRandom()
{
    if (Node * n = selectedNode())
        return n;

    if (use_suggested_graphs)
        if (Node *n = getNodeForLabel("a"))
            return n;


    QList<Node*> nodes_ = nodes();
    return  nodes_[rand() % nodes_.size()];
}

Edge *Graph::selectedEdgeOrRandom()
{
    if (Edge * e = selectedEdge())
        return e;

    // if node is selected then use random (first in memory) edge near it
    if (Node *n = selectedNode()) {
        const QList<Edge*> &edges_ = n->edges();
        if (!edges_.isEmpty())
            return edges_.first();
    }

    // completely random edge

    QList<Edge*> edges_ = edges();
    return  edges_[rand() % edges_.size()];
}

void Graph::resetColours()
{
    for (Node *n : nodes())
        n->resetStyling();

    for (Edge *e : edges())
        e->resetStyling();
}

void Graph::deselect()
{
    setSelected(false);
    for (Node *n : selectedNodes())
        n->setSelected(false);

    for (Edge *e : selectedEdges())
        e->setSelected(false);
}

void Graph::editable(bool is_editable_)
{
    is_editable = is_editable_;

    for (Edge *edge : edges()) {
        Qt::TextInteractionFlags flags = edge->getLabelItem()->textInteractionFlags();

        if (is_editable)
            flags |= Qt::TextInteractionFlag::TextEditable;
        else
            flags &= ~Qt::TextInteractionFlag::TextEditable;

        edge->getLabelItem()->setTextInteractionFlags(flags);
    }

}

bool Graph::editable()
{
    return is_editable;
}


// based on this answer: https://stackoverflow.com/a/55655445/4620679
// from this question:   https://stackoverflow.com/questions/55655137/how-do-i-create-a-dynamic-alphabet-id-system
QString Graph::IdToLabel(int id_)
{
    static const char * alphabet = "abcdefghijklmnopqrstuvwxyz";
    static const int alphabet_sz = 26;
    if (id_ < alphabet_sz)
        return QString().sprintf("%c", alphabet[id_]);
    return IdToLabel(qFloor(id_ / alphabet_sz) - 1) + IdToLabel(id_ % alphabet_sz);
}


int Graph::LabelToId(QString label_)
{
    static const int alphabet_sz = 26;

    int sum = 0;
    int mult = label_.size();
    for (QChar c : label_) {
        int i = c.toLatin1() - 'a';
        int j = pow(alphabet_sz, --mult);
        sum += (i+1) * j - (mult?0:j); // conditional needed because 'a' is 0
    }
    return sum;
}

QString Graph::IncLabel(QString label) {
    return IdToLabel(LabelToId(label) + 1);
}


void Graph::weightsShown(bool state)
{
    weights_shown = state;
    for (Edge *e : edges()) {
        e->weightVisible(weights_shown);
    }
}

bool Graph::has(Edge *e)
{
    if (!e)
        return false;
    for (QGraphicsItem *item : childItems())
        if (e == dynamic_cast<Edge *>(item))
            return true;
    return false;
}

bool Graph::has(Node *n)
{
    if (!n)
        return false;
    for (QGraphicsItem *item : childItems())
        if (n == dynamic_cast<Node *>(item))
            return true;
    return false;
}

void Graph::printExportCode()
{
    int i = 0;
    for (Edge *e : edges()) {
        Node *ns = e->sourceNode();
        Node *nd = e->destNode();
        qDebug().noquote() << QString("g->addEdge(\"%2\", \"%3\", %4);") // Edge *e%1 =
                    //.arg(i++)
                    .arg(ns->getLabel())
                    .arg(nd->getLabel())
                    .arg(e->getWeight());
    }

    QPoint offset = nodesOffsetFromCenter();

    qDebug() << " ";
    for (Node *n : nodes(/*sorted*/true)) {
        qDebug().noquote() << QString("g->getNodeForLabel(\"%1\")->setPos(%2, %3);")
                    .arg(n->getLabel())
                    .arg(n->pos().x() - offset.x())
                    .arg(n->pos().y() - offset.y());
    }
}



Node * Graph::getNodeForLabel(QString label) {
    foreach (QGraphicsItem *item, childItems()) {
        if (Node * node = dynamic_cast<Node*>(item))
            if (node->getLabel().compare(label) == 0)
                return node;
    }
    Node *new_node = new Node(label);
    node_labels_map[label] = true;
    new_node->setParentItem(this);

    //int node_id = LabelToId(label);
    return new_node;
}


void Graph::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() == Qt::MouseButton::RightButton) {
        printExportCode();
        qDebug() << toJson();


        bool ok = true, duplicate_found = true;
        QString label = "Graph name:";

        Graph *g = clone(/*centered*/ true);
        g->node_labels_map = node_labels_map;

        while (ok && duplicate_found) {
            QString text = QInputDialog::getText(main_window, "Input graph name",
                                                 label, QLineEdit::Normal,
                                                 name(), &ok);
            if (ok && !text.isEmpty()) {
                g->name(text);
                if (custom_graphs->add(g)) {
                    duplicate_found = false;
                    select_graph_widget->addGraph(g);
                }
                else {
                    label = QString("Graph with that name '%1' already exists, try different one:")
                            .arg(text);
                }
            }
        }

        if (!ok)
            delete g;
    }

    while (Node *selected_node = selectedNode())
        selected_node->setSelected(false);

    while (Edge *selected_edge = selectedEdge())
        selectedEdge()->setSelected(false);

    MovableGraphicsItem::mousePressEvent(event);
}



QPainterPath Graph::shape() const
{
    QPainterPath path;
    //for (QGraphicsItem *item : childItems())
    //    path.addPath(item->shape());
    path.addRect(boundingRect());
    return path;
}

void Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->fillRect(boundingRect(), Qt::cyan);
    MovableGraphicsItem::paint(painter, option, widget);
}

// creates temporary scene, adds graph to it for a moment
// renders contents into painter which paints into pixmap
QPixmap Graph::generatePixmap() {
    bool was_in_one_scene = false;
    QGraphicsScene *scene;
    if (scene = this->scene()) {
        was_in_one_scene = true;
        scene->removeItem(this);
    }

    QGraphicsScene *scene_temp = new QGraphicsScene();
    scene_temp->addItem(this);
    //QGraphicsTextItem *name_item = scene_temp->addText(name());
    scene_temp->setSceneRect(boundingRect().marginsAdded({10,10,10,10}));
    QPixmap pixmap(scene_temp->sceneRect().size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);
    painter.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::RenderHint::TextAntialiasing);
    painter.setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);
    painter.translate(-boundingRect().x(),-boundingRect().y());
    scene_temp->render(&painter, scene_temp->sceneRect(), boundingRect());
    scene_temp->removeItem(this);
    delete scene_temp;

    if (was_in_one_scene)
        scene->addItem(this);

    return pixmap;
}

Graph *Graph::clone(bool centered)
{
    Graph *g = new Graph();
    for (Edge *e : edges())
        g->addEdgeByCloning(e);
    g->weightsShown(weights_shown);
    g->name(name());
    if (centered)
        g->centerNodes();
    g->node_labels_map = node_labels_map;
    return g;
}

QJsonObject Graph::toJson()
{
    QJsonObject json;
    QJsonArray json_edges, json_node_positions;
    for (Edge *e : edges())
        json_edges.append(QString("%1,%2,%3")
                          .arg(e->sourceNode()->getLabel())
                          .arg(e->destNode()->getLabel())
                          .arg(e->getWeight()));

    QPoint offset = nodesOffsetFromCenter();
    for (Node *n : nodes(/*sorted*/ true)) {
        json_node_positions.append(QString("%1,%2,%3")
                                   .arg(n->getLabel())
                                   .arg(n->pos().x() - offset.x())
                                   .arg(n->pos().y() - offset.y()));
    }

    json["edges"] = json_edges;
    json["node_positions"] = json_node_positions;
    json["name"] = name();
    json["weights_shown"] = weights_shown;
    QVariantMap vmap;
    QMapIterator<NodeLabel, bool> i(node_labels_map);
    while (i.hasNext()) {
        i.next();
        vmap.insert(i.key().q_string, i.value());
    }
    json["node_labels_map"] = QJsonObject::fromVariantMap(vmap);
    return json;
}

Graph *Graph::fromJson(const QJsonObject &obj)
{
    Graph *g = new Graph();
    for (QJsonValue json_edge_csv : obj["edges"].toArray()) {
        QList<QString> v = json_edge_csv.toString().split(',');
        g->addEdge(v[0], v[1], v[2].toInt());
    }

    for (QJsonValue json_node_pos_csv : obj["node_positions"].toArray()) {
        QList<QString> v = json_node_pos_csv.toString().split(',');
        g->getNodeForLabel(v[0])->setPos(v[1].toInt(), v[2].toInt());
    }
    g->name(obj["name"].toString());
    g->weightsShown(obj["weights_shown"].toBool());

    QVariantMap vmap = obj["node_labels_map"].toObject().toVariantMap();
    for (const QString &key : vmap.keys()) {
        g->node_labels_map[key] = vmap[key].toBool();
    }
    return g;
}

void Graph::centerNodes()
{
    QPoint offset = nodesOffsetFromCenter();
    for (Node *n : nodes())
        n->setPos(n->pos() - offset);
}

QPoint Graph::nodesOffsetFromCenter()
{
    int min_x = INT_MAX, min_y = INT_MAX;
    int max_x = INT_MIN, max_y = INT_MIN;

    QList<Node *> nodes_ =  nodes();
    for (Node *n : nodes_) {
        int x = n->pos().x();
        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;

        int y = n->pos().y();
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
    }
    int x_to_add = (max_x + min_x) / 2;
    int y_to_add = (max_y + min_y) / 2;
    return {x_to_add, y_to_add};
}





