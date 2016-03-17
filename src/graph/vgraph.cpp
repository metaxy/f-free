#include "vgraph.h"
#include <cassert>
#include <iostream>
VGraph::VGraph()
{
}
VGraph::~VGraph()
{
}
bool VGraph::connected(const Edge &e) const
{
    assert(false);
    return false;
}
void VGraph::setConnected(const Edge &e, bool connected)
{
    assert(false);
}
void VGraph::addEdge(const Edge &e)
{
    assert(false);
}
void VGraph::flip(const Edge &e)
{
    assert(false);
}
void VGraph::clear()
{
    assert(false);
}

vector<NodeT> VGraph::nodes() const
{
    assert(false);
}

vector<Edge> VGraph::allEdges() const
{
    assert(false);
}
vector<Edge> VGraph::connectedEdges() const
{
    assert(false);
}
int VGraph::nodeCount() const
{
    assert(false);
    return 0;
}


Model VGraph::createModel() const
{
    Model model;
    for(Edge e : this->allEdges()) {
         model[e] = connected(e) ? 1 : 0;
    }
    return model;
}
vector<Edge> VGraph::difference(const VGraph *other) const
{
    vector<Edge> list;
    for(Edge edge : this->allEdges()) {
        if(this->connected(edge) != other->connected(edge)) {
            list.push_back(edge);
        }
    }
    return list;
}

void VGraph::printEdge(const Edge &e) const
{
    std::cout << m_input.nodeName(e.first) << " " << m_input.nodeName(e.second) << std::endl;
}

void VGraph::printEdges(vector<Edge> edges) const
{
    for(const Edge &e: edges)
        printEdge(e);
}

