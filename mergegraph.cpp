#include "mergegraph.h"
#include <fstream>
#include <sstream>
#include <cassert>
MergeGraph::MergeGraph()
{
    de("MergeGraph::MergeGraph()");
}

MergeGraph::MergeGraph(int nodeCount) : m_nodeCount(nodeCount)
{
    de("MergeGraph::MergeGraph(int) ");
    m_matrix = new NodeT*[m_nodeCount];

    //init vector where is saved whether the node is deleted,
    //when value is -1, this means it is not deleted
    //when value is >= 0, this is node it was merged with
    m_deleted = new NodeT[m_nodeCount];

    memset(m_deleted, -1, m_nodeCount * sizeof(NodeT));

    //init main matrix
    for (int i=0; i<m_nodeCount; i++) {
       m_matrix[i] = new NodeT[m_nodeCount];
       memset(m_matrix[i], -1, m_nodeCount * sizeof(NodeT));
    }
}
MergeGraph::MergeGraph(GGraph input) : MergeGraph(input.nodeCount())
{
    //m_input = input;
    for(Edge e : input.edges()) {
        this->addEdge(e);
    }
}
MergeGraph::MergeGraph(MergeGraph *copy)
{
    de("MergeGraph::MergeGraph(MergeGraph*)");
    m_nodeCount = copy->m_nodeCount;
    m_matrix = new NodeT*[m_nodeCount];
    m_deleted = new NodeT[m_nodeCount];
    for(int i = 0; i < m_nodeCount; i++) {
        m_deleted[i] = copy->m_deleted[i];
        m_matrix[i] = new NodeT[m_nodeCount];
        for(int j = 0; j < m_nodeCount; j++) {
            m_matrix[i][j] = copy->m_matrix[i][j];
        }
    }
}
MergeGraph::MergeGraph(const MergeGraph &copy)
{
    de("MergeGraph::MergeGraph(MergeGraph&)");
    m_nodeCount = copy.m_nodeCount;
    m_matrix = new NodeT*[m_nodeCount];
    m_deleted = new NodeT[m_nodeCount];
    for(int i = 0; i < m_nodeCount; i++) {
        m_deleted[i] = copy.m_deleted[i];
        m_matrix[i] = new NodeT[m_nodeCount];
        for(int j = 0; j < m_nodeCount; j++) {
            m_matrix[i][j] = copy.m_matrix[i][j];
        }
    }
}

MergeGraph::~MergeGraph()
{
}
void MergeGraph::addEdge(const Edge &e)
{
    setWeight(e, 1);
}
int MergeGraph::absolut(NodeT u, NodeT v) const
{
    return abs(m_matrix[u][v]);
}
int MergeGraph::absolut(const Edge &e) const
{
    return absolut(e.first,e.second);
}
bool MergeGraph::isDeleted(NodeT x) const
{
    assert(x >= 0);
    assert(x < m_nodeCount);
    return m_deleted[x] != -1;
}
bool MergeGraph::connected(NodeT x, NodeT y) const
{
    assert(x >= 0);
    assert(x < m_nodeCount);
    assert(y >= 0);
    assert(y < m_nodeCount);
    return m_matrix[x][y] > 0;
}
bool MergeGraph::connected(const Edge &e) const
{
    return connected(e.first,e.second);
}
void MergeGraph::setWeight(Edge e, int weight)
{
    m_matrix[e.first][e.second] = weight;
    m_matrix[e.second][e.first] = weight;
}
void MergeGraph::flip(const Edge &e)
{
    m_matrix[e.first][e.second] = -m_matrix[e.first][e.second];
    m_matrix[e.second][e.first] = -m_matrix[e.second][e.first];
}

int MergeGraph::getWeight(Edge e) const
{
    return m_matrix[e.first][e.second];
}
int MergeGraph::getWeight(NodeT x, NodeT y) const
{
    return m_matrix[x][y];
}
void MergeGraph::clear()
{
    for(Edge e : edges()) {
        setWeight(e, -1); //not connected
    }
}

int MergeGraph::merge(const Edge e)
{
    const NodeT u = e.first;
    const NodeT v = e.second;

    int depth = 0;

    for(int i = 0; i< m_nodeCount; i++) {
        if(i != v && i != u && !isDeleted(i)) {
            if(sgn(m_matrix[i][u]) != sgn(m_matrix[i][v])) {
                depth += min(abs(m_matrix[i][u]), abs(m_matrix[i][v]));
            }
            m_matrix[i][u] += m_matrix[i][v];
            m_matrix[u][i] = m_matrix[i][u];
        }
    }

    m_deleted[v] = u;//v is deleted
    return depth;
}
int MergeGraph::dismerge(const Edge e)
{
    const NodeT u = e.first;
    const NodeT v = e.second;
    int depth = 0;

    for(int i = 0; i< m_nodeCount; i++) {
        if(i != v && i != u && !isDeleted(i)) {
            m_matrix[i][u] -= m_matrix[i][v];
            m_matrix[u][i] = m_matrix[i][u];
            if(sgn(m_matrix[i][u]) != sgn(m_matrix[i][v])) {
                depth += min(abs(m_matrix[i][u]), abs(m_matrix[i][v]));
            }
        }
    }

    //node v is not deleted
    m_deleted[v] = -1;
    return depth;
}

vector<NodeT> MergeGraph::nodes() const
{
    vector<NodeT> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(!isDeleted(i)) {
            list.push_back(i);
        }
    }
    return list;
}

set<NodeT> MergeGraph::nodesSet() const
{
    set<NodeT> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(!isDeleted(i)) {
            list.insert(i);
        }
    }
    return list;
}


vector<Edge> MergeGraph::edges() const
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;
        for(int j = i+1; j < m_nodeCount; j++) {
            if(isDeleted(i)) continue;
            list.push_back(Edge(i,j));
        }
    }
    return list;
 }
vector<Edge> MergeGraph::connectedEdges() const
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;
        for(int j = i+1; j < m_nodeCount; j++) {
            if(!isDeleted(j) && connected(i,j)) {
                list.push_back(Edge(i,j));
            }
        }
    }
    return list;
}

Model MergeGraph::createModel() const
{
    Model model;
    for(int i = 0; i < m_nodeCount; i++) {
       for(int j = i+1; j < m_nodeCount; j++) {
           if(!isDeleted(i) && !isDeleted(j)) {
               model[Edge(i,j)] = getWeight(i,j);
           }
       }
   }
   return model;
}
Edge MergeGraph::normalize(Edge e)
{
    if(e.first > e.second) return Edge(e.second, e.first);
    return e;
}

Edge MergeGraph::edge(NodeT x, NodeT y)
{
    if(x > y) return Edge(y,x);
    return Edge(x,y);
}
bool MergeGraph::hasSameNeighbours(NodeT u, NodeT v)
{
    NodeT i;
    for (i=0; i<u; i++) if (!isDeleted(i) && (connected(i,u) != connected(i, v))) return false;
    for (i++; i<v; i++) if (!isDeleted(i) && (connected(u,i) != connected(i, v))) return false;
    for (i++; i<m_nodeCount; i++) if (!isDeleted(i) && (connected(u,i) != connected(v, i))) return false;
    return true;
}
P3 MergeGraph::findP3() const
{
    de("find all p3s");
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;

        for(int j = 0; j < m_nodeCount; j++) {
            if(isDeleted(j) || i == j || !connected(i,j)) continue;

            for(int k = 0; k < m_nodeCount; k++) {
                if(isDeleted(k)) continue;

                if(j != k && i != k && connected(j,k) && !connected(i,k)) {
                    return P3(i,j,k);
                }
            }

        }
    }
    return P3(0,0,0);
}
vector<P3> MergeGraph::findAllP3s() const
{
    de("find all p3s");
    vector<P3> ret;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;

        for(int j = 0; j < m_nodeCount; j++) {
            if(isDeleted(j) || i == j || !connected(i,j)) continue;

            for(int k = 0; k < m_nodeCount; k++) {
                if(isDeleted(k)) continue;

                if(j != k && i != k && connected(j,k) && !connected(i,k)) {
                    ret.push_back(P3(i,j,k));
                }
            }

        }
    }
    return ret;
}
P3 MergeGraph::findNextBestP3(int minCost) const
{
    P3 bestP3(0,0,0);
    int bestCost = -1;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;

        for(int j = 0; j < m_nodeCount; j++) {
            if(isDeleted(j) || i == j || !connected(i,j)) continue;

            const int cost = mergeCost(i,j);
            bool has_p3 = false;
            for(int k = 0; k < m_nodeCount; k++) {
                if(isDeleted(k)) continue;
                if(j != k && i != k && connected(j,k) && !connected(i,k)) {
                    bestP3 = P3(i,j,k);
                    assert(isP3(bestP3));
                    bestCost = cost;
                    has_p3 = true;
                    break;
                }
            }
            if(has_p3 && minCost > 0 && cost > minCost) return bestP3;
        }

    }
    if(bestP3 == P3(0,0,0)) return findP3();
    assert(isP3(bestP3) || bestP3 == P3(0,0,0));
    return bestP3;
}

int MergeGraph::nodeCount() const
{
    return m_nodeCount;
}

vector<Edge> MergeGraph::difference(MergeGraph *other)
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;
        for(int j = i+1; j < m_nodeCount; j++) {
            if(isDeleted(j)) continue;
            if(other->m_matrix[i][j] - m_matrix[i][j] != 0) {
                //clog << other->m_matrix[i][j] << m_matrix[i][j] << endl;
                list.push_back(Edge(i,j));
            }
        }
    }
    return list;
}

void MergeGraph::restoreMerges()
{
    int count = 0;
    while(count <= m_nodeCount) {
        for(int i = 0; i< m_nodeCount; i++) {
            if(isDeleted(i)) {
                NodeT mergeTarget = m_deleted[i];
                if(!isDeleted(mergeTarget)) {
                    for(int t=0; t < m_nodeCount; t++) {
                        if(t == mergeTarget) {
                            m_matrix[i][t] = 1;
                            m_matrix[t][i] = 1;
                        } else {
                            m_matrix[i][t] = connected(mergeTarget,t) ? 1 : -1;
                            m_matrix[t][i] = connected(mergeTarget,t) ? 1 : -1;
                        }
                    }
                    m_deleted[i] = -1;//i is not anymore deleted
                } else {
                    count = -1;
                }
            }
            count++;
        }
    }
}

void MergeGraph::normalize()
{
   for(int i=0; i< m_nodeCount; i++) {
       for(int j=0; j<m_nodeCount; j++) {
           m_matrix[i][j] = connected(i,j) ? 1 : -1;
       }
   }

}
set<NodeT> MergeGraph::neighborhood(NodeT node) const
{
    set<NodeT> ret;
    for(int i = 0; i< m_nodeCount; i++) {
        if(isDeleted(i) || node == i) continue;
        if(connected(node, i)) ret.insert(i);
    }
    return ret;
}
set<NodeT> MergeGraph::costlyNeighborhood(NodeT node, int maxCost) const
{
    set<NodeT> ret;
    for(int i = 0; i< m_nodeCount; i++) {
        if(isDeleted(i) || node == i) continue;
        if(mergeCost(node,i) <= maxCost) {
            ret.insert(i);
        }
    }
    return ret;
}
set<NodeT> MergeGraph::closedNeighborhood(NodeT node) const
{
    set<NodeT> ret = neighborhood(node);
    ret.insert(node);
    return ret;
}
set<NodeT> MergeGraph::closedCostlyNeighborhood(NodeT node, int maxCost) const
{
    set<NodeT> ret = costlyNeighborhood(node, maxCost);
    ret.insert(node);
    return ret;
}
int MergeGraph::costMakingClique(NodeT node)
{
    set<NodeT> n = closedNeighborhood(node);
    int ret = 0;
    for(NodeT v : n) {
        for(NodeT w : n) {
            if(!connected(v,w)) {
                ret += abs(getWeight(v,w));
            }
        }
    }
    return ret;
}
int MergeGraph::costCutting(NodeT node)
{
    set<NodeT> n = closedNeighborhood(node);
    int ret = 0;
    for(NodeT v : n) {
        for(NodeT w : nodes()) {
            if(n.count(w) == 0 && connected(v,w)) {
                ret += getWeight(v,w);
            }
        }
    }
    return ret;
}
bool MergeGraph::fast2K(NodeT node, float c, float cu, float size, float diff) const
{
    set<NodeT> n = closedNeighborhood(node);
    int clique = 0;
    int cutting = 0;
    for(NodeT v : n) {
        for(NodeT w : n) {
            if(!connected(v,w)) {
                clique += abs(m_matrix[v][w]);
            }
        }
        //todo: improve by deleteing nodes of n from nodes()
        for(NodeT w : nodes()) {
            if(n.count(w) == 0 && connected(v,w)) {
                cutting += m_matrix[v][w];
            }
        }
    }
    return diff*c*((float)clique) + cu*((float)cutting) <= size*n.size();
}
int MergeGraph::mergeCost(NodeT u, NodeT v) const
{
    int depth = 0;
    for(int t = 0; t< m_nodeCount; t++) {
        if(t != u && t != v && !isDeleted(t)) {
            if(sgn(m_matrix[t][u]) != sgn(m_matrix[t][v])) {
                depth += min(abs(m_matrix[t][u]), abs(m_matrix[t][v]));
            }
        }
    }
    assert(depth >= 0);
    return depth;
}
int MergeGraph::mergeCost(Edge e) const
{
    return mergeCost(e.first,e.second);
}
bool MergeGraph::isP3(P3 p3) const
{
    const NodeT a = get<0>(p3);
    const NodeT b = get<1>(p3);
    const NodeT c = get<2>(p3);
    return !connected(a,c) && connected(a,b) && connected(b,c) && !isDeleted(a) && !isDeleted(b) && !isDeleted(c);
}
