#ifndef STATE_RANDOM_HALF_H
#define STATE_RANDOM_HALF_H
#include "src/state/mstate.h"
class StateRandomHalf : public MState
{
public:
    StateRandomHalf(Config conf);
    MGraph solve();
    MGraph solveSingle(MGraph input, MGraph forbidden);
};

#endif // STATE_RANDOM_H
