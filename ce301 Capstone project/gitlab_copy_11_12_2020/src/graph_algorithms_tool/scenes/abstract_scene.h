#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

#include <QDockWidget>
#include <QGraphicsScene>
#include <QMap>

class CodeGraphicsItem;
class Edge;
class LegendGraphicsItem;


static const int size_mult = 10;

class Graph;
class AdjacencyListGraphicsItem;
class Dock;
class SelectAlgorithmWidget;
class ControlledAlgorithm;

class AbstractScene : public QGraphicsScene
{
public:
    AbstractScene(QObject *parent, QRectF scene_rect = QRectF(-350 * size_mult, -200 * size_mult, 700 * size_mult, 400 * size_mult));
    virtual ~AbstractScene() override;



    Graph *mainGraph() { return main_graph; }

    static AdjacencyListGraphicsItem *adjacency_list_graphics_item;
    //static Dock *dock_step_description;
    static QDockWidget *dock_step_log;
    static Graph *main_graph;

    ControlledAlgorithm *algorithm;

protected:    
    LegendGraphicsItem *legend;


    void enableRestorableEdgeWeights();
    void initMainGraph(QString algorithm_name, Graph *g = nullptr);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    //virtual void focusInEvent(QFocusEvent *event) override;
protected:
    CodeGraphicsItem *code_graphics_item;

private:
    virtual void onAlgorithmStart() = 0;
    virtual void onAlgorithmStep() = 0;
    virtual void onAlgorithmEnd() = 0;
    virtual void onAlgorithmEvent() = 0;
    bool algorithm_started;

    // when AbstractScene is created, it will check if initially main graph had weights shown
    // then scene like BfsScene may hide them. When algorithm is changed, the initial state will be restored.
    bool main_graph_had_weights_shown;

    // similar to above, but
    bool restorable_main_graph_edge_weights;
    QMap<Edge *, QString> initial_edge_weights;



    void restoreWeights();
};

#endif // ABSTRACTSCENE_H
