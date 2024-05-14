/**
 * Created by Filip Dabkowski on 30/04/2024.
 */

#ifndef GRAPH_H
#define GRAPH_H
#define DEBUG

#include "List.h"

using vertex_t = int;


class Graph {
	dst::List<vertex_t>* t_adjancencyList;
	int t_numberVertices;

	void lowPointDFS(vertex_t current, vertex_t ancestor, int& dfsNumber, int* dfsDiscovery, int* lowPoints1,
					 int* lowPoints2, int* branchPoints);

public:
	Graph() : t_adjancencyList(nullptr), t_numberVertices(0) {}

	void setSize(int n) {
		t_numberVertices = n;
		t_adjancencyList = new dst::List<vertex_t>[t_numberVertices];
	}

	void addVertex(int v, int u) { t_adjancencyList[v - 1].insertBack(u); }

	dst::List<vertex_t>& getNeighbours(vertex_t of) { return t_adjancencyList[of - 1]; }

	bool isPlanar();

	void clear() { delete[] t_adjancencyList; }

	~Graph() = default;
};


#endif // GRAPH_H
