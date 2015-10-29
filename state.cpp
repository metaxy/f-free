#include "state.h"
#include "vf.h"
#include <sstream>
State::State(Config conf) : m_config(conf)
{
    m_input = this->getInput();
    m_forbidden = this->getForbidden();
}
MGraph State::getInput()
{
    string fileName = m_config["input"];
    if(fileName.empty()) {
        fileName = "../model/cluster/k_034_n_010.txt";
    }
    return MGraph(Common::graphFromFile(fileName));
}
vector<MGraph> State::getForbidden()
{
    string folder = m_config["forbidden"];
    if(folder.empty()) {
        folder = "../forbidden/claw";
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
        input.restoreMerges();
        input.normalize();
    }
    return input;
}

MGraph State::solve()
{
    MGraph input = m_input;
    for(MGraph needle : m_forbidden) {
        input = this->solveSingle(input, needle);
        input.restoreMerges();
        input.normalize();
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
        if(!testSolved(solved)) continue;
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
    //bestSolved.writeGraph("solved");

    if(!testSolved(bestSolved)) {
        clog << "NOT SOLVED" << endl;
    }
    return bestSolved;
}

double State::getDouble(const string &name, double def)
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        std::istringstream i(iter->second);
        double x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}
int State::getInt(const string &name, int def)
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        std::istringstream i(iter->second);
        int x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}

bool State::testSolved(MGraph output)
{
    for(MGraph needle : m_forbidden) {
        if(!VF::subgraphIsoOne(&output, &needle).empty()) {
            return false;
        }
    }
    return true;
}
