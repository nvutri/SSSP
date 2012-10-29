/**
 * To run the program:
 *     g++ -lcppunit -ldl -Wall TestSSSP.c++ -o TestSSSP.app
 */
// --------
// includes
// --------
#include <iostream>

#include "Graph.h"
#include "Dijkstra.h"
#include "Ford.h"


using namespace std;

/**
 * Function Prototype
 */
void read_graph(Graph& A);
int read_graph_dimension();

// ----
// main
// ----

int main() {
    int NUM_NODES = read_graph_dimension();
    Graph A(NUM_NODES);
	read_graph(A);
	dijkstra(A, 1);
	dijkstra_print(NUM_NODES);
	return 0;
}

/**
 * Read Graph Dimension
 * @return: graph number of nodes
 */

int read_graph_dimension(){
    char line_type;
    char graph_type[5];
    char line[256];
    int NUM_NODES = -1, NUM_EDGES;
    while (NUM_NODES == -1 && cin >> line_type ) {
        if (line_type == 'c') {
            cin.getline(line, 256, '\n');
        } else if (line_type == 'p') {
            cin >> graph_type;
            cin >> NUM_NODES; // Number of nodes
            cin >> NUM_EDGES; // Number of edges
        }
    }
    NUM_NODES++ ;// Graph starts from Node 1
    return NUM_NODES;
}
/**
 * Read the input files.
 * Put data in graph's storage
 */
void read_graph(Graph& A) {
    char line_type;
    char line[256];
    int x , y;
    double weight;

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

