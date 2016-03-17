#ifndef STATE_EXTEND_H
#define STATE_EXTEND_H
#include "src/state/bstate.h"
class StateExtend : public BState
{
public:
    StateExtend(Config conf);
    BoostGraph solve();
    void final();

protected:
    bool extend(BoostGraph *graph);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
};

#endif // STATE_GROW_REDUCE_H
