#include "Graph.h"
#include "Utilities.h"


template<typename T>
void insertionSort(T* array, int size) {
	for (int i = 0; i < size; ++i) {
		T value = array[i];
		int j = i - 1;
		while (j >= 0 && value < array[j]) {
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = value;
	}
}

void inputVertex(Graph& graph, vertex_t current) {
	int nNeighbours = inputNumber(' ');
	if (nNeighbours == 0) {
		getchar(); // um magic in the input
	}

	graph.rememberDeg(current, nNeighbours);
	graph.updateDegreeSum(nNeighbours);
	graph.setVertexDegree(current, nNeighbours);

	for (int i = 0; i < nNeighbours - 1; ++i) {
		int neighbour = inputNumber(' ');
		graph.addVertex(current, neighbour);
	}
	if (nNeighbours >= 1) {
		int neighbour = inputNumber('\n');
		graph.addVertex(current, neighbour);
	}
	// insertionSort(graph.getRawNeighboursArray(current), nNeighbours);
}

void inputGrapth(Graph& graph) {
	int nVertices = inputNumber('\n');
	graph.setGraphOrder(nVertices);
	graph.initGrapthOrder(nVertices);

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

	static int COUNT = 0;
	int N_CASE = 1;

	Graph graph;
	int nGrapths = inputNumber('\n');
	for (int i = 0; i < nGrapths; ++i) {
		COUNT++;
		inputGrapth(graph);

		// printf("?\n");
		graph.printDegSequence();
		graph.removeDegreeSequence(); // no longer needed

		printf("%d\n", graph.numberOfComponents());
		// printf("?\n");

		// printf("?\n");
		printBool(graph.isBipartite());

		// printf("?\n"); // eccentricity
		graph.vertexEccentricity();

		// graph.isPlanar();
		printf("?\n");

		// printf("?\n"); // Greedy
		graph.vertexColorsGreedy();
		graph.printColours();

		// printf("?\n"); // LF
		graph.vertexColorsLF();
		graph.printColours();

		// printf("?\n"); // SLF
		graph.vertexColorsSLF();
		graph.printColours();

		printf("?\n"); // count C4
		// printf("%lld\n", graph.countOfC4());

		// printf("?\n");
		printf("%lli\n", graph.complementEdges());

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
	 *	9. the number of different C4 subgraphs; Number of missing edges to complete Graph. // out: 5
	 *	10. the number of the graph complement's edges; all complete edges - edges count;    // out: 81
	 */

	return 0;
}
