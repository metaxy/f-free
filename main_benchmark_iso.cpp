#include "common.h"
#include "mgraph.h"
#include "boostgraph.h"
#include "vf.h"
#include "timer.h"
#include "special_subgraph_isomorphism.h"
#include "boostgraph.h"
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
    vector<BoostGraph> bgraphs;
    if(conf.find("instances") == conf.end()) {
        conf["instances"] = "/home/paul/coding/f-free/model/bio1/";
    }
    vector<string> files = Common::listFiles(conf["instances"]);
    for(string file : files) {
        if(!Common::endsWith(file, ".txt") && !Common::endsWith(file, ".graph")) continue;
        graphs.push_back(MGraph(Common::graphFromFile(conf["instances"] + "/"+ file)));
        bgraphs.push_back(BoostGraph(Common::graphFromFile(conf["instances"] + "/"+ file)));
    }

    MGraph p3 = MGraph(Common::graphFromFile("/home/paul/coding/f-free/forbidden/cluster/p3.txt"));
    BoostGraph bp3 = BoostGraph(Common::graphFromFile("/home/paul/coding/f-free/forbidden/cluster/p3.txt"));
    vector<MGraph> p3s;
    p3s.push_back(p3);

    vector<BoostGraph> bp3s;
    bp3s.push_back(bp3);

    Timer timer;


    cout << "==== find all p3s ====" << endl;

    for(MGraph graph : graphs) {
        SpecialSubgraphIsomorphism::findAllP3(&graph);
    }
    cout << "SP:" << timer.elapsed() << endl;

    timer.reset();
    for(MGraph graph : graphs) {
        VF::subgraphIsoAll(&graph, &p3);
    }
    cout << "VF:" << timer.elapsed() << endl;

    timer.reset();
    for(BoostGraph graph : bgraphs) {
        graph.subgraphIsoAll(&bp3);
    }
    cout << "BO:" << timer.elapsed() << endl;





    cout << endl <<"==== count all p3s === " << endl;
    timer.reset();

    for(MGraph graph : graphs) {
        SpecialSubgraphIsomorphism::countAllP3(&graph);
    }
    cout << "SP:" << timer.elapsed() << endl;

    timer.reset();
    for(MGraph graph : graphs) {
        VF::subgraphIsoCountAll(&graph, &p3);
    }
    cout << "VF:" << timer.elapsed() << endl;


    timer.reset();
    for(BoostGraph graph : bgraphs) {
        graph.subgraphIsoCountAll(&bp3);
    }
    cout << "BO:" << timer.elapsed() << endl;


    cout << endl << "==== has a p3 === " << endl;
    timer.reset();

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

    timer.reset();
    for(BoostGraph graph : bgraphs) {
        for(int i  = 0; i < 100; i++)
            graph.subgraphIsoHasOne(bp3s);
    }
    cout << "BO:" << timer.elapsed() << endl;

    //test p3*/


}
 
