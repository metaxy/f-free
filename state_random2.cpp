#include "state_random2.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
StateRandom2::StateRandom2(Config conf) : State(conf)
{
}
MGraph StateRandom2::solve()
{
    MGraph input(m_input);
    for(MGraph needle : m_forbidden) {
        input = this->solveSingle(input, needle);
    }
    return input;
}
MGraph StateRandom2::solveSingle(MGraph input, MGraph forbidden)
{
    NodeMapping mapping = VF::subgraphIsoOne(&input, &forbidden);
    while(!mapping.empty()) {
        Edge e = Common::transformEdge(r->randomElement(forbidden.edges()), &mapping);
        input.flip(e);
        m_countSteps++;
        mapping = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
void StateRandom2::final()
{
    m_debug["countSteps"] = std::to_string(m_countSteps);
}
