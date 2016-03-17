#ifndef VF_H
#define VF_H
#include "src/graph/mgraph.h"
#include "argraph.h"
class VF
{
public:
    VF();

    static NodeMapping subgraphIsoOne(const MGraph *haystack, const MGraph *needle);
    static bool subgraphIsoHasOne(const MGraph *haystack, vector<MGraph> needle);
    static vector<NodeMapping> subgraphIsoAll(const MGraph *haystack, const MGraph *needle);
    static Subgraph createSubgraph(const MGraph *needle, map<NodeT, NodeT> mapping);
    static vector<NodeMapping> subgraphIso(const MGraph *haystack, const MGraph *needle, int count);
    static vector<NodeMapping> subgraphMono(const MGraph *haystack, const MGraph *needle, int count);
    static int subgraphIsoCountAll(const MGraph *haystack, const MGraph *needle);

private:
    static Graph createGraph(const MGraph *input);

};

#endif // VF_H
