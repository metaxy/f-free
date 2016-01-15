#ifndef STATE_BLP_H
#define STATE_BLP_H
#include "state.h"
class StateBlp : public State
{
public:
    StateBlp(Config conf);
    MGraph solve();
};

#endif // STATE_RANDOM_H
