#ifndef STATE_BLP_H
#define STATE_BLP_H
#include "state.h"
class StateBlp : public State
{
public:
    StateBlp(Config conf);
    MGraph solve();
    MGraph solveSingle(MGraph input, MGraph forbidden);
};

#endif // STATE_RANDOM_H
