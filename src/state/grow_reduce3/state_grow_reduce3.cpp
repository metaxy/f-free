#include "state_grow_reduce3.h"
#include "src/iso/vf.h"
#include "src/iso/forbidden.h"
StateGrowReduce3::StateGrowReduce3(Config conf) : MState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
    m_factorSize = getDouble("factorSize", 1.0);
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
            graph.setConnected(e, m_input.connected(e));
        }

        this->reduce(&graph);
        if(timeLeft() < 2) break;
        if(m_input.difference(&graph).size() == 0)
            break;
    }
    if(timeLeft() < 2) return graph;
    this->extend(&graph);
    return graph;
}

void StateGrowReduce3::grow(MGraph *graph, NodeT node)
{

}


void StateGrowReduce3::reduce(MGraph *graph)
{
    clog << "reduce" << endl;
    for(auto forbidden : m_forbidden) {
        vector<Edge> forbiddenEdges = forbidden.allEdges();
        NodeMapping mapping = VF::subgraphIsoOne(graph, &forbidden);
        while(!mapping.empty()) {
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            int size_before = VF::subgraphIsoCountAll(graph, &forbidden);//this is very time expensive
            graph->flip(foundEdge);
            if(VF::subgraphIsoCountAll(graph, &forbidden) >= m_factorSize * float(size_before)) {
                graph->flip(foundEdge);
            }
            mapping = VF::subgraphIsoOne(graph, &forbidden);
        }
    }
}

//this does not work well
void StateGrowReduce3::reduceSimple(MGraph *graph)
{
    for(auto forbidden : m_forbidden) {
        vector<Edge> forbiddenEdges = forbidden.allEdges();
        NodeMapping mapping = VF::subgraphIsoOne(graph, &forbidden);
        while(!mapping.empty()) {
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            graph->flip(foundEdge);
            mapping = VF::subgraphIsoOne(graph, &forbidden);
        }
    }
}
void StateGrowReduce3::extend(MGraph *graph)
{
    clog << "extend" << endl;
    if(timeLeft() < 1.5) return;

    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
        if(timeLeft() < 1.5) break;
    }
}

void StateGrowReduce3::final()
{
}
