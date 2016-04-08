#include "bstate.h"
#include <sstream>

BState::BState(Config conf) : State(conf)
{
    m_input = this->getInput();
    m_forbidden = this->getForbidden();
}

BoostGraph BState::getInput()
{
    string fileName = m_config["input"];
    if(fileName.empty()) {
        fileName = "../model/bio1/n_0044_nr_0330.txt";
    }
    return BoostGraph(Common::graphFromFile(fileName));
}
vector<BoostGraph*> BState::getForbidden()
{
    string folder = m_config["forbidden"];
    if(folder.empty()) {
        folder = "../forbidden/splitcluster";
    }
    vector<string> files = Common::listFiles(folder);
    vector<BoostGraph*> ret;
    for(string file : files) {
        if(file == "." || file == "..") continue;
        ret.push_back(new BoostGraph(Common::graphFromFile(folder + "/"+ file)));
    }
    return ret;
}
bool BState::isValid(const BoostGraph *input)
{
    return !input->subgraphIsoHasOne(m_forbidden);
}
VGraph* BState::solveMultiple(int count)
{
    vector<Edge> bestEdges;
    BoostGraph bestSolved;
    int bestSize = -1;
    for(int i = 0; i< count; ) {
        BoostGraph solved = this->solve();
        vector<Edge> edges = this->difference(&solved);
        if(edges.size() < bestSize) {
            bestSolved = solved;
            bestEdges = edges;
            bestSize = edges.size();
        }
        i++;
    }
    this->final();
    cout << "#k: " << bestSize << endl;
    cout << "#debug: " << debug() << endl;
    assert(bestSolved.subgraphIsoHasOne(m_forbidden) == false);
    bestSolved.printEdges(bestEdges);
    cout << "#k_repeat: " << bestSize << endl;
    return new BoostGraph(bestSolved);
}
vector<Edge> BState::difference(BoostGraph *solved) const
{
    return m_input.difference(solved);
}
