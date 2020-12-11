#ifndef DOCK_H
#define DOCK_H

#include <QDockWidget>
#include <QGraphicsView>

#include "mainwindow.h"

class Dock : public QDockWidget
{
    Q_OBJECT
public:
    // dock class intends to allow conveniently create a separate dock for
    // a single QGraphicsItem
    // independence_identifier is used to distinguish Docks for storing/restoring their geometry
    Dock(QGraphicsItem *item, QString title, Qt::DockWidgetArea area, QString independence_indentifier = "", QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~Dock() override;

    void update();

    void updateDockArea(Qt::DockWidgetArea a) { area = a; }

    void adjustSize();

    virtual int heightForWidth ( int w ) const override;
private:
    QGraphicsItem *item;
    QGraphicsView *view;
    QGraphicsScene *scene;

    Qt::DockWidgetArea area;
    //bool is_visible;

    QString settings_dir;

    void topLevelChangedSlot();


    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
};

class Dockable {
public:
    Dockable() : dock_(nullptr) {}
    ~Dockable() {
        if(!is_exiting)
            delete dock_;
    }
    void createDock(QGraphicsItem *item, QString title, QString independence_identifier = "") {
        if (dock_)
            delete dock_;

        dock_ = new Dock(item, title, Qt::DockWidgetArea::LeftDockWidgetArea, independence_identifier);
        dock_->hide();
        dock_->update();
    }
    void showDock() { if (dock_) { dock_->update(); dock_->show(); } }
    void hideDock() { if (dock_) { dock_->hide(); } }
    void updateDock() { if (dock_) { showDock(); } }
    Dock *dock() { return dock_; }

private:
    Dock *dock_;
};

#endif // DOCK_H
