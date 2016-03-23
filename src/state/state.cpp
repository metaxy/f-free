#include "state.h"
#include "src/iso/vf.h"
#include <sstream>

State::State(Config conf) : m_config(conf)
{
    r = new Randomize(getInt("seed", 5489));
    m_begin_time = clock();
    m_hasTime = float(getInt("time", 10));//many time per default
}
double State::getDouble(const string &name, double def) const
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        std::istringstream i(iter->second);
        double x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}
int State::getInt(const string &name, int def) const
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        std::istringstream i(iter->second);
        int x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}
string State::getString(const string &name, string def) const
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        return iter->second;
    }
    return def;
}
string State::debug() const
{
    return Common::json(m_debug);
}
void State::final()
{

}
float State::timeLeft() const
{
    float time = float( clock () - m_begin_time ) /  CLOCKS_PER_SEC;
    clog << "time left:" << m_hasTime - time << endl;
    return m_hasTime - time;
}
float State::timePerIteration() const
{
    return m_hasTime / float(getInt("rounds", 1));
}
