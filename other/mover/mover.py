# load networkit
from networkit import *

# Read a graph. Note that multiple formats are supported.
G = readGraph(sys.argv[1], Format.EdgeListSpaceZero)

# Generate edge ids (needed for triangle counting)
G.indexEdges()

print(G.edges())

# because QuasiThresholdEditingLocalMover would return one edit on many graphs which require no edits, because they are already quasi-thresholds graphs
#if(properties.IsQuasiThresholdGraph(G).run().isQuasiThresholdGraph()): 
  #print("#k: 0");
 # exit(0);



# Execute the initialization (in one linear scan, therefore the name)
linEd = community.QuasiThresholdEditingLinear(G).run()

# Execute the actual QTM algorithm initialized with the result the
# initialization. The last parameter determines the max. number of iterations.
qtm = community.QuasiThresholdEditingLocalMover(G, linEd.getParents(), 100).run()

# Get the edited graph
qg = qtm.getQuasiThresholdGraph()

a = G.edges()
b = qg.edges()

for edge in a:
  if not edge in b:
    print(str(edge[0]) + " " + str(edge[1]))
  
for edge in b:
  if not edge in a:
    print(str(edge[0]) + " " + str(edge[1]))
print("#k: " + str(qtm.getNumberOfEdits()))