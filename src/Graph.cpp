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

void Graph::dfs(vertex_t current, bool* visited) {
	set(visited, current, true);
	for (Edge neighbour: getNeighbours(current)) {
		if (!get(visited, neighbour.vertex)) {
			dfs(neighbour.vertex, visited);
		}
	}
}
void Graph::colorVertex(vertex_t vertex, int* colors, bool* colorsUsed) {
	int maxColorUsed = 0;

	for (Edge neighbour: getNeighbours(vertex)) {
		int neighbourColor = get(colors, neighbour.vertex);

		if (neighbourColor != 0) {
			colorsUsed[neighbourColor - 1] = false;

			if (neighbourColor > maxColorUsed) {
				maxColorUsed = neighbourColor;
			}
		}
	}

	int minColorPicked = 0;
	for (int j = 0; j < maxColorUsed; ++j) {
		if (colorsUsed[j] && minColorPicked == 0) {
			minColorPicked = j + 1;
		}
		colorsUsed[j] = true;
	}
	if (minColorPicked == 0) {
		minColorPicked = maxColorUsed + 1;
	}
	set(colors, vertex, minColorPicked);
}

int Graph::numberOfComponents() {
	int dfsCount = 0;

	// t_discoveryTime = new int[t_numberVertices];
	// t_lowPoints1 = new int[t_numberVertices];
	// t_lowPoints2 = new int[t_numberVertices];

	auto* visited = new bool[t_numberVertices];
	for (int i = 0; i < t_numberVertices; ++i) {
		visited[i] = false;
	}

	for (int i = 0; i < t_numberVertices; ++i) {
		if (!visited[i]) {
			t_componnets.push(i + 1);
			dfs(i + 1, visited);
			// lowPointDfs(i + 1, dfsCount, t_discoveryTime, t_lowPoints1, t_lowPoints2);
		}
	}

	delete[] visited;
	return t_componnets.getSize();
}

bool Graph::bipartiteDfs(vertex_t current, int previousColor) {
	int newColor = previousColor % 2 + 1;
	set(t_colours, current, newColor);

	for (Edge neighbour: getNeighbours(current)) {
		int neighbourColor = get(t_colours, neighbour.vertex);
		if (neighbourColor == 0) {
			if (!bipartiteDfs(neighbour.vertex, newColor)) {
				return false;
			}
		} else if (neighbourColor == newColor) {
			return false;
		}
	}
	return true;
}

bool Graph::isBipartite() {
	resetColours();

	bool bipartite = true;
	for (vertex_t componnet: t_componnets) {
		bipartite = bipartiteDfs(componnet, 2);
		if (!bipartite) {
			break;
		}
	}

	return bipartite;
}
void Graph::vertexEccentricity() {}
void Graph::vertexColorsGreedy() {
	resetColours();

	auto* colorsUsed = new bool[t_numberVertices];
	for (int i = 0; i < t_numberVertices; ++i) {
		vertex_t current = i + 1;
		if (get(t_colours, current) == 0) {
			colorVertex(current, t_colours, colorsUsed);
		}
	}

	delete[] colorsUsed;
}
void Graph::vertexColorsLF() {
	int nBuckets = t_maximumDegree - t_minimumDegree + 1;
	auto* degreesBuckets = new dst::List<vertex_t>[nBuckets];

	for (int i = 0; i < t_numberVertices; ++i) {
		int degree = (int) t_adjancencyList[i].getSize();
		degreesBuckets[degree - t_minimumDegree].push(i + 1);
	}

	int verticesColored = 0;
	int currentBucket = nBuckets-1;

	auto* colorsUsed = new bool[t_maximumDegree + 1];
	while (verticesColored < t_numberVertices) {
		for (vertex_t current: degreesBuckets[currentBucket]) {

			if (get(t_colours, current) == 0) {
				colorVertex(current, t_colours, colorsUsed);
			}

			verticesColored++;
		}
		currentBucket--;
	}

	delete[] degreesBuckets;
	delete[] colorsUsed;
	// int smallestDegree = t_numberVertices;
	// vertex_t leastConnected = 0;
	// for (int i = 0; i < t_numberVertices; ++i) {
	// 	int degree = (int) t_adjancencyList[i].getSize();
	// 	degreeSequence[i] = degree;
	// 	if (degree < smallestDegree) {
	// 		smallestDegree = degree;
	// 		leastConnected = i + 1;
	// 	}
	// }
	//
	// for (int i = 0; i < t_numberVertices; ++i) {
	// }
	// for (int i = 0; i < t_numberVertices; ++i) {
	// 	vertiveQueque.put(leastConnected);
	// 	set(degreeSequence, leastConnected, 0);
	// 	for (Edge neighbour: getNeighbours(leastConnected)) {
	// 		degreeSequence[neighbour.vertex - 1] -= 1;
	// 	}
	// }

	// delete[] degreeSequence;
}
void Graph::vertexColorsSLF() {}
int Graph::countOfC4() { return 0; }
long long int Graph::complementEdges() const {
	long long int numberOfEdgesForKGraph = 0;

	// Safely compute the number of edges in a complete graph with the same number of vertices
	if (t_numberVertices % 2 == 0) {
		// (n / 2) * (n - 1)
		long long int halfVertices = t_numberVertices / 2;
		numberOfEdgesForKGraph = halfVertices * (t_numberVertices - 1);
	} else {
		// ((n - 1) / 2) * n
		long long int halfVerticesMinusOne = (t_numberVertices - 1) / 2;
		numberOfEdgesForKGraph = halfVerticesMinusOne * t_numberVertices;
	}

	// Divide first to prevent overflow
	long long int halfDegreeSum = t_degreeSum / 2;
	// Safely subtract (degree sum / 2)
	numberOfEdgesForKGraph -= halfDegreeSum;

	return numberOfEdgesForKGraph;
}

void Graph::print() const {
	for (int i = 0; i < t_numberVertices; ++i) {
		for (Edge vertexInfo: t_adjancencyList[i]) {
			printf(" %d: %d, ", vertexInfo.vertex, vertexInfo.info);
		}
		printf("\n");
	}
}

void Graph::printDegSequence() const {
	for (int i = t_numberVertices; i >= 0; --i) {
		for (int j = 0; j < t_degreeCounts[i]; ++j) {
			printf("%d ", i);
		}
	}
	printf("\n");
}

void Graph::printColours() const {
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", t_colours[i]);
		t_colours[i] = 0;
	}
	printf("\n");
}
