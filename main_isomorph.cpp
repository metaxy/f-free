#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"

int main(int argc, char* argv[])
{
    vector<string> options = {"big", "small"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    MGraph big(Common::graphFromFile(conf["big"]));
    vector<MGraph> smallGraphs;

    if(Common::isDir(conf["small"])) {
        vector<string> files = Common::listFiles(conf["small"]);
        for(string file : files) {
            if(file == "." || file == "..") continue;
            smallGraphs.push_back(MGraph(Common::graphFromFile(conf["small"] + "/"+ file)));
        }

    } else {
        smallGraphs.push_back(Common::graphFromFile(conf["small"]));
    }

    if(VF::subgraphIsoHasOne(&big, smallGraphs)) {
        cout << "Isomorph" << endl;
        return 0;
    } else {
        cout << "Not Isomorph" << endl;
        return 1;
    }
}
 
