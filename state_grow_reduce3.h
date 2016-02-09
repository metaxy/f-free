#ifndef STATE_GROW_REDUCE3_H
#define STATE_GROW_REDUCE3_H
#include "state.h"
class StateGrowReduce3 : public State
{
public:
    StateGrowReduce3(Config conf);
    MGraph solve();
    void final();

    void grow(MGraph *graph, NodeT node);
    void reduce(MGraph *graph);
    void reduceSimple(MGraph *graph);
protected:
    void extend(MGraph *graph);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;
};

#endif // STATE_GROW_REDUCE3_H
