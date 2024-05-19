/**
 * Created by Filip Dabkowski on 30/04/2024.
 */

#ifndef GRAPH_H
#define GRAPH_H
#define DEBUG

#include "List.h"
#include "Vector.h"

using vertex_t = int;


class Graph {
	struct Edge {
		vertex_t vertex;
		int info;
	};

	dst::Vector<Edge>* t_adjancencyList;
	int t_numberVertices;

	int* t_degSequence;
	dst::List<vertex_t> t_componnets;

	int* t_colours;

	// tables for planarity
	int* t_discoveryTime;
	int* t_lowPoints1;
	int* t_lowPoints2;

	void lowPointDfs(vertex_t current, int& dfsCounter, int* dfsDiscovery, int* lowPoints1, int* lowPoints2);
	bool bipartiteDfs(vertex_t current, int previousColor);
	void dfs(vertex_t current, bool* visited);

public:
	Graph() :
		t_adjancencyList(nullptr), t_numberVertices(0), t_degSequence(nullptr), t_colours(nullptr), t_discoveryTime(nullptr),
		t_lowPoints1(nullptr), t_lowPoints2(nullptr) {}

	void setGraphOrder(int order) {
		t_numberVertices = order;
		t_degSequence = new int[order + 1];
		for (int i = 0; i < order + 1; ++i) {
			t_degSequence[i] = 0;
		}

		t_colours = new int[t_numberVertices];

		t_adjancencyList = new dst::Vector<Edge>[t_numberVertices];
	}

	void setVertexDegree(vertex_t v, int degree) { t_adjancencyList[v - 1].resize(degree);}
	void addVertex(int v, int u) { t_adjancencyList[v - 1].pushBack({u, 0}); }
	void rememberDeg(int degree) { t_degSequence[degree] += 1; }

	dst::Vector<Edge>& getNeighbours(vertex_t of) { return t_adjancencyList[of - 1]; }

	/* Tests */

	bool isPlanar();

	int numberOfComponents();

	bool isBipartite();

	void vertexEccentricity();

	void vertexColorsGreedy();

	void vertexColorsLF();

	void vertexColorsSLF();

	int countOfC4();

	int complementEdges();

	/* Printing */

	void print();

	void printDegSequence() const;

	void printColours() const;

	void removeDegreeSequence() {
		delete[] t_degSequence;
		t_degSequence = nullptr;
	}

	void resetColours() {
		for (int i = 0; i < t_numberVertices; ++i) {
			t_colours[i] = 0;
		}
	}

	void clear() {
		t_componnets.clear();

		delete[] t_degSequence;
		t_degSequence = nullptr;

		delete[] t_colours;
		t_colours = nullptr;

		for (int i = 0; i < t_numberVertices; ++i) {
			t_adjancencyList[i].clear();
		}
		delete[] t_adjancencyList;
		t_adjancencyList = nullptr;

		t_numberVertices = 0;
	}

	~Graph() {
		clear();
	};
};

template<typename T>
void set(T* inTable, vertex_t forVertex, T toValue) { inTable[forVertex - 1] = toValue; }

template<typename T>
T get(const T* inTable, vertex_t ofVertex) { return inTable[ofVertex - 1]; }


#endif // GRAPH_H
