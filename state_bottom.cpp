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
    int nothing = 0;
    while(nothing < m_input.nodeCount()*m_input.nodeCount()) {
        Edge e = r->randomElement(m_input.difference(&input));
        input.flip(e);
        if(!isValid(&input)) {
            input.flip(e);
            nothing++;
        } else {
            nothing = 0;
        }

    }
    return input;
}

bool StateBottom::isValid(MGraph *input)
{
    return VF::subgraphIsoHasOne(input, m_forbidden);
}
