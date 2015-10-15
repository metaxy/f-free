#include "state_random.h"
#include "vf.h"
#include "randomize.h"
StateRandom::StateRandom(Config conf) : State(conf)
{
}

MGraph StateRandom::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r(getInt("seed", 5489));
    NodeMapping mapping = VF::subgraphIsoOne(&input, &forbidden);
    int countSteps = 0;
    while(!mapping.empty()) {
        Edge e = Common::transformEdge(r.randomElement(forbidden.edges()), &mapping);
        input.flip(e);
        countSteps++;
        mapping = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
