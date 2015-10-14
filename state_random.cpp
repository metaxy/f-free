#include "state_random.h"
#include "vf.h"
#include "randomize.h"
StateRandom::StateRandom(Config conf) : State(conf)
{
}

MGraph StateRandom::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r;
    Subgraph found = VF::subgraphIsoOne(&input, &forbidden);
    input.writeGraph("input1");
    forbidden.writeGraph("forbidden");
    input.debugEdges(found);

    int countSteps = 0;
    while(found.size() != 0) {
        Edge e = r.randomElement(found);
        input.flip(e);
        countSteps++;
        found = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
