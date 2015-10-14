#ifndef STATE_RANDOM_H
#define STATE_RANDOM_H
#include "state.h"
class StateRandom : public State
{
public:
    StateRandom(Config conf);

    MGraph solveSingle(MGraph input, MGraph forbidden);
};

#endif // STATE_RANDOM_H
