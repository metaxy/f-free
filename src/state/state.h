#ifndef STATE_H
#define STATE_H
#include "src/graph/mgraph.h"
#include <vector>
#include <time.h>
#include "src/common/randomize.h"
class State
{
public:
    State(Config conf);

    virtual void final();

protected:
    Config m_config;
    Randomize *r;


    map<string,string> m_debug;
    double getDouble(const string &name, double def) const;
    int getInt(const string &name, int def) const;
    string getString(const string &name, string def) const;

    float timeLeft() const;
    float timePerIteration() const;

    string debug() const;
    clock_t m_begin_time;

    float m_hasTime;

};

#endif // STATE_H
