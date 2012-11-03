#ifndef _FORD_H
#define _FORD_H

//Global Container distance
extern std::vector<double> dist;


/*
 * procedure BellmanFord(list vertices, list edges, vertex source)
 */

void Bellmanford(Graph& A, const int SOURCE) {
    const int N = A.num_nodes();
    int num_edge, v;
    double cost;

    for (int x = 0; x < N; ++x) {
        for (int u = 0; u < N; ++u) {
            num_edge = A.num_edges(u);
            for (int e = 0; e < num_edge; ++e) {
                v = A.vertex(u, e);
                cost = dist[u] + A(u, e);
                if (cost < dist[v]) {
                    dist[v] = cost;
                }

            }
        }
    }
}

#endif
