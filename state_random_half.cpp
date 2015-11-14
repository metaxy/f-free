#include "state_random_half.h"
#include "vf.h"
#include "randomize.h"
#include <math.h>
StateRandomHalf::StateRandomHalf(Config conf) : State(conf)
{
}

MGraph StateRandomHalf::solveSingle(MGraph input, MGraph forbidden)
{
    vector<NodeMapping> mappings = VF::subgraphIsoAll(&input, &forbidden);
    MGraph copy(input);
    for(NodeMapping mapping : mappings) {
        for(const Edge &edge : forbidden.edges()) {
            Edge e = Common::transformEdge(edge, &mapping);
            int weight = input.getWeight(e);
            if(weight > 0)
                copy.setWeight(e,  weight + 1 );
            else
                copy.setWeight(e,  weight - 1 );
        }
    }

    NodeMapping mapping = VF::subgraphIsoOne(&input, &forbidden);
    while(!mapping.empty()) {
        for(const Edge &edge : forbidden.edges()) {
            Edge e = Common::transformEdge(edge, &mapping);
            int weight = abs(input.getWeight(e));

            if(r->choice((atan(weight*0.5)/(M_PI/2)))) {
                input.flip(e);
                break;
            }
        }
        mapping = VF::subgraphIsoOne(&input, &forbidden);
    }
    return input;
}
