#ifndef _FORD_H
#define _FORD_H

#include <pthread.h>
#define NUM_THREADS 4
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
typedef thread_parm_t* p_thread_parm_t;

/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *node_relax(void *parm) {
    thread_parm_t *p = (thread_parm_t *) parm;
    int num_edge, v;
    double cost;

    int u = p->node;
//    int threadid = p->thread_id;
//    std::cerr << threadid << ": Relaxing node " << u << std::endl;
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
 * @A: the graph
 */
void Bellmanford_parallel(Graph& A) {
    pthread_t threads[NUM_THREADS];
    p_thread_parm_t parm[NUM_THREADS];
    int rc;

    /**
     * Allocate memory for passing parameters to the threads
     */
    for (int t = 0; t < NUM_THREADS; ++t) {
        parm[t] = new thread_parm_t;
    }

    const int N = A.num_nodes();

    /**
     * Running the Program in multiple Threads.
     * The Inner Loop is Unrolled to NUM_THREADS to parallel
     */
    for (int x = 0; x < N; ++x) {
        for (int u = 0; u < N; u += NUM_THREADS) {
            int y = u;
            for (int thread_id = 0; (thread_id < NUM_THREADS && y < N);
                    ++thread_id, ++y) {

                //Assigning Data to the thread
                parm[thread_id]->A = &A;
                parm[thread_id]->node = y;
                parm[thread_id]->thread_id = thread_id;

                rc = pthread_create(&threads[thread_id], NULL, node_relax,
                                    (void *) parm[thread_id]);

                if (rc) {
                    std::cout << "ERROR; return code from pthread_create() is "
                              << thread_id << std::endl;
                }
            }

            //Join the running threads, so that a running thread will not be called on
            for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
                pthread_join(threads[thread_id], NULL);
            }
        }
    }

    //Clean up
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        delete(parm[thread_id]);
    }
}
#endif
