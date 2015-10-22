#include "state_random2.h"
#include "vf.h"
#include "randomize.h"
StateRandom2::StateRandom2(Config conf) : State(conf)
{
}

MGraph StateRandom2::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r(getInt("seed", 5489));
    int batch = getInt("random2_subgraph_batch", 10);
    vector<NodeMapping> mappings = VF::subgraphIso(&input, &forbidden, batch);
    int countSteps = 0;
    while(!mappings.empty()) {
        for(NodeMapping mapping : mappings) {
            Edge e = Common::transformEdge(r.randomElement(forbidden.edges()), &mapping);
            input.flip(e);
            countSteps++;
        }
        mappings = VF::subgraphIso(&input, &forbidden, batch);
    }
    return input;
}
