#ifndef STATE_BOTTOM_H
#define STATE_BOTTOM_H
#include "src/state/mstate.h"
#include <list>
class StateBottom : public MState
{
public:
    StateBottom(Config conf);
    MGraph solve();
    void final();

protected:
    list<Edge> sortedVector(const vector<Edge> &input, MGraph *weightedGraph);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;
};

#endif // STATE_RANDOM_H
