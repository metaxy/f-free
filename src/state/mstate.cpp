#include "mstate.h"
#include "src/iso/vf.h"
#include <sstream>

MState::MState(Config conf) : State(conf)
{
    m_input = this->getInput();
    m_forbidden = this->getForbidden();
}

MGraph MState::getInput()
{
    string fileName = m_config["input"];
    if(fileName.empty()) {
        fileName = "../model/albert_barabasi/n_10_m_3.txt";
    }
    return MGraph(Common::graphFromFile(fileName));
}
vector<MGraph> MState::getForbidden()
{
    string folder = m_config["forbidden"];
    if(folder.empty()) {
        folder = "../forbidden/p5_c5";
    }
    vector<string> files = Common::listFiles(folder);
    vector<MGraph> ret;
    for(string file : files) {
        if(file == "." || file == "..") continue;
        ret.push_back(MGraph(Common::graphFromFile(folder + "/"+ file)));
    }
    return ret;
}
bool MState::isValid(const MGraph *input)
{
    return !VF::subgraphIsoHasOne(input, m_forbidden);
}
VGraph* MState::solveMultiple(int count)
{
    vector<Edge> bestEdges;
    MGraph bestSolved;
    int bestSize = -1;
    for(int i = 0; i< count; ) {
        MGraph solved = this->solve();
        vector<Edge> edges = this->difference(&solved);
        if(edges.size() < bestSize) {
            bestSolved = solved;
            bestEdges = edges;
            bestSize = edges.size();
        }
        if(timeLeft() < timePerIteration() || timeLeft() < 1) break;
        i++;
    }
    this->final();
    cout << "#k: " << bestSize << endl;
    cout << "#debug: " << debug() << endl;

    bestSolved.printEdges(bestEdges);
    cout << "#k_repeat: " << bestSize << endl;
    return new MGraph(bestSolved);
}
vector<Edge> MState::difference(MGraph *solved) const
{
    return m_input.difference(solved);
}
