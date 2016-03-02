#include "state.h"
#include "vf.h"
#include <sstream>

State::State(Config conf) : m_config(conf)
{
    m_input = this->getInput();
    m_forbidden = this->getForbidden();
    r = new Randomize(getInt("seed", 5489));
    m_begin_time = clock();
    m_hasTime = float(getInt("time", 10));
}

MGraph State::getInput()
{
    string fileName = m_config["input"];
    if(fileName.empty()) {
        fileName = "../model/bio1/n_0094_nr_0676.txt";
    }
    return MGraph(Common::graphFromFile(fileName));
}
vector<MGraph> State::getForbidden()
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
        if(timeLeft() < timePerIteration() || timeLeft() < 1) break;
        i++;
    }
    this->final();
    cout << "#k: " << bestSize << endl;
    cout << "#debug: " << debug() << endl;

    bestSolved.printEdges(bestEdges);
    cout << "#k_repeat: " << bestSize << endl;
    if(!testSolved(&bestSolved)) {
        clog << "NOT SOLVED" << endl;
    }
    return bestSolved;
}

double State::getDouble(const string &name, double def) const
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
int State::getInt(const string &name, int def) const
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
string State::getString(const string &name, string def) const
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        return iter->second;
    }
    return def;
}
bool State::testSolved(MGraph *output) const
{
    for(MGraph needle : m_forbidden) {
        if(!VF::subgraphIsoOne(output, &needle).empty()) {
            clog << "not solved" << endl;
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
bool State::isValid(const MGraph *input)
{
    return !VF::subgraphIsoHasOne(input, m_forbidden);
}
float State::timeLeft() const
{
    float time = float( clock () - m_begin_time ) /  CLOCKS_PER_SEC;
    clog << "time left:" << m_hasTime - time << endl;
    return m_hasTime - time;
}
float State::timePerIteration() const
{
    return m_hasTime / float(getInt("rounds", 1));
}
