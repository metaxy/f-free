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
        //clog << "difference size" << m_input.difference(&graph).size() << endl;
        this->reduce(&graph);
         if(m_input.difference(&graph).size() == 0) break;
    }
    this->extend(&graph);
    return graph;
}

void StateGrowReduce::grow(MGraph *graph, NodeT node)
{
    set<NodeT> neighborhood = m_input.neighborhood(node);
    clog << "neigborhood size " << neighborhood.size() << endl;
    for(NodeT n: neighborhood) {
        Edge e(n, node);
        graph->setConnected(e, m_input.connected(e));
    }
}

void StateGrowReduce::reduce(MGraph *graph)
{
    for(auto forbidden : m_forbidden) {
        clog << "forbidden start" << endl;
        vector<Edge> forbiddenEdges = forbidden.allEdges();
        NodeMapping mapping = VF::subgraphIsoOne(graph, &forbidden);
        while(!mapping.empty()) {
            //clog << "test mapping" << endl;
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            int size_before = VF::subgraphIsoCountAll(graph, &forbidden);
           // clog << size_before << endl;
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