#include "state_random_half.h"
#include "vf.h"
#include "randomize.h"
StateRandomHalf::StateRandomHalf(Config conf) : State(conf)
{
}

MGraph StateRandomHalf::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r(getInt("seed", 5489));

    vector<NodeMapping> mappings = VF::subgraphIso(&input, &forbidden, 1000);
    cout << mappings.size() << endl;
    /*while(!mappings.empty()) {
        for(NodeMapping mapping : mappings) {
            for(const Edge &edge : forbidden.edges()) {
                Edge e = Common::transformEdge(edge, &mapping);
                int weight = input.reduceWeight(e, 2);
            }
        }

        mappings = VF::subgraphIso(&input, &forbidden, batch);
    }*/
    return input;
}
