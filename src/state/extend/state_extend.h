#ifndef STATE_EXTEND_H
#define STATE_EXTEND_H
#include "src/state/mstate.h"
class StateExtend : public MState
{
public:
    StateExtend(Config conf);
    MGraph solve();
    void final();

protected:
    bool extend(MGraph *graph);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
};

#endif // STATE_GROW_REDUCE_H
