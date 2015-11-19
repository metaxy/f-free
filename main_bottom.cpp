#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"

#include "state_bottom.h"
int main(int argc, char* argv[])
{

    Config conf = Common::parseConfig(argc, argv);
    StateBottom state(conf);
    state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    return 0;
}
 
