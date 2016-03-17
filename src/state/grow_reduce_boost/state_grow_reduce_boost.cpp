#include "state_grow_reduce_boost.h"
StateGrowReduceBoost::StateGrowReduceBoost(Config conf) : BState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
    m_factorSize = getDouble("factorSize", 1.0);
}
BoostGraph StateGrowReduceBoost::solve()
{
    BoostGraph graph(m_input.m_input);
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
            graph.setConnected(e, m_input.connected(e));
        }

        this->reduce(&graph);
        if(m_input.difference(&graph).size() == 0)
            break;
    }
    this->extend(&graph);
    return graph;
}

void StateGrowReduceBoost::reduce(BoostGraph *graph)
{
    clog << "reduce" << endl;
    for(auto forbidden : m_forbidden) {
        vector<Edge> forbiddenEdges = forbidden.allEdges();
        NodeMapping mapping = graph->subgraphIsoOne(&forbidden);
        while(!mapping.empty()) {
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            int size_before = graph->subgraphIsoCountAll(&forbidden);//this is very time expensive
            graph->flip(foundEdge);
            if(graph->subgraphIsoCountAll(&forbidden) >= m_factorSize * float(size_before)) {
                graph->flip(foundEdge);
            }
            mapping = graph->subgraphIsoOne(&forbidden);
        }
    }
}
void StateGrowReduceBoost::extend(BoostGraph *graph)
{
    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
    }
}

void StateGrowReduceBoost::final()
{
}
