#ifndef STATE_EXTEND_BOOST_D_H
#define STATE_EXTEND_BOOST_H
#include "src/state/bstate.h"
class StateExtendBoost : public BState
{
public:
    StateExtendBoost(Config conf);
    BoostGraph solve();
    void final();

protected:
    bool extend(BoostGraph *graph);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
};

#endif // STATE_GROW_REDUCE_H
