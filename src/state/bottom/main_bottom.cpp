#include "state_bottom.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed", "maxPercent", "time"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateBottom state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
