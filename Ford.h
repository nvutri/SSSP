#ifndef _FORD_H
#define _FORD_H

/*
 * procedure BellmanFord(list vertices, list edges, vertex source)
 */

void Bellmanford(Graph& A, const int SOURCE) {
    const int N = A.num_nodes();
    double cost;

    for (int i = 0; i < N; ++i) {
        for (int x = 0; x < N; ++x) {
            std::list<Edge> edges = A[x];
            std::list<Edge>::const_iterator iterator;
            for (iterator = edges.begin(); iterator != edges.end(); ++iterator) {
                Edge node = *iterator;
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
