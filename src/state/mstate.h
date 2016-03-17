#ifndef MSTATE_H
#define MSTATE_H
#include "state.h"
class MState : public State
{
public:
    MState(Config conf);
    virtual MGraph solve() = 0;

    VGraph* solveMultiple(int count);
    vector<Edge> difference(MGraph *solved) const;
protected:
    MGraph m_input;
    vector<MGraph> m_forbidden;
    bool isValid(const MGraph *input);
    MGraph getInput();
    vector<MGraph> getForbidden();

};

#endif // MSTATE_H
