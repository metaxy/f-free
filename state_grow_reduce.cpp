#include "state_grow_reduce.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
#include <math.h>
StateGrowReduce::StateGrowReduce(Config conf) : State(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
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
        if(m_input.difference(&graph).size() == 0) break;
        this->reduce(&graph);
    }
    this->extend(&graph);
    return graph;
}

bool StateGrowReduce::isValid(MGraph *input)
{
    return !VF::subgraphIsoHasOne(input, m_forbidden);
}

void StateGrowReduce::grow(MGraph *graph, NodeT node)
{
    set<NodeT> neighborhood = m_input.neighborhood(node);
    clog << "neigborhood size " << neighborhood.size() << endl;
    for(NodeT n: neighborhood) {
        Edge e(n, node);
        graph->setWeight(e, m_input.getWeight(e));
    }
}

void StateGrowReduce::reduce(MGraph *graph)
{
    for(auto forbidden : m_forbidden) {
        clog << "forbidden start" << endl;
        vector<Edge> forbiddenEdges = forbidden.edges();
        NodeMapping mapping = VF::subgraphIsoOne(graph, &forbidden);
        while(!mapping.empty()) {
            //clog << "test mapping" << endl;
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            int size_before = VF::subgraphIsoAll(graph, &forbidden).size();
           // clog << size_before << endl;
            graph->flip(foundEdge);
            if(VF::subgraphIsoAll(graph, &forbidden).size() >= size_before) {
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