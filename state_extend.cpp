#include "state_extend.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
#include <math.h>
StateExtend::StateExtend(Config conf) : State(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
}
MGraph StateExtend::solve()
{
    MGraph graph(m_input);
    if(isValid(graph)) return graph;
    graph.clear();
    while(this->extend(&graph)) {}
    return graph;
}

bool StateExtend::extend(MGraph *graph)
{
    bool some = false;
    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
        else
            some = true;
    }
    return some;
}

void StateExtend::final()
{
}
