/**
 * To run the program:
 *     g++ -lcppunit -ldl -Wall TestSSSP.c++ -o TestSSSP.app
 */
// --------
// includes
// --------
#include <iostream>
#include "Dijkstra.h"
//#include "Ford.h"

/**
 * Global Data Storage
 */

/* Matrix storage */
//TODO: switch to Compressed Row Storage
const int MAX_SIZE = 1000;
int A[MAX_SIZE][MAX_SIZE];
/* Boolean array to specify whether a node is still valid */
bool valid[MAX_SIZE]; // might not need this?
/* Priority Queue for dijkstra workload */
std::priority_queue<int> work;


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


//    cout << "Done." << endl;
    return 0;
}

/**
 * Read the input files.
 * Put data in graph's storage
 */
void read_graph(){

}

/**
 * Initialize storage to correct state
 */
void initialize(){

}

