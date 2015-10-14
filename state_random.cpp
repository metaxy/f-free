#include "state_random.h"
#include "vf.h"
#include "randomize.h"
StateRandom::StateRandom(Config conf) : State(conf)
{
}

MGraph StateRandom::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r;
    NodeMapping mapping = VF::subgraphIsoOne(&input, &forbidden);
    int countSteps = 0;
    while(!mapping.empty()) {
        Edge e = Common::transformEdge(r.randomElement(forbidden.edges()), &mapping);
        if(r.choice(0.5))
            input.flip(e);
        else
            input.merge(e);

        countSteps++;
        mapping = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
