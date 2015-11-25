import networkx as nx 
import tools as tools

for n in range(10,150, 10):
  for m in range(1,8,2):
    tools.save(nx.barabasi_albert_graph(n,m), "n_"+str(n)+"_m_"+str(m)+".txt")