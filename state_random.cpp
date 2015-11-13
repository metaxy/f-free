#include "state_random.h"
#include "vf.h"
#include "randomize.h"
StateRandom::StateRandom(Config conf) : State(conf), m_countSteps(0)
{
}

MGraph StateRandom::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r(getInt("seed", 5489));
    NodeMapping mapping = VF::subgraphIsoOne(&input, &forbidden);
    while(!mapping.empty()) {
        Edge e = Common::transformEdge(r.randomElement(forbidden.edges()), &mapping);
        input.flip(e);
        m_countSteps++;
        mapping = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
void StateRandom::final()
{
    m_debug["countSteps"] = std::to_string(m_countSteps);
}
