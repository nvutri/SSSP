#ifndef _ROUND_BASED_H
#define _ROUND_BASED_H

#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include <algorithm>



#include "Thread_Handler.h"

#define MAX_THREADS 16

//Global Container distance
extern std::vector<double> dist;
std::deque<int> work(1000);

/**
 * Locking for BF parrallel
 */
pthread_mutex_t rb_dist_lock;

/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *rb_node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int num_edge, v;
    double cost;
    Graph& A = *(p->A);
    const int LEFT = p->left;
    const int RIGHT = p->right;

    std::cerr << LEFT << " " << RIGHT << " "
              << p->thread_id << std::endl;
    for (int i = LEFT; i < RIGHT; ++i) {
        int u = work[i];

        num_edge = A.num_edges(u);
        for (int e = 0; e < num_edge; ++e) {
            v = A.vertex(u, e);
            //Crictical computation and decision
            pthread_mutex_lock(&rb_dist_lock);
            cost = dist[u] + A(u, e);
            if (cost < dist[v]) {
//                std::cerr << "relaxing " << v;
                work.push_back(v);
                dist[v] = cost;
            }
            pthread_mutex_unlock(&rb_dist_lock);
        }
    }

    pthread_exit(NULL);
    return NULL;
}


void assign_jobs(p_thread_parm_t* parm, pthread_t* threads, int N, int NUM_THREADS){

    //Determine the workload on each thread
    int RANGE;
    //Check if nodes can be divided up evenly
    if ((N % NUM_THREADS) == 0)
        RANGE = N / NUM_THREADS;
    else
        RANGE = N / (NUM_THREADS - 1);

    int rc;
    int left, right;
    left = 0;
    right = RANGE;
    /**
     * Running the Program in multiple Threads.
     */

    //TODO: Divide the work loads to threads evenly based on work.size()
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        right = left + RANGE;
        if ( thread_id == NUM_THREADS - 1)
            right = N;

        parm[thread_id]->left = left;
        parm[thread_id]->right = right;
        parm[thread_id]->thread_id = thread_id;

        rc = pthread_create(&threads[thread_id], NULL, rb_node_relax,
                            (void *) parm[thread_id]);
        if (rc) {
            std::cerr << "ERROR; return code from pthread_create() is "
                      << thread_id << std::endl;
            exit(-1);
        }

        left += RANGE;
    }
}
/**
 * Parallel Ford Bellman
 * @A: the graph
 */
void Round_Based(Graph& A, const int SOURCE, const int NUM_THREADS) {
    const int NUM_NODE = A.num_nodes();
//    work = std::deque<int>(2*NUM_NODE);

    assert(NUM_THREADS <= MAX_THREADS);
    p_thread_parm_t parm[MAX_THREADS];
    pthread_t threads[MAX_THREADS];

    //Init Locking
    pthread_mutex_init(&rb_dist_lock, NULL);

    //Init param for threads
    init_threads(parm, MAX_THREADS, A);

    //Start out with 1 node;
    work.clear();
    work.push_back(SOURCE);

    while (!work.empty()) {
        const int N = work.size();
        //Determine the number of needed threads
        assign_jobs(parm, threads, N, std::min(N, NUM_THREADS));

        join_threads(threads, NUM_THREADS);
        //Wait for all threads to finish
        //Clear out the old items deque
        for (int i = 0; i < N; ++i)
            work.pop_front();
    }

    //Clean up data passed to threads
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id)
        delete (parm[thread_id]);
}

#endif
