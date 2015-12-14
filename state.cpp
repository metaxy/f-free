#include "state.h"
#include "vf.h"
#include <sstream>
State::State(Config conf) : m_config(conf)
{
    m_input = this->getInput();
    m_forbidden = this->getForbidden();
    r = new Randomize(getInt("seed", 5489));
}
MGraph State::getInput()
{
    string fileName = m_config["input"];
    if(fileName.empty()) {
        fileName = "../model/newman_watts_strogatz/n_70_m_2_p_0.2.txt";
    }
    return MGraph(Common::graphFromFile(fileName));
}
vector<MGraph> State::getForbidden()
{
    string folder = m_config["forbidden"];
    if(folder.empty()) {
        folder = "../forbidden/triangle";
    }
    vector<string> files = Common::listFiles(folder);
    vector<MGraph> ret;
    for(string file : files) {
        if(file == "." || file == "..") continue;
        ret.push_back(MGraph(Common::graphFromFile(folder + "/"+ file)));
    }
    return ret;
}

MGraph State::solveMultiple(int count)
{
    vector<Edge> bestEdges;
    MGraph bestSolved;
    int bestSize = -1;
    for(int i = 0; i< count; ) {
        MGraph solved = this->solve();
        if(!testSolved(&solved)) continue;
        vector<Edge> edges = m_input.difference(&solved);
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

    bestSolved.printEdges(bestEdges);

    if(!testSolved(&bestSolved)) {
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

bool State::testSolved(MGraph *output)
{
    for(MGraph needle : m_forbidden) {
        if(!VF::subgraphIsoOne(output, &needle).empty()) {
            return false;
        }
    }
    return true;
}

string State::debug() const
{
    return Common::json(m_debug);
}
void State::final()
{

}
