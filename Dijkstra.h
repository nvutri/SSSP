/* Includes */
#include <queue>
#include <vector>
#include <cassert>
#include <limits.h>
#include "Graph.h"


#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

/**
 * Process Dijkstra algorithm.
 * Read the graph, initialize and run dijkstra algorithm
 */
void Dijkstra(Graph& A, const int SOURCE) {
	// Priority Queue for dijkstra workload
	std::priority_queue<int, std::vector<int>, CompareNode > queue;
	int x, y;
	queue.empty();
	queue.push(SOURCE);

	while (!queue.empty()){
		x = queue.top();
		queue.pop();
        std::list<Edge>& edges = A[x];
        std::list<Edge>::const_iterator iterator;
        for ( iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Edge node = *iterator;
		    y = node._vertex;
			if ( dist[y] > dist[x] + node._weight ){
				dist[y] = dist[x] +  node._weight;
				queue.push(y);
			}
		}
	}

}

#endif
