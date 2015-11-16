#include "state_random.h"
#include "vf.h"
#include "randomize.h"
#include <unordered_map>
StateRandom::StateRandom(Config conf) : State(conf), m_countSteps(0)
{
}

MGraph StateRandom::solveSingle(MGraph input, MGraph forbidden)
{
    map<Edge, int> modified;
    vector<Edge> forbiddenEdges = forbidden.edges();
    double forbiddenEdgesSize = forbiddenEdges.size();

    NodeMapping mapping = VF::subgraphIsoOne(&input, &forbidden);
    while(!mapping.empty()) {
        Edge foundEdge;
        while(true) {
            Edge e = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            bool mod = modified.find(e) != modified.end();
            double prob = 0.0;
            if(mod) {
                prob = 1.0/(forbiddenEdgesSize*4.0);
            } else {
                prob = 1.0/(forbiddenEdgesSize);
            }
            if(r->choice(prob)) {
                foundEdge = e;
                break;
            }

        }

        input.flip(foundEdge);
        m_countSteps++;
        mapping = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
void StateRandom::final()
{
    m_debug["countSteps"] = std::to_string(m_countSteps);
}
