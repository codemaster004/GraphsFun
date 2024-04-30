#include "Utilities.h"


void inputVertex() {

}

void inputGrapth() {
	int nVertices = inputNumber('\n');
	for (int i = 0; i < nVertices; ++i) {
		inputVertex();
	}
}


int main() {
	int nGrapths = inputNumber('\n');
	for (int i = 0; i < nGrapths; ++i) {
		inputGrapth();
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
	 */

	return 0;
}
