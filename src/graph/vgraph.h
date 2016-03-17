#ifndef VGRAPH_H
#define VGRAPH_H
#include "ggraph.h"

#include <vector>
#include <set>

class VGraph
{
public:
    VGraph();
    virtual ~VGraph();

    virtual bool connected(const Edge &e) const;
    virtual void setConnected(const Edge &e, bool connected);
    virtual void addEdge(const Edge &e);
    virtual void flip(const Edge &e);
    virtual void clear();

    virtual vector<NodeT> nodes() const;

    virtual vector<Edge> allEdges() const;
    virtual vector<Edge> connectedEdges() const;
    virtual int nodeCount() const;




    virtual Model createModel() const;
    virtual vector<Edge> difference(const VGraph *other) const;
    virtual void printEdge(const Edge &e) const;
    virtual void printEdges(vector<Edge> edges) const;
    GGraph m_input;

};

#endif // GRAPH_H
