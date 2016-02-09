#include "state_grow_reduce3.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
#include <math.h>
StateGrowReduce3::StateGrowReduce3(Config conf) : State(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
}
MGraph StateGrowReduce3::solve()
{
    MGraph graph(m_input);
    graph.clear();
    vector<NodeT> nodes = r->randomVector(m_input.nodes());
    set<NodeT> explored;
    for(NodeT node: nodes) {
        explored.insert(node);
        set<NodeT> neighborhood = m_input.neighborhood(node);
        for(NodeT n: neighborhood) {
            if(explored.find(node) == explored.end())
                continue;
            Edge e(n, node);
            graph.setWeight(e, m_input.getWeight(e));
        }
        this->reduce(&graph);
         if(m_input.difference(&graph).size() == 0) break;
    }
    this->extend(&graph);
    return graph;
}

void StateGrowReduce3::grow(MGraph *graph, NodeT node)
{

}

void StateGrowReduce3::reduce(MGraph *graph)
{
    for(auto forbidden : m_forbidden) {
        vector<Edge> forbiddenEdges = forbidden.edges();
        NodeMapping mapping = VF::subgraphIsoOne(graph, &forbidden);
        while(!mapping.empty()) {
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            int size_before = VF::subgraphIsoAll(graph, &forbidden).size();
            graph->flip(foundEdge);
            if(VF::subgraphIsoAll(graph, &forbidden).size() >= size_before) {
                graph->flip(foundEdge);
            }
            mapping = VF::subgraphIsoOne(graph, &forbidden);
        }
    }
}
void StateGrowReduce3::extend(MGraph *graph)
{
    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
    }
}

void StateGrowReduce3::final()
{
}