#include <sstream>
#include <fstream>
#include "graph.h"

Graph::Graph() : nodeCount(0), firstEdge(NULL), firstNode(NULL) {}


void Graph::reset()
{
	while (firstEdge != NULL)
	{
		GEdge *e = firstEdge->next;
		delete firstEdge;
		firstEdge = e;
	}
	while (firstNode != NULL)
	{
		GNode *n = firstNode->next;
		delete firstNode;
		firstNode = n;
	}
    nodeCount = 0;
}



void Graph::intNodes(int n)
{
	for (int i=0; i<n; i++)
	{
		ostringstream node;
		node << i;
        nodePosition(node.str());
	}
}

int Graph::insertEdge(NodeT u, NodeT v)
{
	ostringstream node1, node2;
	node1 << u;
	node2 << v;
	return insertEdge(node1.str(), node2.str());
}

int Graph::insertEdge(string Node1, string Node2)
{
    int u = nodePosition(Node1), v = nodePosition(Node2);
    if (u == v) return 0;
    GEdge *edge = new GEdge(u, v, firstEdge);
    firstEdge = edge;
    return 1;
}

int Graph::nodePosition(string name)
{
    GNode *node = firstNode;
    for (int i = nodeCount; --i >= 0; node = node->next)
        if (name == node->name) return i;
    node = new GNode(name, firstNode);
    firstNode = node;
    nodeCount ++;
    return nodeCount - 1;
}

string Graph::nodeName(int value)
{
    GNode *node = firstNode;
    if (value >= nodeCount) return "unbekannt";
    while (++value < nodeCount) node = node->next;
    return node->name;
}

int Graph::edgesCount()
{
	int n=0;
	for (GEdge *e=firstEdge; e!=NULL; e=e->next) n++;
	return n;
}

void Graph::output()
{
	for (GEdge *e=firstEdge; e!=NULL; e=e->next)
        cout << nodeName(e->u) << " " << nodeName(e->v) << endl;
}
