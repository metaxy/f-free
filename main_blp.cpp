#include "state_blp.h"
int main(int argc, char* argv[])
{
    Config conf = Common::parseConfig(argc, argv);
    StateBlp state(conf);
    MGraph result = state.solve();
    result.printEdges(state.difference(&result));
    return 0;
}
 
