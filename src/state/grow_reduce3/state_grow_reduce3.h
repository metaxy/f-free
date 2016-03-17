#ifndef STATE_GROW_REDUCE3_H
#define STATE_GROW_REDUCE3_H
#include "src/state/mstate.h"
class StateGrowReduce3 : public MState
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

    int m_factorSize;
};

#endif // STATE_GROW_REDUCE3_H
