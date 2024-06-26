/**
 * @class Graph
 * @file Graph.cpp
 * @brief [Description of file]
 *
 * @author Filip Dabkowski
 * @date 30/04/2024
 */
#include "Graph.h"

#include "HashSet.h"
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

void Graph::lowPointDfs(vertex_t current, int& dfsCounter) {
	dfsCounter++;
	// degreeSum += (int) t_adjancencyList[current - 1].getSize();
	set(t_discoveryTime, current, dfsCounter);
	set(t_lowPoints1, current, dfsCounter);
	set(t_lowPoints2, current, dfsCounter);

	for (auto& [vertex, weight]: getNeighbours(current)) {

		if (get(t_discoveryTime, vertex) != 0) {
			updateLowpoints(current, vertex, t_lowPoints1, t_lowPoints2, t_discoveryTime, false);
		} else {
			set(t_ancestor, vertex, current);
			lowPointDfs(vertex, dfsCounter);
			updateLowpoints(current, vertex, t_lowPoints1, t_lowPoints2, t_discoveryTime, true);
		}

		// is a frond
		if (get(t_discoveryTime, vertex) < get(t_discoveryTime, current)) {
			weight = 2 * get(t_discoveryTime, vertex);
		} else if (get(t_lowPoints2, vertex) == get(t_discoveryTime, current)) {
			weight = 2 * get(t_lowPoints1, vertex);
		} else if (get(t_lowPoints2, vertex) < get(t_discoveryTime, current)) {
			weight = 2 * get(t_lowPoints1, vertex) + 1;
		} else {
			weight = get(t_discoveryTime, vertex);
		}
	}
}


bool Graph::isPlanar() {

	bool isPlanar = true;
	bool isNotPlanar = false;
	for (int componentIndex = 0; componentIndex < t_componnets.getSize(); componentIndex++) {
		vertex_t current = t_componnets[componentIndex].startingPoint;
		int nVertices = (int) t_componentsVertices[componentIndex].getCapacity();
		if (nVertices < 5) {
			continue;
		}
		// if (t_componnets[componentIndex].size > 3 * t_componnets[componentIndex].order - 6) {
		// 	isNotPlanar = true;
		// 	break;
		// }
		isPlanar = false; // todo: remove, this is temporary

		int maxWeight = 2 * nVertices + 1;

		auto* buckets = new dst::List<Edge>[maxWeight];
		for (vertex_t vertex: t_componentsVertices[componentIndex]) {
			for (Edge neighbour: getNeighbours(vertex)) {
				buckets[neighbour.info].push(neighbour);
			}
			int outputIndex = 0;
			for (int i = 0; i < maxWeight; ++i) {
				while (!buckets[i].isEmpty()) {
					t_adjancencyList[vertex - 1][outputIndex] = buckets[i].pop();
					outputIndex++;
				}
			}
		}

		delete[] buckets;

		set(t_branchPoint, current, 0);
		branchPointDfs(current);
		embedBranchDfs(current);
	}

	if (isNotPlanar) {
		printf("F\n");
	} else if (isPlanar) {
		printf("T\n");
	} else {
		printf("?\n");
	}
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
void Graph::branchPointDfs(vertex_t current) {
	bool firstTime = true;
	for (auto [vertex, _]: getNeighbours(current)) {
		if (get(t_ancestor, vertex) == current) {
			if (firstTime) {
				set(t_branchPoint, vertex, get(t_branchPoint, current));
			} else {
				set(t_branchPoint, vertex, current);
			}
			branchPointDfs(vertex);
		}
		firstTime = false;
	}
}
void Graph::embedBranchDfs(vertex_t current) {
	for (auto [vertex, weight]: getNeighbours(current)) {
		t_notPlanar = true;
		if (get(t_ancestor, vertex) == current) {
			if (get(t_branchPoint, vertex) == current) {
				int lowPoint = get(t_lowPoints1, vertex);
				if (t_leftFace.isEmpty() || t_leftFace.top() < lowPoint) {
					// t_leftFace.put(lowPoint);
				}
			}
			embedBranchDfs(vertex);
			if (t_notPlanar) {
				return;
			}
		} else if (get(t_discoveryTime, vertex) < get(t_discoveryTime, current)) {
			if (!embedFrond(current, vertex, weight)) {
				return;
			}
		}
	}
	t_notPlanar = false;
}
bool Graph::embedFrond(vertex_t frondRoot, vertex_t frondEnd, int weight) {
	if (t_leftFace.isEmpty() || t_leftFace.front() < weight) {
		// t_leftFace.put(weight);
		return true;
	}
	if (t_rightFace.isEmpty() || t_rightFace.front() < weight) {
		// t_rightFace.put(weight);
		return true;
	}
	return false;
}

void Graph::eccenricityBfs(vertex_t startingPoint, int* eccentricity, int currentComponentIndex) {
	int vertexIndex = 0;
	t_componentsVertices[currentComponentIndex][0] = startingPoint;

	bool firstIteration = true;
	int componentConsistencyNumber = (int) t_componentsVertices[currentComponentIndex].getCapacity();
	t_componentsVertices[currentComponentIndex].setSize(
		(int) t_componentsVertices[currentComponentIndex].getCapacity());

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
	t_discoveryTime = new int[t_numberVertices];
	t_ancestor = new int[t_numberVertices];
	t_lowPoints1 = new int[t_numberVertices];
	t_lowPoints2 = new int[t_numberVertices];
	t_branchPoint = new int[t_numberVertices];

	auto* visited = new bool[t_numberVertices];

	for (int i = 0; i < t_numberVertices; ++i) {
		t_discoveryTime[i] = 0;
		visited[i] = false;
	}

	for (int i = 0; i < t_numberVertices; ++i) {
		// if (!visited[i]) {
		if (t_discoveryTime[i] == 0) {
			int dfsCount = 0;
			int degreeSum = 0;
			// dfs(i + 1, visited, dfsCount);
			set(t_ancestor, i + 1, 0);
			lowPointDfs(i + 1, dfsCount);
			t_componnets.push({i + 1, dfsCount});
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
		bipartite = bipartiteDfs(componnet.startingPoint, 2);
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
		} else if (componentSize == 2) {
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
	int nBuckets = t_maximumDegree + 1;
	auto* degreesBuckets = new dst::List<vertex_t>[nBuckets];

	for (int i = 0; i < t_numberVertices; ++i) {
		int degree = (int) t_adjancencyList[i].getSize();
		degreesBuckets[degree].push(i + 1);
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

void Graph::updateIndexTable(const SaturationInfo& value, int newIndex, int* table) {
	table[value.vertex - 1] = newIndex;
}

void Graph::vertexColorsSLF() {
	auto* saturations = new int[t_numberVertices];
	auto* queueIndexTable = new int[t_numberVertices];

	dst::Vector<dst::HashSet<bool>> verticesColorsSets(t_numberVertices);

	dst::PriorityQueue<SaturationInfo> queue(t_numberVertices);
	for (int i = 0; i < t_numberVertices; ++i) {
		saturations[i] = 0;
		if (getDegree(i + 1) == 0) {
			set(t_colours, i + 1, 1);
			continue;
		}
		queue.insert(SaturationInfo{saturations[i], getDegree(i + 1), i + 1});
	}

	SaturationInfo* heapData_p = queue._getRawTable();
	for (int i = 1; i <= queue.getSize(); ++i) {
		queueIndexTable[heapData_p[i].vertex - 1] = i;
	}

	queue.setIndexTableCallback(updateIndexTable, queueIndexTable);

	while (!queue.isEmpty()) {
		SaturationInfo current = queue.pop();

		int color = 1;
		for (; color <= t_maximumDegree; ++color) {
			if (!verticesColorsSets[current.vertex - 1].existsIn(color)) {
				break;
			}
		}
		set(t_colours, current.vertex, color);

		for (auto [vertex, _]: getNeighbours(current.vertex)) {
			if (get(t_colours, vertex) == 0) {
				verticesColorsSets[vertex - 1].setValue(color, true);
				set(saturations, vertex, verticesColorsSets[vertex - 1].getSize());
				queue.updateAtIndex(queueIndexTable[vertex - 1], {get(saturations, vertex), getDegree(vertex), vertex});
			}
		}
	}

	delete[] saturations;
	delete[] queueIndexTable;
}

long long int Graph::countOfC4() {

	long long c4Count = 0;

	auto* neighbours = new bool[t_numberVertices];
	for (int i = 0; i < t_numberVertices; ++i) {
		neighbours[i] = false;
	}
	for (int componentIndex = 0; componentIndex < t_componnets.getSize(); ++componentIndex) {
		int componentSize = (int) t_componentsVertices[componentIndex].getCapacity();
		if (componentSize < 4) {
			continue;
		}
		for (int i = 0; i < componentSize; ++i) {
			vertex_t v = t_componentsVertices[componentIndex][i];
			int degreeV = getDegree(v);
			if (degreeV < 2) {
				continue;
			}
			for (auto [vertex, _]: getNeighbours(v)) {
				neighbours[vertex - 1] = true;
			}

			for (int j = i + 1; j < componentSize; ++j) {
				vertex_t u = t_componentsVertices[componentIndex][j];
				int degreeU = getDegree(u);
				if (degreeU < 2) {
					continue;
				}

				int neighbourCount = 0;
				for (auto [vertex, _]: getNeighbours(u)) {
					if (neighbours[vertex - 1]) {
						neighbourCount++;
					}
				}

				// printf("%lld ", neighbourCount);
				c4Count += neighbourCount * (neighbourCount - 1) / 2;
			}

			for (auto [vertex, _]: getNeighbours(v)) {
				neighbours[vertex - 1] = false;
			}
		}
	}
	delete[] neighbours;

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

	// for (int i = 0; i < t_componnets.getSize(); ++i) {
	// 	int verticesInComponent = (int) t_componentsVertices[i].getCapacity();
	// 	auto* countTable = new int[verticesInComponent];
	// 	for (int j = 0; j < verticesInComponent; ++j) {
	// 		countTable[j] = 0;
	// 	}
	//
	// 	for (int v = 0; v < verticesInComponent; ++v) {
	// 		int degreeV = (int) t_adjancencyList[v].getSize();
	//
	// 		for (auto u: getNeighbours(v + 1)) {
	// 			int degreeU = getDegree(u.vertex);
	// 			if (degreeU < degreeV || (degreeU == degreeV && u.vertex < v + 1)) {
	// 				for (auto w: getNeighbours(u.vertex)) {
	// 					int degreeW = getDegree(w.vertex);
	// 					if (degreeW < degreeV || (degreeW == degreeV && w.vertex < v + 1)) {
	// 						int countForVertex = get(countTable, w.vertex);
	// 						c4Count += countForVertex;
	// 						set(countTable, w.vertex, countForVertex + 1);
	// 					}
	// 				}
	// 			}
	// 		}
	// 		for (auto u: getNeighbours(v + 1)) {
	// 			int degreeU = getDegree(u.vertex);
	// 			if (degreeU < degreeV || (degreeU == degreeV && u.vertex < v + 1)) {
	// 				for (auto w: getNeighbours(u.vertex)) {
	// 					set(countTable, w.vertex, 0);
	// 				}
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
