#ifndef STATE_H
#define STATE_H
#include "mgraph.h"
#include <vector>
#include <time.h>
#include "randomize.h"
class State
{
public:
    State(Config conf);
    virtual MGraph solve() = 0;

    virtual void final();
    MGraph solveMultiple(int count);

    bool testSolved(MGraph *output) const;

    //virtual MGraph solveSingle(MGraph forbidden) = 0;
protected:
    Config m_config;
    MGraph m_input;
    vector<MGraph> m_forbidden;
    Randomize *r;

    map<string,string> m_debug;
    double getDouble(const string &name, double def) const;
    int getInt(const string &name, int def) const;
    string getString(const string &name, string def) const;
    bool isValid(const MGraph *input);

    float timeLeft() const;
    float timePerIteration() const;

private:
    MGraph getInput();
    vector<MGraph> getForbidden();

    string debug() const;
    clock_t begin_time;

};

#endif // STATE_H
