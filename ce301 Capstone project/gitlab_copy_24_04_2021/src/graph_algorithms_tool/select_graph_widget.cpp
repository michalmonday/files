#include "select_graph_widget.h"
#include "ui_select_graph_widget.h"

#include "graph/graph.h"
#include "graph/preset_graphs.h"
#include "functions.h"
#include "colors.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphics_view.h"
#include "select_algorithm_widget.h"
#include "graph/custom_graphs.h"

#include <QGraphicsView>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>

#include <string.h>
#include <QGraphicsTextItem>

SelectGraphWidget * select_graph_widget = nullptr;

const int col_limit = 3;

SelectGraphWidget::SelectGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectGraphWidget),
    row(0),
    col(0),
    index(0),
    grid_layout(nullptr)
{
    setWindowFlag(Qt::WindowType::WindowStaysOnTopHint);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);

    QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = windowTitle() + "/";
    if (settings.value(prefix + "was_saved", false).toBool())
        restoreGeometry( settings.value(prefix + "geometry").toByteArray() );

    grid_layout = new QGridLayout(ui->scrollArea);

    area = new QWidget(this);

    const QList<Graph *> &preset_graphs = PresetGraphs::all();
    first_deletable_index = preset_graphs.size();
    for (Graph *g : preset_graphs + custom_graphs->all()) {
        addGraph(g);
    }
    area->setObjectName("area");
    area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    area->setLayout(grid_layout);
    ui->scrollArea->setWidget(area);
}


SelectGraphWidget::~SelectGraphWidget()
{
    if (!is_exiting)
        for (Graph *g : all_graphs)
            delete g;

    QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = windowTitle() + "/";
    settings.setValue(prefix + "was_saved", true);
    settings.setValue(prefix + "geometry", saveGeometry());

    delete ui;
}


void SelectGraphWidget::addGraph(Graph *g)
{
    int index = row/2 * col_limit + col;
    all_graphs << g;


    QGraphicsView *view = new QGraphicsView(this);
    all_views << view;

    view->setObjectName(QString("graph_view_%1").arg(index));
    view->setMinimumHeight(200);
    view->setFrameStyle(0);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHints(view->renderHints() | QPainter::Antialiasing);
    view->setStyleSheet("background: transparent");

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setObjectName(QString("graph_scene_%1").arg(index));

    QGraphicsPixmapItem *pixmap_item = new QGraphicsPixmapItem(g->generatePixmap());
    pixmap_item->setTransformationMode(Qt::SmoothTransformation);
    scene->addItem(pixmap_item);
    scene->setSceneRect(pixmap_item->boundingRect());

    //name_item->setPos(scene->sceneRect().center().x() - name_item->boundingRect().width()/2*name_item->scale(), scene->sceneRect().bottom() - name_item->boundingRect().height() * name_item->scale());

    view->setScene(scene);
    view->update();
    view->fitInView(scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);

    QLabel *label = new QLabel(g->name(), this);
    grid_layout->addWidget(label, row, col);
    all_labels << label;
    grid_layout->addWidget(view, row + 1, col);
    all_grid_positions << QPoint(col, row + 1);
    col++;
    if (col >= col_limit) {
        col = 0;
        row += 2; // 1 for label and 1 for view
    }

    view->setMouseTracking(true);
    view->installEventFilter(this);
    //scene->installEventFilter(this);

    view->update();
    grid_layout->update();
    area->update();


    QTimer::singleShot(1000, [view, scene](){view->fitInView(scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatio);});
}

void SelectGraphWidget::fitInViewAll()
{
    for (QGraphicsView *view : findChildren<QGraphicsView*>())
        view->fitInView(view->sceneRect(), Qt::KeepAspectRatio);
}

void SelectGraphWidget::resizeEvent(QResizeEvent *event)
{
    fitInViewAll();
}

bool SelectGraphWidget::eventFilter(QObject *watched, QEvent *event)
{
    QEvent::Type type = event->type();
    if (type == QEvent::MouseButtonPress) {
        QMouseEvent *m_event = static_cast<QMouseEvent*>(event);
        if (watched->objectName().startsWith("graph_view_")) {
            QGraphicsView *view = static_cast<QGraphicsView *>(watched);

            if (m_event->button() == Qt::LeftButton) {
                //qDebug() << view->objectName();
                //int graph_index = 0;
                //sscanf(watched->objectName().toLatin1(), "graph_view_%d", &graph_index);
                select_algorithm_widget->useSuggestedGraphs(false);
                int i = all_views.indexOf(view);
                main_window->ui->graphicsView->setGraph(all_graphs[i]->clone());
                return true;
            }

            if (m_event->button() == Qt::RightButton) {
                int i = all_views.indexOf(view);
                Graph *g = all_graphs[i];
                QLabel *label = all_labels[i];
                if (i < first_deletable_index) {
                    QMessageBox::information(this, "Graph deletion", "Preset graphs can't be deleted.");
                } else if (QMessageBox::Yes == QMessageBox::question(this, "Graph deletion",
                            QString("Would you like to delete custom graph '%1'").arg(label->text()),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No )) {
                    if (custom_graphs->removeByName(g->name())) {
                        grid_layout->removeWidget(view);
                        grid_layout->removeWidget(label);
                        view->removeEventFilter(this);
                        view->deleteLater();
                        label->deleteLater();
                        delete g;
                        if (--col < 0) {
                            col = col_limit-1;
                            if ((row-=2) < 0)
                                row = 0;
                        }

                        all_graphs.removeAt(i);
                        all_labels.removeAt(i);
                        all_views.removeAt(i);
                        all_grid_positions.removeLast();
                        for (int j = i; j < all_grid_positions.size(); j++) {
                            QPoint new_pos = all_grid_positions[j];
                            QWidget *view_to_move = all_views[j];
                            QWidget *label_to_move = all_labels[j];
                            grid_layout->removeWidget(view_to_move);
                            grid_layout->addWidget(view_to_move, new_pos.y(), new_pos.x());
                            grid_layout->removeWidget(label_to_move);
                            grid_layout->addWidget(label_to_move, new_pos.y()-1, new_pos.x());
                        }
                        return false;
                    }
                }
            }
        }
    }

    if (type == QEvent::Leave || type == QEvent::Enter) {
        if (watched->objectName().startsWith("graph_view_")) {

            QGraphicsView *view = static_cast<QGraphicsView *>(watched);
            bool entered = event->type() == QEvent::Enter;
            //view->scene()->setBackgroundBrush(entered ? COLOR_CONSIDERED.lighter() : Qt::transparent);
            view->setFrameStyle(entered ? QFrame::Box : 0);
            return true;
        }
    }
    if (type == QEvent::KeyPress || type == QEvent::KeyRelease) {
        QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
        QKeyEvent *e = new QKeyEvent ( key_event->type(), key_event->key(), key_event->modifiers());
        QCoreApplication::postEvent (main_window->ui->graphicsView->scene(), e);
    }
    return QObject::eventFilter(watched, event);
}



/*
void SelectGraphWidget::keyPressEvent(QKeyEvent *event)
{
    QKeyEvent *e = new QKeyEvent ( event->type(), event->key(), event->modifiers());
    QCoreApplication::postEvent (main_window->ui->graphicsView->scene(), e);
}

void SelectGraphWidget::keyReleaseEvent(QKeyEvent *event)
{
    QKeyEvent *e = new QKeyEvent ( event->type(), event->key(), event->modifiers());
    QCoreApplication::postEvent (main_window->ui->graphicsView->scene(), e);
}*/


/*
class GraphGraphicsView : public QGraphicsView {
    public:
    GraphGraphicsView(QString graph_name_, QWidget *parent = nullptr)
        : QGraphicsView(parent),
          name_item(new QGraphicsTextItem(graph_name_))
    {
        setMinimumHeight(200);
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setRenderHints(renderHints() | QPainter::Antialiasing);
        setStyleSheet("background: transparent");
        setFrameStyle(0);


    }
    virtual ~GraphGraphicsView() override {}

    // QGraphicsView interface
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override {
        //name_item->setScale(0.111 * rect.height());
        QFont font = QFont();
        font.setPixelSize(0.1 * this->rect().height());
        name_item->setFont(font);
        int x = rect.center().x() - name_item->boundingRect().width()/2;
        int y = rect.top() + name_item->boundingRect().height();
        painter->setFont(font);
        painter->drawText(x, y, name_item->toPlainText());
    }

private:
    QGraphicsTextItem *name_item;
};*/
