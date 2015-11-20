#include "forbidden.h"
#include "vf.h"
Forbidden::Forbidden()
{
}

vector<MGraph> Forbidden::posibleSolutions(MGraph forbidden)
{
    vector<MGraph> ret;
    for(Edge e: forbidden.edges()) {
        MGraph n(forbidden.nodeCount(), 1); // start with complete graph
        n.flip(e);
        //if(VF::subgraphIsoAll(&n, &forbidden).empty()) {
            ret.push_back(n);
    }
    return ret;
}

MGraph Forbidden::forbiddenWeight(const MGraph *input, vector<MGraph> forbidden)
{
    MGraph copy(*input);
    for(const MGraph &f : forbidden) {
        vector<NodeMapping> mappings = VF::subgraphIsoAll(input, &f);
        for(NodeMapping mapping : mappings) {
            for(const Edge &edge : f.edges()) {
             Edge e = Common::transformEdge(edge, &mapping);
             int weight = copy.getWeight(e);
             if(weight > 0)
                    copy.setWeight(e,  weight + 1 );
             else
                    copy.setWeight(e,  weight - 1 );
            }
        }
    }
    return copy;
}
