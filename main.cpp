#include "graph.h"
#include "common.h"
#include "mgraph.h"
int main()
{
    GGraph g = Common::graphFromFile("model/k_011_n_110.txt");
    GGraph cluster = Common::graphFromFile("forbidden/cluster.graph");

    MGraph mg(g);
    MGraph mcluster(cluster);

    mg.findInducedSubgraph(&mcluster);

    return 0;
}
 
