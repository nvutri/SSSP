/* Includes */
#include <queue>
#include <vector>
#include <cassert>
#include <limits.h>
#include "Graph.h"


#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

extern std::vector<int> dist;

/**
 * Class to compare 2 nodes for the priority queue
 */
class CompareNode {
    public:
    bool operator()(int n1, int n2) // Returns true if t1 is earlier than t2
    {
       return dist[n1] > dist[n2];
    }
};

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
        std::list<Node>& edges = A[x];
        std::list<Node>::const_iterator iterator;
        for ( iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Node node = *iterator;
		    y = node._vertex;
			if ( dist[y] > dist[x] + node._weight ){
				dist[y] = dist[x] +  node._weight;
				queue.push(y);
			}
		}
	}

}

#endif
