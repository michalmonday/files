#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QDebug>
#include <QDockWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QWindow>
#include <QJsonObject>

#include "graph/edge.h"
#include "graph/node.h"

#include "select_algorithm_widget.h"
#include "select_graph_widget.h"
#include "tutorial.h"

#include "scenes/abstract_scene.h"
#include "ui_select_algorithm_widget.h"
#include "select_algorithm_widget.h"

#include "graph/custom_graphs.h"
#include "graph/graph.h"


MainWindow *main_window = nullptr;
QSettings *settings = nullptr;

bool is_exiting = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);

    main_window = this;

    custom_graphs = new CustomGraphs();

    select_graph_widget = new SelectGraphWidget();
    select_graph_widget->show();
    select_graph_widget->fitInViewAll();

    ui->setupUi(this);

    select_algorithm_widget = new SelectAlgorithmWidget();
    select_algorithm_widget->show();





    tutorial = new Tutorial({
            {"", "This program is designed to help with understanding graph algorithms. It allows to visualise what is going on at each step of an algorithm. The main "},
            {"authors.png", "2 of the minimum cost spanning tree algorithms presented in this program are Kruskal's algorithm developed by Joseph Kruskal (left), and Prim's algorithm, initially developed by Vojtech Jarnik and later rediscovered by Robert Clay Prim (right)."}
        },this);

    /*QRect screen_geometry = QGuiApplication::primaryScreen()->geometry();
    int height = screen_geometry.height();
    int width = screen_geometry.width();*/

    if (settings->value("first_run", true).toBool()) {
        qDebug() << "First run detected";
        settings->setValue("first_run", false);
    } else {
        qDebug() << "It's not a first run.";
    }

    if (settings->value("dont_show_tutorial_again", false).toBool()) {
        tutorial->hide();
    } else {
        tutorial->show();
    }

    if (settings->value("saved_state_and_geometry", false).toBool()) {
        bool restored = restoreGeometry(settings->value("mainwindow/geometry").toByteArray());
        if (!restored)
            qDebug() << "Couldn't restore geometry for some reason...";

        setWindowState((Qt::WindowState)settings->value("mainwindow/windowStateFunc").toInt());
    }

    QString algorithm_name = settings->value("last_selected_algorithm", "Prims").toString();
    QComboBox *combobox = select_algorithm_widget->ui->comboBox_algorithm;
    combobox->setCurrentText(algorithm_name);

    if (settings->value("last_graph_saved", false).toBool()) {
        QJsonObject json_graph = QJsonObject::fromVariantMap( settings->value("last_graph").toMap() );
        Graph *last_graph = Graph::fromJson(json_graph);
        ui->graphicsView->setGraph(last_graph);
    }

    //ui->graphicsView->setAlgorithm(algorithm_name);
}

MainWindow::~MainWindow()
{
    is_exiting = true;
    delete ui;
}




void MainWindow::closeEvent(QCloseEvent *event)
{
    settings->setValue("mainwindow/geometry", saveGeometry());
    settings->setValue("mainwindow/windowState", saveState());
    settings->setValue("mainwindow/windowStateFunc", (int)windowState());
    settings->setValue("saved_state_and_geometry", true);
    //for (QDockWidget *dock : findChildren<QDockWidget *>()) {

    //}
    QMainWindow::closeEvent(event);
}

bool MainWindow::event(QEvent *event)
{
    static bool initial_geometry_checked = false;
    if (!initial_geometry_checked && event->type() == QEvent::ShowToParent) {
        if (QWindow *window = windowHandle()) {
            initial_geometry_checked = true;
            //qDebug() << "initial geometry = " << window->geometry();
            //qDebug() << "incorrect geometry = " << geometry();
            ui->graphicsView->init();
        }
    }
    //qDebug() << event->type();
    return QMainWindow::event(event);
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //if (QWindow *window = windowHandle())
    //    window->startSystemMove();
    QMainWindow::mousePressEvent(event);
}
