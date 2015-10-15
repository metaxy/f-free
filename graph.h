#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <map>
#include <vector>
using namespace std;
typedef int NodeT;
typedef pair<NodeT,NodeT> Edge;
typedef vector<Edge> Subgraph;
typedef tuple<NodeT,NodeT,NodeT> P3;

typedef map<Edge,int> Model;
typedef map<Edge,float> ModelRelaxed;

class GGraph
{
public:

    GGraph();
    void insertEdge(string Node1, string Node2);
    int nodePosition(string Name) const;
    string nodeName(int value) const;
    int edgesCount() const;
    int nodeCount() const;
    vector<Edge> edges() const;
private:
    int m_nodeCount;
    map<string, int> m_nodeNames;
    vector<Edge> m_edges;
};

#endif // GRAPH_H
