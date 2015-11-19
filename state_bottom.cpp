#include "state_bottom.h"
#include "vf.h"
#include "randomize.h"
#include <math.h>
StateBottom::StateBottom(Config conf) : State(conf)
{
}
MGraph StateBottom::solve()
{
    MGraph input(m_input);
    input.clear();
    while(true) {
        vector<Edge> diff = r->randomVector(m_input.difference(&input));
        bool oneChange = false;
        for(const Edge &e: diff) {
            input.flip(e);
            if(!isValid(&input)) {
                input.flip(e);
            } else {
                oneChange = true;
            }
        }
        if(!oneChange) {
            break;
        }

    }
    return input;
}

bool StateBottom::isValid(MGraph *input)
{
    return !VF::subgraphIsoHasOne(input, m_forbidden);
}
