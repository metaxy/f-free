#ifndef COMMONGRAPH_H
#define COMMONGRAPH_H
#include "common.h"
#include <vector>
#include <set>
#define M_CONNECTED (1)
#define M_NOT_CONNECTED (0)

class CommonGraph
{
public:
    CommonGraph();
    CommonGraph(int nodeCount, NodeT defValue = M_NOT_CONNECTED);
    CommonGraph(GGraph input);
    CommonGraph(CommonGraph *copy);
    CommonGraph(const CommonGraph &copy);
    ~CommonGraph();

    virtual bool connected(const Edge &e) const = 0;
    bool connected(NodeT x, NodeT y) const;

    void addEdge(const Edge &e);
    void flip(const Edge &e);


    virtual int setWeight(Edge e, int weight) = 0;
    virtual int getWeight(Edge e) const = 0;
    int absolut(const Edge &e) const;
    int absolut(NodeT u, NodeT v) const ;

    void clear();
    void normalize();

    static Edge normalize(Edge e);
    static Edge edge(NodeT x, NodeT y);

    virtual  vector<NodeT> nodes() const = 0;
    virtual  set<NodeT> nodesSet() const = 0;
    virtual vector<Edge> edges() const = 0;
    virtual  vector<Edge> connectedEdges() const = 0;

    virtual int nodeCount() const = 0;

    virtual vector<Edge> difference(const CommonGraph *other) = 0;


   protected:
    virtual int getWeight(NodeT x, NodeT y) const = 0;
};

#endif // CommonGraph_H
