#include "boostgraph.h"
#include "reducednodemapping.h"
BoostGraph::BoostGraph()
{
}
BoostGraph::BoostGraph(VGraph *graph)
{
    m_input = graph->m_input;
    for(Edge e: graph->connectedEdges()) {
        addEdge(e);
    }
}
BoostGraph::BoostGraph(GGraph input)
{
    m_input = input;
    for(Edge e: input.edges()) {
        addEdge(e);
    }
}

BoostGraph::~BoostGraph()
{
}
bool BoostGraph::connected(const Edge &e) const
{
    return edge(e.first, e.second, m_graph).second;
}
void BoostGraph::addEdge(const Edge &e)
{
    add_edge(e.first, e.second, m_graph);
}
void BoostGraph::setConnected(const Edge &e, bool connect)
{
    if(connect) {
        if(!this->connected(e))
            add_edge(e.first, e.second, m_graph);
    } else {
        if(this->connected(e))
            remove_edge(e.first, e.second, m_graph);
    }
}

void BoostGraph::flip(const Edge &e)
{
    setConnected(e, !connected(e));
}

void BoostGraph::clear()
{
    for(Edge e: this->connectedEdges()) {
        remove_edge(e.first, e.second, m_graph);
    }
}

vector<NodeT> BoostGraph::nodes() const
{
    vector<NodeT> ret;
    graph_traits<boost_graph_type>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(m_graph); vi != vi_end; ++vi) {
        ret.push_back(*vi);
    }
    return ret;
}
int BoostGraph::nodeCount() const
{
    return this->nodes().size();
}

vector<Edge> BoostGraph::allEdges() const
{
    vector<Edge> ret;
    vector<NodeT> n = this->nodes();
    for(NodeT a : n) {
        for(NodeT b : n) {
            if(a < b) {
                ret.push_back(Edge(a,b));
            }
        }
    }
    return ret;
}

vector<Edge> BoostGraph::connectedEdges() const
{
    vector<Edge> ret;
    graph_traits<boost_graph_type>::edge_iterator i, end;
    for (tie(i, end) = edges(m_graph); i != end; ++i) {
       ret.push_back(Edge(source(*i, m_graph), target(*i, m_graph)));
    }
    return ret;
}

set<NodeT> BoostGraph::neighborhood(NodeT node) const
{
    //todo: faster implemnatation: http://programmingexamples.net/wiki/Boost/BGL/AdjacentVertices
    set<NodeT> ret;
    for(NodeT i : this->nodes()) {
        if(node == i) continue;
        if(connected(Edge(node, i))) ret.insert(i);
    }
    return ret;
}

struct vf2_callback_collect {

  vf2_callback_collect(const boost_graph_type& graph1, const boost_graph_type& graph2, ReducedNodeMapping *mapping) : graph1_(graph1), graph2_(graph2)
  {
    m_map = mapping;
  }

  template <typename CorrespondenceMap1To2,
            typename CorrespondenceMap2To1>
  bool operator() (CorrespondenceMap1To2 f, CorrespondenceMap2To1) const
  {
    NodeMapping nodeMapping;
    BGL_FORALL_VERTICES_T(v, graph1_, boost_graph_type)
        nodeMapping[get(vertex_index_t(), graph1_, v)] = get(vertex_index_t(), graph1_, get(f, v));
    return !m_map->add(nodeMapping);
  }

private:
  const boost_graph_type& graph1_;
  const boost_graph_type& graph2_;
  ReducedNodeMapping *m_map;
};

struct vf2_callback_count {

  vf2_callback_count(const boost_graph_type& graph1, const boost_graph_type& graph2, int *count) : graph1_(graph1), graph2_(graph2)
  {
      m_count = count;
  }
  template <typename CorrespondenceMap1To2,
            typename CorrespondenceMap2To1>
  bool operator() (CorrespondenceMap1To2 f, CorrespondenceMap2To1) const
  {
      (*m_count)++;
      return true;
  }

private:
  const boost_graph_type& graph1_;
  const boost_graph_type& graph2_;
   int *m_count;

};

struct vf2_callback_has {

  vf2_callback_has(const boost_graph_type& graph1, const boost_graph_type& graph2, int *count) : graph1_(graph1), graph2_(graph2)
  {
      m_count = count;
  }
  template <typename CorrespondenceMap1To2,
            typename CorrespondenceMap2To1>
  bool operator() (CorrespondenceMap1To2 f, CorrespondenceMap2To1) const
  {
      (*m_count) = 1;
      return false;
  }

private:
  const boost_graph_type& graph1_;
  const boost_graph_type& graph2_;
   int *m_count;

};
NodeMapping BoostGraph::subgraphIsoOne(BoostGraph *needle) const
{
    ReducedNodeMapping rmap(1);
    vf2_callback_collect callback(m_graph, needle->m_graph, &rmap);
    vf2_subgraph_iso(needle->m_graph, m_graph, callback);
    NodeMapping ret;
    return ret;
}

bool BoostGraph::subgraphIsoHasOne(vector<BoostGraph> needle) const
{
    int count = 0;
    for(BoostGraph g : needle) {
        vf2_callback_has callback(m_graph, g.m_graph, &count);
        vf2_subgraph_iso(m_graph, g.m_graph, callback);
        if(count == 1) return true;
    }
    return false;
}

vector<NodeMapping> BoostGraph::subgraphIsoAll(const BoostGraph *needle) const
{
    ReducedNodeMapping rmap;
    vf2_callback_collect callback(m_graph, needle->m_graph, &rmap);
    vf2_subgraph_iso(needle->m_graph, m_graph, callback);
    return rmap.get();
}

int BoostGraph::subgraphIsoCountAll(const BoostGraph *needle) const
{
    int count = 0;
    vf2_callback_count callback(m_graph, needle->m_graph, &count);
    //vf2_print_callback<boost_graph_type, boost_graph_type> callback(m_graph, needle->m_graph);
    vf2_subgraph_iso(needle->m_graph, m_graph, callback);
    return count;
}
