#ifndef STATE_GROW_REDUCE_BOOST_H
#define STATE_GROW_REDUCE_BOOST_H
#include "src/state/bstate.h"
class StateGrowReduceBoost : public BState
{
public:
    StateGrowReduceBoost(Config conf);
    BoostGraph solve();
    void final();

protected:
    void extend(BoostGraph *graph);

    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;

    int m_factorSize;
};

#endif // STATE_GROW_REDUCE3_H
