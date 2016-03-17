#include "state_random.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed", "time", "markVisited", "convergenceMaxValue", "visitedProb"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateRandom state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
