#include "mergegraph.h"
#include "state_approx2k.h"
int main(int argc, char* argv[])
{
    vector<string> options = {"input", "forbidden", "rounds", "seed"};
    Config conf = Common::parseConfigOptions(argc, argv, options);
    StateApprox2K state;
    string fileName = conf["input"];
    if(fileName.empty()) {
        fileName = "../model/albert_barabasi/n_120_m_3.txt";
    }
    GGraph i = Common::graphFromFile(fileName);
    MergeGraph input(i);

    MergeGraph solved = state.solve(input);
    solved.restoreMerges();
    solved.normalize();

    auto edges = input.difference(&solved);

    for(const Edge &e: edges) {
        cout << i.nodeName(e.first) << " " << i.nodeName(e.second) << endl;
    }
    cout << "#k: " << edges.size() << endl;
    return 0;
}
 
