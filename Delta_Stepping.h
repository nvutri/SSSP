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

//static deque<int> node_list;
/**
 * Init Thread Data in Delta Stepping Style
 */
void init_delta_thread_data(p_thread_parm_t* parm, Graph* const p_A,
                      const int num_threads,
                      const int MIN_VAL,
                      const int MAX_VAl){

//    const int N = node_list.size();
    const int N = p_A->num_nodes();
    //Determine the workload on each thread
    NUM_THREADS = num_threads;

    int RANGE = (int) ceil( (float) N / (float) NUM_THREADS );

    //Initialize the thread_work list
    int left, right;
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        left = thread_id * RANGE;
        right = min (left + RANGE, N);

        p_thread_parm_t& thread = parm[thread_id];

        thread->A = p_A;
        thread->thread_id = thread_id;
        thread->parm = parm;
        thread->busy = false;

//        deque<int>::iterator it;
        //Initialize the nodes on the thread in simply sequential order
//        for (it = node_list.begin(); it != node_list.end(); it++){
//            int node = *it;
//                node_list.erase(it);
        for (int node = left; node < right; ++node)
            if (MIN_VAL <= dist[node] && dist[node] < MAX_VAl) {
                thread->work_list.push(node);
            }
        left = right;
    }

}

/**
 * Chaotic Relaxation
 * @A: the graph
 */
void Delta_Stepping(Graph& A, const int SOURCE, const int NUM_THREADS) {
    /**
     * Define threads and parameters for them
     */
    const int DELTA = 100;
    const int num_steps = 10;
    assert(NUM_THREADS <= MAX_THREADS);
    p_thread_parm_t parm[MAX_THREADS];
    pthread_t threads[MAX_THREADS];

    //Init Spin Locking
    work_list_lock = SpinLock();

    //Init param for threads
    create_threads_storage(parm, NUM_THREADS);

    //Create a list of nodes
//    node_list.clear();
//    for (int i = 0; i < A.num_nodes(); ++i) {
//        node_list.push_back(i);
//    }

    int min_val, max_val;
    for (int i_d = 0; i_d < num_steps ; ++i_d) {
        min_val  = i_d * DELTA;
        max_val = (i_d + 1) * DELTA;
        init_delta_thread_data(parm, &A, NUM_THREADS, min_val, max_val);

        /**
         * Start All Threads
         */
        int rc = 0;
        for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
            rc = pthread_create(&threads[thread_id], NULL, chaotic_node_relax,
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
