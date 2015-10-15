#include <sstream>
#include <fstream>
#include "graph.h"

GGraph::GGraph() : m_nodeCount(0)
{

}


void GGraph::insertEdge(string node1, string node2)
{
    int u,v;
    if(m_nodeNames.find(node1) != m_nodeNames.end()) {
        u = m_nodeNames[node1];
    } else {
        u = m_nodeCount;
        m_nodeNames[node1] = u;
        m_nodeCount++;
    }
    if(m_nodeNames.find(node2) != m_nodeNames.end()) {
        v = m_nodeNames[node2];
    } else {
        v = m_nodeCount;
        m_nodeNames[node2] = v;
        m_nodeCount++;
    }
    m_edges.push_back(Edge(u,v));
}

int GGraph::nodePosition(string name) const
{
    return m_nodeNames.find(name)->second;
}

string GGraph::nodeName(int value) const
{
    auto it = m_nodeNames.begin();
    std::advance(it, value);
    return it->first;
}

int GGraph::edgesCount() const
{
    return m_edges.size();
}
int GGraph::nodeCount() const
{
    return m_nodeCount;
}

vector<Edge> GGraph::edges() const
{
    return m_edges;
}
