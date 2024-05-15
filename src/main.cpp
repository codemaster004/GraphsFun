#include "Graph.h"
#include "Utilities.h"


void inputVertex(Graph& graph, int currentVertex) {
	int nNeighbours = inputNumber(' ');
	graph.rememberDeg(nNeighbours);
	for (int i = 0; i < nNeighbours; ++i) {
		int neighbour = inputNumber();
		graph.addVertex(currentVertex, neighbour);
	}
}

void inputGrapth(Graph& graph) {
	int nVertices = inputNumber('\n');
	graph.setGraphOrder(nVertices);

	for (int i = 0; i < nVertices; ++i) {
		inputVertex(graph, i + 1);
	}
}


void printBool(bool flag) {
	if (flag) {
		printf("T\n");
	} else {
		printf("F\n");
	}
}

int main() {
	Graph graph;
	int nGrapths = inputNumber('\n');
	for (int i = 0; i < nGrapths; ++i) {
		inputGrapth(graph);

		graph.printDegSequence();
		graph.removeDegreeSequence(); // no longer needed

		printf("%d\n", graph.numberOfComponents());
		printBool(graph.isBipartite());

		printf("?\n"); // eccentricity

		// graph.isPlanar();
		printf("?\n");

		printf("?\n"); // Greedy
		printf("?\n"); // LF
		printf("?\n"); // SLF

		printf("0\n"); // count C4
		printf("%d\n", graph.complementEdges()); // number of complement

		graph.clear();
	}

	/*
	 * 1. the degree sequence; Table with how many neighbours each Node has                 // out: 5 5 4 3 2 1
	 * 2. the number of components; How many smaller sub-grapth exist.                      // out: 6
	 * 3. bipartiteness; Can the grapth be collored with only 2 colors                      // out: T|F
	 * 4. the eccentricity of vertices; For vertex distance from the most distance vertex.  // out: 3 1 2 3 2 1 4
	 * 5. planarity; Graph can be drawn on a flat surface.                                  // out: T|F
	 * 6. vertices colours; How many and what colours can be used to color vetrtices        // out: 1 2 1 2 2 3 2 1
	 *		a. Greedy
	 *		b. LF
	 *		c. SLF
	 *	7. the number of different C4 subgraphs; Number of missing edges to complete Graph. // out: 5
	 *	8. the number of the graph complement's edges; all complete edges - edges count;    // out: 81
	 */

	return 0;
}
