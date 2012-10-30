/* Includes */
#include <queue>
#include <vector>
#include <cassert>
#include <limits.h>
#include "Graph.h"


#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

/**
 * Global container
 */

std::vector<double> dist;


/**
 * Function prototype
 */
void verify_dijkstra(Graph& A, const int N);
void dijkstra_init(int SOURCE, const int N);
void dijkstra_print(const int N);
/**
 * Class to compare 2 nodes for the priority queue
 */
class CompareNode {
    public:
    bool operator()(int n1, int n2) // Returns true if t1 is earlier than t2
    {
       return dist[n1] < dist[n2];
    }
};

/**
 * Process Dijkstra algorithm.
 * Read the graph, initialize and run dijkstra algorithm
 */
void dijkstra(Graph& A, const int SOURCE) {
	// Priority Queue for dijkstra workload
	std::priority_queue<int, std::vector<int>, CompareNode > queue;
	const int N = A.num_nodes();
	int x, y, NUM_EDGES;
	dijkstra_init(SOURCE, N);


	queue.push(SOURCE);

	while (!queue.empty()){
		x = queue.top();
		queue.pop();
		NUM_EDGES = A.num_edges(x);
		for (int edge_index = 0; edge_index < NUM_EDGES; ++edge_index){
		    y = A.vertex(x, edge_index);
			if ( dist[y] > dist[x] + A(x, edge_index) ){
				dist[y] = dist[x] +  A(x, edge_index);
				queue.push(y);
			}
		}
	}
	verify_dijkstra(A, N);
}

void verify_dijkstra(Graph& A, const int N){
	for (int x = 0; x < N; ++x){
	    const int NUM_EDGES = A.num_edges(x);
		for (int edge_index = 0; edge_index < NUM_EDGES; ++edge_index) {
		    int y = A.vertex(x, edge_index);
		    assert(dist[x] + A(x, edge_index) >= dist[y]);
		}
	}
}

void dijkstra_init(int SOURCE, const int N){
	dist = std::vector<double>(N);
	for (int i = 0; i < N; ++i) {
		dist[i] = LONG_MAX;
	}
	dist[SOURCE] = 0;
}

void dijkstra_print(const int N){
    for (int i = 1; i < N; ++i) {
        std::cout << i << " " << dist[i] << std::endl;
    }
}
#endif
