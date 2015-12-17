#include "state_grow_reduce2.h"
#include "vf.h"
#include "randomize.h"
#include "forbidden.h"
#include <math.h>
StateGrowReduce2::StateGrowReduce2(Config conf) : State(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
}
MGraph StateGrowReduce2::solve()
{
    //clog << "solve" << endl;
    MGraph graph(m_input);
    graph.clear();
    for(int i = 0; i < 20; i++) {
        clog << "iteration " << i << "----------------" <<endl;
        //int sim = this->growIsomorph(&graph);
        /*if(sim == 0 || simBefore == sim) {
            this->grow(&graph, r->randomElement(m_input.nodes()));
        }*/
        simBefore = sim;
        this->reduce(&graph);
        this->extend(&graph);
        if(m_input.difference(&graph).size() == 0) break;
    }
    this->extend(&graph);
    return graph;
}

bool StateGrowReduce2::isValid(MGraph *input)
{
    return !VF::subgraphIsoHasOne(input, m_forbidden);
}
void StateGrowReduce2::grow(MGraph *graph, NodeT node)
{
    clog << "grow" << endl;
    set<NodeT> neighborhood = m_input.neighborhood(node);
    for(NodeT n: neighborhood) {
        Edge e(n, node);
        graph->setWeight(e, m_input.getWeight(e));
    }
}
int StateGrowReduce2::growIsomorph(MGraph *graph)
{
    clog << "grow isomorhph" << endl;
    vector<Edge> diff = m_input.difference(graph);
    map<Edge, int> modified;
    int count1 = 0;
    int count0 = 0;
    int sizeBefore = m_input.difference(graph).size();
    for(auto forbidden : m_forbidden) {
        vector<MGraph> solutions = Forbidden::posibleSolutions(forbidden);
        for(const MGraph solution : solutions) {
            vector<NodeMapping> mappings = VF::subgraphMono(&m_input, &solution, 100);
            for(const NodeMapping mapping: mappings) {
                int sim = Forbidden::similarity(mapping, &forbidden, diff);
                if(sim < 1) {
                    Edge e = Common::transformEdge(r->randomElement(forbidden.edges()), &mapping);
                    bool mod = modified.find(e) != modified.end();
                    if(!mod) {
                        graph->flip(e);
                        modified[e] = 1;
                    }
                }
                if(sim == 0)
                    count0++;
                if(sim == 1)
                    count1++;
            }
        }
    }
    //clog << "after: " << sizeBefore - m_input.difference(graph).size() << endl;
    clog << "1: " << count1 << " 0: " << count0 << endl;
    return count1;
}

void StateGrowReduce2::reduce(MGraph *graph)
{
    for(auto forbidden : m_forbidden) {
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
void StateGrowReduce2::extend(MGraph *graph)
{
    for(Edge e : m_input.difference(graph)) {
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
    }
}

void StateGrowReduce2::final()
{
}
