#include "vf.h"

#include "argraph.h"
#include "argedit.h"
#include "graph.h"
VF::VF()
{

}

ARGraph VF::createGraph(GGraph *input)
{
    ARGEdit ed;  // The object used to create the graph
    int i,j;
  for(i=0; i<4; i++)
     ed.InsertNode(NULL); // The inserted node will have index i.
                          // NULL stands for no semantic attribute.

   // Insert the edges
   for(i=0; i<4; i++)
     for(j=0; j<4; j++)
       if (i!=j)
             ed.InsertEdge(i, j, NULL); // NULL stands for no sem. attribute.


   // Now the Graph can be constructed...
   ARGraph g(&ed);
}
