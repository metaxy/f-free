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

    virtual bool connected(const Edge &e) const = 0;
    virtual void setConnected(const Edge &e, bool connected) = 0;
    virtual void addEdge(const Edge &e) = 0;
    virtual void flip(const Edge &e) = 0;

    virtual void clear() = 0;

    virtual vector<NodeT> nodes() const = 0;

    virtual vector<Edge> allEdges() const = 0;
    virtual vector<Edge> connectedEdges() const = 0;
    virtual int nodeCount() const = 0;




    virtual Model createModel() const;
    virtual vector<Edge> difference(const VGraph *other) const;
    virtual void printEdge(const Edge &e) const;
    virtual void printEdges(vector<Edge> edges) const;
    GGraph m_input;

};

#endif // GRAPH_H
