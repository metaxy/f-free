#ifndef STATE_GROW_REDUCE2_H
#define STATE_GROW_REDUCE2_H
#include "src/state/mstate.h"
class StateGrowReduce2 : public MState
{
public:
    StateGrowReduce2(Config conf);
    MGraph solve();
    void final();

    void grow(MGraph *graph, NodeT node);
    int growIsomorph(MGraph *graph);
    void reduce(MGraph *graph);
protected:
    void extend(MGraph *graph);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;
};

#endif // STATE_GROW_REDUCE_H
