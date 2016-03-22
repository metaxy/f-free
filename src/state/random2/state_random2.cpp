#include "state_random2.h"
#include "src/iso/vf.h"
StateRandom2::StateRandom2(Config conf) : MState(conf)
{
}
MGraph StateRandom2::solve()
{
    MGraph graph(m_input);
    map<Edge,int> modified;
    for(MGraph needle : m_forbidden) {
        NodeMapping mapping = VF::subgraphIsoOne(&graph, &needle);
        while(!mapping.empty()) {
            for(int i = 0; i < needle.allEdges().size(); i++) {
                Edge e = Common::transformEdge(r->randomElement(needle.allEdges()), &mapping);
                if(modified.find(e) != modified.end()) {
                    graph.flip(e);
                    modified[e] = 1;
                    break;
                }
            }
            m_countSteps++;
            mapping = VF::subgraphIsoOne(&graph, &needle);
        }
    }
    return graph;
}
void StateRandom2::final()
{
    m_debug["countSteps"] = std::to_string(m_countSteps);
}
