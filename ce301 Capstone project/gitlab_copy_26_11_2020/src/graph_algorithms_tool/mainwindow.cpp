#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QDebug>
#include <QDockWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QWindow>

#include "graph/edge.h"
#include "graph/node.h"

#include "tutorial.h"

bool use_docks = true;

MainWindow *main_window = nullptr;
QSettings *settings = nullptr;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);

    main_window = this;
    ui->setupUi(this);




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
}

MainWindow::~MainWindow()
{
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

void MainWindow::on_comboBox_algorithms_currentTextChanged(const QString &algorithm_name)
{
    ui->graphicsView->setAlgorithm(algorithm_name);
    ui->graphicsView->setFocus();
}
