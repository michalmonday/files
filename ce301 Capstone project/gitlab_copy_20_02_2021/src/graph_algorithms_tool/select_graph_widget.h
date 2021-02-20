#ifndef SELECT_GRAPH_WIDGET_H
#define SELECT_GRAPH_WIDGET_H

#include <QGraphicsView>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>


class Graph;
namespace Ui {
class SelectGraphWidget;
}

class SelectGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectGraphWidget(QWidget *parent = nullptr);
    ~SelectGraphWidget();

    void fitInViewAll();

    void addGraph(Graph *);

private:
    Ui::SelectGraphWidget *ui;
    QWidget * area;
    QList<Graph *> all_graphs;
    QList<QGraphicsView *> all_views;
    QList<QLabel *> all_labels;
    QList<QPoint> all_grid_positions;
    QGridLayout *grid_layout;
    int row, col, index;

    // preset graphs are not deletable and are positioned before custom ones
    int first_deletable_index;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    //virtual void keyPressEvent(QKeyEvent *event) override;
    //virtual void keyReleaseEvent(QKeyEvent *event) override;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

};

extern SelectGraphWidget *select_graph_widget;

#endif // SELECT_GRAPH_WIDGET_H
