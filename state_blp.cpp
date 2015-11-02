#include "state_blp.h"
#include "vf.h"
#include "randomize.h"
#include "gurobilp.h"
#include "forbidden.h"
StateBlp::StateBlp(Config conf) : State(conf)
{
}

MGraph StateBlp::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r(getInt("seed", 5489));
    GurobiLP g(input.nodeCount());
    Model model = input.createModel();
    g.addModelVars(model);
    g.setObjective(model);

    vector<NodeMapping> mappings = VF::subgraphIsoAll(&input, &forbidden);
    int step = 0;
    while(!mappings.empty()) {
        clog << "[" << step << "] found isomorphisms " << mappings.size() << endl;
        for(NodeMapping mapping : mappings) {
            g.addConstraint(&input, &forbidden, &mapping);
        }
       
        Model ret = g.optimize();
        for(const auto &i: ret) {
            if(i.second > 0) {
                input.setWeight(i.first, 1);
            } else {
                input.setWeight(i.first, -1);
            }
        }
        //copy.printEdges(copy.difference(&input));
        step++;
        mappings = VF::subgraphIsoAll(&input, &forbidden);
    }
    return input;
}
