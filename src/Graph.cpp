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

	auto* colorUsed = new bool[t_numberVertices];
	for (int i = 0; i < t_numberVertices; ++i) {
		vertex_t current = i + 1;
		if (get(t_colours, current) == 0) {
			int maxColorUsed = 0;

			for (Edge neighbour: getNeighbours(current)) {
				int neighbourColor = get(t_colours, neighbour.vertex);

				if (neighbourColor != 0) {
					colorUsed[neighbourColor - 1] = false;

					if (neighbourColor > maxColorUsed) {
						maxColorUsed = neighbourColor;
					}
				}
			}

			int minColorPicked = 0;
			for (int j = 0; j < maxColorUsed; ++j) {
				if (colorUsed[j] && minColorPicked == 0) {
					minColorPicked = j + 1;
				}
				colorUsed[j] = true;
			}
			if (minColorPicked == 0) {
				minColorPicked = maxColorUsed + 1;
			}
			set(t_colours, current, minColorPicked);
		}
	}

	delete[] colorUsed;
}
void Graph::vertexColorsLF() {
	dst::Queue<vertex_t> vertiveQueque;
	auto* degreeSequence = new int[t_numberVertices];
	dst::List<Edge> vertexDegrees;

	int smallestDegree = t_numberVertices;
	vertex_t leastConnected = 0;
	for (int i = 0; i < t_numberVertices; ++i) {
		int degree = t_adjancencyList[i].getSize();
		degreeSequence[i] = degree;
		if (degree < smallestDegree) {
			smallestDegree = degree;
			leastConnected = i + 1;
		}
	}

	for (int i = 0; i < t_numberVertices; ++i) {
	}
	for (int i = 0; i < t_numberVertices; ++i) {
		vertiveQueque.put(leastConnected);
		set(degreeSequence, leastConnected, 0);
		for (Edge neighbour: getNeighbours(leastConnected)) {
			degreeSequence[neighbour.vertex - 1] -= 1;
		}
	}

	delete[] degreeSequence;
}
void Graph::vertexColorsSLF() {}
int Graph::countOfC4() { return 0; }
int Graph::complementEdges() const {
	int numberOfEdgesForKGraph = t_numberVertices * (t_numberVertices - 1);
	numberOfEdgesForKGraph -= t_degreeSum;
	return numberOfEdgesForKGraph / 2;
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
		for (int j = 0; j < t_degSequence[i]; ++j) {
			printf("%d ", i);
		}
	}
	printf("\n");
}

void Graph::printColours() const {
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", t_colours[i]);
	}
	printf("\n");
}
