#include "src/iso/vf.h"
#include "argedit.h"
#include "vf2_sub_state.h"
#include "vf2_mono_state.h"
#include "ull_sub_state.h"
#include "match.h"
#include "reducednodemapping.h"
#define MAXNODES 50


VF::VF()
{

}

Graph VF::createGraph(const MGraph *input)
{
    ARGEdit ed;  // The object used to create the graph
    for(int i = 0; i < input->nodeCount(); i++) {
        ed.InsertNode(NULL);
    }
    for(const Edge e: input->connectedEdges()) {
        ed.InsertEdge(e.first, e.second, NULL);
        ed.InsertEdge(e.second, e.first, NULL);
    }
    Graph g(&ed);
    return g;
}
Subgraph VF::createSubgraph(const MGraph *needle, map<NodeT, NodeT> mapping)
{
    Subgraph a = needle->connectedEdges();
    Subgraph ret;
    for(Edge e : a) {
        Edge newEdge(mapping[e.first], mapping[e.second]);
        ret.push_back(newEdge);
    }
    return ret;
}
NodeMapping VF::subgraphIsoOne(const MGraph *haystack, const MGraph *needle)
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
bool VF::subgraphIsoHasOne(const MGraph *haystack, vector<MGraph> needle)
{
     Graph big = VF::createGraph(haystack);
     for(MGraph n : needle) {
         Graph small = VF::createGraph(&n);
         UllSubState s0(&small, &big);
         int count;
         node_id ni1[MAXNODES], ni2[MAXNODES];
         if (match(&s0, &count, ni1, ni2)) {
             return true;
         }
     }
     return false;
}


bool subgraphIsoAllVisitor(int n, node_id ni1[], node_id ni2[], void *usr_data)
{
    ReducedNodeMapping *subgraphIsoData = (ReducedNodeMapping *) usr_data;
    NodeMapping mapping;
    for(int i=0; i<n; i++) {
        mapping[ni1[i]] = ni2[i];
    }
    return subgraphIsoData->add(mapping);
}


bool subgraphIsoCountAllVisitor(int n, node_id ni1[], node_id ni2[], void *usr_data)
{
    int* count = (int*) usr_data;
    (*count)++;
    return false;
}

vector<NodeMapping> VF::subgraphIsoAll(const MGraph *haystack, const MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    ReducedNodeMapping subgraphIsoData;
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);
    return subgraphIsoData.get();
}

vector<NodeMapping> VF::subgraphIso(const MGraph *haystack, const MGraph *needle, int count)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    ReducedNodeMapping subgraphIsoData(count);
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);
    return subgraphIsoData.get();
}

int VF::subgraphIsoCountAll(const MGraph *haystack, const MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    int count = 0;
    match(&s0, subgraphIsoCountAllVisitor, &count);
    return count;
}

vector<NodeMapping> VF::subgraphMono(const MGraph *haystack, const MGraph *needle, int count)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2MonoState s0(&small, &big);
    ReducedNodeMapping subgraphIsoData(count);
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);
    return subgraphIsoData.get();
}


