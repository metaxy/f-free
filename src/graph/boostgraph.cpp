#include "boostgraph.h"
#include "src/iso/reducednodemapping.h"
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
BoostGraph::BoostGraph(const BoostGraph &graph)
{
    m_input = graph.m_input;
    m_graph = graph.m_graph;
}

BoostGraph::BoostGraph(const BoostGraph *graph)
{
   m_input = graph->m_input;
   m_graph = graph->m_graph;
}

BoostGraph::~BoostGraph()
{
}
bool BoostGraph::connected(const Edge &e) const
{
    return boost::edge(e.first, e.second, m_graph).second == true;
}
void BoostGraph::addEdge(const Edge &e)
{
    boost::add_edge(e.first, e.second, m_graph);
}
void BoostGraph::setConnected(const Edge &e, bool connect)
{
    if(connect) {
        if(!this->connected(e)) {
            boost::add_edge(e.first, e.second, m_graph);
        }
    } else {
        if(this->connected(e)) {
            boost::remove_edge(e.first, e.second, m_graph);
         }
    }
}

void BoostGraph::flip(const Edge &e)
{
    if(!connected(e)) {
        boost::add_edge(e.first, e.second, m_graph);
    } else {
        boost::remove_edge(e.first, e.second, m_graph);
    }
}

void BoostGraph::clear()
{
    for(Edge e: this->connectedEdges()) {
        boost::remove_edge(e.first, e.second, m_graph);
    }
}

vector<NodeT> BoostGraph::nodes() const
{
    vector<NodeT> ret;
    boost::graph_traits<boost_graph_type>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(m_graph); vi != vi_end; ++vi) {
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
    boost::graph_traits<boost_graph_type>::edge_iterator i, end;
    for (boost::tie(i, end) = boost::edges(m_graph); i != end; ++i) {
       ret.push_back(Edge(boost::source(*i, m_graph), boost::target(*i, m_graph)));
    }
    return ret;
}

set<NodeT> BoostGraph::neighborhood(NodeT node) const
{
     set<NodeT> ret;
     typedef boost::graph_traits <boost_graph_type>::adjacency_iterator adjacency_iterator;

     std::pair<adjacency_iterator, adjacency_iterator> neighbors = boost::adjacent_vertices(boost::vertex(node,m_graph), m_graph);

     for(; neighbors.first != neighbors.second; ++neighbors.first){
        ret.insert(*neighbors.first);
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
        nodeMapping[boost::get(boost::vertex_index_t(), graph1_, v)] = boost::get(boost::vertex_index_t(), graph1_, boost::get(f, v));
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
    auto ret = rmap.get();
    if(ret.size() == 0){
        return NodeMapping();
    } else {
        return rmap.get()[0];
    }
}

bool BoostGraph::subgraphIsoHasOne(vector<BoostGraph*> needle) const
{
    for(BoostGraph *g : needle) {
        int count = 0;
        vf2_callback_has callback(g->m_graph, m_graph, &count);
        vf2_subgraph_iso(g->m_graph, m_graph, callback);
        if(count == 1) return true;
    }
    return false;
}

vector<NodeMapping> BoostGraph::subgraphIsoAll(const BoostGraph *needle) const
{
    ReducedNodeMapping rmap;
    vf2_callback_collect callback(needle->m_graph, m_graph,  &rmap);
    vf2_subgraph_iso(needle->m_graph, m_graph, callback);
    return rmap.get();
}

int BoostGraph::subgraphIsoCountAll(const BoostGraph *needle) const
{
    int count = 0;
    vf2_callback_count callback(needle->m_graph, m_graph, &count);
    vf2_subgraph_iso(needle->m_graph, m_graph, callback);
    return count;
}
