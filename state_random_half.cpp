#include "state_random_half.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
StateRandomHalf::StateRandomHalf(Config conf) : State(conf)
{
}

MGraph StateRandomHalf::solveSingle(MGraph input, MGraph forbidden)
{
    Randomize r(getInt("seed", 5489));
    /*
    vector<NodeMapping> mappings = VF::subgraphIso(&input, &forbidden, 100);
    while(!mappings.empty()) {
        for(NodeMapping mapping : mappings) {
            for(const Edge &edge : forbidden.edges()) {
                Edge e = Common::transformEdge(edge, &mapping);
                //int weight = input.reduceWeight(e, 2);
            }
        }
        //mappings = VF::subgraphIso(&input, &forbidden, batch);
    }*/

    vector<MGraph> sols = Forbidden::posibleSolutions(forbidden);
    clog << "forbidden sols size " << sols.size() << endl;
    int i = 0;
    for(MGraph a : sols) {
        i++;
        a.writeGraph("forbidden" + std::to_string(i));
    }

    vector<NodeMapping> mappings = VF::subgraphIsoAll(&input, &forbidden);
    int step = 0;
    while(!mappings.empty()) {
        clog << "[" << step << "] found isomorphisms " << mappings.size() << endl;
        for(NodeMapping mapping : mappings) {
            for(auto i: mapping) {
                clog << i.first << "->" << i.second << " ";
            }
            clog << endl;
            for(MGraph potentialSol : sols) {
                for(Edge edge: potentialSol.edges()) {
                    Edge trans = Common::transformEdge(edge, &mapping);
                    if(potentialSol.connected(edge)) {
                        clog << "+ " << input.printEdgeLatex(trans);
                    } else {
                        clog << "- " << input.printEdgeLatex(trans);
                    }
                }
                clog << " =< 1" << endl;
            }
        }
       exit(-1);
    }
    return input;
}
