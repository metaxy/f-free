#include "vgraph.h"
#include <cassert>
#include <iostream>
VGraph::VGraph()
{
}
VGraph::~VGraph()
{

}

Model VGraph::createModel() const
{
    Model model;
    for(Edge e : edges()) {
         model[e] = getWeight(e);
    }
    return model;
}
vector<Edge> VGraph::difference(const VGraph *other) const
{
    vector<Edge> list;
    assert(this->nodeCount() == other->nodeCount());
    for(Edge edge : this->edges()) {
        if(this->getWeight(edge) != other->getWeight(edge)) {
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

