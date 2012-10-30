#ifndef _FORD_H
#define _FORD_H

/*
 * procedure BellmanFord(list vertices, list edges, vertex source)
 // This implementation takes in a graph, represented as lists of vertices
 // and edges, and modifies the vertices so that their distance and
 // predecessor attributes store the shortest paths.

 // Step 1: initialize graph
 for each vertex v in vertices:
 if v is source tn v.distance := 0
 else v.distance := infinity
 v.predecessor := null

 // Step 2: relax edges repeatedly
 for i from 1 to size(vertices)-1:
 for each edge uv in edges: // uv is the edge from u to v
 u := uv.source
 v := uv.destination
 if u.distance + uv.weight < v.distance:
 v.distance := u.distance + uv.weight
 v.predecessor := u

 // Step 3: check for negative-weight cycles
 for each edge uv in edges:
 u := uv.source
 v := uv.destination
 if u.distance + uv.weight < v.distance:
 error "Graph contains a negative-weight cycle"
 [edit]
 */

extern std::vector<double> dist;
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
