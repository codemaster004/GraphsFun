/**
 * Created by Filip Dabkowski on 30/04/2024.
 */

#ifndef GRAPH_H
#define GRAPH_H
#define DEBUG

#include "List.h"

using vertex_t = int;


class Graph {
	struct Node {
		vertex_t vertex;
		int info;
	};

	dst::List<Node>* t_adjancencyList;
	int t_numberVertices;

	int* t_degSequence;

	// tables for planarity
	int* t_discoveryTime;
	int* t_lowPoints1;
	int* t_lowPoints2;

	void lowPointDfs(vertex_t current, int& dfsCounter, int* dfsDiscovery, int* lowPoints1, int* lowPoints2);

public:
	Graph() :
		t_adjancencyList(nullptr), t_numberVertices(0), t_degSequence(nullptr), t_discoveryTime(nullptr),
		t_lowPoints1(nullptr), t_lowPoints2(nullptr) {}

	void setGraphOrder(int order) {
		t_numberVertices = order;
		t_degSequence = new int[order + 1];
		for (int i = 0; i < order + 1; ++i) {
			t_degSequence[i] = 0;
		}
		t_adjancencyList = new dst::List<Node>[t_numberVertices];
	}

	void addVertex(int v, int u) { t_adjancencyList[v - 1].insertBack({u, 0}); }
	void rememberDeg(int degree) { t_degSequence[degree] += 1; }

	dst::List<Node>& getNeighbours(vertex_t of) { return t_adjancencyList[of - 1]; }

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

	void removeDegreeSequence() {
		delete[] t_degSequence;
		t_degSequence = nullptr;
	}

	void clear() {
		t_numberVertices = 0;

		delete[] t_degSequence;
		t_degSequence = nullptr;

		delete[] t_adjancencyList;
		t_adjancencyList = nullptr;
	}

	~Graph() = default;
};


#endif // GRAPH_H
