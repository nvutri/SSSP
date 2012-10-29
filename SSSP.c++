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
void initialize();
// ----
// main
// ----

int main() {
	initialize();
	read_graph();
	dijkstra(A[0]);
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
	int n1, n2;
	int NUM_NODES, NUM_EDGES;
	double weight;

	while (cin >> line_type) {
		if (line_type == 'c') {
			cin.getline(line, 256, '\n');
		} else if (line_type == 'p') {
			cin >> graph_type;
			cin >> NUM_NODES; // Number of nodes
			cin >> NUM_EDGES; // Number of edges
		} else if (line_type == 'a') {
			cin >> n1 >> n2 >> weight;
			Node node = A[ n1 ];
			A[ n1 ][ n2 ] = weight;
//			cout << n1 << " "  << n2 << " "<< A[n1][n2]<< endl;
		} else
			break;
	}
}

/**
 * Initialize storage to correct state
 */
void initialize() {
	//Init all edges to max value
}

