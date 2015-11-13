#ifndef STATE_RANDOM_H
#define STATE_RANDOM_H
#include "state.h"
class StateRandom : public State
{
public:
    StateRandom(Config conf);

    MGraph solveSingle(MGraph input, MGraph forbidden);
    void final();
protected:
    int m_countSteps;
};

#endif // STATE_RANDOM_H
