#ifndef STATE_RANDOM2_H
#define STATE_RANDOM2_H
#include "src/state/mstate.h"
class StateRandom2 : public MState
{
public:
    StateRandom2(Config conf);
    MGraph solve();
    MGraph solveSingle(MGraph input, MGraph forbidden);
    void final();
protected:
    int m_countSteps;
};

#endif // STATE_RANDOM_H
