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
#include <set>

#include "Graph.h"
#include "SpinLock.h"
#include "Thread_Handler.h"
#include "Atomic.h"

using namespace std;

static set<int> bucket;
static SpinLock bucket_lock;
static vector<bool> node_flag;
static int DELTA_FLAG;

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
        //getting work
        work_list_lock.acquire();
        u = work.top();
        work.pop();
        work_list_lock.unlock();

        //getting all the edges
        list<Edge>& edges = A[u];
        list<Edge>::iterator iterator;

        for (iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Edge edge = *iterator;
            v = edge._vertex;
            // light situation
            cost = dist[u] + edge._weight;
            changed = atomic_min(&dist[v], cost);
            if (changed) {
                //Push node v to the work list
                if (dist[v] >= DELTA_MAX){
                    bucket_lock.acquire();
                    if (bucket.count(v) == 0){
                        bucket.insert(v);
                    }
                    bucket_lock.unlock();
                }
                else{
                    work_list_lock.acquire();
                    work.push(v);
                    work_list_lock.unlock();
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

    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        p_thread_parm_t& thread = parm[thread_id];
        thread->A = p_A;
        thread->thread_id = thread_id;
        thread->parm = parm;
        thread->DELTA_MAX = DELTA_MAX;
        thread->busy = false;
    }
    //Initialize the thread_work list
    int thread_id = 0;
    set<int>::iterator it;
    for (it = bucket.begin(); it!=bucket.end(); it++){
        int v = *it;
        if (dist[v] < DELTA_MAX){
            p_thread_parm_t& thread = parm[thread_id];
            thread->work_list.push(v);
            bucket.erase(it);
            thread_id = (thread_id + 1) % NUM_THREADS;
            DELTA_FLAG = 0;
        }
    }
    cout << " Work Size: " <<  parm[0]->work_list.size() << endl;
    //Increase DELTA_FLAG
    DELTA_FLAG++ ;
}

int find_max_edge(Graph&A){
    int N = A.num_nodes();
    int maxEdge = 0;

    for (int u = 0; u < N; ++u) {
        list<Edge>& edges = A[u];
        list<Edge>::iterator iterator;
        for (iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Edge edge = *iterator;
            maxEdge = max(maxEdge, edge._weight);
        }
    }
    return maxEdge;
}
/**
 * Chaotic Relaxation
 * @A: the graph
 */
void Delta_Stepping(Graph& A, const int SOURCE, const int NUM_THREADS,
                    const int DELTA_ALLOWED_TIMES = 1) {
    /**
     * Define threads and parameters for them
     */

    assert(NUM_THREADS <= MAX_THREADS);
    p_thread_parm_t parm[MAX_THREADS];
    pthread_t threads[MAX_THREADS];

    //Init param for threads
    create_threads_storage(parm, NUM_THREADS);

    //Init bucket work list
    bucket.clear();
    bucket.insert(SOURCE);


    const int maxEdge = find_max_edge(A);


    const int DELTA = maxEdge / DELTA_ALLOWED_TIMES + 1;
    cout << "DELTA: " << DELTA << endl;
    int DELTA_MAX = 0;
    int num_runs = 0;
    DELTA_FLAG = 0;

    while (!bucket.empty() && DELTA_FLAG <= DELTA_ALLOWED_TIMES
             && DELTA_MAX < MAX_VALUE ) {
        DELTA_MAX = (num_runs + 1) * DELTA;
        num_runs++;
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
    //Clean up data passed to threads
    delete_threads_data(parm, NUM_THREADS);

}
#endif /* DELTA_STEPPING_H_ */
