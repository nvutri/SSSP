/**
 * To run the program:
 *     g++ -lcppunit -ldl -Wall TestSSSP.c++ -o TestSSSP.app
 */
// --------
// includes
// --------
#include <iostream>
#include <limits.h>
#include "Dijkstra.h"

const int MAX_SIZE = 1000;

/**
 * Global Data Storage
 */

// Matrix storage TODO: switch to Compressed Row Storage
double A[MAX_SIZE][MAX_SIZE];

/**
 * Global Variables
 */
int NUM_NODES, NUM_EDGES;
using namespace std;

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
	//TODO: dijkstra();
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
	double weight ;
	while (cin >> line_type) {
		if (line_type == 'c') {
			cin.getline(line, 256, '\n');
//			cout << line << "\n";
		} else if (line_type == 'p') {
			cin >> graph_type;
			cin >> NUM_NODES; // Number of nodes
			cin >> NUM_EDGES; // Number of edges
//			cout << graph_type << "\n";
//			cout << NUM_NODES << " " << NUM_EDGES << "\n";
		} else if (line_type == 'a') {
			cin >> n1 >> n2 >> weight;
			A[n1][n2] = weight;
			A[n2][n1] = weight;
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
	for (int i = 0; i < NUM_NODES; ++i)
		for (int j = 0; j < NUM_NODES; ++j) {
			A[i][j] = LONG_MAX;
		}
}

