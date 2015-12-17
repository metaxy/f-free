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
    graph.clear();
    for(int i = 0; i < 20; i++) {
        this->extend(&graph);
    }
    return graph;
}

void StateExtend::extend(MGraph *graph)
{
    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
    }
}

void StateExtend::final()
{
}
