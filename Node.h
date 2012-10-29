#include <vector>
#include <limits.h>
#ifndef _NODE_H
#define _NODE_H
class Node {
private:
	std::vector<double> _val;
	std::vector<int> _col_ind;
public:
	int num_edges() {
		return _col_ind.size();
	}
	double &operator[](int node);
	void print();
};

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
		std::cout << _col_ind[i] << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < num_edges(); ++i) {
		std::cout << _val[i] << " ";
	}
	std::cout << std::endl;
}

#endif
