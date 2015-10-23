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
        /*} else {
            clog << "is isomorh " << endl;
        }*/
    }
    return ret;
}
