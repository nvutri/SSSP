#include <vector>
#include <limits.h>

#ifndef _NODE_H
#define _NODE_H

class Node {
private:
	std::vector<double> _val;
	std::vector<int> _col_ind;

//	int _id;

public:
//	Node(int id);
	//Distance from another node
	double dist;

	int num_edges() {
		return _col_ind.size();
	}

	Node();
	double &operator[](int node);
	bool operator <(Node rhs);
	int get_index(int index){
		return _col_ind[index];
	}
	void print();
};

Node::Node() {
	//initialize distance to maximum value
	dist = LONG_MAX;
}

// Comparing 2 nodes based on distance
// To be used by priority queue.
bool Node::operator <(Node rhs) {
	return (this->dist < rhs.dist);
}

double &Node::operator [](int node) {
	for (int i = 0; i < num_edges(); ++i)
		if (_col_ind[i] == node) {
			return _val[i];
		}

	_col_ind.push_back(node);
	_val.push_back(0);

	return _val[num_edges() - 1];
}
void Node::print() {
	for (int i = 0; i < num_edges(); ++i) {
		std::cout << _col_ind[i] << " " << _val[i] << std::endl;
	}
}
#endif
