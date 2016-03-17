#include "state_random2.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed", "time"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateRandom2 state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
