#include "state_bottom_random.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed", "time", "useWeight"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateBottomRandom state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
