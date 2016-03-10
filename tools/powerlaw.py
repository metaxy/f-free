import networkx as nx 
import tools as tools
# see https://networkx.github.io/documentation/latest/reference/generators.html
for n in range(10, 160, 5):
      tools.save(nx.random_powerlaw_tree(n, tries=10000),"./model/powerlaw/n_"+str(n)+".txt")