#include "mgraph.h"
#include <fstream>
#include <sstream>
#include <cassert>
MGraph::MGraph()
{
    //de("MGraph::Mgraph()");
}

MGraph::MGraph(int nodeCount, NodeT defValue) : m_nodeCount(nodeCount)
{
    m_matrix = new NodeT*[m_nodeCount];

    //init main matrix
    for (int i=0; i<m_nodeCount; i++) {
       m_matrix[i] = new NodeT[m_nodeCount];
       for (int j=0; j<m_nodeCount; j++) {
           if(i == j)
               m_matrix[i][j] = -1; //
           else
               m_matrix[i][j] = defValue;
       }
    }
}

MGraph::MGraph(GGraph input) : MGraph(input.nodeCount())
{
    //de("MGraph::Mgraph(Graph)");
    m_input = input;
    for(Edge e : input.edges()) {
        this->addEdge(e);
    }
}
MGraph::MGraph(MGraph *copy)
{
    //de("MGraph::Mgraph(MGraph*)");
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

MGraph::~MGraph()
{
   /* if(m_deleted != nullptr)
        delete[] m_deleted;
    for(int i = 0; i < m_nodeCount; i++) {
        delete [] m_matrix[i];
    }*/
}
void MGraph::addEdge(const Edge &e)
{
    setWeight(e, 1);
}
int MGraph::absolut(NodeT u, NodeT v) const
{
    return abs(m_matrix[u][v]);
}
int MGraph::absolut(const Edge &e) const
{
    return absolut(e.first,e.second);
}
bool MGraph::connected(NodeT x, NodeT y) const
{
    assert(x >= 0);
    assert(x < m_nodeCount);
    assert(y >= 0);
    assert(y < m_nodeCount);
    return m_matrix[x][y] > 0;
}
bool MGraph::connected(const Edge &e) const
{
    return connected(e.first,e.second);
}
int MGraph::setWeight(Edge e, int weight)
{
    m_matrix[e.first][e.second] = weight;
    m_matrix[e.second][e.first] = weight;
    return weight;
}
void MGraph::flip(const Edge &e)
{
    m_matrix[e.first][e.second] = -m_matrix[e.first][e.second];
    m_matrix[e.second][e.first] = -m_matrix[e.second][e.first];
}

int MGraph::getWeight(Edge e) const
{
    return m_matrix[e.first][e.second];
}
int MGraph::getWeight(NodeT x, NodeT y) const
{
    return m_matrix[x][y];
}

void MGraph::clear()
{
    for(Edge e : edges()) {
        setWeight(e, -1); //not connected
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

set<NodeT> MGraph::nodesSet() const
{
    set<NodeT> list;
    for(int i = 0; i < m_nodeCount; i++) {
        list.insert(i);
    }
    return list;
}


vector<Edge> MGraph::edges() const
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

Model MGraph::createModel() const
{
    Model model;
    for(int i = 0; i < m_nodeCount; i++) {
       for(int j = i+1; j < m_nodeCount; j++) {
            model[Edge(i,j)] = getWeight(i,j);
       }
   }
   return model;
}
Edge MGraph::normalize(Edge e)
{
    if(e.first > e.second) return Edge(e.second, e.first);
    return e;
}

Edge MGraph::edge(NodeT x, NodeT y)
{
    if(x > y) return Edge(y,x);
    return Edge(x,y);
}
bool MGraph::hasSameNeighbours(NodeT u, NodeT v)
{
    NodeT i;
    for (i=0; i<u; i++) if (connected(i,u) != connected(i, v)) return false;
    for (i++; i<v; i++) if (connected(u,i) != connected(i, v)) return false;
    for (i++; i<m_nodeCount; i++) if (connected(u,i) != connected(v, i)) return false;
    return true;
}
int MGraph::nodeCount() const
{
    return m_nodeCount;
}

vector<Edge> MGraph::difference(MGraph *other)
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        for(int j = i+1; j < m_nodeCount; j++) {
            if(other->m_matrix[i][j] - m_matrix[i][j] != 0) {
                list.push_back(Edge(i,j));
            }
        }
    }
    return list;
}

void MGraph::normalize()
{
   for(int i=0; i< m_nodeCount; i++) {
       for(int j=0; j<m_nodeCount; j++) {
           m_matrix[i][j] = connected(i,j) ? 1 : -1;
       }
   }

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
set<NodeT> MGraph::closedNeighborhood(NodeT node) const
{
    set<NodeT> ret = neighborhood(node);
    ret.insert(node);
    return ret;
}
void MGraph::printMatrix() const
{
    #ifdef _DEBUG
    Common::printMatrix(m_matrix, m_nodeCount);
    #endif
}

string MGraph::printGraph(vector<Subgraph> highlight)
{
    #ifdef _DEBUG
    string ret = "digraph G {\n";
    ret += "edge [dir=none]\n";
    for(int x = 0; x < m_nodeCount; x++) {

        for(int y = x+1; y < m_nodeCount; y++) {
            if(connected(x,y)) {
                ret += "" +  m_input.nodeName(x) + " -> " +  m_input.nodeName(y) + " [label=\""+std::to_string(m_matrix[x][y]) + "\"";
                for(int i = 0; i < highlight.size(); i++) {
                    bool found = false;
                    for(Edge e : highlight[i]) {
                        if((e.first == x && e.second == y) || (e.first == y && e.second == x)) {
                            ret += ",color=\"" + Common::dotColor(i, highlight.size()) + "\"";
                            clog << "found " << "i = " << i << " of " << highlight.size() << endl;
                            found = true;
                            break;
                        }
                    }
                    if(found) break;
                }
                ret += "];\n";
            }
        }
    }
    ret += "\n}\n";
    return ret;
    #endif
}




int MGraph::writeGraph(string fileName, vector<Subgraph> highlight)
{
    std::ofstream fout(fileName+".dot");
    fout << printGraph(highlight);
    fout.close();
    std::stringstream stream;
    stream << "dot -Tpng \"" << fileName << ".dot\" -o \"" << fileName << ".png\"";
    int ret = system(stream.str().c_str());
    remove((fileName+".dot").c_str());
    return ret;
}
int MGraph::writeGraph(string fileName)
{
    vector<Subgraph> highlight;
    return this->writeGraph(fileName, highlight);
}

void MGraph::printEdge(const Edge &e)
{
    cout << m_input.nodeName(e.first) << " " << m_input.nodeName(e.second) << endl;
}
string MGraph::printEdgeLatex(const Edge &e)
{
    return "e_{"+m_input.nodeName(e.first) + "," + m_input.nodeName(e.second) +"}";
}

void  MGraph::printEdges(vector<Edge> edges)
{
    for(const Edge &e: edges)
        printEdge(e);
}


void MGraph::debugEdge(const Edge &e)
{
    clog << m_input.nodeName(e.first) << " " << m_input.nodeName(e.second) << endl;
}

void  MGraph::debugEdges(vector<Edge> edges)
{
    for(const Edge &e: edges)
        printEdge(e);
}

string MGraph::info()
{
    return "nodeCount = " + std::to_string(m_nodeCount);
}
