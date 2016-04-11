#include "state_simple_random.h"
StateSimpleRandom::StateSimpleRandom(Config conf) : BState(conf), m_countSteps(0)
{
}
BoostGraph StateSimpleRandom::solve()
{
    BoostGraph graph(m_input);
    map<Edge,int> modified;
    while(!isValid(&graph)) {
        for(BoostGraph *forbidden : m_forbidden) {
            while(true) {
                m_countSteps++;
                NodeMapping mapping = graph.subgraphIsoOne(forbidden);
                if(mapping.empty()) {
                     break;
                }

                for(int i = 0; i < forbidden->allEdges().size(); i++) {
                    Edge e = Common::transformEdge(r->randomElement(forbidden->allEdges()), &mapping);
                    if(modified.find(e) == modified.end()) {
                        graph.flip(e);
                        modified[e] = 1;
                        break;
                    }
                }
            }
        }
    }
    return graph;
}
void StateSimpleRandom::final()
{
    m_debug["countSteps"] = std::to_string(m_countSteps);
}
BoostGraph* StateSimpleRandom::randomElement(vector<BoostGraph*> list)
{
    std::uniform_int_distribution<> dis(0, list.size() - 1);
    return list[dis(r->m_rgen)];
}
