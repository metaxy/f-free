#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"

#include "state_bottom_random.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed", "useWeight"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateBottomRandom state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
