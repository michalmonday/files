#ifndef DOCK_H
#define DOCK_H

#include <QDockWidget>
#include <QGraphicsView>



class Dock : public QDockWidget
{
    Q_OBJECT
public:
    // dock class intends to allow conveniently create a separate dock for
    // a single QGraphicsItem
    Dock(QGraphicsItem *item, QString title, Qt::DockWidgetArea area, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~Dock() override;

    void update();

    void updateDockArea(Qt::DockWidgetArea a) { area = a; }

    virtual int heightForWidth ( int w ) const override;
private:
    QGraphicsItem *item;
    QGraphicsView *view;
    QGraphicsScene *scene;

    Qt::DockWidgetArea area;
    //bool is_visible;

    void topLevelChangedSlot();
    void adjustSize();

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // DOCK_H
