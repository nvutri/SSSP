#ifndef _FORD_H
#define _FORD_H

#include <pthread.h>
#define NUM_THREADS 2
/*
 * procedure BellmanFord(list vertices, list edges, vertex source)
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

typedef struct {
    Graph* A;
    int node;
    int thread_id;
} thread_parm_t;
/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *node_relax(void *parm) {
    thread_parm_t *p = (thread_parm_t *) parm;
    int num_edge, v;
    double cost;

    int u = p->node;
    int threadid = p->thread_id;
//    std::cout << threadid << ": Relaxing node " << u << std::endl;
    Graph& A = *(p->A);
    num_edge = A.num_edges(u);
    for (int e = 0; e < num_edge; ++e) {
        v = A.vertex(u, e);
        cost = dist[u] + A(u, e);
        if (cost < dist[v]) {
            dist[v] = cost;
        }
    }
    pthread_exit(NULL);
    return NULL;
}

/**
 * Parallel Ford Bellman
 */
void Bellmanford_parallel(Graph& A) {
    pthread_t threads[NUM_THREADS];

    int rc, t = 0;
    const int N = A.num_nodes();
    for (int x = 0; x < N; ++x) {
        for (int u = 0; u < N; ++u) {
            t = u % NUM_THREADS;
            thread_parm_t *parm = new thread_parm_t;
            parm->A = &A;
            parm->node = u;
            parm->thread_id = t;
            rc = pthread_create(&threads[t], NULL, node_relax, (void *) parm);
//            pthread_join(threads[t], NULL);
            if (rc) {
                std::cout << "ERROR; return code from pthread_create() is " << t
                          << std::endl;
                exit(-1);
            }
//            delete(parm);
        }
        for (int t = 0; t < NUM_THREADS; ++t) {
            pthread_join(threads[t], NULL);
        }
    }

//    pthread_exit(NULL);
}
#endif
