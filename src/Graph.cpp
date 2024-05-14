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


void set(int* inTable, vertex_t forVertex, int toValue) {
	inTable[forVertex - 1] = toValue;
}

int get(const int* inTable, vertex_t ofVertex) {
	return inTable[ofVertex - 1];
}

void updateLowpoints(vertex_t currectVertex, vertex_t neighbourVertex, const int* neighbourValues, int* lowPoints1, int* lowPoints2) {
	if (neighbourValues[neighbourVertex - 1] < lowPoints1[currectVertex - 1]) {
		lowPoints2[currectVertex - 1] = lowPoints1[currectVertex - 1];
		lowPoints1[currectVertex - 1] = neighbourValues[neighbourVertex - 1];
	} else if (neighbourValues[neighbourVertex - 1] < lowPoints2[currectVertex - 1]) {
		lowPoints2[currectVertex - 1] = neighbourValues[neighbourVertex - 1];
	}
}

void Graph::lowPointDFS(vertex_t current, vertex_t ancestor, int& dfsNumber, int* dfsDiscovery, int* lowPoints1,
						int* lowPoints2, int* branchPoints) {
	dfsNumber++;
	set(dfsDiscovery, current, dfsNumber);
	set(lowPoints1, current, dfsNumber);
	set(lowPoints2, current, dfsNumber);

	for (auto neighbour: getNeighbours(current)) {
		// if (firstIteration) {
		// 	if (ancestor!=0) {
		// 		set(branchPoints, neighbour, get(branchPoints, ancestor));
		// 	} else {
		// 		set(branchPoints, neighbour, 0);
		// 	}
		// } else {
		// 	set(branchPoints, neighbour, current);
		// }

		if (get(dfsDiscovery, neighbour) != 0) {
			updateLowpoints(current, neighbour, dfsDiscovery, lowPoints1, lowPoints2);
			continue;
		}

		lowPointDFS(neighbour, current, dfsNumber, dfsDiscovery, lowPoints1, lowPoints2, branchPoints);
		updateLowpoints(current, neighbour, lowPoints1, lowPoints1, lowPoints2);
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
	return false;
}
