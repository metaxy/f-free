#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"
int main()
{
    GGraph g = Common::graphFromFile("../model/k_003_n_038.txt");
    GGraph cluster = Common::graphFromFile("../forbidden/cluster.graph");
    MGraph mg(g);
    MGraph mcluster(cluster);

    vector<Subgraph> graphs = VF::subgraphIsoAll(&mg, &mcluster);
    /*for(Subgraph g : graphs) {
        Common::printSubgraph(&g);
    }*/

    return 0;
}
 
