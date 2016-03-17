#include "state_grow_reduce_boost.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed", "reduce", "time", "factorSize"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateGrowReduceBoost state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
