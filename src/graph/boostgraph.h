#ifndef BOOSTGRAPH_H
#define BOOSTGRAPH_H
#include "src/graph/vgraph.h"
#include "src/common/common.h"

#include "src/boost_1_60_0/boost/graph/adjacency_list.hpp"
#include "src/boost_1_60_0/boost/graph/vf2_sub_graph_iso.hpp"

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> boost_graph_type;
class BoostGraph : public VGraph
{
public:
    BoostGraph();
    BoostGraph(const BoostGraph &graph);
    BoostGraph(const BoostGraph *graph);
    BoostGraph(VGraph *graph);
    BoostGraph(GGraph input);

    ~BoostGraph();
    bool connected(const Edge &e) const;
    void setConnected(const Edge &e, bool connect);
    void addEdge(const Edge &e);
    void flip(const Edge &e);

    void clear();

    vector<NodeT> nodes() const ;
    int nodeCount() const;

    vector<Edge> allEdges() const;
    vector<Edge> connectedEdges() const;

    set<NodeT> neighborhood(NodeT node) const;

    //subgraph ismorphism

    NodeMapping subgraphIsoOne(BoostGraph *needle) const;
    bool subgraphIsoHasOne(vector<BoostGraph*> needle) const;
    vector<NodeMapping> subgraphIsoAll(const BoostGraph *needle) const;
    int subgraphIsoCountAll(const BoostGraph *needle) const;

protected:
    boost_graph_type m_graph;
};

#endif // BOOSTGRAPH_H
