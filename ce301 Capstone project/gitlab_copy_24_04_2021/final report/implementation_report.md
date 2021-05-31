# Implementation details

# List of contents
* [Technology used](#technology-used)  
    * [About Qt Graphics View architecture](#about-qt-graphics-view-architecture)  
	* [Deploying Qt program on Windows](#deploying-qt-program-on-windows)  
* [File structure and design patterns](#file-structure-and-design-patterns)  
    * [Separation of logic from graphics](#separation-of-logic-from-graphics)  
	* [Convenient QDockWidget implementation](#convenient-qdockwidget-implementation)  

## Technology used
I used Qt and C++ to create the program. It has a lot of convenient utilities, data structures, and a nice GUI designer called "Qt Creator". The software versions used by me are:  
* Qt Creator 4.13.1 IDE/design tool  
* Qt 5.15.0 framework  
* MinGW 8.1.0 64-bit compiler  

<img src="https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/qt_creator.png" alt="IMAGE DIDNT SHOW" width="700">  

### About Qt Graphics View architecture  
There are 3 main classes used by Qt to display 2D graphic items. These are:  
* [QGraphicsView](https://doc.qt.io/qt-5/qgraphicsview.html) - **view** acts as a host widget and a window for a scene, allowing for "pan and crop" kind of operations (and much more) of the scene (if someone is familiar with video editing)   
* [QGraphicsScene](https://doc.qt.io/qt-5/qgraphicsscene.html) - **scene** acts as a coordinate and transformation system for items, items can be added and removed from a scene   
* [QGraphicsItem](https://doc.qt.io/qt-5/qgraphicsitem.html) - **items** are like "sticky notes" on the scene    

Image from page 94 of "Game Programming Using Qt 5" book shows what it looks like:  

<img src="https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/qt_graphics_view_framework.png" alt="IMAGE DIDNT SHOW" width="700">  

Regarding QGraphicsItem, Qt provides many subclasses such as:  
* [QGraphicsTextItem](https://doc.qt.io/qt-5/qgraphicstextitem.html) - drawing text (that can be coloured and styled with HTML)  
* [QGraphicsSimpleTextItem](https://doc.qt.io/qt-5/qgraphicssimpletextitem.html) - drawing text   
* [QGraphicsLineItem](https://doc.qt.io/qt-5/qgraphicslineitem.html) - drawing lines  
* [QGraphicsRectItem](https://doc.qt.io/qt-5/qgraphicsrectitem.html) - drawing rectangles  
* [QGraphicsEllipseItem](https://doc.qt.io/qt-5/qgraphicsellipseitem.html) - drawing circles/ellipses  
* [QGraphicsPixmapItem](https://doc.qt.io/qt-5/qgraphicspixmapitem.html) - drawing images (I used it in graph selection widget)  
* [QGraphicsPolygonItem](https://doc.qt.io/qt-5/qgraphicspolygonitem.html) - drawing polygons  
* [QGraphicsPathItem](https://doc.qt.io/qt-5/qgraphicspathitem.html) - drawing custom shaped items that may consist of mixture of any items above (any item can be converted to path itself and added to QGraphicsPathItem)  

These classes are convenient and I used many of them in this project, but in most cases I sub-classed the "QGraphicsItem" class itself and overriden 2 key virtual methods provided by it:  
```cpp
QRectF boundingRect() const override;
void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
```

Implementation of "boundingRect" method within QGraphicsItem sub-class notifies the scene about the size of the item and its' horizontal and vertical boundaries. Implementation of "paint" method  allows to customize how exactly the custom item will be drawn. For example, the "Node" class takes into account if the node is:  
- highlighted (e.g. because it was recently expanded in some algorithm)  
- selected by clicking on it (to make algorithm start from it)  
- under the mouse cursor (to make it look responsive)  
```cpp  
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    bool is_hovered = option->state & QStyle::State_MouseOver;
    if (highlighted) {
        painter->setBrush(highlight_color);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(boundingRect());
    }
    painter->setBrush(color.lighter());
    QPen pen(Qt::black, 1);
    if (isSelected() || is_hovered || v_entry_is_hoovered)
        pen.setWidth(3);
    painter->setPen(pen);
    QRectF rect(-radius, -radius, radius*2, radius*2);
    painter->drawEllipse(rect);
    painter->drawText(rect, Qt::AlignCenter, label);
}
```

### Deploying Qt program on Windows
Normally if we build a program with Qt Creator and copy it to a new machine (which may not have Qt installed), then the program will not work (resulting in "entry point not located" or "missing dll" error).  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/deployment_error.png)  

For that reason it is necessary to include required libraries and put them in the same folder. Luckily, Qt has a program called [windeployqt](https://doc.qt.io/qt-5/windows-deployment.html), which can do that automatically. So to deploy a Qt program, we have to:  
- build it from Qt Creator  
- copy executable to some empty folder (optional)  
- find [windeployqt.exe](https://doc.qt.io/qt-5/windows-deployment.html) (on my PC it's at: `C:\Qt\5.15.0\mingw81_64\bin`)  
- open command prompt in that location and run: `windeployqt.exe PATH_TO_OUR_PROGRAM`  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/deployment_command.png)  

All necessary dynamically linked libraries and resources will be put in the directory of our program:  

<kbd><img src="https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/deployment_result.png" alt="IMAGE DIDNT SHOW"></kbd> 

That's how I produced [release.zip](https://cseegit.essex.ac.uk/ce301_2020/ce301_borowski_michal/-/blob/master/release.zip) files.  

## File structure and design patterns
	
Image below shows the file structure I used. I moved groups of related files into folders to improve clarity when nagivating through files. I used different colours to mark recurring design pattern that involved subclassing a common parent class in order to avoid code duplication and force consistency.      

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/files_marked.png)  

Diagrams below describe relationships between these parent classes and their subclasses:  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/inheritance_diagrams.png)  

Having a separate class for each algorithm makes each algorithm customizable and easy to extend. Having a common parent class for all of separate algorithms helps to avoid mess which otherwise would occur when dealing with such large number of classes and files (because common behaviour is defined once and applied to all algorithms).   

### Separation of logic from graphics

Instead of mixing algorithm logic with graphical representation I separated implementation of these into separate classes/files, as shown on the diagram below:  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/each_algorithm_has.png)  

That was an attempt to avoid spaghetti code where the exact purpose of objects is not known. This way it is kind of clear which object is created for the sake of running the algorithm, and which object is created for the sake of displaying information about it.  

### Convenient QDockWidget implementation 

There are various QDockWidget objects used by the program to make the user interface adjustable. That is accomplished by using 2 classes:  
* Dock class (from [dock.h](https://cseegit.essex.ac.uk/ce301_2020/ce301_borowski_michal/-/blob/master/src/graph_algorithms_tool/dock.h)/.cpp files)   
    Inheriting from and implementing custom behaviour of the QDockWidget class provided by Qt  
* Dockable class (from [dock.h](https://cseegit.essex.ac.uk/ce301_2020/ce301_borowski_michal/-/blob/master/src/graph_algorithms_tool/dock.h) file)  
    It can be inherited by any QGraphicsItem object and handle creation/destruction of the Dock conveniently.
	
Using this pattern limits the Dock-related code used by QGraphicsItem objects to minimum. All that QGraphicsItem has to do is to inherit "Dockable" and call "createDock(item, title)" in the constructor. It is used by several classes in the project as shown below:  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/dockable.png)  

Thanks to the inheritance instructions shown above and "createDock" calls in constructors, these graphics items get a movable, resizable and nicely scaled docks with preserved size/position after program restart.  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/mvp/docks.png)  





