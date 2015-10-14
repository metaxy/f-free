#include "randomize.h"
#include <stdlib.h>
#include <random>
#include <iterator>
using namespace std;
Randomize::Randomize() : m_rgen(m_rseed())
{
}
bool Randomize::choice(float probability)
{
    std::uniform_int_distribution<int> idist(0,10000);
    return idist(m_rgen) < probability*10000;
}


NodeT Randomize::randomElement(vector<NodeT> list)
{
    std::uniform_int_distribution<> dis(0, list.size() - 1);
    return list[dis(m_rgen)];
}

NodeT Randomize::randomElement(set<NodeT> list)
{
    std::uniform_int_distribution<> dis(0, list.size() - 1);
    auto it(list.begin());
    advance(it, dis(m_rgen));
    return *it;
}
