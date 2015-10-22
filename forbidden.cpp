#include "forbidden.h"
#include "vf.h"
Forbidden::Forbidden()
{
}

vector<MGraph> Forbidden::posibleSolutions(MGraph forbidden)
{
    vector<MGraph> ret;
    for(Edge e: forbidden.edges()) {
        MGraph n(forbidden);
        n.flip(e);
        if(VF::subgraphIsoAll(&n, &forbidden).empty()) {
            ret.push_back(n);
        }
    }
    return ret;
}
