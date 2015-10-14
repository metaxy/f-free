#ifndef VF_H
#define VF_H
#include "mgraph.h"
#include "argraph.h"
class VF
{
public:
    VF();

    static Subgraph subgraphIsoOne(MGraph *haystack, MGraph *needle);
    static vector<Subgraph> subgraphIsoAll(MGraph *haystack, MGraph *needle);

private:
    static Graph createGraph(MGraph *input);

};

#endif // VF_H
