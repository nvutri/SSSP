#include <vector>
#include <list>
#include <limits.h>
#include <cassert>
#include <algorithm>

#ifndef _GRAPH_H
#define _GRAPH_H

struct Edge{
    int _vertex;
    int _weight;
    Edge(int vertex, int weight):
        _vertex(vertex), _weight(weight){
    }

};
class Graph {
  private:
    /*
     * Adjacency List Data structure
     */
    std::vector < std::list<Edge> > _m;
    const int _NUM_NODES;
    const int _NUM_EDGES;

  public:
    int num_nodes() const;
    int num_edges() const;
    Graph(int NUM_NODES, int NUM_EDGES);

    void insert(int x, int y, int weight);
    std::list<Edge>& operator [] (int x);
    void print();
};

/**
 * Constructor:
 * Require number of nodes, and number of edges
 */
Graph::Graph(int NUM_NODES, int NUM_EDGES)
        : _m(NUM_NODES+1),
          _NUM_NODES(NUM_NODES),
          _NUM_EDGES(NUM_EDGES) {
}

/**
 * @return: number of nodes;
 */
int Graph::num_nodes() const{
    return _NUM_NODES;
}
/**
 * @return: total number of edges:
 */
int Graph::num_edges() const{
    return _NUM_EDGES;
}
/**
 * Insert an edge.
 */
void Graph::insert(int x, int y, int weight) {
    Edge node(y, weight);
    _m[x].push_back(node );
}

/**
 * @return: the edges from node,
 * @param: x node
 */
std::list<Edge>& Graph::operator [] (int x){
    return _m[ x ];
}

/**
 * Print matrix containers
 */
void Graph::print() {
    for (unsigned int i = 0; i < _m.size(); ++i) {
        std::list<Edge> x = _m[i];
        std::list<Edge>::iterator iterator;
        for (iterator = x.begin(); iterator != x.end(); ++iterator) {
            Edge node = *iterator;
            std::cout << i << " " << node._vertex << " " << node._weight << std::endl;
        }
    }
}
#endif
