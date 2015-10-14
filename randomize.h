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
    Randomize(unsigned int seed);
    bool choice(float probability);
    NodeT randomElement(vector<NodeT> a);
    Edge randomElement(vector<Edge> a);
    Subgraph randomElement(vector<Subgraph> a);
    NodeT randomElement(set<NodeT> a);
private:
    std::random_device m_rseed;
    std::mt19937 m_rgen;
};

#endif // RANDOMIZE_H
