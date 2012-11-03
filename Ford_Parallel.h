#ifndef _FORD_PARALLEL_H
#define _FORD_PARALLEL_H

#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cassert>

#include "Thread_Handler.h"

#define MAX_THREADS 16

//Global Container distance
extern std::vector<double> dist;

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
    int num_edge, v;
    double cost;
    Graph& A = *(p->A);
    const int N = A.num_nodes();

    for (int u = 0; u < N; ++u) {
        num_edge = A.num_edges(u);
        for (int e = 0; e < num_edge; ++e) {
            v = A.vertex(u, e);
            //Crictical computation and decision
            pthread_mutex_lock(&dist_lock);
            cost = dist[u] + A(u, e);
            if (cost < dist[v]) {
                dist[v] = cost;
            }
            pthread_mutex_unlock(&dist_lock);
        }
    }

    p->busy = false;
    pthread_exit(NULL);
    return NULL;
}
/**
 * Parallel Ford Bellman
 * @A: the graph
 */
void Bellmanford_parallel(Graph& A, const int SOURCE, const int NUM_THREADS) {

    assert(NUM_THREADS <= MAX_THREADS);
    pthread_t threads[MAX_THREADS];
    p_thread_parm_t parm[MAX_THREADS];

    //Init Locking
    pthread_mutex_init(&dist_lock, NULL);


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
#endif // !_FORD_PARALLEL_H
