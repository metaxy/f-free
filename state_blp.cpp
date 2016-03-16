#include "state_blp.h"
#include "vf.h"
#include "randomize.h"
#include "gurobilp.h"
#include "forbidden.h"
StateBlp::StateBlp(Config conf) : State(conf)
{
}
MGraph StateBlp::solve()
{
    MGraph input(m_input);
    GurobiLP g(input.nodeCount());
    Model model = input.createModel();
    g.addModelVars(model);
    g.setObjective(model);
    for(const MGraph &needle : m_forbidden) {
        vector<NodeMapping> mappings = VF::subgraphIsoAll(&input, &needle);
        int step = 0;
        while(!mappings.empty()) {
            for(NodeMapping mapping : mappings) {
                g.addConstraint(&input, &needle, &mapping);
            }

            Model ret = g.optimize();
            for(const auto &i: ret) {
                if(i.second > 0) {
                    input.setConnected(i.first, true);
                } else {
                    input.setConnected(i.first, false);
                }
            }
            step++;
            mappings = VF::subgraphIsoAll(&input, &needle);
        }
    }
    return input;
}

