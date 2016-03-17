#include "forbidden.h"
#include "src/iso/vf.h"
#include <algorithm>
Forbidden::Forbidden()
{
}

vector<MGraph> Forbidden::posibleSolutions(MGraph forbidden)
{
   /* vector<MGraph> ret;
    for(Edge e: forbidden.allEdges()) {
        MGraph n(forbidden.nodeCount(), M_CONNECTED); // start with complete graph
        n.flip(e);
        if(VF::subgraphIsoAll(&n, &forbidden).empty()) {
            ret.push_back(n);
        }
    }
    return ret;*/
}

int Forbidden::similarity(NodeMapping mapping, MGraph *forbidden, vector<Edge> diff)
{
    int ret = 0;
    for(Edge edge: forbidden->allEdges()) {
        Edge transformedEdge = Common::transformEdge(edge, &mapping);
        if(find(diff.begin(), diff.end(), transformedEdge) == diff.end())
            ret++;
    }
    return ret;
}

MGraph Forbidden::forbiddenWeight(const MGraph *input, vector<MGraph> forbidden)
{
    MGraph copy(*input);
    for(const MGraph &f : forbidden) {
        vector<NodeMapping> mappings = VF::subgraphIsoAll(input, &f);
        for(NodeMapping mapping : mappings) {
            for(const Edge &edge : f.allEdges()) {
             Edge transformedEdge = Common::transformEdge(edge, &mapping);
             int weight = copy.getWeight(transformedEdge);
             if(weight > 0)
                copy.setWeight(transformedEdge,  weight + 1 );
             else
                copy.setWeight(transformedEdge,  weight - 1 );
            }
        }
    }
    return copy;
}
MGraph Forbidden::forbiddenWeight2(const MGraph *input, vector<MGraph> forbidden)
{
    MGraph copy(*input);
    for(const MGraph &f : forbidden) {
        vector<NodeMapping> mappings = VF::subgraphIsoAll(input, &f);
        for(NodeMapping mapping : mappings) {
            for(const Edge &edge : f.allEdges()) {
                Edge transformedEdge = Common::transformEdge(edge, &mapping);
                int weight = copy.getWeight(transformedEdge);
                int shouldBe = 1 - f.getWeight(edge);

                if(shouldBe == 1) {
                    copy.setWeight(transformedEdge,  weight + 1 );
                } else {
                    copy.setWeight(transformedEdge,  weight - 1 );
                }
                //clog << "shouldBe " << shouldBe << " weight " << weight << " new weight" << copy.getWeight(transformedEdge) << endl;
            }
        }
    }
    return copy;
}
