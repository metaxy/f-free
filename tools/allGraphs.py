import networkx as nx 
import tools as tools

for n in range(2,6,1):
  for m in [2,4,6,8]:
    for p in [2,4,6,8]:
      tools.save(nx.newman_watts_strogatz_graph(n,m, p*0.1),"n_"+str(n)+"_m_"+str(m)+"_p_"+str(p*0.1)+".txt")