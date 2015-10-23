#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"

int main(int argc, char* argv[])
{
    vector<string> options = {"big", "small"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    MGraph big(Common::graphFromFile(conf["big"]));
    MGraph small(Common::graphFromFile(conf["small"]));

    if(VF::subgraphIsoOne(&big, &small).empty()) {
        cout << "Not Isomorph" << endl;
    } else {
        cout << "Isomorph" << endl;
    }
    return 0;
}
 
