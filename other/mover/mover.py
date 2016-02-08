# load networkit
from networkit import *

# Read a graph. Note that multiple formats are supported.
G = readGraph("/home/paul/n_80_m_8_p_0.8.txt", Format.EdgeListSpaceZero)

# Generate edge ids (needed for triangle counting)
G.indexEdges()

# Execute the initialization (in one linear scan, therefore the name)
linEd = community.QuasiThresholdEditingLinear(G).run()

# Execute the actual QTM algorithm initialized with the result the
# initialization. The last parameter determines the max. number of iterations.
qtm = community.QuasiThresholdEditingLocalMover(G, linEd.getParents(), 100).run()

# Get the edited graph
qg = qtm.getQuasiThresholdGraph()

# Write the edited graph
writeGraph(qg, "/home/paul/1.txt", Format.EdgeListSpaceOne)

# Or the number of needed edits:
print(qtm.getNumberOfEdits())

