#ifndef MGRAPH_H
#define MGRAPH_H
#include "src/common/common.h"
#include "src/graph/vgraph.h"
#include <vector>
#include <set>
#define M_CONNECTED (1)
#define M_NOT_CONNECTED (0)

class MGraph: public VGraph
{
public:
    MGraph();
    MGraph(GGraph input);
    MGraph(MGraph *copy);
    MGraph(const MGraph &copy);
    MGraph(VGraph *other);
    ~MGraph();
    bool connected(NodeT x, NodeT y) const;
    bool connected(const Edge &e) const;
    void setConnected(const Edge &e, bool connected);


    void addEdge(const Edge &e);
    void flip(const Edge &e);

    int setWeight(Edge e, int weight);
    int getWeight(Edge e) const;

    void clear();

    vector<NodeT> nodes() const;
    vector<Edge> allEdges() const;
    vector<Edge> connectedEdges() const;

    int nodeCount() const;

    set<NodeT> neighborhood(NodeT node) const;

private:
    MGraph(int nodeCount, NodeT defValue = M_NOT_CONNECTED);
    int getWeight(NodeT x, NodeT y) const;

    int m_nodeCount;
    NodeT **m_matrix = nullptr;

};

#endif // MGRAPH_H
