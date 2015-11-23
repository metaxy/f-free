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
        weighted = Forbidden::forbiddenWeight(&m_input, m_forbidden);

        list<Edge> diff = this->sortedVector(m_input.difference(&input), &weighted);
        bool oneChange = false;
        for(const Edge &e: diff) {
            input.flip(e);
            if(!isValid(&input)) {
                input.flip(e);
                m_invalidChanges++;
            } else {
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
        edges.insert(std::make_pair(weightedGraph->getWeight(e), e));
    }

    list<Edge> ret;
    for(const auto &e : edges) {
        ret.push_back(e.second);
    }
    return ret;
}

bool StateBottom::isValid(MGraph *input)
{
    return !VF::subgraphIsoHasOne(input, m_forbidden);
}
void StateBottom::final()
{
    m_debug["countIteration"] = std::to_string(m_countIteration);
    m_debug["validChanges"] = std::to_string(m_validChanges);
    m_debug["invalidChanges"] = std::to_string(m_invalidChanges);
    m_debug["skipBecauseOfWeight"] = std::to_string(m_skipBecauseOfWeight);
}
