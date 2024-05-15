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


void set(int* inTable, vertex_t forVertex, int toValue) { inTable[forVertex - 1] = toValue; }

int get(const int* inTable, vertex_t ofVertex) { return inTable[ofVertex - 1]; }

void updateLowpoints(vertex_t currect, vertex_t neighbour, int* lowPoints1, int* lowPoints2, const int* dfsValues,
					 bool useLowPoints) {
	int currentLow1 = lowPoints1[currect - 1];
	int currentLow2 = lowPoints2[currect - 1];
	if (useLowPoints && lowPoints1[neighbour - 1] != lowPoints2[neighbour - 1]) {
		if (lowPoints1[neighbour - 1] < currentLow1) {
			lowPoints2[currect - 1] = currentLow1;
			lowPoints1[currect - 1] = lowPoints1[neighbour - 1];
			// here current 1 since we assignted this value in prev line
			if (lowPoints2[neighbour - 1] < currentLow1) {
				lowPoints2[currect - 1] = lowPoints2[neighbour - 1];
			}
		} else if (lowPoints2[neighbour - 1] < currentLow2) {
			lowPoints2[currect - 1] = lowPoints2[neighbour - 1];
		}
	} else {
		int newValue;
		if (useLowPoints) {
			newValue = lowPoints1[neighbour - 1];
		} else {
			newValue = dfsValues[neighbour - 1];
		}
		if (newValue < currentLow1) {
			lowPoints2[currect - 1] = currentLow1;
			lowPoints1[currect - 1] = newValue;
		} else if (newValue < currentLow2 && newValue != currentLow1) {
			lowPoints2[currect - 1] = newValue;
		}
	}
}

// if (firstIteration) {
// 	if (ancestor!=0) {
// 		set(branchPoints, neighbour, get(branchPoints, ancestor));
// 	} else {
// 		set(branchPoints, neighbour, 0);
// 	}
// } else {
// 	set(branchPoints, neighbour, current);
// }

void Graph::lowPointDfs(vertex_t current, int& dfsCounter, int* dfsDiscovery, int* lowPoints1, int* lowPoints2) {
	dfsCounter++;
	set(dfsDiscovery, current, dfsCounter);
	set(lowPoints1, current, dfsCounter);
	set(lowPoints2, current, dfsCounter);

	for (Edge& neighbour: getNeighbours(current)) {

		if (get(dfsDiscovery, neighbour.vertex) != 0) {
			updateLowpoints(current, neighbour.vertex, lowPoints1, lowPoints2, dfsDiscovery, false);
		} else {
			lowPointDfs(neighbour.vertex, dfsCounter, dfsDiscovery, lowPoints1, lowPoints2);
			updateLowpoints(current, neighbour.vertex, lowPoints1, lowPoints2, dfsDiscovery, true);
		}

		// is a frond
		if (get(dfsDiscovery, neighbour.vertex) < get(dfsDiscovery, current)) {
			neighbour.info = 2 * get(dfsDiscovery, neighbour.vertex);
		} else if (get(lowPoints2, neighbour.vertex) == get(dfsDiscovery, current)) {
			neighbour.info = 2 * get(lowPoints1, neighbour.vertex);
		} else if (get(lowPoints2, neighbour.vertex) < get(dfsDiscovery, current)) {
			neighbour.info = 2 * get(lowPoints1, neighbour.vertex) + 1;
		} else {
			neighbour.info = get(dfsDiscovery, neighbour.vertex);
		}
	}
}

bool Graph::isPlanar() {

	printf("D:  ");
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", t_discoveryTime[i]);
	}
	printf("\n");

	printf("L1: ");
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", t_lowPoints1[i]);
	}
	printf("\n");

	printf("L2: ");
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", t_lowPoints2[i]);
	}
	printf("\n");

	print();

	return false;
}

int Graph::numberOfComponents() {
	int dfsCount = 0;

	t_discoveryTime = new int[t_numberVertices];
	t_lowPoints1 = new int[t_numberVertices];
	t_lowPoints2 = new int[t_numberVertices];

	for (int i = 0; i < t_numberVertices; ++i) {
		if (!t_discoveryTime[i]) {
			t_componnets.push(i + 1);
			lowPointDfs(i + 1, dfsCount, t_discoveryTime, t_lowPoints1, t_lowPoints2);
		}
	}
	return t_componnets.getSize();
}

bool Graph::bipartiteDfs(int* colours, vertex_t current, int previousColor) {
	int newColor = previousColor % 2 + 1;
	set(colours, current, newColor);

	for (Edge neighbour: getNeighbours(current)) {
		int neighbourColor = get(colours, neighbour.vertex);
		if (neighbourColor == 0) {
			if (!bipartiteDfs(colours, neighbour.vertex, newColor)) {
				return false;
			}
		} else if (neighbourColor == newColor) {
			return false;
		}
	}
	return true;
}

bool Graph::isBipartite() {
	auto* vertexColoUrs = new int[t_numberVertices];
	for (int i = 0; i < t_numberVertices; ++i) {
		vertexColoUrs[i] = 0;
	}

	bool bipartite = true;
	for (vertex_t componnet: t_componnets) {
		bipartite = bipartiteDfs(vertexColoUrs, componnet, 2);
		if (!bipartite) {
			break;
		}
	}

	delete[] vertexColoUrs;
	return bipartite;
}
void Graph::vertexEccentricity() {}
void Graph::vertexColorsGreedy() {}
void Graph::vertexColorsLF() {}
void Graph::vertexColorsSLF() {}
int Graph::countOfC4() { return 0; }
int Graph::complementEdges() { return 0; }

void Graph::print() {
	for (int i = 0; i < t_numberVertices; ++i) {
		for (Edge vertexInfo: t_adjancencyList[i]) {
			printf(" %d: %d, ", vertexInfo.vertex, vertexInfo.info);
		}
		printf("\n");
	}
}

void Graph::printDegSequence() const {
	for (int i = t_numberVertices; i >= 0; --i) {
		for (int j = 0; j < t_degSequence[i]; ++j) {
			printf("%d ", i);
		}
	}
	printf("\n");
}
