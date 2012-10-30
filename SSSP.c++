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
 * Global container
 */

std::vector<double> dist;


/**
 * Function Prototype
 */
void read_graph(Graph& A);
void read_graph_dimension(int& NUM_NODES, int& NUM_EDGES);

void dist_init(int SOURCE, const int N);
void dist_verify(Graph& A, const int N);
void dist_print(const int N);

// ----
// main
// ----

int main() {
    int NUM_NODES, NUM_EDGES;
    read_graph_dimension(NUM_NODES, NUM_EDGES);
    Graph A(NUM_NODES, NUM_EDGES);
	read_graph(A);

	int SOURCE = 1;
	//Initialization
	dist_init(SOURCE, NUM_NODES);

	Bellmanford(A, SOURCE);

	dist_verify(A, NUM_NODES);
	dist_print(NUM_NODES);

	return 0;
}

/**
 * Read Graph Dimension
 * @return: graph number of nodes
 */

void read_graph_dimension(int& NUM_NODES, int& NUM_EDGES){
    char line_type;
    char graph_type[5];
    char line[256];
    NUM_NODES = -1;
    NUM_EDGES = -1;

    while (NUM_NODES == -1 && cin >> line_type ) {
        if (line_type == 'c') {
            cin.getline(line, 256, '\n');
        } else if (line_type == 'p') {
            cin >> graph_type;
            cin >> NUM_NODES; // Number of nodes
            cin >> NUM_EDGES; // Number of edges
        }
    }
    // Graph starts from Node 1
    NUM_NODES++ ;
    NUM_EDGES++ ;

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
			A.insert(x, y , weight);
		} else
			break;
	}
//	A.print();
}

void dist_init(int SOURCE, const int N) {
    dist = std::vector<double>(N);
    for (int i = 0; i < N; ++i) {
        dist[i] = LONG_MAX;
    }
    dist[SOURCE] = 0;
}

void dist_verify(Graph& A, const int N){
    for (int x = 0; x < N; ++x){
        const int NUM_EDGES = A.num_edges(x);
        for (int edge_index = 0; edge_index < NUM_EDGES; ++edge_index) {
            int y = A.vertex(x, edge_index);
            assert(dist[x] + A(x, edge_index) >= dist[y]);
        }
    }
}

void dist_print(const int N){
    for (int i = 1; i < N; ++i) {
        std::cout << i << " " << dist[i] << std::endl;
    }
}


