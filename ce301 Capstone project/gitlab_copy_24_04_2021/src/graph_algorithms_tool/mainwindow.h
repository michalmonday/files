#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph/edge.h"

#include <QListWidget>
#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;

    // QObject interface
public:
    virtual bool event(QEvent *event) override;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
private slots:

};

extern MainWindow *main_window;
extern QSettings *settings;
extern bool is_exiting;

#endif // MAINWINDOW_H
