/**
 * @class Graph
 * @file Graph.cpp
 * @brief [Description of file]
 *
 * @author Filip Dabkowski
 * @date 30/04/2024
 */
#include "Graph.h"
#include "Utilities.h"


void Graph::lowPointDFS(int current, int ancestor, bool* visited, int& dfsNumber, int* lowPoints1) {
	visited[current - 1] = true;
	dfsNumber++;

	lowPoints1[current - 1] = dfsNumber;

	for (auto neighbour: t_adjancencyList[current - 1]) {
		if (neighbour == ancestor) {
			continue;
		}
		if (visited[neighbour - 1]) {
			lowPoints1[current - 1] = min(lowPoints1[neighbour - 1], lowPoints1[current - 1]);
			continue;
		}
		lowPointDFS(neighbour, current, visited, dfsNumber, lowPoints1);
		lowPoints1[current - 1] = min(lowPoints1[neighbour - 1], lowPoints1[current - 1]);

	}
}

bool Graph::isPlanar() {
	int dfsCount = 0;
	auto* visitedVertices = new bool[t_numberVertices];
	int* lowPoints1 = new int[t_numberVertices];

	for (int i = 0; i < t_numberVertices; ++i) {
		if (!visitedVertices[i]) {
			lowPointDFS(i + 1, 0, visitedVertices, dfsCount, lowPoints1);
		}
	}

	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d", lowPoints1[i]);
	}
	return false;
}
