#include "algorithms/controlled_algorithm.h"
#include "graph/graph.h"

ControlledAlgorithm::ControlledAlgorithm(Graph *g)
    :
      graph_(g)
      //stages_to_proceed(0),
      //steps_to_proceed(0)
{

}

void ControlledAlgorithm::reset()
{
    V.clear();
}

void ControlledAlgorithm::setState(int s)
{
    previous_state = state;
    state = s;
}

int ControlledAlgorithm::getState()
{
    return this->state;
}

int ControlledAlgorithm::getPreviousState()
{
    return this->previous_state;
}

void ControlledAlgorithm::updateGraph(Graph *g_)
{
    if (g_)
        graph_ = g_;
    reset();
}

/*
void ControlledAlgorithm::nextStage()
{
    stages_to_proceed += 1;
    steps_to_proceed = 0;
}

void ControlledAlgorithm::nextStep()
{
    steps_to_proceed += 1;
}*/
