#ifndef STATE_BOTTOM_H
#define STATE_BOTTOM_H
#include "state.h"
class StateBottom : public State
{
public:
    StateBottom(Config conf);
    MGraph solve();
protected:
    bool isValid(MGraph *input);
};

#endif // STATE_RANDOM_H
