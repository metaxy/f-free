#ifndef STATE_RANDOM_HALF_H
#define STATE_RANDOM_HALF_H
#include "state.h"
class StateRandomHalf : public State
{
public:
    StateRandomHalf(Config conf);

    MGraph solveSingle(MGraph input, MGraph forbidden);
};

#endif // STATE_RANDOM_H
