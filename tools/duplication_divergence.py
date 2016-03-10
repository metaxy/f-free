import networkx as nx 
import tools as tools
# see https://networkx.github.io/documentation/latest/reference/generators.html
for n in range(10, 100, 10):
    for p in [1,2,2.5,4,5,8]:
      tools.save(nx.duplication_divergence_graph(n, p*0.1),"./model/duplication_divergence/n_"+str(n)+"_p_"+str(p*0.1)+".txt")