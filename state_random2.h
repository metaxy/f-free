#ifndef STATE_RANDOM2_H
#define STATE_RANDOM2_H
#include "state.h"
class StateRandom2 : public State
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
