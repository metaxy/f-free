#include "state_approx2k.h"


StateApprox2K::StateApprox2K()
{
}

MergeGraph StateApprox2K::solve(MergeGraph graph)
{
    for(Edge e : graph.connectedEdges()) {
        if(graph.mergeCost(e) == 0)
            graph.merge(e);
    }

    float clique = 1.0;
    float cutting = 1.0;
    float size = 1;
    /*while(graph.findP3() != P3(0,0,0)) {
        bool someone_merged = false;
        for(NodeT u : graph.nodes()) {
            if(graph.fast2K(u, clique, cutting, size, m_2k_diff)) {
                set<NodeT> n = graph.closedNeighborhood(u);
                for(NodeT a : n) {
                    for(NodeT b : n) {
                        if(a == b || graph.isDeleted(a) || graph.isDeleted(b)) continue;
                        graph.merge(Edge(a,b));
                        someone_merged = true;
                    }
                }
            }
        }
        clique = 0.95*clique - 0.01;
        cutting = 0.95*cutting - 0.95;
        if(clique < 0) clique = 0;
        if(cutting < 0) cutting = 0;
        if(size < 0) size = 0;
    }*/

    float a = 1.0;
    while(graph.findP3() != P3(0,0,0)) {
       for(NodeT u : graph.nodes()) {
           set<NodeT> n = graph.neighborhood(u);
           if(2*a*graph.costMakingClique(u) + a*graph.costCutting(u) < n.size()) {
               for(NodeT a : n) {
                   for(NodeT b : n) {
                       if(a == b || graph.isDeleted(a) || graph.isDeleted(b)) continue;
                       graph.merge(Edge(a,b));
                   }
               }
           }
       }
       a = 0.99*a - 0.1;
    }
    return graph;
}
