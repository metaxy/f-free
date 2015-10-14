#ifndef VF_H
#define VF_H
#include "mgraph.h"
#include "argraph.h"
class VF
{
public:
    VF();

    static NodeMapping subgraphIsoOne(MGraph *haystack, MGraph *needle);
    static vector<NodeMapping> subgraphIsoAll(MGraph *haystack, MGraph *needle);
    static Subgraph createSubgraph(MGraph *needle, map<NodeT, NodeT> mapping);

private:
    static Graph createGraph(MGraph *input);

};

#endif // VF_H
