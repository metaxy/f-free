#ifndef STATE_GROW_REDUCE_H
#define STATE_GROW_REDUCE_H
#include "state.h"
class StateGrowReduce : public State
{
public:
    StateGrowReduce(Config conf);
    MGraph solve();
    void final();

    void grow(MGraph *graph, NodeT node);
    void reduce(MGraph *graph);
protected:
    bool isValid(MGraph *input);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;
};

#endif // STATE_GROW_REDUCE_H
