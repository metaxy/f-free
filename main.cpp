#include "graph.h"
#include "common.h"
#include "mgraph.h"
int main()
{
    Graph g = Common::graphFromFile("model/k_011_n_110.txt");
    Graph cluster = Common::graphFromFile("forbidden/cluster.graph");

    MGraph mg(g);
    MGraph mcluster(cluster);

    return mg.findInducedSubgraphs(mcluster);

    return 0;
}
 
