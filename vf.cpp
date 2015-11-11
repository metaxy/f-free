#include "vf.h"
#include "argedit.h"
#include "vf2_sub_state.h"
#include "ull_sub_state.h"
#include "match.h"
#include <chrono>
#include <set>
#include <unordered_map>

#define MAXNODES 200

class VFSorted {
public:
    VFSorted() : m_max(-1) {

    }

    VFSorted(int max) : m_max(max) {
        //m_data.reserve(max);
    }

    bool add(NodeMapping mapping) {
        string key;
        for(const auto &m : mapping) {
            key += "."+std::to_string(m.first);
        }
        m_data[key] = mapping;

        if(m_max == -1) {
            return false; //do not stop
        } else {
            if(m_max - 1 == m_data.size())
                return true; // stop, its enought
            return false;
        }
    }

    vector<NodeMapping> get() {
        vector<NodeMapping> ret(m_data.size());
        for(const auto &m : m_data) {
            ret.push_back(m.second);
        }
        return ret;
    }

private:
    int m_max;
    unordered_map<string, NodeMapping> m_data;
};


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
    VFSorted *subgraphIsoData = (VFSorted *) usr_data;
    NodeMapping mapping;
    for(int i=0; i<n; i++) {
        mapping[ni1[i]] = ni2[i];
    }
    return subgraphIsoData->add(mapping);
}

vector<NodeMapping> VF::subgraphIsoAll(MGraph *haystack, MGraph *needle)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    VFSorted subgraphIsoData;
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);
    return subgraphIsoData.get();
}

vector<NodeMapping> VF::subgraphIso(MGraph *haystack, MGraph *needle, int count)
{
    Graph big = VF::createGraph(haystack);
    Graph small = VF::createGraph(needle);
    VF2SubState s0(&small, &big);
    VFSorted subgraphIsoData(count);
    match(&s0, subgraphIsoAllVisitor, &subgraphIsoData);
    return subgraphIsoData.get();
}

