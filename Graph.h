#include <vector>
#include <limits.h>
#include <cassert>
#include <algorithm>

#ifndef _GRAPH_H
#define _GRAPH_H

const int DEFAULT_SIZE = 100;

class Graph {
  private:
    std::vector<double> _val;
    std::vector<int> _col_ind;
    std::vector<int> _row_ptr;
    const int _NUM_NODES;
    const int _NUM_EDGES;

  public:
    int num_nodes() const;
    Graph(int NUM_NODES, int NUM_EDGES);

    void insert(int x, int y, double weight);
    int num_edges(int x);
    int vertex(int x, int index);
    double &operator()(int x, int y);
    void print();
};

/**
 * Constructor:
 * Require number of nodes, and number of edges
 */
Graph::Graph(int NUM_NODES, int NUM_EDGES)
        : _val(NUM_EDGES, LONG_MAX),
          _col_ind(NUM_EDGES, 0),
          // it needs an extra +1 to know where to stop
          _row_ptr(NUM_NODES + 1, 0),

          _NUM_NODES(NUM_NODES),
          _NUM_EDGES(NUM_EDGES) {
    _row_ptr[1] = 1;
}

/**
 * @return: number of nodes;
 */
int Graph::num_nodes() const{
    return _NUM_NODES;
}
/**
 * Insert an edge to the CPR Graph
 */
void Graph::insert(int x, int y, double weight) {
    //First time insertion on a node
    if (_row_ptr[x + 1] == 0) {
        _row_ptr[x + 1] = _row_ptr[x];
    }

    int index = _row_ptr[x + 1];
    _row_ptr[x + 1]++;
    _col_ind[index] = y;
    _val[index] = weight;
}

/**
 * @return: the edge from node,
 * @param: u node
 * @param: index index of the arc on node u
 */
double& Graph::operator () (int x, int index){
    int begin, end;
    begin = _row_ptr[ x ];
    end  = _row_ptr[ x + 1 ];
    assert (begin + index < end);
    return _val[ begin + index];
}

/**
 * @return number of edges of node x
 */
int Graph::num_edges(int x){
    int num = std::max(_row_ptr[x + 1] - _row_ptr[ x ], 0);
    return num;
}

/**
 * @return: the vertex of node x, arc index
 */
int Graph::vertex(int x, int index){
    int begin, end;
    begin = _row_ptr[ x ];
    end  = _row_ptr[ x + 1 ];
    assert (begin + index < end);
    return _col_ind[ begin + index];
}

/**
 * Print matrix containers
 */
void Graph::print() {
    for (unsigned int i = 0; i < _val.size(); ++i) {
        std::cout << _val[i] << " ";
    }
    std::cout << std::endl;
    for (unsigned int i = 0; i < _col_ind.size(); ++i) {
        std::cout << _col_ind[i] << " ";
    }
    std::cout << std::endl;
    for (unsigned int i = 0; i < _row_ptr.size(); ++i) {
        std::cout << _row_ptr[i] << " ";
    }
    std::cout << std::endl;
}
#endif
