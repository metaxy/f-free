#ifndef SPECIAL_SUBGRAPH_ISOMORPHISM_H
#define SPECIAL_SUBGRAPH_ISOMORPHISM_H
#include "src/graph/mgraph.h"
class SpecialSubgraphIsomorphism
{
public:
    SpecialSubgraphIsomorphism();

    static vector<NodeMapping> findAllP3(MGraph *graph);
    static int countAllP3(MGraph *graph);

    static int hasP3(MGraph *graph);
};


#endif // SPECIAL_SUBGRAPH_ISOMORPHISM_H
