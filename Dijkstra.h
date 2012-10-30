/* Includes */
#include <queue>
#include <vector>
#include <cassert>
#include <limits.h>
#include "Graph.h"


#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

extern std::vector<double> dist;

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
	int x, y, NUM_EDGES;

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
}

#endif
