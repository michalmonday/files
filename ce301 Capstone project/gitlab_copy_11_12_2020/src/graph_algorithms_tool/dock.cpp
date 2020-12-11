#include "dock.h"

/*  Dock class intends to allow conveniently create a separate dock for a single QGraphicsItem.

    It creates its own QGraphicsView, with its own QGraphicsScene.

    Size of view should match the size of supplied QGraphicsItem.

    Resizing dock should make the item larger (this way user can tweak with visibility himself.
    */

#include <QGraphicsItem>
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QWindow>

#include "colors.h"


Dock::Dock(QGraphicsItem *item_, QString title, Qt::DockWidgetArea area_, QString independence_indentifier, QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags),
    item(item_),
    view(new QGraphicsView(this)),
    scene(new QGraphicsScene(view)),
    area(area_)
{
    setAllowedAreas(Qt::DockWidgetArea::NoDockWidgetArea);
    // setting object name is not really needed anymore because windowTitle is used when saving position and size
    setObjectName("obj_dock_" + title);

    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHints(view->renderHints() | QPainter::Antialiasing);

    scene->setSceneRect(item->boundingRect());
    scene->addItem(item);
    scene->setBackgroundBrush(COLOR_BACKGROUND);
    view->setScene(scene);

    //view->setBackgroundBrush(QColor(0,0,255, 50));

    //view->fitInView(scene->sceneRect(), Qt::AspectRatioMode::KeepAspectRatioByExpanding);

    setWidget(view);

    connect(this, &QDockWidget::topLevelChanged, this, &Dock::topLevelChangedSlot);

    //QSizePolicy policy = sizePolicy();
    //policy.setHeightForWidth(true);
    //setSizePolicy(policy);

    settings_dir = "dock_" + windowTitle() + "_" + independence_indentifier + "/";
    if (settings->value(settings_dir + "was_saved", false).toBool()) {
        bool is_floating = settings->value(settings_dir + "is_top_level").toBool();

        Qt::DockWidgetArea area_ = (Qt::DockWidgetArea)settings->value(settings_dir + "QtDockWidgetArea").toInt();

        // "no dock" is claimed to be invalid when used in "addDockWidget"
        if (area_ == Qt::NoDockWidgetArea)
            area_ = area;

        main_window->addDockWidget(area_, this);
        setFloating(is_floating);
        restoreGeometry( settings->value(settings_dir + "geometry").toByteArray() );
        //setVisible( settings->value(prefix + "is_visible").toBool() );
    } else {
        main_window->addDockWidget(area, this);
    }

    //adjustSize();

    /*
    bool iv = isVisible();
    //main_window->restoreDockWidget(this);
    setVisible(iv);
    */

    connect(this, &Dock::dockLocationChanged, this, &Dock::updateDockArea);

    update();

    //setWindowFlag(Qt::WindowType::WindowDoesNotAcceptFocus, true);

    //is_visible = isVisible();
}

Dock::~Dock()
{
    // can't use global settings object because its destructor could be called already
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue(settings_dir + "was_saved", true);
    settings.setValue(settings_dir + "geometry", saveGeometry());
    settings.setValue(settings_dir + "is_top_level", isWindow());
    //settings.setValue(prefix + "is_visible", is_visible);

    // isWindow may not work well if 2 docks are tabbed
    // (e.g. if dock is tabbed but not selected, it could return false... but idk)
    settings.setValue(settings_dir + "QtDockWidgetArea", (int)area);
    //qDebug() << prefix + " destructor";
}

void Dock::update()
{
    scene->setSceneRect(item->boundingRect());
    scene->update();
    item->update();
    view->update();

    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    //if (isTopLevel())
    //    adjustSize();
}

int Dock::heightForWidth(int w) const {
    //return w * item->boundingRect().width() / item->boundingRect().height();
    return w*1.6;
}

void Dock::topLevelChangedSlot()
{
    adjustSize();
}

void Dock::adjustSize()
{
    //QSizeF diff = (item->boundingRect().size() - geometry().size()) / 2;
    //QSizeF diff = (view->rect().size() - geometry().size()) / 2;
    //qDebug() << "diff = " << diff;
    //setGeometry(geometry().adjusted(-diff.width(), -diff.height(), diff.width(), diff.height()));
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    item->setPos(0,0);
}



void Dock::resizeEvent(QResizeEvent *event)
{
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    /*
    if (QApplication::mouseButtons() == Qt::LeftButton) {
        qreal dock_ratio = event->size().width() / event->size().height();
        qreal item_ratio = item->boundingRect().width() / item->boundingRect().height();

        if (dock_ratio != item_ratio)
            setGeometry(geometry().adjusted());
    }
    */
}


void Dock::showEvent(QShowEvent *event)
{
    /*QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = "dock_" + windowTitle() + "/";
    if (settings.value(prefix + "was_saved", false).toBool()) {
        restoreGeometry( settings.value(prefix + "geometry").toByteArray() );
    }*/
    //is_visible = true;
    //qDebug()<< "show event";
    adjustSize();
}

void Dock::hideEvent(QHideEvent *event)
{
    /*QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = "dock_" + windowTitle() + "/";
    settings.setValue(prefix + "geometry", saveGeometry());*/
    //is_visible = false;
}


void Dock::mousePressEvent(QMouseEvent *event)
{
    if (QWindow *window = windowHandle())
        window->startSystemMove();

    QDockWidget::mousePressEvent(event);
}
