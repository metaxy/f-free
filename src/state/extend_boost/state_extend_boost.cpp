#include "state_extend_boost.h"
#include <math.h>
#include <unordered_map>
StateExtendBoost::StateExtendBoost(Config conf) : BState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0)
{
}
BoostGraph StateExtendBoost::solve()
{
    BoostGraph graph(m_input);
    if(isValid(&graph)) return graph;
    graph.clear();

    map<Edge, int> modified;
    while(true) {
        m_countIteration++;
        bool some = false;
        clog << "diff = " << m_input.difference(&graph).size() << endl;
        vector<Edge> edges = r->randomVector(m_input.difference(&graph));
        for(Edge e : edges) {
            if(modified.find(e) != modified.end()) {
                continue;
            }
            graph.flip(e);
            if(!isValid(&graph)) {
                graph.flip(e);
                m_invalidChanges++;
            } else {
                some = true;
                modified[e] = 1;
                m_validChanges++;
            }
        }
        if(!some)
            break;
        clog << "valid changes " << m_validChanges << " invalid" << m_invalidChanges << endl;
    }
    return graph;
}

bool StateExtendBoost::extend(BoostGraph *graph)
{

}

void StateExtendBoost::final()
{
    m_debug["countIteration"] = std::to_string(m_countIteration);
    m_debug["validChanges"] = std::to_string(m_validChanges);
    m_debug["invalidChanges"] = std::to_string(m_invalidChanges);
}
