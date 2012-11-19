/**
 * To run the program:
 *     g++ -lcppunit -ldl -Wall TestSSSP.c++ -o TestSSSP.app
 */
// --------
// includes
// --------
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "Graph.h"
#include "Distance.h"
#include "Dijkstra.h"
#include "Round_Based.h"
#include "Chaotic_Relax.h"
#include "Delta_Stepping.h"

using namespace std;

/**
 * Function Prototype
 */
void read_graph(Graph& A);
void read_graph_dimension(int& NUM_NODES, int& NUM_EDGES);

// ----
// main
// ----

int main(int argc, char** argv) {
    int NUM_NODES, NUM_EDGES;

    /**
     * Reading Graph Data
     */
    read_graph_dimension(NUM_NODES, NUM_EDGES);
    Graph A(NUM_NODES, NUM_EDGES);
    read_graph(A);

    if (argc < 3) {
        std::cerr << "Wrong Number of Inputs" << std::endl;
        std::cerr << "USAGE: SSSP.app [Algorithm] [Number of threads]"
                  << std::endl;
        exit(-1);
    }

    const int SOURCE = 1;
    const char* ALGORITHM = argv[1];
    const int NUM_THREADS = atoi(argv[2]);
    int DELTA_TIMES = 1;
    if (argc > 3)
        DELTA_TIMES = atoi(argv[3]);

    //Initialization
    dist_init(SOURCE, NUM_NODES);

//    fprintf(stderr, "%s\n%d\n", ALGORITHM, NUM_THREADS);
    int start = clock();
    if (strcmp(ALGORITHM, "DIJK") == 0)
        Dijkstra(A, SOURCE);
    if (strcmp(ALGORITHM, "RB") == 0)
        Round_Based(A, SOURCE, NUM_THREADS);
    if (strcmp(ALGORITHM, "CT") == 0)
        Chaotic_Relaxation(A, SOURCE, NUM_THREADS);
    if (strcmp(ALGORITHM, "DT") == 0)
        Delta_Stepping(A, SOURCE, NUM_THREADS, DELTA_TIMES);

    int stop = clock();
    int elapsed_time = stop - start;
    cout << "Execution Time: " << ((float) elapsed_time) / CLOCKS_PER_SEC << endl;

    dist_verify(A, NUM_NODES);
//    dist_print(NUM_NODES);

    return 0;
}

/**
 * Read Graph Dimension
 * @return: graph number of nodes
 */

void read_graph_dimension(int& NUM_NODES, int& NUM_EDGES) {
    char line_type;
    char graph_type[5];
    char line[256];
    NUM_NODES = -1;
    NUM_EDGES = -1;

    while (NUM_NODES == -1 && cin >> line_type) {
        if (line_type == 'c') {
            cin.getline(line, 256, '\n');
        } else if (line_type == 'p') {
            cin >> graph_type;
            cin >> NUM_NODES;  // Number of nodes
            cin >> NUM_EDGES;  // Number of edges
        }
    }
    // Graph starts from Node 1
    NUM_NODES++;
    NUM_EDGES++;
}
/**
 * Read the input files.
 * Put data in graph's storage
 */
void read_graph(Graph& A) {
    char line_type;
    char line[256];
    int x, y;
    int weight;

    while (cin >> line_type) {
        if (line_type == 'c') {
            cin.getline(line, 256, '\n');
        } else if (line_type == 'a') {
            cin >> x >> y >> weight;
            A.insert(x, y, weight);
        } else
            break;
    }
}

