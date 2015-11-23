#ifndef MGRAPH_H
#define MGRAPH_H
#include "common.h"
#include <vector>
#include <set>
class MGraph
{
public:
    MGraph();
    MGraph(int nodeCount, NodeT defValue = -1);
    MGraph(GGraph input);
    MGraph(MGraph *copy);
    MGraph(const MGraph &copy);
    ~MGraph();

    bool connected(const Edge &e) const;
    bool connected(NodeT x, NodeT y) const;

    void addEdge(const Edge &e);
    void flip(const Edge &e);


    int setWeight(Edge e, int weight);
    int getWeight(Edge e) const;
    int absolut(const Edge &e) const;

    void clear();
    void normalize();

    static Edge normalize(Edge e);
    static Edge edge(NodeT x, NodeT y);

    vector<NodeT> nodes() const;
    set<NodeT> nodesSet() const;
    vector<Edge> edges() const;
    vector<Edge> connectedEdges() const;


    Model createModel() const;

    int nodeCount() const;

    vector<Edge> difference(MGraph *other);

    set<NodeT> neighborhood(NodeT node) const;
    set<NodeT> closedNeighborhood(NodeT node) const;
    bool hasSameNeighbours(NodeT u, NodeT v);
    set<NodeT> costlyNeighborhood(NodeT node, int maxCost) const;
    int degree(NodeT node) const;

    //DEBUG FUNCTIONS

    void printMatrix() const;
    string printGraph(vector<Subgraph> highlight);
    int writeGraph(string fileName, vector<Subgraph> highlight);
    int writeGraph(string fileName);

    void printEdges(vector<Edge> edges);
    void printEdge(const Edge &e);

    void debugEdges(vector<Edge> edges);
    void debugEdge(const Edge &e);

    string printEdgeLatex(const Edge &e);
    string info();

   private:
    int m_nodeCount;

    GGraph m_input;
    int absolut(NodeT u, NodeT v) const;
    int getWeight(NodeT x, NodeT y) const;
    
    NodeT **m_matrix = nullptr;

};

#endif // MGRAPH_H
