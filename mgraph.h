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
    void addEdge(const Edge &e);
    bool connected(const Edge &e) const;
    int absolut(const Edge &e) const;
    bool isDeleted(NodeT x) const;
    void flip(const Edge &e);
    int setWeight(Edge e, int weight);
    int getWeight(Edge e) const;
    int increaseWeight(const Edge &e, int factor);
    string printEdgeLatex(const Edge &e);
    string info();



    void clear();

    int merge(Edge e);
    int dismerge(Edge e);

    vector<NodeT> nodes() const;
    set<NodeT> nodesSet() const;
    vector<Edge> edges() const;
    set<NodeT> neighborhood(NodeT node) const;
    Model createModel() const;


    static Edge normalize(Edge e);
    static Edge edge(NodeT x, NodeT y);

    int nodeCount() const;

    vector<Edge> difference(MGraph *other);

    void restoreMerges();
    void normalize();

    void printMatrix() const;
    string printGraph(vector<Subgraph> highlight);
    int writeGraph(string fileName, vector<Subgraph> highlight);
    int writeGraph(string fileName);

    int mergeCost(NodeT u, NodeT v) const;
    int mergeCost(Edge e) const;
    vector<Edge> connectedEdges() const;

    bool connected(NodeT x, NodeT y) const;

    int costCutting(NodeT node);
    int costMakingClique(NodeT node);
    set<NodeT> closedNeighborhood(NodeT node) const;
    bool hasSameNeighbours(NodeT u, NodeT v);
    set<NodeT> costlyNeighborhood(NodeT node, int maxCost) const;
    set<NodeT> closedCostlyNeighborhood(NodeT node, int maxCost) const;


   void printEdges(vector<Edge> edges);
   void printEdge(const Edge &e);

   void debugEdges(vector<Edge> edges);
   void debugEdge(const Edge &e);

   P3 findP3() const;


   vector<Edge> absHeightesEdgeOfEachRow();

   private:
    int m_nodeCount;

    GGraph m_input;
    int absolut(NodeT u, NodeT v) const;
    int getWeight(NodeT x, NodeT y) const;
    
    NodeT **m_matrix = nullptr;
    NodeT *m_deleted = nullptr;

};

#endif // MGRAPH_H
