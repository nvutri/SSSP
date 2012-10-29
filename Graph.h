#include <vector>
#include "Node.h"

#ifndef _GRAPH_H
#define _GRAPH_H

const int DEFAULT_SIZE = 100;

class Graph {
private:
	std::vector< Node > _nodes;
	;
public:
	int num_nodes() {
		return _nodes.size();
	}
	Graph();
	Graph(int num_node);
	Node &operator[](int node);
	void print();
};

Graph::Graph(){
	// Initialize an empty _node vector
	for (int i = 0; i < DEFAULT_SIZE; ++i) {
		_nodes.push_back(Node());
	}
}

Graph::Graph(int num_node){
	// Initialize an empty _node vector
	for (int i = 0; i < num_node; ++i) {
		_nodes.push_back(Node());
	}
}

//Todo change to CPR
Node& Graph::operator [] (int node){
	return _nodes[ node ] ;
}

#endif
