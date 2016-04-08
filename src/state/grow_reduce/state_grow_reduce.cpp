#include "state_grow_reduce.h"
#include "src/iso/vf.h"
StateGrowReduce::StateGrowReduce(Config conf) : MState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
}
MGraph StateGrowReduce::solve()
{
    MGraph graph(m_input);
    graph.clear();
    vector<NodeT> nodes = r->randomVector(m_input.nodes());
    for(NodeT node: nodes) {
        this->grow(&graph, node);
        while(!isValid(&graph)) {
            this->reduce(&graph);
        }
        assert(VF::subgraphIsoHasOne(&graph, m_forbidden) == false);
    }
    assert(VF::subgraphIsoHasOne(&graph, m_forbidden) == false);
    this->extend(&graph);
    return graph;
}

void StateGrowReduce::grow(MGraph *graph, NodeT node)
{
    set<NodeT> neighborhood = m_input.neighborhood(node);
    for(NodeT n: neighborhood) {
        Edge e(n, node);
        graph->setConnected(e, m_input.connected(e));
    }
}

void StateGrowReduce::reduce(MGraph *graph)
{
    clog << "start reduce" << endl;
    for(auto forbidden : m_forbidden) {
        vector<Edge> forbiddenEdges = forbidden.allEdges();
        NodeMapping mapping = VF::subgraphIsoOne(graph, &forbidden);
        while(!mapping.empty()) {
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            int size_before = VF::subgraphIsoCountAll(graph, &forbidden);
            graph->flip(foundEdge);
            if(VF::subgraphIsoCountAll(graph, &forbidden) >= size_before) {
                graph->flip(foundEdge);
            }
            mapping = VF::subgraphIsoOne(graph, &forbidden);
        }
    }
}
void StateGrowReduce::extend(MGraph *graph)
{
    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
    }
}

void StateGrowReduce::final()
{
}
