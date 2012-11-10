#ifndef _CHAOTIC_RELAXATION_H
#define _CHAOTIC_RELAXATION_H

#include <pthread.h>
#include <vector>
#include <stdlib.h>
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
static pthread_spinlock_t ct_spin_lock;
static int NUM_THREADS;
/**
 * @return: true if stole some work. Or All Threads are finished.
 * @return: false if unable to steal but there are unfinished threads
 */

//TODO: Have to check if other threads are really finished.
// Empty work list doesn't determine this!

bool steal_work(p_thread_parm_t* parm, thread_parm_t::work_type& my_work){

    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id){
        thread_parm_t::work_type& other_work = parm[thread_id]->work_list;
        if (!other_work.empty()){
            //Start stealing
            for (int i = 0; i < other_work.size() * STEALING_PERCENTAGE; ++i) {
                my_work.push(other_work.top());
                other_work.pop();
            }
            return true;
        }
    }
    return false;
}
/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *chaotic_node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int v;
    double cost;
    Graph& A = *(p->A);
    thread_parm_t::work_type& work = p->work_list;

//    cerr << "thread ID: " << p->thread_id << endl;
//    for (int i = 0; i < work.size(); ++i) {
//        cerr << work[i] << " ";
//    }
//    cerr << endl;

    while (!work.empty()){
        int u = work.top();
        work.pop();

        list<Node>& edges = A[u];
        list<Node>::iterator iterator;

        for ( iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Node node = *iterator;
            v = node._vertex;
            //Crictical computation and decision
            //Acquire Spin Lock
            rb_spin_lock.acquire();

            cost = dist[u] + node._weight;

            if (cost < dist[v]) {
                //Push node v to the work list
                work.push(v);
                dist[v] = cost;
            }
            //Release the Spin Lock
            rb_spin_lock.unlock();

        }

        bool stole = false;
        while  ( work.empty() && !stole){
            stole = steal_work(p->parm, work );
        }
    }

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
    rb_spin_lock = SpinLock();

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
