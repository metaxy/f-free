#ifndef COMMON_H
#define COMMON_H

//#define _DEBUG
#include "graph.h"
#include <cstring>
#include <tuple>
#include <set>
#include <map>
#include "graph.h"
class Common
{
public:
    Common();
    Graph static graphFromStdin();
    Graph static graphFromFile(string fileName);

    static void printMatrix(int **matrix, unsigned int size);
    static void printVector(int *vector, unsigned int size);

    static map<string, string> parseConfig(int argc, char* argv[]);

};

template <typename T> int sgn(T val) {
    if(val < T(0)) return -1;
    return 1;
}

#ifdef _DEBUG
#define de(str) do { std::clog << str << std::endl; } while( false )
#else
#define de(str) do { } while ( false )
#endif

template<typename T> set<T> set_intersect(set<T> a, set<T> b)
{
    set<T> ret;
    for(const T &t : a) {
        if(b.count(t) == 1) ret.insert(t);
    }
    return ret;
}
template<typename T> set<T> set_union(set<T> a, set<T> b)
{
    set<T> ret;
    for(const T &t : a) {
        ret.insert(t);
    }
    for(const T &t : b) {
        ret.insert(t);
    }
    return ret;
}
#endif // COMMON_H
