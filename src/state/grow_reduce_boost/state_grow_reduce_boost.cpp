#include "state_grow_reduce_boost.h"
#include <algorithm>
StateGrowReduceBoost::StateGrowReduceBoost(Config conf) : BState(conf), m_countIteration(0), m_validChanges(0), m_invalidChanges(0), m_skipBecauseOfWeight(0)
{
}
BoostGraph StateGrowReduceBoost::solve()
{
    BoostGraph graph(m_input);
    graph.clear();
    string sortType = this->getString("sort", "hits");
    string reduceType = this->getString("reduce", "random");
    vector<NodeT> nodes;
    if(sortType == "random") {
        nodes = r->randomVector(m_input.nodes());
    } else if(sortType == "neighbors") {
        nodes = m_input.nodes();
        std::sort (nodes.begin(), nodes.end(), [this](NodeT a, NodeT b){ return m_input.neighborhood(a).size() < m_input.neighborhood(b).size(); });
    } else if(sortType == "neighbors_rev") {
        nodes = m_input.nodes();
        std::sort (nodes.begin(), nodes.end(), [this](NodeT a, NodeT b){ return m_input.neighborhood(b).size() < m_input.neighborhood(a).size(); });
    } else if(sortType == "hits" || sortType == "hits_rev") {
        nodes = m_input.nodes();
        map<NodeT, int> hits;
        for(NodeT n : nodes) {
            hits[n] = 0;
        }

        for(BoostGraph *forbidden : m_forbidden) {
            vector<NodeMapping> n = m_input.subgraphIsoAll(forbidden);
            for(NodeMapping mapping : n) {
                for(const auto &m : mapping) {
                     hits[m.second] = hits[m.second] + 1;
                }
            }
        }
        if(sortType == "hits") {
            std::sort (nodes.begin(), nodes.end(), [hits](NodeT a, NodeT b){ return hits.at(a) < hits.at(b); });
        } else {
            std::sort (nodes.begin(), nodes.end(), [hits](NodeT a, NodeT b){ return hits.at(b) < hits.at(a); });
        }
    }  else {
        clog << "false sortType" << endl;
        exit(-1);
    }
    set<NodeT> explored;
    map<Edge,int> modified;
    clog << nodes.size() << endl;
    for(NodeT node: nodes) {
        explored.insert(node);

        //grow phase
        set<NodeT> neighborhood = m_input.neighborhood(node);
        BoostGraph explore(graph);

        clog << neighborhood.size() << endl;
        for(NodeT n: neighborhood) {
            if(explored.find(node) == explored.end())
                continue;
            Edge e(n, node);
            if(modified.find(e) == modified.end()) {
                explore.setConnected(e, m_input.connected(e));
            }
        }
        if(reduceType == "random") {
            while(!isValid(&explore)) {
                for(BoostGraph *forbidden : r->randomVector(m_forbidden)) {
                    for(int i = 0; i< 1000; i++) {
                        NodeMapping mapping = explore.subgraphIsoOne(forbidden);
                        if(mapping.empty()) {
                             break;
                        }

                        for(int i = 0; i < forbidden->allEdges().size(); i++) {
                            Edge e = Common::transformEdge(r->randomElement(forbidden->allEdges()), &mapping);
                            if(modified.find(e) == modified.end()) {
                                explore.flip(e);
                                modified[e] = 1;
                                break;
                            }
                        }
                    }
                }
                this->reduceByCount(&explore);
             }
            if(!isValid(&explore)) {
                for(Edge e : graph.difference(&explore)) {
                    explore.flip(e);
                    if(isValid(&explore)) break;
                }
            }
            graph = explore;
        } else if(reduceType == "count") {
            while(!isValid(&explore))
                reduceByCount(&explore);
            graph = explore;
        } else {
            clog << "false reducetype" << endl;
            exit(-1);
        }
        /*clog << "modified " << modified.size() << endl;
        for(BoostGraph *forbidden : m_forbidden) {
            clog << "isomorhisms: " << graph.subgraphIsoCountAll(forbidden) << endl;
        }*/
        if(timeLeft() < 2)
            break;
    }

    if(timeLeft() > 1) {
        this->extend(&graph);
    }
    return graph;
}

void StateGrowReduceBoost::extend(BoostGraph *graph)
{
    clog << "extend" << endl;
    for(Edge e : m_input.difference(graph)) {
        if(timeLeft() < 0.1) return;
        graph->flip(e);
        if(!isValid(graph))
            graph->flip(e);
    }
}
void StateGrowReduceBoost::reduceByCount(BoostGraph *explore)
{
    for(auto forbidden :  r->randomVector(m_forbidden)) {
        vector<Edge> forbiddenEdges = forbidden->allEdges();
        NodeMapping mapping = explore->subgraphIsoOne(forbidden);
        while(!mapping.empty()) {
            Edge foundEdge = Common::transformEdge(r->randomElement(forbiddenEdges), &mapping);
            int size_before = explore->subgraphIsoCountAll(forbidden);//this is very time expensive
            explore->flip(foundEdge);
            if(explore->subgraphIsoCountAll(forbidden) >=  size_before) {
                explore->flip(foundEdge);
            }
            mapping = explore->subgraphIsoOne(forbidden);
        }
    }
    return;
}
void StateGrowReduceBoost::final()
{
}
