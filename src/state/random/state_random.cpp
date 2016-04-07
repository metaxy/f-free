#include "state_random.h"
#include "src/iso/vf.h"
#include <unordered_map>
StateRandom::StateRandom(Config conf) : MState(conf), m_countSteps(0)
{
}
MGraph StateRandom::solve()
{
    MGraph input(m_input);
    for(MGraph needle : m_forbidden) {
        input = this->solveSingle(input, needle);
    }
    return input;
}
MGraph StateRandom::solveSingle(MGraph input, MGraph forbidden)
{
    map<Edge, int> modified;
    vector<Edge> forbiddenEdges = forbidden.allEdges();
    double forbiddenEdgesSize = forbiddenEdges.size();
    int do_convergence = VF::subgraphIsoCountAll(&input, &forbidden) < this->getInt("convergenceMaxValue", 5000);
    NodeMapping mapping = VF::subgraphIsoOne(&input, &forbidden);
    while(!mapping.empty()) {
        Edge foundEdge;
        while(true) {
            Edge e = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            bool mod = modified.find(e) != modified.end();
            if(this->getInt("markVisited", 1) == 0) {
                mod = false;
            }
            double prob = 0.0;
            if(mod) {
                prob = 1.0/(forbiddenEdgesSize*this->getDouble("visitedProb", 4.0));
            } else {
                prob = 1.0/(forbiddenEdgesSize);
            }
            if(r->choice(prob)) {
                foundEdge = e;
                break;
            }

        }

        if(do_convergence) {
            int size_before = VF::subgraphIsoCountAll(&input, &forbidden);
            input.flip(foundEdge);
            if(VF::subgraphIsoCountAll(&input, &forbidden) > size_before) {
             input.flip(foundEdge);
            } else {
                modified[foundEdge] = 1;
            }
        } else {
            input.flip(foundEdge);
            modified[foundEdge] = 1;
        }
        m_countSteps++;
        mapping = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
void StateRandom::final()
{
    m_debug["countSteps"] = std::to_string(m_countSteps);
}
