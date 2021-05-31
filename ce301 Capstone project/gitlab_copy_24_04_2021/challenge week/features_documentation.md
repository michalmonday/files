## Features documentation  
This file was created to show technical advancements done during the challenge week. 


## List of contents:    
* [Overview](#overview)  
* [Add and delete new vertices and edges](#add-and-delete-new-vertices-and-edges)  
* [Input new edge weights](#input-new-edge-weights)  
* [Adjacency list graphics item](#adjacency-list-graphics-item)  
* User interface features
    * [Movable graphics item](#movable-graphics-item)  
    * [Dock-based interface](#dock-based-interface)  
    * [Window state preservation](#window-state-preservation)  
    * [Zoom](#zoom)  
    * [Keep graph on sight](#keep-graph-on-sight)  
* [Implemented Prim's algorithm](#implemented-prims-algorithm)  
    * [Legend graphical item](#legend-graphical-item)  
    * [V table graphical item](#v-table-graphical-item)  
    * [Code graphical item](#code-graphical-item) (pseudocode, python)  
	* [Step log](#step-log)  


## Overview  
The image below shows the file structure at this point. I organized some of the files into separate folders, this way it's easier to find specific file.  

![MAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/files_structure.png)  


## Add and delete new vertices and edges  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/add_delete_nodes_edges.gif)  

Here's a part of the code responsible for deleting the nodes and edges.  
```cpp
// abstract_scene.cpp file

void AbstractScene::keyPressEvent(QKeyEvent *event) {
    if (key == Qt::Key_Delete) {
        if (Node * selected_node = main_graph->selectedNode()) {
            main_graph->removeNode(selected_node);
        }

        if (Edge * selected_edge = main_graph->selectedEdge()) {
            if (selected_edge->getLabelItem()->hasFocus() == false)
                main_graph->removeEdge(selected_edge);
        }		
    }
}
```


## Input new edge weights  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/weight_input.gif)  

Edge weights are displayed neatly in the middle of edge graphics item. This is accomplished by EdgeTextItem class which is created for every edge and defines behaviour of the weight displaying rectangle. The code below shows the code responsible for filtering the entered text (using regular expression).  

```cpp
// edge_text_item.cpp

void EdgeTextItem::focusOutEvent(QFocusEvent *event)
{
    QString digits_only = this->toPlainText().remove(QRegExp("[^0-9]"));
    if (digits_only.isEmpty())
        digits_only = previous_value;
    setPlainText(digits_only);
    edge->adjust();
    scene()->update();
    emit textEntered(digits_only);

    QGraphicsTextItem::focusOutEvent(event);
}
```

User input is handled carefully ensuring that only digits can be supplied. It also prevents the text field from being empty (by restoring previously stored value if that is about to happen).  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/weight_input_handling.gif)  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/weight_input_handling_2.gif)  

## Adjacency list graphics item
It allows viewing the graph using adjacency list representation regardless of which algorithm was selected. It gets updated whenever a node is added or removed from a graph.    
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/adjacency_list.png)  

I'm planning to implement equivalent adjacency matrix graphical item.  


## Movable graphics item  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/movable_graphics_item.gif)  

As I mentioned in the background reading description. The built-in way of moving structured QGraphicsItem objects (with at least 1 level of parent-child relation), is [not working well](https://www.qtcentre.org/threads/39152-Moving-child-QGraphicsItem) by default.  

I created "[MovableGraphicsItem](https://cseegit.essex.ac.uk/ce301_2020/ce301_borowski_michal/-/blob/master/src/graph_algorithms_tool/movable_graphics_item.h)" class which implements behaviour needed to correctly move a parent item and all the child items. Within mousePressEvent it recognizes the start of movement:  
```cpp
// movable_graphics_item.cpp file

void MovableGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    is_moved = true;
    grabbed_offset = event->scenePos() - scenePos();
}
```

Within mouseMoveEvent it performs the movement (assuming it was started). It can be noticed that movement has certain granularity which allows to easily align the item to aestheticaly position it among other items. In case if the user wanted a fine control over position there's a possibility to press control key while moving the item (which will put it directly under the mouse).  
```cpp
// movable_graphics_item.cpp file

void MovableGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    const int granularity = 10;

    if (is_moved) {
        QPointF new_pos = event->scenePos() - grabbed_offset;
        if (QGraphicsItem *parent_item = parentItem())
            new_pos -= parent_item->scenePos();

		// use "GetClosestPoint" only if control key is not pressed
        if (~event->modifiers() & Qt::ControlModifier)
            new_pos = GetClosestPoint(new_pos, granularity);
			
        if (pos() != new_pos) {
            setPos(new_pos);
            if (QGraphicsScene * scene = this->scene())
                scene->update();
        }
    }
    else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

```

Within mouseReleaseEvent it recognizes that the movement is finished:  
```cpp
// movable_graphics_item.cpp file

void MovableGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    is_moved = false;
	...
```

Currently, several classes inherit from MovableGraphicsItem class, for example:  
* Graph  
* Node  
* PrimsLegendItem  
* VtableGraphicsItem  
* CodeGraphicsItem  


## Dock-based interface
Initially I created an interface within a single window and quickly realized that the more types of graphical items I create, the more I worry about utilizing space. One way of dealing with it was to create multi-window interface using QDockWidget class which allows:  
* resizing/scaling objects to suit user preferences  
* moving window to separate monitor independently of other windows  
* opening and closing  
* docking sub-window within the side of the main window (however it is not really needed in this program and could be disabled)  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/dock_interface.gif)  


## Window state preservation  
As described above, the user interface is adjustable. It could be frustrating to put effort into adjusting user interface, restarting program and losing the changes. For that reason I implemented saving and loading size and position of all windows/docks. It required some tweaks to work well with both:  
* main window
* QDockWidget based sub-windows  

That's how it is implemented for the **main window**:  
```cpp
// mainwindow.cpp file

// LOADING
MainWindow::MainWindow(QWidget *parent) {
    if (settings->value("saved_state_and_geometry", false).toBool()) {
        bool restored = restoreGeometry(settings->value("mainwindow/geometry").toByteArray());
        if (!restored)
            qDebug() << "Couldn't restore geometry for some reason...";

        setWindowState((Qt::WindowState)settings->value("mainwindow/windowStateFunc").toInt());
    }
}

// STORING
void MainWindow::closeEvent(QCloseEvent *event) {
    settings->setValue("mainwindow/geometry", saveGeometry());
    settings->setValue("mainwindow/windowState", saveState());
    settings->setValue("mainwindow/windowStateFunc", (int)windowState());
    settings->setValue("saved_state_and_geometry", true);
    QMainWindow::closeEvent(event);
}
```

That's how it is implemented for the **QDockWidget based sub-windows**:  
```cpp
// dock.cpp file

// LOADING 
Dock::Dock(QGraphicsItem *item_, QString title, Qt::DockWidgetArea area_, QWidget *parent, Qt::WindowFlags flags) {
    const QString prefix = "dock_" + windowTitle() + "/";
    if (settings->value(prefix + "was_saved", false).toBool()) {
        bool is_floating = settings->value(prefix + "is_top_level").toBool();
        Qt::DockWidgetArea area_ = (Qt::DockWidgetArea)settings->value(prefix + "QtDockWidgetArea").toInt();
        // "no dock" is claimed to be invalid when used in "addDockWidget"
        if (area_ == Qt::NoDockWidgetArea)
            area_ = area;

        main_window->addDockWidget(area_, this);
        setFloating(is_floating);
        restoreGeometry( settings->value(prefix + "geometry").toByteArray() );
    } else {
        main_window->addDockWidget(area, this);
    }
}

// STORING
Dock::~Dock()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    const QString prefix = "dock_" + windowTitle() + "/";
    settings.setValue(prefix + "was_saved", true);
    settings.setValue(prefix + "geometry", saveGeometry());
    settings.setValue(prefix + "is_top_level", isWindow());
    settings.setValue(prefix + "QtDockWidgetArea", (int)area);
}
```

## Zoom
It's possible to zoom-in and zoom-out of the graph by using mouse wheel. Using mouse cursor position as an anchor makes user experience more comfortable.  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/zoom.gif)  

It could be possible to lose track where the graph is when zooming too far. For that reason I limited the ability to zoom excessive amounts. Max zoom-out:  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/max_zoom_out.png)  

Max zoom-in:  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/max_zoom_in.png)  

I noticed that examples found online are not perfect because the QGraphicsScene gets centered when certain zoom-out level is reached. I wanted to avoid that and tackled that problem by:  
* making QGraphicsScene much larger and setting certain initial zoom-in level  
* limiting possibility to zoom-out (this way the graph is never centered when zooming-out)  

## Keep graph on sight
If someone carelessly resized the window, it would be possible to lose track where the graph is. For that reason I implemented feature that keeps the graph in sight when the main window is resized.  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/keep_graph_in_sight.gif)  


## Implemented Prim's algorithm
It includes the following graphical items:  
* Legend - describes what are the meanings of colours  
* V table - displays key data used by the algorithm (with key changes highlighted)   
* Code flow - displays key behaviour performed by algorithm (with last action highlighted)  

Right arrow button proceeds to the next step of algorithm. The graph is coloured so it is possible to observe which nodes get added to MST, it is also possible to see which nodes get updated in the V table (using orange colour).  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/gifs/prims_slowed.gif)  


### Legend graphical item  
Currently the legend item was created only for the Prim's algorithm.  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/legend.png)  


### V table graphical item
It does not only show the current state of data. It also directs the attention of the user on what changes were made and what these changes could mean. For example it shows:  
* colour coded node labels (allowing to easily distinguish which node is already a part of MST, which node is considered, and which node is not considered)  
* orange rectangles around entries that were changed during the last iteration of the algorithm  
* "<- min" label indicating entry with the lowest positive value within the table (which means that this edge will be added to MST)  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/vtable.png)  

Using "tooltip" it also shows the previous value of entry when mouse cursor stays on top of an entry.

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/vtable_tooltip_previous_value.png)  


### Code graphical item
Currently it has 2 settings, pseudocode and python.  

**Pseudocode:**  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/code_item_pseudocode.png)  

**Python:**  
![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/code_item_python.png)  

They can be switched by right-clicking on the code graphical item. Being able to see behaviour of an algorithm by reading pseudocode can be helpful for learning. Ideally there could be a large set of languages to choose from, however implementing large number of languages for large number of algorithms would be very time consuming, especially if neat colour coding was going to be used. In Qt, the commonly preferred way of colouring different parts of single text is to use HTML syntax. Currently it looks like this:  
```cpp
    if (language_type == CODE_LANG_PSEUDOCODE) {
        language_name_item->setText(" (pseudocode)");

        text_select_random_node->setHtml("Select random node");
        text_while_mcst_not_completed->setHtml("While MCST isn't completed:");
        text_add_to_mcst->setHtml(QString("Add <span style='color:%1'>selected node</span> to MCST (set V[<span style='color:%1'>selected_node</span>] to <span style='color:%1'>0</span>)").arg(COLOR_MCST.name()));
        text_update_nodes_loop->setHtml(QString("For each edge between newly <span style='color:%1'>selected node</span> and <span style='color:%2'>adjacent nodes</span>:").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
        text_update_nodes_loop_if->setHtml(QString("If edge weight &lt; V[<span style='color:%2'>adjacent_node</span>]:").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
        text_update_nodes_loop_then->setHtml(QString("Set V[<span style='color:%2'>adjacent_node</span>] "
                                                    "to '(<span style='color:%2;'>edge_weight</span>, <span style='color:%1'>selected_node</span>)'").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
        text_get_lowest_positive_v->setHtml(QString("The lowest positive value from V table becomes the new <span style='color:%1'>selected node.</span>").arg(COLOR_MCST.name()).arg(COLOR_CONSIDERED.name()));
    }

    if (language_type == CODE_LANG_PYTHON) {
        language_name_item->setText(" (python)");

        text_select_random_node->setHtml(QString("<span style='color:%1'>selected_node</span> = random.choice(nodes)").arg(COLOR_MCST.name(), COLOR_CONSIDERED.name()));
        text_while_mcst_not_completed->setHtml(QString("<span style='color:%1'>for</span> _<span style='color:%1'> in</span> nodes:").arg(COLOR_PYTHON_ORANGE.name()));
        text_add_to_mcst->setHtml(QString("V[<span style='color:%1'>selected_node</span>][0] = <span style='color:%1'>0</span> <span style='color:%2'>#includes node in MCST</span>").arg(COLOR_MCST.name(), COLOR_PYTHON_RED.name()));
        text_update_nodes_loop->setHtml(QString("<span style='color:%1'>for</span> <span style='color:%3'>edge</span> <span style='color:%1'>in</span> adjacency_list[<span style='color:%2'>selected_node</span>]:").arg(COLOR_PYTHON_ORANGE.name(), COLOR_MCST.name(), COLOR_CONSIDERED.name()));
        text_update_nodes_loop_if->setHtml(QString("<span style='color:%1'>if</span> <span style='color:%2'>edge.weight</span> &lt; V[<span style='color:%2'>adjacent_node</span>][0]:").arg(COLOR_PYTHON_ORANGE.name(), COLOR_CONSIDERED.name()));
        text_update_nodes_loop_then->setHtml(QString("V[<span style='color:%2'>adjacent_node</span>] = (<span style='color:%2;'>edge.weight</span>, <span style='color:%1;'>selected_node</span>)").arg(COLOR_MCST.name(), COLOR_CONSIDERED.name()));
        text_get_lowest_positive_v->setHtml(QString("<span style='color:%1'>selected_node</span> = minimalEntry(V)").arg(COLOR_MCST.name(), COLOR_CONSIDERED.name()));
    }
```

If someone takes a look at the code above, probably the first thought would be: "why not supply the whole string instead of having so many smaller ones?". But in order to create the rectangle indicating the previously executed instruction, it is necessary for each instruction to be a separate graphical item. That's why it looks like one big mess. For these reasons I'm not planning to extend the number of available languages.  


## Step log
On one side of the step log window there's a list of selectable step labels.  
On the right side there's a more in-depth description, explaining what changes occur and why they occur.  
It keeps track of previous steps and allows to select previous step by clicking on the label (on the left side).  
Consistent colouring was used to improve understanding, trying to make it more intuitive.  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/step_log.png)  

As mentioned on [Jira](https://cseejira.essex.ac.uk/browse/A301079-23), the descriptions are **dynamic** and **conditional**. 
By "dynamic" it is meant that descriptions should not only describe the principle in general, but also include labels of nodes/edges that were affected during a step. By "conditional" it is meant that descriptions should vary depending on circumnstances, for example:  
* if all adjacent nodes (that supposed to be updated in V table) are already part of MST then appropriate message should be displayed as shown on image below  

![IMAGE DIDNT SHOW](https://raw.githubusercontent.com/michalmonday/files/master/ce301%20Capstone%20project/pics/step_log_conditional_description.png)  



![IMAGE DIDNT SHOW]()  

