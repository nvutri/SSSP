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

static priority_queue<int, vector<int>, CompareNode> bucket;
static SpinLock bucket_lock;
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
    const int DELTA_MAX = p->DELTA_MAX;
    while (!work.empty()) {

        u = work.top();
        work.pop();

        list<Node>& edges = A[u];
        list<Node>::iterator iterator;

        for (iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Node node = *iterator;
            v = node._vertex;
            // light situation
            cost = dist[u] + node._weight;
            changed = atomic_min(&dist[v], cost);
            if (changed) {
                //Push node v to the work list
                if (dist[v] < DELTA_MAX){
                    work.push(v);
                }
                else{
                    bucket_lock.acquire();
                    bucket.push(v);
                    bucket_lock.unlock();
                }
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
                            const int num_threads, const int DELTA_MAX) {

    //Determine the workload on each thread
    int NUM_THREADS = num_threads;
//    cerr << "Delta = " << DELTA_MAX << endl;

    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        p_thread_parm_t& thread = parm[thread_id];
        thread->A = p_A;
        thread->thread_id = thread_id;
        thread->parm = parm;
        thread->DELTA_MAX = DELTA_MAX;
        thread->busy = false;
    }
//    dist_print(p_A->num_nodes());
    //Initialize the thread_work list
    int thread_id = 0;
    while (!bucket.empty() && dist[bucket.top()] < DELTA_MAX) {
        int v = bucket.top();
        p_thread_parm_t& thread = parm[thread_id];
//        cerr << "Thread: " << thread_id << " | " << v << endl;
        thread->work_list.push(v);
        bucket.pop();
        thread_id = (thread_id + 1) % NUM_THREADS;
    }
}

int maxEdge(Graph&A){
    int N = A.num_nodes();
    int maxWeight = 0;

    for (int u = 0; u < N; ++u) {
        list<Node>& edges = A[u];
        list<Node>::iterator iterator;
        for (iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Node node = *iterator;
            if (node._weight > maxWeight)
                maxWeight = node._weight;
        }
    }

    return maxWeight;
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

    bucket.push(SOURCE);

    const int DELTA = maxEdge(A);
    int DELTA_MAX = 0;
    int i_d = 0;
    while (!bucket.empty() && DELTA_MAX < MAX_VALUE ) {
        DELTA_MAX = (i_d + 1) * DELTA;
        i_d++;
        init_delta_thread_data(parm, &A, NUM_THREADS, DELTA_MAX);

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
//    cerr << "NUM STEPS: " << i_d << endl;
//Clean up data passed to threads
    delete_threads_data(parm, NUM_THREADS);

}
#endif /* DELTA_STEPPING_H_ */
