#include "mgraph.h"
#include <fstream>
#include <sstream>
#include <cassert>
MGraph::MGraph()
{

}
/**
 * @brief MGraph::MGraph is private constuctor
 * @param nodeCount how many nodes has this graph
 * @param defValue is a edge per default connected or not
 */
MGraph::MGraph(int nodeCount, NodeT defValue) : m_nodeCount(nodeCount)
{
    m_matrix = new NodeT*[m_nodeCount];

    //init main matrix
    for (int i=0; i<m_nodeCount; i++) {
       m_matrix[i] = new NodeT[m_nodeCount];
       for (int j=0; j<m_nodeCount; j++) {
           if(i == j)
               m_matrix[i][j] = M_NOT_CONNECTED; //
           else
               m_matrix[i][j] = defValue;
       }
    }
}

MGraph::MGraph(GGraph input) : MGraph(input.nodeCount())
{
    m_input = input;
    for(Edge e : input.edges()) {
        this->addEdge(e);
    }
}
MGraph::MGraph(MGraph *copy)
{
    m_nodeCount = copy->m_nodeCount;
    m_input = copy->m_input;
    m_matrix = new NodeT*[m_nodeCount];
    for(int i = 0; i < m_nodeCount; i++) {
        m_matrix[i] = new NodeT[m_nodeCount];
        for(int j = 0; j < m_nodeCount; j++) {
            m_matrix[i][j] = copy->m_matrix[i][j];
        }
    }
}
MGraph::MGraph(const MGraph &copy)
{
    //de("MGraph::Mgraph(MGraph&)");
    m_nodeCount = copy.m_nodeCount;
    m_input = copy.m_input;
    m_matrix = new NodeT*[m_nodeCount];
    for(int i = 0; i < m_nodeCount; i++) {
        m_matrix[i] = new NodeT[m_nodeCount];
        for(int j = 0; j < m_nodeCount; j++) {
            m_matrix[i][j] = copy.m_matrix[i][j];
        }
    }
}
MGraph::MGraph(VGraph *other) :  MGraph(other->nodeCount())
{
    m_input = other->m_input;
    for(Edge e : other->connectedEdges()) {
        this->addEdge(e);
    }
}

MGraph::~MGraph()
{
    for(int i = 0; i < m_nodeCount; i++) {
        delete [] m_matrix[i];
    }
}
void MGraph::addEdge(const Edge &e)
{
    setWeight(e, M_CONNECTED);
}
bool MGraph::connected(NodeT x, NodeT y) const
{
    assert(x >= 0);
    assert(x < m_nodeCount);
    assert(y >= 0);
    assert(y < m_nodeCount);
    return m_matrix[x][y] > M_NOT_CONNECTED;
}
bool MGraph::connected(const Edge &e) const
{
    return connected(e.first,e.second);
}

void MGraph::setConnected(const Edge &e, bool connected)
{
    setWeight(e, connected ? M_CONNECTED : M_NOT_CONNECTED);
}

int MGraph::setWeight(Edge e, int weight)
{
    m_matrix[e.first][e.second] = weight;
    m_matrix[e.second][e.first] = weight;
    return weight;
}
void MGraph::flip(const Edge &e)
{
    int newVal  = connected(e) ? 0 : 1;
    setWeight(e, newVal);
}

int MGraph::getWeight(Edge e) const
{
    return m_matrix[e.first][e.second];
}

void MGraph::clear()
{
    for(Edge e : allEdges()) {
        setWeight(e, M_NOT_CONNECTED); //not connected
    }
}


vector<NodeT> MGraph::nodes() const
{
    vector<NodeT> list;
    for(int i = 0; i < m_nodeCount; i++) {
        list.push_back(i);
    }
    return list;
}

vector<Edge> MGraph::allEdges() const
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        for(int j = i+1; j < m_nodeCount; j++) {
            list.push_back(Edge(i,j));
        }
    }
    return list;
 }
vector<Edge> MGraph::connectedEdges() const
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        for(int j = i+1; j < m_nodeCount; j++) {
            if(connected(i,j)) {
                list.push_back(Edge(i,j));
            }
        }
    }
    return list;
}
int MGraph::nodeCount() const
{
    return m_nodeCount;
}

set<NodeT> MGraph::neighborhood(NodeT node) const
{
    set<NodeT> ret;
    for(int i = 0; i< m_nodeCount; i++) {
        if(node == i) continue;
        if(connected(node, i)) ret.insert(i);
    }
    return ret;
}
