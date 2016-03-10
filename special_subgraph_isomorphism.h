#ifndef SPECIAL_SUBGRAPH_ISOMORPHISM_H
#define SPECIAL_SUBGRAPH_ISOMORPHISM_H
#include "mgraph.h"
class SpecialSubgraphIsomorphism
{
public:
    SpecialSubgraphIsomorphism();

    static vector<NodeMapping> findAllP3(MGraph *graph);
};

#endif // SPECIAL_SUBGRAPH_ISOMORPHISM_H
