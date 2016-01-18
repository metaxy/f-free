#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"

#include "state_grow_reduce3.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateGrowReduce3 state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
