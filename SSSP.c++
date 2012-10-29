/**
 * To run the program:
 *     g++ -lcppunit -ldl -Wall TestSSSP.c++ -o TestSSSP.app
 */
// --------
// includes
// --------
#include <iostream>

#include "Dijkstra.h"
#include "Graph.h"

using namespace std;

/**
 * Global Variable
 */

Graph A;
/**
 * Function Prototype
 */
void read_graph();
// ----
// main
// ----

int main() {
	read_graph();
	dijkstra(1);
	return 0;
}

/**
 * Read the input files.
 * Put data in graph's storage
 */
void read_graph() {
	char line_type;
	char graph_type[5];
	char line[256];
	int x, y;
	int NUM_NODES, NUM_EDGES;
	double weight;
	while (cin >> line_type) {
		if (line_type == 'c') {
			cin.getline(line, 256, '\n');
		} else if (line_type == 'p') {
			cin >> graph_type;
			cin >> NUM_NODES; // Number of nodes
			cin >> NUM_EDGES; // Number of edges
			break;
		}
	}

	NUM_NODES++ ;// Graph starts from Node 1
	A = Graph(NUM_NODES);

	while (cin >> line_type) {
		if (line_type == 'c') {
			cin.getline(line, 256, '\n');
		}
		else if (line_type == 'a') {
			cin >> x >> y >> weight;
			A(x, y) = weight;
			A(y, x) = weight;
//			cout << x << " " << y << " " << A(x, y) << endl;
		} else
			break;
	}
}

