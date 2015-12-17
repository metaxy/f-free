#ifndef STATE_EXTEND_H
#define STATE_EXTEND_H
#include "state.h"
class StateExtend : public State
{
public:
    StateExtend(Config conf);
    MGraph solve();
    void final();

protected:
    void extend(MGraph *graph);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;
};

#endif // STATE_GROW_REDUCE_H
