#ifndef STATE_BOTTOM_H
#define STATE_BOTTOM_H
#include "state.h"
class StateBottom : public State
{
public:
    StateBottom(Config conf);
    MGraph solve();
    void final();
protected:
    bool isValid(MGraph *input);
    int m_countIteration;
    int m_validChanges;
    int m_invalidChanges;
    int m_skipBecauseOfWeight;
};

#endif // STATE_RANDOM_H
