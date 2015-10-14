#include "vf.h"
#include "argedit.h"
#include "vf2_sub_state.h"
#include "match.h"
#include <chrono>
#define MAXNODES 200
VF::VF()
{

}

Graph VF::createGraph(MGraph *input)
{
    ARGEdit ed;  // The object used to create the graph
    for(int i = 0; i < input->nodeCount(); i++) {
        ed.InsertNode(NULL);
    }
    for(const Edge e: input->connectedEdges()) {
        //clog << e.first << e.second << endl;
        ed.InsertEdge(e.first, e.second, NULL);
        //ed.InsertEdge(e.second, e.first, NULL);
    }
    Graph g(&ed);
    return g;
}
Subgraph VF::createSubgraph(MGraph *needle, map<NodeT, NodeT> mapping)
{
    Subgraph a = needle->connectedEdges();
    Subgraph ret;
    for(Edge e : a) {
        Edge newEdge(mapping[e.first], mapping[e.second]);
        ret.push_back(newEdge);
    }
    return ret;
}
Subgraph VF::subgraphIsoOne(MGraph *haystack, MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    Subgraph ret;
    int n;
    node_id ni1[MAXNODES], ni2[MAXNODES];
    if (!match(&s0, &n, ni1, ni2)) {
        de("no match found");
        return ret;
    }
    map<NodeT, NodeT> mapping;
    for(int i=0; i<n; i++) {
        mapping[ni1[i]] = ni2[i];
    }
    return VF::createSubgraph(needle, mapping);
}
bool subgraphIsoAllVisitor(int n, node_id ni1[], node_id ni2[], void *usr_data)
{
    vector<map<NodeT, NodeT>> *subgraphIsoData = (vector<map<NodeT, NodeT>> *) usr_data;
    Subgraph ret;
    map<NodeT, NodeT> mapping;
    for(int i=0; i<n; i++) {
        mapping[ni1[i]] = ni2[i];
    }
    subgraphIsoData->push_back(mapping);
    // Return false to search for the next matching
    return false;
}

vector<Subgraph> VF::subgraphIsoAll(MGraph *haystack, MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    vector<map<NodeT, NodeT>> subgraphIsoData;
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);

    vector<Subgraph> ret;
    for(map<NodeT, NodeT> mapping : subgraphIsoData) {
        ret.push_back(VF::createSubgraph(needle, mapping));
    }
    return ret;
}
