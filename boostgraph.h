#ifndef BOOSTGRAPH_H
#define BOOSTGRAPH_H
#include "vgraph.h"
#include "common.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>
using namespace boost;

typedef adjacency_list<setS, vecS, bidirectionalS> boost_graph_type;
class BoostGraph : public VGraph
{
public:
    BoostGraph();
    BoostGraph(VGraph *graph);
    BoostGraph(GGraph input);

    ~BoostGraph();
    bool connected(const Edge &e) const;
    void setConnected(const Edge &e, bool connected);
    void addEdge(const Edge &e);
    void flip(const Edge &e);

    void clear();

    vector<NodeT> nodes() const ;
    int nodeCount() const;

    vector<Edge> allEdges() const;
    vector<Edge> connectedEdges() const;

    //subgraph ismorphism

    NodeMapping subgraphIsoOne(BoostGraph *needle);
    bool subgraphIsoHasOne(vector<BoostGraph> needle);
    vector<NodeMapping> subgraphIsoAll(const BoostGraph *needle);
    int subgraphIsoCountAll(const BoostGraph *needle);

protected:
    boost_graph_type m_graph;
};

#endif // BOOSTGRAPH_H
