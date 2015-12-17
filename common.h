#ifndef COMMON_H
#define COMMON_H

#define _DEBUG
#include "graph.h"
#include <cstring>
#include <iostream>
#include <tuple>
#include <set>
#include <map>
#include <unordered_map>
#include "graph.h"
#include "mgraph.h"

typedef map<string, string> Config;
typedef map<NodeT, NodeT> NodeMapping;
class Common
{
public:
    Common();
    GGraph static graphFromStdin();
    GGraph static graphFromFile(string fileName);

    static void printMatrix(int **matrix, unsigned int size);
    static void printVector(int *vector, unsigned int size);
    static void printNodeMapping(NodeMapping map);

    static void printSubgraph(Subgraph *subgraph);

    static Config parseConfig(int argc, char* argv[]);
   static Config parseConfigOptions(int argc, char* argv[], vector<string> options);
    static string dotColor(float id, int size);

    static vector<string> listFiles(string path = ".");
    static  Edge transformEdge(const Edge &e, const NodeMapping *mapping);
    static double getDouble(Config *m_config, const string &name, double def);
    static int getInt(Config *m_config, const string &name, int def);

    static string json(map<string,string> map);
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
