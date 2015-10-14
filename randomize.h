#ifndef RANDOMIZE_H
#define RANDOMIZE_H
#include <random>
#include <iterator>
#include <set>
#include <vector>
#include "common.h"
using namespace std;
class Randomize
{
public:
    Randomize();

    bool choice(float probability);
    NodeT randomElement(vector<NodeT> a);
    NodeT randomElement(set<NodeT> a);
private:
    std::random_device m_rseed;
    std::mt19937 m_rgen;
};

#endif // RANDOMIZE_H
