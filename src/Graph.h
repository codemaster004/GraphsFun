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

	void lowPointDFS(int current, int ancestor, bool* visited, int& dfsNumber, int* lowPoints1);

public:
	Graph() : t_adjancencyList(nullptr) {}

	void setSize(int n) {
		t_numberVertices = n;
		t_adjancencyList = new dst::List<vertex_t>[t_numberVertices];
	}

	void addVertex(int v, int u) { t_adjancencyList[v - 1].insertBack(u); }

	bool isPlanar();

	void clear() { delete[] t_adjancencyList; }

	~Graph() = default;
};


#endif // GRAPH_H
