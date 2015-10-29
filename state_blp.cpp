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
    vector<MGraph> sols = Forbidden::posibleSolutions(forbidden);
    /*int i = 0;
    for(MGraph a : sols) {
        i++;
        a.writeGraph("forbidden" + std::to_string(i));
    }*/
    GurobiLP g(input.nodeCount());
    Model model = input.createModel();
    g.addModelVars(model);
    g.setObjective(model);


    vector<NodeMapping> mappings = VF::subgraphIsoAll(&input, &forbidden);
    while(!mappings.empty()) {
        for(NodeMapping mapping : mappings) {
            for(MGraph potentialSol : sols) {
                g.addConstraint(&potentialSol, &mapping);
            }
        }
       
        Model ret = g.optimize();
        for(const auto &i: ret) {
            if(i.second > 0) {
                input.setWeight(i.first, 1);
            } else {
                input.setWeight(i.first, -1);
            }
        }
        mappings = VF::subgraphIsoAll(&input, &forbidden);
    }
    return input;
}
