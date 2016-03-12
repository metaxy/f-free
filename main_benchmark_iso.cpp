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
        conf["instances"] = "/home/paul/coding/f-free/model/bio1/";
    }
    vector<string> files = Common::listFiles(conf["instances"]);
    for(string file : files) {
        if(!Common::endsWith(file, ".txt") && !Common::endsWith(file, ".graph")) continue;
        graphs.push_back(MGraph(Common::graphFromFile(conf["instances"] + "/"+ file)));
    }
    MGraph p3 = MGraph(Common::graphFromFile("/home/paul/coding/f-free/forbidden/cluster/p3.txt"));
    vector<MGraph> p3s;
    p3s.push_back(p3);

    cout << "==== find all p3s ====" << endl;
    Timer timer;
   /* for(MGraph graph : graphs) {
        SpecialSubgraphIsomorphism::findAllP3(&graph);
    }
    cout << "special:" << timer.elapsed() << endl;

    timer.reset();
    for(MGraph graph : graphs) {
        VF::subgraphIsoAll(&graph, &p3);
    }
    cout << "SP:" << timer.elapsed() << endl;

    cout << endl <<"==== count all p3s === " << endl;
    timer.reset();

    for(MGraph graph : graphs) {
        SpecialSubgraphIsomorphism::countAllP3(&graph);
    }
    cout << "special:" << timer.elapsed() << endl;

    timer.reset();
    for(MGraph graph : graphs) {
        VF::subgraphIsoCountAll(&graph, &p3);
    }
    cout << "VF:" << timer.elapsed() << endl;

    cout << endl << "==== has a p3 === " << endl;
    timer.reset();*/

    for(MGraph graph : graphs) {
        for(int i  = 0; i < 100; i++)
            SpecialSubgraphIsomorphism::hasP3(&graph);
    }
    cout << "SP:" << timer.elapsed() << endl;

    timer.reset();
    for(MGraph graph : graphs) {
        for(int i  = 0; i < 100; i++)
            VF::subgraphIsoHasOne(&graph, p3s);
    }
    cout << "VF:" << timer.elapsed() << endl;

    //test p3


}
 
