#ifndef STATE_BLP_H
#define STATE_BLP_H
#include "src/state/mstate.h"
class StateBlp : public MState
{
public:
    StateBlp(Config conf);
    MGraph solve();
};

#endif // STATE_RANDOM_H
