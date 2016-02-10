#include "graph.h"
#include "common.h"
#include "mergegraph.h"
#include "vf.h"

#include "state_approx2k.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateApprox2K state;
    GGraph i = Common::graphFromFile(conf["input"]);
    MergeGraph input(i);
    MergeGraph solved = state.solve(input);
    for(const Edge &e: input.difference(&solved)) {
        cout << i.nodeName(e.first) << " " << i.nodeName(e.second) << endl;
    }
    return 0;
}
 
