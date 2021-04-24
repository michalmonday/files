#include "preset_graphs.h"
#include "edge.h"
#include "node.h"
#include "graph.h"

PresetGraphs::PresetGraphs()
{

}

Graph *PresetGraphs::generateForAlgorithm(QString algorithm_name)
{
    if (algorithm_name == "Prims")
        return prims();
    if (algorithm_name == "Dijkstra")
        return dijkstra();
    if (algorithm_name == "Breadth first search")
        return bfs();
    if (algorithm_name == "Depth first search")
        return dfs();
    if (algorithm_name == "Kruskals")
        return kruskals();

    return prims();
}


QList<Graph *> PresetGraphs::all()
{
    return {circular(), dijkstra(), tree(), treeLarge(), square4x4(), square5x5()};
}

Graph *PresetGraphs::prims()
{
    return circular();
}

Graph *PresetGraphs::kruskals()
{
    return circular();
}

Graph *PresetGraphs::dijkstra()
{
    Graph *g = new Graph();
    g->name("For Dijkstra");

    g->addEdge("a", "b", 2);
    g->addEdge("a", "c", 4);
    g->addEdge("b", "d", 7);
    g->addEdge("d", "e", 2);
    g->addEdge("e", "f", 5);
    g->addEdge("f", "d", 1);
    g->addEdge("c", "e", 3);
    g->addEdge("b", "c", 1);

    g->getNodeForLabel("a")->setPos(-120, 0);
    g->getNodeForLabel("b")->setPos(-50, -50);
    g->getNodeForLabel("c")->setPos(-50, 50);
    g->getNodeForLabel("d")->setPos(50, -50);
    g->getNodeForLabel("e")->setPos(50, 50);
    g->getNodeForLabel("f")->setPos(120, 0);
    return g;
}

Graph *PresetGraphs::bfs()
{
    return tree();
}

Graph *PresetGraphs::dfs()
{
    return tree();
}

Graph * PresetGraphs::circular() {
    Graph *g = new Graph();
    g->name("Circular");

    g->addEdge("a", "b", 4);
    g->addEdge("c", "d", 2);
    g->addEdge("d", "e", 20);
    g->addEdge("e", "f", 3);
    g->addEdge("f", "g", 22);
    g->addEdge("g", "h", 1);
    g->addEdge("h", "a", 21);
    g->addEdge("b", "c", 23);
    g->addEdge("h", "d", 5);
    g->addEdge("a", "c", 6);
    g->addEdge("g", "e", 7);

    g->getNodeForLabel("a")->setPos(-135, -50);
    g->getNodeForLabel("b")->setPos(-55, -120);
    g->getNodeForLabel("c")->setPos(75, -120);
    g->getNodeForLabel("d")->setPos(135, -50);
    g->getNodeForLabel("e")->setPos(135, 50);
    g->getNodeForLabel("f")->setPos(65, 120);
    g->getNodeForLabel("g")->setPos(-75, 120);
    g->getNodeForLabel("h")->setPos(-135, 50);
    return g;
}



Graph * PresetGraphs::tree() {
    Graph *g = new Graph();
    g->name("Tree");

    g->addEdge("a", "b", 1);
    g->addEdge("f", "d", 1);
    g->addEdge("b", "c", 1);
    g->addEdge("a", "d", 1);
    g->addEdge("d", "g", 1);
    g->addEdge("b", "e", 1);
    g->addEdge("h", "c", 1);
    g->addEdge("c", "i", 1);
    g->addEdge("e", "j", 1);
    g->addEdge("e", "k", 1);
    g->addEdge("f", "l", 1);
    g->addEdge("f", "m", 1);
    g->addEdge("g", "n", 1);
    g->addEdge("g", "o", 1);

    g->getNodeForLabel("a")->setPos(10, -90);
    g->getNodeForLabel("b")->setPos(-70, -30);
    g->getNodeForLabel("c")->setPos(-110, 30);
    g->getNodeForLabel("d")->setPos(80, -30);
    g->getNodeForLabel("e")->setPos(-40, 30);
    g->getNodeForLabel("f")->setPos(40, 30);
    g->getNodeForLabel("g")->setPos(120, 30);
    g->getNodeForLabel("h")->setPos(-140, 90);
    g->getNodeForLabel("i")->setPos(-90, 90);
    g->getNodeForLabel("j")->setPos(-60, 90);
    g->getNodeForLabel("k")->setPos(-20, 90);
    g->getNodeForLabel("l")->setPos(20, 90);
    g->getNodeForLabel("m")->setPos(60, 90);
    g->getNodeForLabel("n")->setPos(100, 90);
    g->getNodeForLabel("o")->setPos(140, 90);
    return g;
}


Graph *PresetGraphs::treeLarge()
{
    Graph *g = new Graph();
    g->name("Large tree");

    g->addEdge("a", "b", 10);
    g->addEdge("a", "c", 6);
    g->addEdge("a", "d", 5);
    g->addEdge("e", "a", 3);
    g->addEdge("f", "b", 17);
    g->addEdge("b", "g", 12);
    g->addEdge("f", "h", 5);
    g->addEdge("g", "i", 3);
    g->addEdge("e", "j", 14);
    g->addEdge("e", "k", 13);
    g->addEdge("j", "l", 3);
    g->addEdge("k", "m", 2);
    g->addEdge("d", "n", 17);
    g->addEdge("n", "o", 19);
    g->addEdge("d", "p", 16);
    g->addEdge("p", "q", 8);
    g->addEdge("c", "r", 7);
    g->addEdge("c", "s", 12);
    g->addEdge("r", "t", 19);
    g->addEdge("s", "u", 10);

    g->getNodeForLabel("a")->setPos(5, -15);
    g->getNodeForLabel("b")->setPos(-65, -15);
    g->getNodeForLabel("c")->setPos(5, 55);
    g->getNodeForLabel("d")->setPos(75, -15);
    g->getNodeForLabel("e")->setPos(5, -85);
    g->getNodeForLabel("f")->setPos(-145, 25);
    g->getNodeForLabel("g")->setPos(-145, -55);
    g->getNodeForLabel("h")->setPos(-225, 65);
    g->getNodeForLabel("i")->setPos(-225, -95);
    g->getNodeForLabel("j")->setPos(-45, -155);
    g->getNodeForLabel("k")->setPos(55, -155);
    g->getNodeForLabel("l")->setPos(-95, -225);
    g->getNodeForLabel("m")->setPos(105, -225);
    g->getNodeForLabel("n")->setPos(155, -65);
    g->getNodeForLabel("o")->setPos(225, -105);
    g->getNodeForLabel("p")->setPos(155, 35);
    g->getNodeForLabel("q")->setPos(225, 75);
    g->getNodeForLabel("r")->setPos(-55, 145);
    g->getNodeForLabel("s")->setPos(55, 145);
    g->getNodeForLabel("t")->setPos(-105, 225);
    g->getNodeForLabel("u")->setPos(95, 225);
    return g;
}

Graph *PresetGraphs::square5x5()
{
    Graph *g = new Graph();
    g->name("Square 5x5");

    g->addEdge("a", "b", 2);
    g->addEdge("a", "c", 8);
    g->addEdge("c", "d", 7);
    g->addEdge("d", "b", 12);
    g->addEdge("b", "e", 19);
    g->addEdge("e", "f", 10);
    g->addEdge("d", "f", 13);
    g->addEdge("c", "g", 12);
    g->addEdge("g", "h", 20);
    g->addEdge("h", "d", 16);
    g->addEdge("h", "i", 15);
    g->addEdge("i", "f", 16);
    g->addEdge("a", "d", 12);
    g->addEdge("b", "f", 3);
    g->addEdge("f", "h", 14);
    g->addEdge("d", "g", 14);
    g->addEdge("g", "j", 5);
    g->addEdge("j", "k", 2);
    g->addEdge("k", "h", 12);
    g->addEdge("k", "l", 14);
    g->addEdge("l", "i", 9);
    g->addEdge("l", "m", 8);
    g->addEdge("m", "n", 5);
    g->addEdge("n", "i", 3);
    g->addEdge("n", "o", 18);
    g->addEdge("o", "f", 18);
    g->addEdge("o", "p", 20);
    g->addEdge("p", "e", 4);
    g->addEdge("e", "o", 2);
    g->addEdge("o", "i", 10);
    g->addEdge("i", "m", 19);
    g->addEdge("h", "l", 17);
    g->addEdge("g", "k", 16);
    g->addEdge("j", "q", 2);
    g->addEdge("q", "r", 14);
    g->addEdge("r", "k", 16);
    g->addEdge("r", "s", 20);
    g->addEdge("s", "l", 19);
    g->addEdge("s", "t", 5);
    g->addEdge("t", "m", 11);
    g->addEdge("t", "u", 18);
    g->addEdge("u", "v", 7);
    g->addEdge("v", "w", 14);
    g->addEdge("w", "x", 7);
    g->addEdge("x", "y", 2);
    g->addEdge("y", "p", 6);
    g->addEdge("o", "x", 5);
    g->addEdge("w", "n", 13);
    g->addEdge("m", "v", 11);
    g->addEdge("k", "q", 10);
    g->addEdge("l", "r", 18);
    g->addEdge("m", "s", 1);
    g->addEdge("v", "t", 18);
    g->addEdge("w", "m", 7);
    g->addEdge("x", "n", 11);
    g->addEdge("y", "o", 2);

    g->getNodeForLabel("a")->setPos(-250, -240);
    g->getNodeForLabel("b")->setPos(-130, -240);
    g->getNodeForLabel("c")->setPos(-250, -120);
    g->getNodeForLabel("d")->setPos(-130, -120);
    g->getNodeForLabel("e")->setPos(0, -240);
    g->getNodeForLabel("f")->setPos(0, -120);
    g->getNodeForLabel("g")->setPos(-250, -10);
    g->getNodeForLabel("h")->setPos(-130, -10);
    g->getNodeForLabel("i")->setPos(0, -10);
    g->getNodeForLabel("j")->setPos(-250, 110);
    g->getNodeForLabel("k")->setPos(-130, 110);
    g->getNodeForLabel("l")->setPos(0, 110);
    g->getNodeForLabel("m")->setPos(120, 110);
    g->getNodeForLabel("n")->setPos(120, -10);
    g->getNodeForLabel("o")->setPos(120, -120);
    g->getNodeForLabel("p")->setPos(120, -240);
    g->getNodeForLabel("q")->setPos(-250, 240);
    g->getNodeForLabel("r")->setPos(-130, 240);
    g->getNodeForLabel("s")->setPos(0, 240);
    g->getNodeForLabel("t")->setPos(120, 240);
    g->getNodeForLabel("u")->setPos(250, 240);
    g->getNodeForLabel("v")->setPos(250, 110);
    g->getNodeForLabel("w")->setPos(250, -10);
    g->getNodeForLabel("x")->setPos(250, -120);
    g->getNodeForLabel("y")->setPos(250, -240);

    return g;
}

Graph *PresetGraphs::square4x4()
{
    Graph *g = new Graph();
    g->name("Square 4x4");

    g->addEdge("a", "b", 2);
    g->addEdge("a", "c", 8);
    g->addEdge("c", "d", 7);
    g->addEdge("d", "b", 12);
    g->addEdge("b", "e", 19);
    g->addEdge("e", "f", 10);
    g->addEdge("d", "f", 13);
    g->addEdge("c", "g", 12);
    g->addEdge("g", "h", 20);
    g->addEdge("h", "d", 16);
    g->addEdge("h", "i", 15);
    g->addEdge("i", "f", 16);
    g->addEdge("a", "d", 12);
    g->addEdge("b", "f", 3);
    g->addEdge("f", "h", 14);
    g->addEdge("d", "g", 14);
    g->addEdge("g", "j", 5);
    g->addEdge("j", "k", 2);
    g->addEdge("k", "h", 12);
    g->addEdge("k", "l", 14);
    g->addEdge("l", "i", 9);
    g->addEdge("l", "m", 8);
    g->addEdge("m", "n", 5);
    g->addEdge("n", "i", 3);
    g->addEdge("n", "o", 18);
    g->addEdge("o", "f", 18);
    g->addEdge("o", "p", 20);
    g->addEdge("p", "e", 4);
    g->addEdge("e", "o", 2);
    g->addEdge("o", "i", 10);
    g->addEdge("i", "m", 19);
    g->addEdge("h", "l", 17);
    g->addEdge("g", "k", 16);

    g->getNodeForLabel("a")->setPos(-185, -175);
    g->getNodeForLabel("b")->setPos(-65, -175);
    g->getNodeForLabel("c")->setPos(-185, -55);
    g->getNodeForLabel("d")->setPos(-65, -55);
    g->getNodeForLabel("e")->setPos(65, -175);
    g->getNodeForLabel("f")->setPos(65, -55);
    g->getNodeForLabel("g")->setPos(-185, 55);
    g->getNodeForLabel("h")->setPos(-65, 55);
    g->getNodeForLabel("i")->setPos(65, 55);
    g->getNodeForLabel("j")->setPos(-185, 175);
    g->getNodeForLabel("k")->setPos(-65, 175);
    g->getNodeForLabel("l")->setPos(65, 175);
    g->getNodeForLabel("m")->setPos(185, 175);
    g->getNodeForLabel("n")->setPos(185, 55);
    g->getNodeForLabel("o")->setPos(185, -55);
    g->getNodeForLabel("p")->setPos(185, -175);

    return g;
}


