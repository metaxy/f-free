import networkx as nx
import sys
import json

edges = []
for line in sys.stdin:
  if line.startswith("#"):
    continue
  nodes = line.split()
  edge = (nodes[0],nodes[1].strip())
  edges.append(edge)
  
G = nx.Graph(edges)
data = {}
data["nodeCount"] = G.number_of_nodes()
data["edgeCount"] = G.number_of_edges()
data["density"] = nx.density(G)
data["minDegree"] = min(G.degree().values())
data["maxDegree"] = max(G.degree().values())
#data["degeneracy"] = max(nx.core_number(G).values())
data["avgDegree"] = G.number_of_edges()/G.number_of_nodes()
#ccs = nx.clustering(G)
#data["avgClustering"] = sum(ccs.values()) / len(ccs)
#data["minClustering"] = min(ccs.values())
#data["maxClustering"] = max(ccs.values())
#cvit = nx.closeness_vitality(G)
#data["avgClosenessVitality"] = sum(cvit.values()) / len(cvit)
#data["minClosenessVitality"] = min(cvit.values())
#data["maxClosenessVitality"] = max(cvit.values())

print(json.dumps(data, sort_keys=True, indent=4, separators=(',', ': ')))
