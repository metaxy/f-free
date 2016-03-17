#ifndef BSTATE_H
#define BSTATE_H
#include "state.h"
#include "src/graph/boostgraph.h"
class BState : public State
{
public:
    BState(Config conf);
    virtual BoostGraph solve() = 0;

    VGraph* solveMultiple(int count);
    vector<Edge> difference(BoostGraph *solved) const;
protected:

    BoostGraph m_input;

    vector<BoostGraph*> m_forbidden;
    bool isValid(const BoostGraph *input);
    BoostGraph getInput();
    vector<BoostGraph*> getForbidden();

};

#endif // BSTATE_H
