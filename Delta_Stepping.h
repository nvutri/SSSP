/*
 * Delta_Stepping.h
 *
 */

#ifndef _DELTA_STEPPING_H_
#define _DELTA_STEPPING_H_

#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <iostream>

#include "Graph.h"
#include "SpinLock.h"
#include "Thread_Handler.h"
#include "Atomic.h"

using namespace std;

static vector<int> bucket;
/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *delta_node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int u, v;
    long cost;
    bool changed;
    Graph& A = *(p->A);
    thread_parm_t::work_type& work = p->work_list;

    p->busy = true;

    while (!work.empty()) {

        work_list_lock.acquire();
        u = work.top();
        work.pop();
        work_list_lock.unlock();

        list<Node>& edges = A[u];
        list<Node>::iterator iterator;

        for (iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Node node = *iterator;
            v = node._vertex;
            // light situation
            if (node._weight < p->DELTA_MAX) {
                cost = dist[u] + node._weight;
                changed = atomic_min(&dist[v], cost);
                if (changed) {
                    //Push node v to the work list
                    work_list_lock.acquire();
                    work.push(v);
                    work_list_lock.unlock();
                }
            } else {
                // heavy situation
                bucket.push_back(v);
            }

        }
    }

    bool stole = false;
    while (work.empty() && !stole) {
        stole = steal_work(p->parm, work, p->thread_id);
    }

    p->busy = false;
    pthread_exit(NULL);
    return NULL;
}

/**
 * Init Thread Data in Delta Stepping Style
 */
void init_delta_thread_data(p_thread_parm_t* parm, Graph* const p_A,
                            const int num_threads, const int DELTA_MIN,
                            const int DELTA_MAX) {

    const int N = bucket.size();
    //Determine the workload on each thread
    NUM_THREADS = num_threads;

    int RANGE = (int) ceil((float) N / (float) NUM_THREADS);

    //Initialize the thread_work list
    int left, right;
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        left = thread_id * RANGE;
        right = min(left + RANGE, N);

        p_thread_parm_t& thread = parm[thread_id];

        thread->A = p_A;
        thread->thread_id = thread_id;
        thread->parm = parm;
        thread->DELTA_MAX = DELTA_MAX;
        thread->busy = false;

        for (int node = left; node < right; ++node) {
            thread->work_list.push(bucket[node]);
        }
        left = right;
    }

    bucket.clear();
}
/**
 * Chaotic Relaxation
 * @A: the graph
 */
void Delta_Stepping(Graph& A, const int SOURCE, const int NUM_THREADS) {
    /**
     * Define threads and parameters for them
     */

    assert(NUM_THREADS <= MAX_THREADS);
    p_thread_parm_t parm[MAX_THREADS];
    pthread_t threads[MAX_THREADS];

//Init Spin Locking
    work_list_lock = SpinLock();

//Init param for threads
    create_threads_storage(parm, NUM_THREADS);

    bucket.push_back(SOURCE);

    const int DELTA = 1;
    int DELTA_MIN, DELTA_MAX;
    int i_d = 0;
    while (!bucket.empty()) {
        DELTA_MIN = i_d * DELTA;
        DELTA_MAX = (i_d + 1) * DELTA;
        init_delta_thread_data(parm, &A, NUM_THREADS, DELTA_MIN, DELTA_MAX);

        /**
         * Start All Threads
         */
        int rc = 0;
        for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
            rc = pthread_create(&threads[thread_id], NULL, delta_node_relax,
                                (void *) parm[thread_id]);
            if (rc) {
                cerr << "ERROR; return code from pthread_create() is "
                     << thread_id << endl;

                delete_threads_data(parm, NUM_THREADS);
                exit(-1);
            }
        }

        /**
         * Wait for all threads to finish
         */
        join_threads(threads, NUM_THREADS);

    }
//Clean up data passed to threads
    delete_threads_data(parm, NUM_THREADS);

}
#endif /* DELTA_STEPPING_H_ */
