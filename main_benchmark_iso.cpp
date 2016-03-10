#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"
#include "timer.h"
#include "special_subgraph_isomorphism.h"

vector<MGraph> getForbidden(string name)
{
    vector<MGraph> graphs;
    vector<string> files = Common::listFiles(name);
    for(string file : files) {
        if(!Common::endsWith(file, ".txt") && !Common::endsWith(file, ".graph")) continue;
        graphs.push_back(MGraph(Common::graphFromFile(name + "/"+ file)));
    }
    return graphs;
}

int main(int argc, char* argv[])
{
    vector<string> options = {"instances"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    vector<MGraph> graphs;
    if(conf.find("instances") == conf.end()) {
        conf["instances"] = "/home/paul/coding/f-free/model/cluster/";
    }
    vector<string> files = Common::listFiles(conf["instances"]);
    for(string file : files) {
        if(!Common::endsWith(file, ".txt") && !Common::endsWith(file, ".graph")) continue;
        graphs.push_back(MGraph(Common::graphFromFile(conf["instances"] + "/"+ file)));
    }

    Timer timer;
    for(MGraph graph : graphs) {
        SpecialSubgraphIsomorphism::findAllP3(&graph);
    }
    cout << timer.elapsed() << endl;

    MGraph p3 = MGraph(Common::graphFromFile("/home/paul/coding/f-free/forbidden/cluster/p3.txt"));
    timer.reset();
    for(MGraph graph : graphs) {
        VF::subgraphIsoAll(&graph, &p3);
    }
    cout << timer.elapsed() << endl;

    //test p3


}
 
