#include "state.h"

State::State(Config conf) : m_config(conf)
{
    m_input = this->getInput();
    m_forbidden = this->getForbidden();
    m_input.writeGraph("input");
}
MGraph State::getInput()
{
    string fileName = m_config["input"];
    if(fileName.empty()) {
        fileName = "../model/k_005_n_026.txt";
    }
    return MGraph(Common::graphFromFile(fileName));
}
vector<MGraph> State::getForbidden()
{
    string folder = m_config["forbidden"];
    if(folder.empty()) {
        folder = "../forbidden/cluster";
    }
    vector<string> files = Common::listFiles(folder);
    vector<MGraph> ret;
    for(string file : files) {
        if(file == "." || file == "..") continue;
        ret.push_back(MGraph(Common::graphFromFile(folder + "/"+ file)));
    }
    return ret;
}


MGraph State::solve(MGraph input, vector<MGraph> forbidden)
{
    for(MGraph needle : forbidden) {
        input = this->solveSingle(input, needle);
    }
    return input;
}

MGraph State::solve()
{
    MGraph input = m_input;
    for(MGraph needle : m_forbidden) {
        input = this->solveSingle(input, needle);
    }
    if(input.findP3() != P3(0,0,0)) {
        clog << "NOT SOLVED !!! " << endl;
    } else {
        clog << "rightly solved" << endl;
    }
    return input;
}


MGraph State::solveMultiple(int count)
{
    vector<Edge> bestEdges;
    MGraph bestSolved;
    int bestSize = -1;
    for(int i = 0; i< count; i++) {
        MGraph solved = this->solve();
        solved.restoreMerges();
        solved.normalize();
        vector<Edge> edges = m_input.difference(&solved);
        if(edges.size() < bestSize) {
            bestSolved = solved;
            bestEdges = edges;
            bestSize = edges.size();
        }
    }
    cout << "#k: " << bestSize << endl;
    bestSolved.printEdges(bestEdges);
    bestSolved.writeGraph("solved");
    return bestSolved;
}
