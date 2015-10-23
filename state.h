#ifndef STATE_H
#define STATE_H
#include "mgraph.h"
#include <vector>

class State
{
public:
    State(Config conf);
    MGraph solve();
    virtual MGraph solve(MGraph input, vector<MGraph> forbidden);
    virtual MGraph solveSingle(MGraph input, MGraph forbidden) = 0;
    MGraph solveMultiple(int count);

    bool testSolved(MGraph output);

    //virtual MGraph solveSingle(MGraph forbidden) = 0;
protected:
    Config m_config;
    MGraph m_input;
    vector<MGraph> m_forbidden;
    double getDouble(const string &name, double def);
    int getInt(const string &name, int def);

private:
    MGraph getInput();
    vector<MGraph> getForbidden();
};

#endif // STATE_H
