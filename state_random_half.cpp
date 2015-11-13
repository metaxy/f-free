#include "state_random_half.h"
#include "vf.h"
#include "randomize.h"

StateRandomHalf::StateRandomHalf(Config conf) : State(conf)
{
}

MGraph StateRandomHalf::solveSingle(MGraph input, MGraph forbidden)
{
    vector<NodeMapping> mappings = VF::subgraphIsoAll(&input, &forbidden);
    while(!mappings.empty()) {
        clog << "found mappings " << mappings.size() << endl;
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
        input.printMatrix();
        copy.printMatrix();
        clog << "abs " << copy.absHeightesEdgeOfEachRow().size() << endl;
        for(Edge e : copy.absHeightesEdgeOfEachRow()) {
            input.flip(e);
        }
        input.normalize();
        mappings = VF::subgraphIsoAll(&input, &forbidden);
    }
    return input;
}
