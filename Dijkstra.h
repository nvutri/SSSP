/* Includes */
#include <queue>
#include <vector>
#include <cassert>
#include "Graph.h"
#include <limits.h>

#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

extern Graph A;
std::vector<double> dist;

/**
 * Function prototype
 */
void verify_dijkstra(const int N);
void dijkstra_init(int SOURCE, const int N);

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
void dijkstra(const int SOURCE) {
	// Priority Queue for dijkstra workload
	std::priority_queue<int, std::vector<int>, CompareNode > queue;
	const int N = A.num_nodes();
	dijkstra_init(SOURCE, N);


	queue.push(SOURCE);

	while (!queue.empty()){
		int x = queue.top();
		queue.pop();
		for (int y = 0; y < N; ++y)
			if ( dist[y] > dist[x] + A(x,y) ){
				dist[y] = dist[x] +  A(x,y);
				queue.push(y);
			}
	}
	verify_dijkstra(N);
}

void verify_dijkstra(const int N){
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (A(i, j) < LONG_MAX)
				assert(dist[i] + A(i, j) >= dist[j] );
}

void dijkstra_init(int SOURCE, const int N){
	dist = std::vector<double>(N);
	for (int i = 0; i < N; ++i) {
		dist[i] = LONG_MAX;
	}
	dist[SOURCE] = 0;
}
#endif
