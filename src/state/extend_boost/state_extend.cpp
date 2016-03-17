#include "state_extend.h"
#include <math.h>
StateExtend::StateExtend(Config conf) : BState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0)
{
}
BoostGraph StateExtend::solve()
{
    BoostGraph graph(m_input);
    if(isValid(&graph)) return graph;
    graph.clear();
    while(this->extend(&graph)) {
        m_countIteration++;
    }
    return graph;
}

bool StateExtend::extend(BoostGraph *graph)
{
    bool some = false;
    clog << "diff = " << m_input.difference(graph).size() << endl;
    vector<Edge> edges = r->randomVector(m_input.difference(graph));
    for(Edge e : edges) {
        graph->flip(e);
        if(!isValid(graph)) {
            graph->flip(e);
            m_invalidChanges++;
        } else {
            some = true;
            m_validChanges++;
        }
    }
    return some;
}

void StateExtend::final()
{
    m_debug["countIteration"] = std::to_string(m_countIteration);
    m_debug["validChanges"] = std::to_string(m_validChanges);
    m_debug["invalidChanges"] = std::to_string(m_invalidChanges);
}
