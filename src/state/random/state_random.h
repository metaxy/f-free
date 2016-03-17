#ifndef STATE_RANDOM_H
#define STATE_RANDOM_H
#include "src/state/mstate.h"
class StateRandom : public MState
{
public:
    StateRandom(Config conf);
    MGraph solve();
    MGraph solveSingle(MGraph input, MGraph forbidden);
    void final();
protected:
    int m_countSteps;
};

#endif // STATE_RANDOM_H
