/* Includes */
#include <queue>
#include <vector>
#include "Node.h"
#include "Graph.h"

#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

extern Graph A;
class CompareNode {
    public:
    bool operator()(Node& n1, Node& n2) // Returns true if t1 is earlier than t2
    {
       return n1.dist < n2.dist;
    }
};
/**
 * Process Dijkstra algorithm.
 * Read the graph, initialize and run dijkstra algorithm
 */
void dijkstra(Node& SOURCE) {
	// Priority Queue for dijkstra workload
	std::priority_queue<Node, std::vector<Node>, CompareNode > queue;

	SOURCE.dist = 0;
	queue.push(SOURCE);
	while (!queue.empty()){
		Node x = queue.top();
		queue.pop();
		for (int i = 0; i < x.num_edges(); ++i)
		{
			int y_id = x.get_index(i);
			Node y = A[y_id];
			if (y.dist > x.dist + x[y_id]){
				y.dist = x.dist + x[y_id];
				queue.push(y);
			}
		}
	}
	for (int i = 0; i < A.num_nodes(); ++i) {
		Node node = A[i];
		std::cout << i << " " << node.dist << std::endl;
	}
}
#endif
