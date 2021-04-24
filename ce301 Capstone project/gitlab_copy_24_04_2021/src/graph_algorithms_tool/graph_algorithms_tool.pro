#-------------------------------------------------
#
# Project created by QtCreator 2020-07-22T14:29:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graph_algorithms_tool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

CONFIG += static

SOURCES += \
    adjacency_list_graphics_item.cpp \
    algorithms/controlled_algorithm.cpp \
    algorithms/controlled_bfs_algorithm.cpp \
    algorithms/controlled_dfs_algorithm.cpp \
    algorithms/controlled_dijkstra_algorithm.cpp \
    algorithms/controlled_kruskals_algorithm.cpp \
    algorithms/controlled_prims_algorithm.cpp \
    code_items/bfs_code_graphics_item.cpp \
    code_items/code_graphics_item.cpp \
    code_items/dfs_code_graphics_item.cpp \
    code_items/dijkstra_code_graphics_item.cpp \
    code_items/kruskals_code_graphics_item.cpp \
    code_items/prims_code_graphics_item.cpp \
    collection_items/collection_graphics_item.cpp \
    collection_items/priority_queue_graphics_item.cpp \
    collection_items/queue_graphics_item.cpp \
    collection_items/stack_graphics_item.cpp \
    dock.cpp \
    functions.cpp \
    graph/custom_graphs.cpp \
    graph/edge.cpp \
    graph/edge_text_item.cpp \
    graph/graph.cpp \
    graph/node.cpp \
    graph/preset_graphs.cpp \
    legend_graphics_item.cpp \
    line_description_graphics_item.cpp \
        main.cpp \
        mainwindow.cpp \
    graphics_view.cpp \
    movable_graphics_item.cpp \
    scenes/abstract_scene.cpp \
    scenes/bfs_scene.cpp \
    scenes/dfs_scene.cpp \
    scenes/dijkstra_scene.cpp \
    scenes/kruskal_scene.cpp \
    scenes/prim_scene.cpp \
    select_algorithm_widget.cpp \
    select_graph_widget.cpp \
    shortest_path_graphics_item.cpp \
    step_description_graphics_item.cpp \
    tutorial.cpp \
    vtable_entry_graphics_item.cpp \
    vtable_entry_hover_text_item.cpp \
    vtable_graphics_item.cpp

HEADERS += \
    adjacency_list_graphics_item.h \
    algorithms/controlled_algorithm.h \
    algorithms/controlled_bfs_algorithm.h \
    algorithms/controlled_dfs_algorithm.h \
    algorithms/controlled_dijkstra_algorithm.h \
    algorithms/controlled_kruskals_algorithm.h \
    algorithms/controlled_prims_algorithm.h \
    code_items/bfs_code_graphics_item.h \
    code_items/code_graphics_item.h \
    code_items/dfs_code_graphics_item.h \
    code_items/dijkstra_code_graphics_item.h \
    code_items/kruskals_code_graphics_item.h \
    code_items/prims_code_graphics_item.h \
    collection_items/collection_graphics_item.h \
    collection_items/priority_queue_graphics_item.h \
    collection_items/queue_graphics_item.h \
    collection_items/stack_graphics_item.h \
    colors.h \
    dock.h \
    functions.h \
    graph/custom_graphs.h \
    graph/edge.h \
    graph/edge_text_item.h \
    graph/graph.h \
    graph/node.h \
    graph/preset_graphs.h \
    legend_graphics_item.h \
    line_description_graphics_item.h \
        mainwindow.h \
    graphics_view.h \
    movable_graphics_item.h \
    scenes/abstract_scene.h \
    scenes/bfs_scene.h \
    scenes/dfs_scene.h \
    scenes/dijkstra_scene.h \
    scenes/kruskal_scene.h \
    scenes/prim_scene.h \
    select_algorithm_widget.h \
    select_graph_widget.h \
    shortest_path_graphics_item.h \
    step_description_graphics_item.h \
    tutorial.h \
    typedefs.h \
    ui_tutorial.h \
    vtable_entry_graphics_item.h \
    vtable_entry_hover_text_item.h \
    vtable_graphics_item.h


FORMS += \
        mainwindow.ui \
        select_algorithm_widget.ui \
        select_graph_widget.ui \
        tutorial.ui


#INCLUDEPATH += scenes/
#VPATH += scenes/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/tutorial/book pic.jpg

RESOURCES += \
    resources.qrc

