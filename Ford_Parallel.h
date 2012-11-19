#ifndef _FORD_PARALLEL_H
#define _FORD_PARALLEL_H

#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include <algorithm>

#include "Thread_Handler.h"

#define MAX_THREADS 16

static bool graph_changed;

/**
 * Locking for BF parrallel
 */
pthread_mutex_t dist_lock;

/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int v;
    double cost;
    Graph& A = *(p->A);
    const int LEFT = p->left;
    const int RIGHT = p->right;

    for (int u = LEFT; u < RIGHT; ++u) {
        std::list<Edge>& edges = A[u];
        std::list<Edge>::iterator iterator;
        for ( iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Edge node = *iterator;
            v = node._vertex;
            //Crictical computation and decision
            pthread_mutex_lock(&dist_lock);
            cost = dist[u] + node._weight;
            if (cost < dist[v]) {
                graph_changed = true;
                dist[v] = cost;
            }
            pthread_mutex_unlock(&dist_lock);
        }
    }

    pthread_exit(NULL);
    return NULL;
}
/**
 * Parallel Ford Bellman
 * @A: the graph
 */
void Bellmanford_parallel(Graph& A, const int SOURCE, int NUM_THREADS) {

    assert(NUM_THREADS <= MAX_THREADS);
    pthread_t threads[MAX_THREADS];
    p_thread_parm_t parm[MAX_THREADS];

    //Init Locking
    pthread_mutex_init(&dist_lock, NULL);
    const int N = A.num_nodes();

    /**
     * Allocate memory for passing parameters to the threads
     * Free and initialize them
     */
    for (int t = 0; t < NUM_THREADS; ++t) {
        parm[t] = new thread_parm_t;
        parm[t]->A = &A;
        parm[t]->left = 0;
        parm[t]->right = 0;
    }

    //Determine the number of needed threads
    if (NUM_THREADS > N) {
        NUM_THREADS = N;
    }

    //Determine the workload on each thread
    int RANGE;
    //Check if nodes can be divided up evenly
    if ((N % NUM_THREADS) == 0)
        RANGE = N / NUM_THREADS;
    else
        RANGE = N / (NUM_THREADS - 1);

    /**
     * Running the Program in multiple Threads.
     */
    int rc;
    graph_changed = true;
    int left, right;
    while (graph_changed) {
        graph_changed = false;
        left = 0;
        right = RANGE;
        for (int thread_id = 0; thread_id < NUM_THREADS && left <= right;
                ++thread_id) {
            //Assigning Data to the thread.
            //Marking thread as busy
            right = left + RANGE;
            if ( thread_id == NUM_THREADS - 1)
                right = N;

            parm[thread_id]->left = left;
            parm[thread_id]->right = right;
            parm[thread_id]->thread_id = thread_id;

            rc = pthread_create(&threads[thread_id], NULL, node_relax,
                                (void *) parm[thread_id]);
            if (rc) {
                std::cerr << "ERROR; return code from pthread_create() is "
                          << thread_id << std::endl;
                exit(-1);
            }

            left += RANGE;
        }
        //Wait for all threads to finish
        for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id)
            pthread_join(threads[thread_id], NULL);
    }

    //Clean up data passed to threads
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id)
        delete (parm[thread_id]);
}
#endif // !_FORD_PARALLEL_H
