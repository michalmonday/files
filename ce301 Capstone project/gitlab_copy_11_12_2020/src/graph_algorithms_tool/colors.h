#ifndef COLORS_H
#define COLORS_H

// After updating these values I need to click on: "Build -> Run qmake"
// Why: https://stackoverflow.com/a/23026941/4620679

#define COLOR_MCST QColor("#099400")

#define COLOR_NODE_DEFAULT QColor(Qt::yellow)


// instead of using separate color for newly considered edges/nodes, use amber highlight around shape()
// this way there will be less confusion (and legend will have 1 less entry)

//#define COLOR_NEW_CONSIDERED QColor("#7703fc") // violet
#define COLOR_HIGHLIGHT QColor("#fcba03") // amber
#define COLOR_HIGHLIGHT_RETIRED QColor("#707070")

#define COLOR_CONSIDERED QColor("#0067c7")
//#define COLOR_RETIRED QColor("#a3a3a3")
#define COLOR_RETIRED QColor("#909090")

#define COLOR_ADDED_NODE QColor("#ff1717")

#define COLOR_BACKGROUND QColor("#EAEAEA")




#define COLOR_VTABLE_LOWEST_POSITIVE_VALUE QColor(Qt::black) // QColor("#900090") //QColor("#ff1717")
#define COLOR_VTABLE_AFFECTED_DURING_LAST_STEP QColor("#fcba03") // amber



// PYTHON IDLE
#define COLOR_PYTHON_RED QColor("#DD0000")
#define COLOR_PYTHON_ORANGE QColor("#FF7700")
#define COLOR_PYTHON_BLUE QColor("#0000FF")
#define COLOR_PYTHON_VIOLET QColor("#900090")
#define COLOR_PYTHON_GREEN QColor("#00AA00")

// DIJKSTRA
#define COLOR_DIJKSTRA_START_NODE QColor("#4A21FF")
#define COLOR_DIJKSTRA_EXPANDED QColor("#099400")
#define COLOR_DIJKSTRA_UNEXPANDED QColor(Qt::yellow)
#define COLOR_DIJKSTRA_OPTIMAL QColor("#0067c7")
#define COLOR_DIJKSTRA_SUBOPTIMAL QColor("#909090")

// BFS
#define COLOR_BFS_START_NODE QColor("#4A21FF")
#define COLOR_BFS_EXPANDED QColor("#099400")
#define COLOR_BFS_SEEN QColor("#0067c7")
#define COLOR_BFS_OPTIMAL QColor("#0067c7")
#define COLOR_BFS_NO_BACKTRACK QColor("#909090")

// DFS
#define COLOR_DFS_START_NODE QColor("#4A21FF")
#define COLOR_DFS_EXPANDED QColor("#099400")
#define COLOR_DFS_SEEN QColor("#0067c7")
#define COLOR_DFS_OPTIMAL QColor("#0067c7")
#define COLOR_DFS_NO_BACKTRACK QColor("#909090")


// KRUSKALS
#define COLOR_KRUSKALS_ROOT QColor("#900090")

#endif // COLORS_H
