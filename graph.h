#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
typedef int NodeT;
typedef pair<NodeT,NodeT> Edge;
typedef vector<Edge> Subgraph;

typedef tuple<NodeT,NodeT,NodeT> P3;
typedef map<Edge,int> Model;
typedef map<Edge,float> ModelRelaxed;

class GNode
{
public:
    string name;
    GNode *next;
    GNode(string n, GNode *node) {name = n; next = node;}
};

class GEdge
{
public:
    NodeT u;
    NodeT v;
    GEdge *next;
    GEdge(NodeT i, NodeT j, GEdge *e) {u = i; v = j; next = e;}
    Edge toEdge() {
        return Edge(u,v);
    }
};

class GGraph
{
public:
    int nodeCount;
    GEdge *firstEdge;
    GNode *firstNode;

    GGraph();
    void reset();
	void intNodes(int n);
    int insertEdge(NodeT Node1, NodeT Node2);
    int insertEdge(string Node1, string Node2);
    int nodePosition(string Name);
    string nodeName(int value);
    int edgesCount();
    void output();
};

#endif // GRAPH_H
