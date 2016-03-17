#include "special_subgraph_isomorphism.h"
#include "reducednodemapping.h"
SpecialSubgraphIsomorphism::SpecialSubgraphIsomorphism()
{
}

vector<NodeMapping> SpecialSubgraphIsomorphism::findAllP3(MGraph *graph)
{
    ReducedNodeMapping ret;
    for(int i = 0; i < graph->nodeCount(); i++) {

        for(int j = 0; j < graph->nodeCount(); j++) {
            if(i == j || !graph->connected(i,j)) continue;

            for(int k = 0; k < graph->nodeCount(); k++) {
                if(j != k && i != k && graph->connected(j,k) && !graph->connected(i,k)) {
                    NodeMapping map;
                    map[0] = i;
                    map[1] = j;
                    map[2] = k;
                    ret.add(map);
                }
            }

        }
    }
    return ret.get();
}
int SpecialSubgraphIsomorphism::countAllP3(MGraph *graph)
{
    int count = 0;
    for(int i = 0; i < graph->nodeCount(); i++) {

        for(int j = 0; j < graph->nodeCount(); j++) {
            if(i == j || !graph->connected(i,j)) continue;

            for(int k = 0; k < graph->nodeCount(); k++) {
                if(j != k && i != k && graph->connected(j,k) && !graph->connected(i,k)) {
                    count++;
                }
            }

        }
    }
    return count;
}

int SpecialSubgraphIsomorphism::hasP3(MGraph *graph)
{
    for(int i = 0; i < graph->nodeCount(); i++) {

        for(int j = 0; j < graph->nodeCount(); j++) {
            if(i == j || !graph->connected(i,j)) continue;

            for(int k = 0; k < graph->nodeCount(); k++) {
                if(j != k && i != k && graph->connected(j,k) && !graph->connected(i,k)) {
                    return true;
                }
            }

        }
    }
    return false;
}
