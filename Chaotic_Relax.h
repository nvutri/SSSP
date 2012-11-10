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

using namespace std;
//Global Container distance
extern vector<double> dist;
static vector<int> ct_work;

/**
 * Locking for BF parrallel
 */
static pthread_spinlock_t ct_spin_lock;

/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *chaotic_node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int v;
    double cost;
    Graph& A = *(p->A);
    vector<int>::iterator it_work, begin, end;
    begin = p->work_list.begin();
    end = p->work_list.end();

    for (it_work = begin; it_work != end; ++it_work){
        int u = *it_work;
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
                work.push_back(v);
                dist[v] = cost;
            }
            //Release the Spin Lock
            rb_spin_lock.unlock();

        }
    }

    pthread_exit(NULL);
    return NULL;
}

/**
 * Assign jobs to THREADS.
 * @param parm: parameter data for each thread.
 * @param A: the Graph.
 * @param N: the size of work load. This will be divided up to all threads.
 * @param NUM_THREADS: number of threads to be used.
 */
void ct_assign_jobs(p_thread_parm_t* parm, Graph& A, int N, int NUM_THREADS) {
    pthread_t threads[MAX_THREADS];

    //Determine the workload on each thread
    int RANGE = (int) ceil( (float)N / (float)NUM_THREADS );

    /**
     * Running the Program in multiple Threads.
     */
    int rc = 0;
    //Iterator to access work list
    vector<int>::iterator left, right;
    left = work.begin();

    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id) {
        right = left + RANGE;

        parm[thread_id]->A = &A;
        parm[thread_id]->thread_id = thread_id;
        //Initialize the thread_work list
        parm[thread_id]->work_list = vector<int>(left, right );

        rc = pthread_create(&threads[thread_id], NULL, chaotic_node_relax,
                            (void *) parm[thread_id]);
        if (rc) {
            cerr << "ERROR; return code from pthread_create() is "
                      << thread_id << endl;

            delete_threads_data(parm, NUM_THREADS);
            exit(-1);
        }
        left = right;
    }

    /**
     * Wait for all threads to finish
     */
    join_threads(threads, NUM_THREADS);
}

/**
 * Parallel Ford Bellman Round Based
 * @A: the graph
 */
void Chaotic_Relaxation(Graph& A, const int SOURCE, const int NUM_THREADS) {

    //Resize work to fit. Maximumly need size of 2* NUM_NODE
    const int NUM_NODE = A.num_nodes();
    work.resize(min(2 * NUM_NODE, A.num_edges()), 0);

    assert(NUM_THREADS <= MAX_THREADS);
    p_thread_parm_t parm[MAX_THREADS];

    //Init Spin Locking
    rb_spin_lock = SpinLock();

    //Init param for threads
    create_threads_data(parm, NUM_THREADS);

    //Start out with 1 node;
    work.clear();
    work.push_back(SOURCE);

    while (!work.empty()) {
        const int N = work.size();
        //Determine the number of needed threads
        const int USING_THREADS = min(N, NUM_THREADS);

        /* Assign jobs to all the threads */
        ct_assign_jobs(parm, A, N, USING_THREADS);

        //Clear out the old items deque
        work.erase(work.begin(), work.begin() + N);
    }

    //Clean up data passed to threads
    delete_threads_data(parm, NUM_THREADS);

}

#endif // !_CHAOTIC_RELAXATION_H
