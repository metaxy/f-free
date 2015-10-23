#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"

#include "state_blp.h"
int main(int argc, char* argv[])
{
    Config conf = Common::parseConfig(argc, argv);
    StateBlp state(conf);
    MGraph res = state.solveMultiple(Common::getInt(&conf, "rounds", 1));
    if(!conf["print_output"].empty()) {
        res.writeGraph("output");
    }
    return 0;
}
 
