#ifndef STATE_SIMPLE_RANDOM_H
#define STATE_SIMPLE_RANDOM_H
#include "src/state/bstate.h"
class StateSimpleRandom : public BState
{
public:
    StateSimpleRandom(Config conf);
    BoostGraph solve();
    void final();
protected:
    int m_countSteps;
    BoostGraph* randomElement(vector<BoostGraph*> list);
};

#endif // STATE_RANDOM_H
