#ifndef CONTROLLEDALGORITHM_H
#define CONTROLLEDALGORITHM_H

#include <QList>
#include "typedefs.h"

class Graph;
class Edge;
class Node;

class ControlledAlgorithm
{
public:
    ControlledAlgorithm(Graph *g);

    /*// each stage has few steps
    void nextStage();
    void nextStep();*/


    int getState();
    int getPreviousState();

    virtual void reset();
    virtual void step() = 0;
    virtual QString name() = 0;
    virtual QString brief() = 0;
    virtual QString currentStepLabel() = 0;
    virtual QString currentStepDescription() = 0;
    virtual bool finished() = 0;

    void updateGraph(Graph *g_);
    Graph* graph() { return graph_; }


    Vtable vTable() { return V; }
    QList<Node *> updatedNodes() { return updated_nodes; } // useful for highlighting changes in V table

protected:
    Graph *graph_;
    int state, previous_state;

    // Useful for highlighting changes in V table
    // User will be able to hover over the entry to see its previous value
    // Without it, the user would have to pay very close attention to V table to notice which value got changed
    QList<Node *> updated_nodes;
    Vtable V;

    virtual void setState(int state);




    //int stages_to_proceed, steps_to_proceed;
};

#endif // CONTROLLEDALGORITHM_H
