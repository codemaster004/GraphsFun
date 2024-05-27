/**
 * Created by Filip Dabkowski on 30/04/2024.
 */

#ifndef GRAPH_H
#define GRAPH_H
#define DEBUG

#include "List.h"
#include "Vector.h"

#include <climits>


using long_t = long long int;

using vertex_t = int;


class Graph {
	struct Edge {
		vertex_t vertex;
		int info;

		bool operator<(const Edge& other) const {
			return vertex < other.vertex;
		}

		bool operator>(const Edge& other) const {
			return vertex > other.vertex;
		}
	};
	using VertexInfo = Edge;

	struct SaturationInfo {
		int* saturationValue_p;
		int degree;
		int vertex;
	};


	dst::Vector<Edge>* t_adjancencyList;
	int t_numberVertices;

	int t_minimumDegree;
	int t_maximumDegree;
	int* t_degreeCounts;

	long_t t_degreeSum;

	dst::List<VertexInfo> t_componnets;
	dst::Vector<vertex_t>* t_componentsVertices;

	int* t_colours;

	// tables for planarity
	int* t_discoveryTime;
	int* t_lowPoints1;
	int* t_lowPoints2;

	void lowPointDfs(vertex_t current, int& dfsCounter, int* dfsDiscovery, int* lowPoints1, int* lowPoints2);
	bool bipartiteDfs(vertex_t current, int previousColor);
	void dfs(vertex_t current, bool* visited, int& dfsCount);

	void eccenricityBfs(vertex_t startingPoint, int* eccentricity, int currentComponentIndex);

	void colorVertex(vertex_t vertex, int* colors, bool* colorsUsed);

	static bool slfStructCompare(const SaturationInfo& a, const SaturationInfo& b);

public:
	Graph() :
		t_adjancencyList(nullptr), t_numberVertices(0), t_degreeSum(0), t_minimumDegree(-1),
		t_maximumDegree(0), t_degreeCounts(nullptr), t_colours(nullptr), t_discoveryTime(nullptr), t_lowPoints1(nullptr),
		t_lowPoints2(nullptr) {}

	void initGrapthOrder(int order) {
		t_numberVertices = order;
		t_degreeCounts = new int[order + 1];
		for (int i = 0; i < order + 1; ++i) {
			t_degreeCounts[i] = 0;
		}

		t_colours = new int[t_numberVertices];

		t_adjancencyList = new dst::Vector<Edge>[t_numberVertices];
	}

	void setGraphOrder(int order) { t_numberVertices = order; }

	void setVertexDegree(vertex_t v, int degree) { t_adjancencyList[v - 1].resize(degree); }
	void addVertex(vertex_t v, vertex_t u) { t_adjancencyList[v - 1].pushBack({u, 0}); }
	void rememberDeg(vertex_t vertex, int degree) {
		t_degreeCounts[degree] += 1;
		if (degree > t_maximumDegree) {
			t_maximumDegree = degree;
		} else if (degree < t_minimumDegree || t_minimumDegree == -1) {
			t_minimumDegree = degree;
		}
	}
	void updateDegreeSum(int degree) { t_degreeSum += degree; }
	int getDegree(vertex_t vertex) {
		return (int) t_adjancencyList[vertex - 1].getSize();
	}

	Edge* getRawNeighboursArray(vertex_t forVertex) {
		return t_adjancencyList[forVertex - 1].getRawPointer();
	}

	dst::Vector<Edge>& getNeighbours(vertex_t of) { return t_adjancencyList[of - 1]; }

	/* Tests */

	bool isPlanar();

	int numberOfComponents();

	bool isBipartite();

	void vertexEccentricity();

	void vertexColorsGreedy();

	void vertexColorsLF();

	void vertexColorsSLF();

	long long int countOfC4();

	[[nodiscard]] long long int complementEdges() const;

	/* Printing */

	void print() const;

	void printDegSequence() const;

	void printColours() const;

	void removeDegreeSequence() {
		delete[] t_degreeCounts;
		t_degreeCounts = nullptr;
	}

	void resetColours() {
		for (int i = 0; i < t_numberVertices; ++i) {
			t_colours[i] = 0;
		}
	}

	void clear() {
		t_componnets.clear();

		delete[] t_degreeCounts;
		t_degreeCounts = nullptr;

		delete[] t_colours;
		t_colours = nullptr;

		for (int i = 0; i < t_numberVertices; ++i) {
			t_adjancencyList[i].clear();
		}
		delete[] t_adjancencyList;
		t_adjancencyList = nullptr;

		t_numberVertices = 0;
		t_degreeSum = 0;
		t_minimumDegree = -1;
		t_maximumDegree = 0;
	}

	~Graph() { clear(); };
};

template<typename T>
void set(T* inTable, vertex_t forVertex, T toValue) {
	inTable[forVertex - 1] = toValue;
}

template<typename T>
T get(const T* inTable, vertex_t ofVertex) {
	return inTable[ofVertex - 1];
}


#endif // GRAPH_H
