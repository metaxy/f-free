#include "state_extend.h"
#include "src/iso/vf.h"
#include <math.h>
StateExtend::StateExtend(Config conf) : MState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0)
{
}
MGraph StateExtend::solve()
{
    MGraph graph(m_input);
    if(isValid(&graph)) return graph;
    graph.clear();
    while(this->extend(&graph)) {
        m_countIteration++;
        if(timeLeft() < 0.5) break;
    }
    return graph;
}

bool StateExtend::extend(MGraph *graph)
{
    bool some = false;
    vector<Edge> edges = r->randomVector(m_input.difference(graph));
     clog << "diff = " << m_input.difference(graph).size() << endl;
    for(Edge e : edges) {
        graph->flip(e);
        if(!isValid(graph)) {
            graph->flip(e);
            m_invalidChanges++;
        } else {
            some = true;
            m_validChanges++;
        }
        if(timeLeft() < 0.2) break;
    }
     clog << "valid changes" << m_validChanges << " invalid" << m_invalidChanges << endl;
    return some;
}

void StateExtend::final()
{
    m_debug["countIteration"] = std::to_string(m_countIteration);
    m_debug["validChanges"] = std::to_string(m_validChanges);
    m_debug["invalidChanges"] = std::to_string(m_invalidChanges);
}
