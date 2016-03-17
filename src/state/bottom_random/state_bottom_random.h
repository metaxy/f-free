#ifndef STATE_BOTTOM_random_H
#define STATE_BOTTOM_random_H
#include "src/state/mstate.h"
class StateBottomRandom : public MState
{
public:
    StateBottomRandom(Config conf);
    MGraph solve();
    void final();
protected:
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;
};

#endif // STATE_RANDOM_H
