/**
 * @class Graph
 * @file Graph.cpp
 * @brief [Description of file]
 *
 * @author Filip Dabkowski
 * @date 30/04/2024
 */
#include "Graph.h"

#include "PriorityQueue.h"
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

	for (auto& [vertex, weight]: getNeighbours(current)) {

		if (get(dfsDiscovery, vertex) != 0) {
			updateLowpoints(current, vertex, lowPoints1, lowPoints2, dfsDiscovery, false);
		} else {
			lowPointDfs(vertex, dfsCounter, dfsDiscovery, lowPoints1, lowPoints2);
			updateLowpoints(current, vertex, lowPoints1, lowPoints2, dfsDiscovery, true);
		}

		// is a frond
		if (get(dfsDiscovery, vertex) < get(dfsDiscovery, current)) {
			weight = 2 * get(dfsDiscovery, vertex);
		} else if (get(lowPoints2, vertex) == get(dfsDiscovery, current)) {
			weight = 2 * get(lowPoints1, vertex);
		} else if (get(lowPoints2, vertex) < get(dfsDiscovery, current)) {
			weight = 2 * get(lowPoints1, vertex) + 1;
		} else {
			weight = get(dfsDiscovery, vertex);
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

void Graph::dfs(vertex_t current, bool* visited, int& dfsCount) {
	dfsCount++;
	set(visited, current, true);
	for (auto [vertex, _]: getNeighbours(current)) {
		if (!get(visited, vertex)) {
			dfs(vertex, visited, dfsCount);
		}
	}
}
void Graph::eccenricityBfs(vertex_t startingPoint, int* eccentricity, int currentComponentIndex) {
	int vertexIndex = 0;
	t_componentsVertices[currentComponentIndex][0] = startingPoint;

	bool firstIteration = true;
	int componentConsistencyNumber = (int) t_componentsVertices[currentComponentIndex].getCapacity();
	while (vertexIndex < componentConsistencyNumber) {
		auto* distance = new int[t_numberVertices];
		for (int i = 0; i < t_numberVertices; ++i) {
			distance[i] = -1;
		}

		dst::Vector<vertex_t> bfsQueue(componentConsistencyNumber);
		int bfsIndex = 0;

		vertex_t currentComponentVertex = t_componentsVertices[currentComponentIndex][vertexIndex];
		bfsQueue[0] = currentComponentVertex;
		set(distance, currentComponentVertex, 0);

		int maxDistance = -1;
		int insertedVerticesCount = 1;
		while (insertedVerticesCount < componentConsistencyNumber) {
			vertex_t current = bfsQueue[bfsIndex++];

			int currentDistance = get(distance, current);
			if (currentDistance > maxDistance) {
				maxDistance = currentDistance;
			}

			for (auto [vertex, _]: getNeighbours(current)) {
				if (get(distance, vertex) == -1) {
					set(distance, vertex, currentDistance + 1);
					bfsQueue[insertedVerticesCount++] = vertex;

					if (firstIteration) {
						t_componentsVertices[currentComponentIndex][insertedVerticesCount - 1] = vertex;
					}
				}
			}
		}

		set(eccentricity, currentComponentVertex, maxDistance + 1);

		vertexIndex++;

		delete[] distance;
		firstIteration = false;
	}
}
void Graph::colorVertex(vertex_t current, int* colors, bool* colorsUsed) {
	int maxColorUsed = 0;

	for (auto& [vertex, _]: getNeighbours(current)) {

		if (int neighbourColor = get(colors, vertex); neighbourColor != 0) {
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
	set(colors, current, minColorPicked);
}

int Graph::numberOfComponents() {
	// t_discoveryTime = new int[t_numberVertices];
	// t_lowPoints1 = new int[t_numberVertices];
	// t_lowPoints2 = new int[t_numberVertices];

	auto* visited = new bool[t_numberVertices];
	for (int i = 0; i < t_numberVertices; ++i) {
		visited[i] = false;
	}

	for (int i = 0; i < t_numberVertices; ++i) {
		if (!visited[i]) {
			int dfsCount = 0;
			dfs(i + 1, visited, dfsCount);
			t_componnets.push({i + 1, dfsCount});
			// lowPointDfs(i + 1, dfsCount, t_discoveryTime, t_lowPoints1, t_lowPoints2);
		}
	}

	t_componentsVertices = new dst::Vector<vertex_t>[t_componnets.getSize()];

	delete[] visited;
	return t_componnets.getSize();
}

bool Graph::bipartiteDfs(vertex_t current, int previousColor) {
	int newColor = previousColor % 2 + 1;
	set(t_colours, current, newColor);

	for (auto [vertex, _]: getNeighbours(current)) {
		if (int neighbourColor = get(t_colours, vertex); neighbourColor == 0) {
			if (!bipartiteDfs(vertex, newColor)) {
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
	for (auto componnet: t_componnets) {
		bipartite = bipartiteDfs(componnet.vertex, 2);
		if (!bipartite) {
			break;
		}
	}

	return bipartite;
}
void Graph::vertexEccentricity() {
	auto* verticesEccentricity = new int[t_numberVertices];

	int currentComponentIndex = 0;
	for (auto [vertex, componentSize]: t_componnets) {
		t_componentsVertices[currentComponentIndex].resize(componentSize);
		if (componentSize == 1) {
			set(verticesEccentricity, vertex, 0);
			t_componentsVertices[currentComponentIndex].pushBack(vertex);
		} else if(componentSize == 2) {
			set(verticesEccentricity, vertex, 1);
			set(verticesEccentricity, getNeighbours(vertex)[0].vertex, 1);
			t_componentsVertices[currentComponentIndex].pushBack(vertex);
			t_componentsVertices[currentComponentIndex].pushBack(getNeighbours(vertex)[0].vertex);
		} else {
			eccenricityBfs(vertex, verticesEccentricity, currentComponentIndex);
		}
		currentComponentIndex++;
	}
	for (int i = 0; i < t_numberVertices; ++i) {
		printf("%d ", verticesEccentricity[i]);
	}
	printf("\n");
}
void Graph::vertexColorsGreedy() {
	resetColours();

	auto* colorsUsed = new bool[t_numberVertices];
	for (int i = 0; i < t_numberVertices; ++i) {
		if (vertex_t current = i + 1; get(t_colours, current) == 0) {
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
	int currentBucket = nBuckets - 1;

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
}


bool Graph::slfStructCompare(const SaturationInfo& a, const SaturationInfo& b) {
	if (*a.saturationValue_p > *b.saturationValue_p) {
		return true;
	}
	if (*a.saturationValue_p < *b.saturationValue_p) {
		return false;
	}
	return a.degree > b.degree || (a.degree == b.degree && a.vertex < b.vertex);
}

void Graph::vertexColorsSLF() {
	auto* saturations = new int[t_numberVertices];
	auto* queueIndexTable = new int[t_numberVertices];

	for (VertexInfo componnet: t_componnets) {
		dst::PriorityQueue<SaturationInfo>(componnet.info, slfStructCompare);
	}

	delete[] queueIndexTable;
	delete[] saturations;
}

long long int Graph::countOfC4() {

	long long int c4Count = 0;

	for (int i = 0; i < t_componnets.getSize(); ++i) {
		int componentSize = (int) t_componentsVertices[i].getCapacity();
		if (componentSize < 4) {
			continue;
		}
		for (int v = 0; v < componentSize; ++v) {
			int degreeV = getDegree(v + 1);
			for (int u = v+1; u < componentSize; ++u) {
				int degreeU = getDegree(u + 1);
				int indexU = 0;
				int indexV = 0;

				long long int neighbourCount = 0;
				while (indexV < degreeV && indexU < degreeU) {
					int neighbourOfV = t_adjancencyList[v][indexV].vertex;
					int neighbourOfU = t_adjancencyList[u][indexU].vertex;
					if (neighbourOfV == neighbourOfU) {
						neighbourCount++;
						indexV++;
						indexU++;
					} else if (neighbourOfV < neighbourOfU) {
						indexV++;
					} else {
						indexU++;
					}
				}

				c4Count += neighbourCount * (neighbourCount - 1) / 2;
			}
		}
	}

	c4Count /= 2;

	// for (int i = 0; i < t_numberVertices; ++i) {
	// 	for (int j = i + 1; j < t_numberVertices; ++j) {
	// 		int degreeV = getDegree(i + 1);
	// 		int indexV = 0;
	// 		int degreeU = getDegree(j + 1);
	// 		int indexU = 0;
	//
	// 		long neighbourCount = 0;
	// 		while (indexV < degreeV && indexU < degreeU) {
	// 			int neighbourOfV = t_adjancencyList[i][indexV].vertex;
	// 			int neighbourOfU = t_adjancencyList[j][indexU].vertex;
	// 			if (neighbourOfV == neighbourOfU) {
	// 				neighbourCount++;
	// 				indexV++;
	// 				indexU++;
	// 			} else if (neighbourOfV < neighbourOfU) {
	// 				indexV++;
	// 			} else {
	// 				indexU++;
	// 			}
	// 		}
	//
	// 		c4Count += neighbourCount * (neighbourCount - 1) / 2;
	// 	}
	// }
	// c4Count /= 2;


	// c4Count = 0;
	// int* countTable = new int[t_numberVertices];
	// for (int i = 0; i < t_numberVertices; ++i) {
	// 	countTable[i] = 0;
	// }
	//
	// for (int i = 0; i < t_numberVertices; ++i) {
	// 	int degreeV = (int) t_adjancencyList[i].getSize();
	//
	// 	for (auto neighbour: getNeighbours(i + 1)) {
	// 		int degreeU = getDegree(neighbour.vertex);
	// 		if (degreeU < degreeV || (degreeU == degreeV && neighbour.vertex < i + 1)) {
	// 			for (auto secondNeighbour: getNeighbours(neighbour.vertex)) {
	// 				int degreeW = getDegree(secondNeighbour.vertex);
	// 				if (degreeW < degreeV || (degreeW == degreeV && secondNeighbour.vertex < i + 1)) {
	// 					int countForVertex = get(countTable, secondNeighbour.vertex);
	// 					c4Count += countForVertex;
	// 					set(countTable, secondNeighbour.vertex, countForVertex + 1);
	// 				}
	// 			}
	// 		}
	// 	}
	// 	for (auto neighbour: getNeighbours(i + 1)) {
	// 		int degreeU = getDegree(neighbour.vertex);
	// 		if (degreeU < degreeV || (degreeU == degreeV && neighbour.vertex < i + 1)) {
	// 			for (auto secondNeighbour: getNeighbours(neighbour.vertex)) {
	// 				set(countTable, secondNeighbour.vertex, 0);
	// 			}
	// 		}
	// 	}
	// }
	return c4Count;
}
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
		for (auto [vertex, info]: t_adjancencyList[i]) {
			printf(" %d: %d, ", vertex, info);
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
