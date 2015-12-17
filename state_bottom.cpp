#include "state_bottom.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
#include <math.h>
StateBottom::StateBottom(Config conf) : State(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
}
MGraph StateBottom::solve()
{
    MGraph input(m_input);
    MGraph weighted;
    input.clear();
    while(true) {
        m_countIteration++;
        weighted = Forbidden::forbiddenWeight2(&m_input, m_forbidden);
        //clog << "new iteration" << m_countIteration << endl;
        //clog << "----------------------------------" << endl;
        list<Edge> diff = this->sortedVector(m_input.difference(&input), &weighted);
        bool oneChange = false;
        for(const Edge &e: diff) {
            input.flip(e);
            if(!isValid(&input)) {
                input.flip(e);
                //clog << "invalid change " << e.first << "<->" << e.second << " weight=" << weighted.getWeight(e) << endl;
                m_invalidChanges++;
            } else {
                //clog << "change " << e.first << "<->" << e.second << " with weight=" << weighted.getWeight(e) << endl;
                oneChange = true;
                m_validChanges++;
            }
        }
        if(!oneChange) {
            break;
        }
    }
    return input;
}
list<Edge> StateBottom::sortedVector(const vector<Edge> &input, MGraph *weightedGraph)
{
    multimap<int, Edge> edges;
    for(const Edge &e : input) {
        edges.insert(std::make_pair(abs(weightedGraph->getWeight(e)), e));
    }

    list<Edge> ret;
    for(const auto &e : edges) {
        ret.push_front(e.second);
    }
    return ret;
}
void StateBottom::final()
{
    m_debug["countIteration"] = std::to_string(m_countIteration);
    m_debug["validChanges"] = std::to_string(m_validChanges);
    m_debug["invalidChanges"] = std::to_string(m_invalidChanges);
    m_debug["skipBecauseOfWeight"] = std::to_string(m_skipBecauseOfWeight);
}
