#ifndef _FORD_H
#define _FORD_H

//Global Container distance
extern std::vector<double> dist;

/*
 * procedure BellmanFord(list vertices, list edges, vertex source)
 */

void Bellmanford(Graph& A, const int SOURCE) {
    const int N = A.num_nodes();
    double cost;

    for (int i = 0; i < N; ++i) {
        for (int x = 0; x < N; ++x) {
            std::list<Node> edges = A[x];
            std::list<Node>::const_iterator iterator;
            for (iterator = edges.begin(); iterator != edges.end(); ++iterator) {
                Node node = *iterator;
                int y = node._vertex;
                cost = dist[x] + node._weight;
                if (cost < dist[y]) {
                    dist[y] = cost;
                }
            }
        }
    }
}

#endif
