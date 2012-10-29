#include <vector>
#include <limits.h>

#ifndef _GRAPH_H
#define _GRAPH_H

const int DEFAULT_SIZE = 100;

class Graph {
private:
	std::vector< std::vector<double> > _m;
	;
public:
	int num_nodes() {
		return _m[0].size();
	}
	Graph(int NUM_NODES);
	Graph();
	double &operator()(int x, int y);
	void print();
};

Graph::Graph(){
	_m = std::vector< std::vector<double> >();
}

Graph::Graph(int NUM_NODES) {
	_m = std::vector< std::vector<double> >( NUM_NODES );
	for (int i = 0; i < NUM_NODES; ++i) {
		_m[i] = std::vector<double>(NUM_NODES);
		for (int j = 0; j < NUM_NODES; ++j) {
			_m[i][j] = LONG_MAX;
		}
	}
}
//Todo change to CPR
double& Graph::operator () (int x, int y){
	return _m[x][y] ;
}

#endif
