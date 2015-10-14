#include "vf.h"
#include "argedit.h"
#include "vf2_sub_state.h"
#include "ull_sub_state.h"
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
    //clog << "count edges " << input->connectedEdges().size() << " " << input->edges().size() << endl;
    for(const Edge e: input->connectedEdges()) {
        //clog << e.first << e.second << endl;
        ed.InsertEdge(e.first, e.second, NULL);
        ed.InsertEdge(e.second, e.first, NULL);
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
NodeMapping VF::subgraphIsoOne(MGraph *haystack, MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);

    UllSubState s0(&small, &big);
    NodeMapping mapping;
    int n;
    node_id ni1[MAXNODES], ni2[MAXNODES];
    if (!match(&s0, &n, ni1, ni2)) {
        return mapping;
    }
    for(int i=0; i<n; i++) {
        mapping[ni1[i]] = ni2[i];
    }
    return mapping;
}
bool subgraphIsoAllVisitor(int n, node_id ni1[], node_id ni2[], void *usr_data)
{
    vector<NodeMapping> *subgraphIsoData = (vector<NodeMapping> *) usr_data;
    NodeMapping mapping;
    for(int i=0; i<n; i++) {
        mapping[ni1[i]] = ni2[i];
    }
    subgraphIsoData->push_back(mapping);
    // Return false to search for the next matching
    return false;
}

vector<NodeMapping> VF::subgraphIsoAll(MGraph *haystack, MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    vector<NodeMapping> subgraphIsoData;
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);
    return subgraphIsoData;
}
