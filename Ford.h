#ifndef _FORD_H
#define _FORD_H

#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

#define MAX_THREADS 16

//Global Container distance
extern std::vector<double> dist;

//Thread data container
typedef struct {
    Graph* A;
    int thread_id;
    bool busy;
} thread_parm_t;
typedef thread_parm_t* p_thread_parm_t;

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

/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int num_edge, v;
    double cost;
    Graph& A = *(p->A);
    const int N = A.num_nodes();

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

    p->busy = false;
    pthread_exit(NULL);
    return NULL;
}

/**
 * Find a free thread
 */
int find_thread(p_thread_parm_t* parm, const int NUM_THREADS){
    //Searching for a free thread

    int thread_id = rand() % NUM_THREADS;
    while (parm[thread_id]->busy){
        thread_id = rand() % NUM_THREADS;
    }
    return thread_id;
}
/**
 * Parallel Ford Bellman
 * @A: the graph
 */
void Bellmanford_parallel(Graph& A, const int SOURCE, const int NUM_THREADS) {

    assert(NUM_THREADS <= MAX_THREADS);
    pthread_t threads[MAX_THREADS];
    p_thread_parm_t parm[MAX_THREADS];

    srand ( time(NULL) );
    /**
     * Allocate memory for passing parameters to the threads
     * Free and initialize them
     */

    for (int t = 0; t < NUM_THREADS; ++t) {
        parm[t] = new thread_parm_t;
        parm[t]->A = &A;
        parm[t]->busy = false;
    }

    const int N = A.num_nodes();

    /**
     * Running the Program in multiple Threads.
     */

    int thread_id = 0;
    int rc;

    for (int x = 0; x < N; ++x) {
        thread_id = find_thread(parm, NUM_THREADS);
        assert(!parm[thread_id]->busy);

        //Assigning Data to the thread.
        //Marking thread as busy

        parm[thread_id]->busy = true;
        parm[thread_id]->thread_id = thread_id;

        rc = pthread_create(&threads[thread_id], NULL, node_relax,
                            (void *) parm[thread_id]);
        if (rc) {
//            std::cerr << "ERROR; return code from pthread_create() is "
//                      << thread_id << std::endl;
            parm[thread_id]->busy = false;
        }

    }

    //Wait for all threads to finish
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id)
        if (parm[thread_id]->busy) {
            pthread_join(threads[thread_id], NULL);
        }

    //Clean up data passed to threads
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        assert(!parm[thread_id]->busy);
        delete (parm[thread_id]);
    }
}
#endif
