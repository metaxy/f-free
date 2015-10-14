#include "graph.h"
#include "common.h"
#include "mgraph.h"
#include "vf.h"

#include "state_random.h"
int main(int argc, char* argv[])
{
    Config conf = Common::parseConfig(argc, argv);
    StateRandom state(conf);
    MGraph ret = state.solveMultiple(1);

    //ret.writeGraph("result");

    return 0;
}
 
