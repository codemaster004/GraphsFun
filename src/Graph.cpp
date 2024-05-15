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

void updateLowpoints(vertex_t currect, vertex_t neighbour, int* lowPoints1, int* lowPoints2,
					 const int* dfsValues, bool useLowPoints) {
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

void Graph::lowPointDFS(vertex_t current, vertex_t ancestor, int& dfsNumber, int* dfsDiscovery, int* lowPoints1,
						int* lowPoints2, int* branchPoints) {
	dfsNumber++;
	set(dfsDiscovery, current, dfsNumber);
	set(lowPoints1, current, dfsNumber);
	set(lowPoints2, current, dfsNumber);

	for (Node& neighbour: getNeighbours(current)) {

		if (get(dfsDiscovery, neighbour.vertex) != 0) {
			updateLowpoints(current, neighbour.vertex, lowPoints1, lowPoints2, dfsDiscovery, false);
		} else {
			lowPointDFS(neighbour.vertex, current, dfsNumber, dfsDiscovery, lowPoints1, lowPoints2, branchPoints);
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
	int dfsCount = 0;
	auto* discoveryTime = new int[t_numberVertices];
	auto* lowPoints1 = new int[t_numberVertices];
	auto* lowPoints2 = new int[t_numberVertices];
	auto* branchPoints = new int[t_numberVertices];

	for (int i = 0; i < t_numberVertices; ++i) {
		if (!discoveryTime[i]) {
			lowPointDFS(i + 1, 0, dfsCount, discoveryTime, lowPoints1, lowPoints2, branchPoints);
		}
	}

	printf("D:  ");
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", discoveryTime[i]);
	}
	printf("\n");

	printf("L1: ");
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", lowPoints1[i]);
	}
	printf("\n");

	printf("L2: ");
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", lowPoints2[i]);
	}
	printf("\n");

	printf("B:  ");
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", branchPoints[i]);
	}
	printf("\n");

	print();

	return false;
}

void Graph::print() {
	for (int i = 0; i < t_numberVertices; ++i) {
		for (Node vertexInfo: t_adjancencyList[i]) {
			printf(" %d: %d, ", vertexInfo.vertex, vertexInfo.info);
		}
		printf("\n");
	}
}
