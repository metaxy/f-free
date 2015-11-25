import networkx as nx 

def save(g, fileName):
  f = open(fileName, 'w+')
  for u,v in nx.edges(g):
    f.write(str(u) + " " + str(v)+"\n")
  f.close()
