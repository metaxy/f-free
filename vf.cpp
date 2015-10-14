#include "vf.h"
#include "argedit.h"
#include "vf2_sub_state.h"
#include <match.h>
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
    for(const Edge e: input->edges()) {
        ed.InsertEdge(e.first, e.second, NULL);
    }
    Graph g(&ed);
    return g;
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
        return ret;
    }
    for(int i=0; i<n; i++) {
        ret.push_back(Edge(ni1[i], ni2[i]));
    }
}
bool subgraphIsoAllVisitor(int n, node_id ni1[], node_id ni2[], void *usr_data)
{
    vector<Subgraph> *subgraphIsoData = (vector<Subgraph> *) usr_data;
    Subgraph ret;
    for(int i=0; i<n; i++) {
        ret.push_back(Edge(ni1[i], ni2[i]));
    }
    subgraphIsoData->push_back(ret);
    // Return false to search for the next matching
    return false;
}

vector<Subgraph> VF::subgraphIsoAll(MGraph *haystack, MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    vector<Subgraph> subgraphIsoData;
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);
    return subgraphIsoData;
}
