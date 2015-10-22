#include "state_blp.h"
#include "vf.h"
#include "randomize.h"
#include "gurobilp.h"
StateBlp::StateBlp(Config conf) : State(conf)
{
}

MGraph StateBlp::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r(getInt("seed", 5489));

     GurobiLP g(graph.nodeCount());
     Model model = input.createModel();
     g.addModelVars(model);
     g.setObjective(model);

    vector<NodeMapping> mappings = VF::subgraphIsoAll(&input, &forbidden);
    while(!mappings.empty()) {
        for(NodeMapping mapping : mappings) {
            for(const Edge &edge : forbidden.edges()) {
                Edge e = Common::transformEdge(edge, &mapping);
                //int weight = input.reduceWeight(e, 2);
            }
        }
       
        Model ret = g.optimize();
        for(const auto &i: ret) {
            input.setWeight(i.first, i.second);
        }
        mappings = VF::subgraphIsoAll(&input, &forbidden);
    }
    return input;
}
