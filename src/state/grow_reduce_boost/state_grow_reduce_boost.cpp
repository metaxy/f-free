#include "state_grow_reduce_boost.h"
StateGrowReduceBoost::StateGrowReduceBoost(Config conf) : BState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
    m_factorSize = getDouble("factorSize", 1.0);
}
BoostGraph StateGrowReduceBoost::solve()
{
    BoostGraph graph(m_input);
    graph.clear();

    vector<NodeT> nodes = r->randomVector(m_input.nodes());
    set<NodeT> explored;
    map<Edge,int> modified;
    clog << nodes.size() << endl;
    for(NodeT node: nodes) {
        explored.insert(node);

        //grow phase
        set<NodeT> neighborhood = m_input.neighborhood(node);
        clog << neighborhood.size() << endl;
        for(NodeT n: neighborhood) {
            if(explored.find(node) == explored.end())
                continue;
            Edge e(n, node);
            if(modified.find(e) == modified.end()) {
                graph.setConnected(e, m_input.connected(e));
            }
        }
        for(BoostGraph *forbidden : m_forbidden) {
            for(int i = 0; i < 200; i++) {
                NodeMapping mapping = graph.subgraphIsoOne(forbidden);
                if(mapping.empty()) {
                     break;
                }

                for(int i = 0; i < forbidden->allEdges().size(); i++) {
                    Edge e = Common::transformEdge(r->randomElement(forbidden->allEdges()), &mapping);
                    if(modified.find(e) == modified.end()) {
                        graph.flip(e);
                        modified[e] = 1;
                        break;
                    }
                }
            }
        }
        if(timeLeft() < 2)
            break;

        if(m_input.difference(&graph).size() == 0)
            break;
    }
    this->reduce(&graph);
    this->extend(&graph);
    return graph;
}

void StateGrowReduceBoost::reduce(BoostGraph *graph)
{
    clog << "reduce" << endl;
    map<Edge,int> modified;
    for(BoostGraph *forbidden : m_forbidden) {
        while(true) {
            NodeMapping mapping = graph->subgraphIsoOne(forbidden);
            if(mapping.empty()) {
                 break;
            }

            for(int i = 0; i < forbidden->allEdges().size(); i++) {
                Edge e = Common::transformEdge(r->randomElement(forbidden->allEdges()), &mapping);
                if(modified.find(e) == modified.end()) {
                    graph->flip(e);
                    modified[e] = 1;
                    break;
                }
            }
        }
    }
    clog << "reduce end" << endl;

}
void StateGrowReduceBoost::extend(BoostGraph *graph)
{
    clog << "extend" << endl;
    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
    }
}

void StateGrowReduceBoost::final()
{
}
