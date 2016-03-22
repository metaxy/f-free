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
    NodeT randomElement(set<NodeT> a);
    vector<NodeT> randomVector(vector<NodeT> a);
    vector<Edge> randomVector(vector<Edge> a);
    std::random_device m_rseed;
    std::mt19937 m_rgen;

    int m_seed;
private:

};

#endif // RANDOMIZE_H
