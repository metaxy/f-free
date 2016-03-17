#include "commongraph.h"
CommonGraph::CommonGraph()
{
    //de("CommonGraph::CommonGraph()");
}
bool CommonGraph::connected(const Edge &e) const
{
    return connected(e.first,e.second);
}

void CommonGraph::addEdge(const Edge &e)
{
    setWeight(e, M_CONNECTED);
}
void CommonGraph::flip(const Edge &e)
{
    int newVal  = connected(e) ? M_NOT_CONNECTED : M_CONNECTED;
    setWeight(e, newVal);
}

int CommonGraph::absolut(NodeT u, NodeT v) const
{
    return abs(getWeight(u,v));
}
int CommonGraph::absolut(const Edge &e) const
{
    return absolut(e.first, e.second);
}

void CommonGraph::clear()
{
    for(Edge e : edges()) {
        setWeight(e, M_NOT_CONNECTED); //not connected
    }
}

Edge CommonGraph::normalize(Edge e)
{
    if(e.first > e.second) return Edge(e.second, e.first);
    return e;
}

Edge CommonGraph::edge(NodeT x, NodeT y)
{
    if(x > y) return Edge(y,x);
    return Edge(x,y);
}
