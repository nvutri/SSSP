#ifndef _CHAOTIC_RELAXATION_H
#define _CHAOTIC_RELAXATION_H

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
#include "Thread_Handler.h"

#define MAX_THREADS 16
#define STEALING_PERCENTAGE 1/2

using namespace std;
//Global Container distance
extern vector<double> dist;

/**
 * Locking for BF parrallel
 */
static SpinLock ct_spin_lock;
static SpinLock work_list_lock;

static int NUM_THREADS;
/**
 * @return: true if stole some work. Or All Threads are finished.
 * @return: false if unable to steal but there are unfinished threads
 */

bool steal_work(p_thread_parm_t* parm, thread_parm_t::work_type& my_work, int my_id){
    int v;
    bool exists_busy_threads = false;

    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id){
        thread_parm_t::work_type& other_work = parm[thread_id]->work_list;
        if (parm[thread_id]->busy){
            //Start stealing
            exists_busy_threads = true;
            work_list_lock.acquire();
            for (unsigned i = 0; i < other_work.size() * STEALING_PERCENTAGE; ++i) {

                v = other_work.top();
//                fprintf(stderr, "%d stole %d: %d \n", my_id, thread_id, v);
                my_work.push(v);
                other_work.pop();
            }
//            fprintf(stderr," %d Done Stealing \n", my_id);
//            for (unsigned i = 0; i< my_work.size(); ++i){
//                fprintf(stderr, " %d " , my_work.top());
//                my_work.pop();
//            }
//            fprintf(stderr," %d Done  \n", my_id);
            work_list_lock.unlock();
            return true;
        }
    }

    return !exists_busy_threads;
}
/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *chaotic_node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int u, v;
    double cost;
    Graph& A = *(p->A);
    thread_parm_t::work_type& work = p->work_list;

    p->busy = true;

    while (!work.empty()){

        work_list_lock.acquire();
        u = work.top();
        work.pop();
        work_list_lock.unlock();

        list<Node>& edges = A[u];
        list<Node>::iterator iterator;

        for ( iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Node node = *iterator;
            v = node._vertex;
            //Crictical computation and decision
            //Acquire Spin Lock
            ct_spin_lock.acquire();

            cost = dist[u] + node._weight;

            if (cost < dist[v]) {
                //Push node v to the work list
                work_list_lock.acquire();
                work.push(v);
                work_list_lock.unlock();

                dist[v] = cost;
            }
            //Release the Spin Lock
            ct_spin_lock.unlock();

        }

        bool stole = false;
        while  ( work.empty() && !stole){
            stole = steal_work(p->parm, work, p->thread_id );
        }
    }

    p->busy = false;
    pthread_exit(NULL);
    return NULL;
}

void init_thread_data(p_thread_parm_t* parm, Graph* const p_A, const int num_threads){
    int left, right;
    const int N = p_A->num_nodes();
    //Determine the workload on each thread
    NUM_THREADS = num_threads;
    int RANGE = (int) ceil( (float) N / (float) NUM_THREADS );

    //Initialize the thread_work list
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        left = thread_id * RANGE;
        right = min (left + RANGE, N);

        p_thread_parm_t& thread = parm[thread_id];

        thread->A = p_A;
        thread->thread_id = thread_id;
        thread->parm = parm;
        thread->busy = false;
        //Initialize the nodes on the thread in simply sequential order
        for (int node = left, index = 0;
                node < right;
                ++node, ++index) {
            thread->work_list.push(node);
        }
        left = right;
    }

}

/**
 * Chaotic Relaxation
 * @A: the graph
 */
void Chaotic_Relaxation(Graph& A, const int SOURCE, const int NUM_THREADS) {
    /**
     * Define threads and parameters for them
     */
    assert(NUM_THREADS <= MAX_THREADS);
    p_thread_parm_t parm[MAX_THREADS];
    pthread_t threads[MAX_THREADS];

    //Init Spin Locking
    ct_spin_lock = SpinLock();
    work_list_lock = SpinLock();

    //Init param for threads
    create_threads_storage(parm, NUM_THREADS);
    init_thread_data(parm, &A, NUM_THREADS);

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

    //Clean up data passed to threads
    delete_threads_data(parm, NUM_THREADS);

}

#endif // !_CHAOTIC_RELAXATION_H
