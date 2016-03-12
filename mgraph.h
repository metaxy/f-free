#ifndef MGRAPH_H
#define MGRAPH_H
#include "common.h"
#include "vgraph.h"
#include <vector>
#include <set>
#define M_CONNECTED (1)
#define M_NOT_CONNECTED (0)

class MGraph: public VGraph
{
public:
    MGraph();
    MGraph(int nodeCount, NodeT defValue = M_NOT_CONNECTED);
    MGraph(GGraph input);
    MGraph(MGraph *copy);
    MGraph(const MGraph &copy);
    ~MGraph();
    bool connected(NodeT x, NodeT y) const;
    bool connected(const Edge &e) const;


    void addEdge(const Edge &e);
    void flip(const Edge &e);

    int setWeight(Edge e, int weight);
    int getWeight(Edge e) const;

    void clear();

    vector<NodeT> nodes() const;
    vector<Edge> edges() const;
    vector<Edge> connectedEdges() const;

    int nodeCount() const;

    set<NodeT> neighborhood(NodeT node) const;

    private:
    int m_nodeCount;

    GGraph m_input;
    int getWeight(NodeT x, NodeT y) const;

    
    NodeT **m_matrix = nullptr;

};

#endif // MGRAPH_H
