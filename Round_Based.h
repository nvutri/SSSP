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
std::deque<int> work;

/**
 * Locking for BF parrallel
 */
pthread_spinlock_t rb_spin_lock;

/**
 * Relax all edges surrounding node u
 * @param u: source node to relax around
 */
void *rb_node_relax(void *parm) {
    p_thread_parm_t p = (p_thread_parm_t) parm;
    int v;
    double cost;
    Graph& A = *(p->A);
    const int LEFT = p->left;
    const int RIGHT = p->right;

    for (int i = LEFT; i < RIGHT; ++i) {
        int u = work[i];
        std::list<Node>& edges = A[u];
        std::list<Node>::iterator iterator;
        for ( iterator = edges.begin(); iterator != edges.end(); ++iterator) {
            Node node = *iterator;
            v = node._vertex;
            //Crictical computation and decision
            //Acquire Spin Lock
            pthread_spin_lock(&rb_spin_lock);

            cost = dist[u] + node._weight;
            if (cost < dist[v]) {
                //Push node v to the work list
                work.push_back(v);
                dist[v] = cost;
            }
            //Release the Spin Lock
            pthread_spin_unlock(&rb_spin_lock);

        }
    }

    pthread_exit(NULL);
    return NULL;
}

void assign_jobs(p_thread_parm_t* parm, Graph& A, int N, int NUM_THREADS) {
    pthread_t threads[MAX_THREADS];

    //Determine the workload on each thread
    int RANGE;
    if ((N % NUM_THREADS) == 0)
        RANGE = N / NUM_THREADS;
    else
        RANGE = N / (NUM_THREADS - 1);

    /**
     * Running the Program in multiple Threads.
     */
    int rc = 0;
    int right;

    for (int thread_id = 0, left = 0; thread_id < NUM_THREADS; ++thread_id) {
        right = left + RANGE;
        if (thread_id == NUM_THREADS - 1)
            right = N;

        parm[thread_id]->A = &A;
        parm[thread_id]->left = left;
        parm[thread_id]->right = right;
        parm[thread_id]->thread_id = thread_id;

        rc = pthread_create(&threads[thread_id], NULL, rb_node_relax,
                            (void *) parm[thread_id]);
        if (rc) {
            std::cerr << "ERROR; return code from pthread_create() is "
                      << thread_id << std::endl;

            delete_threads_data(parm, NUM_THREADS);
            exit(-1);
        }
        left += RANGE;
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
void Round_Based(Graph& A, const int SOURCE, const int NUM_THREADS) {

    //Resize work to fit
    const int NUM_NODE = A.num_nodes();
    work.resize(2 * NUM_NODE, 0);

    assert(NUM_THREADS <= MAX_THREADS);
    p_thread_parm_t parm[MAX_THREADS];

    //Init Spin Locking to 0
    pthread_spin_init(&rb_spin_lock, 0);

    //Init param for threads
    create_threads_data(parm, NUM_THREADS);

    //Start out with 1 node;
    work.clear();
    work.push_back(SOURCE);

    while (!work.empty()) {
        const int N = work.size();
        //Determine the number of needed threads
        const int USING_THREADS = std::min(N, NUM_THREADS);
        assign_jobs(parm, A, N, USING_THREADS);

        //Clear out the old items deque
        work.erase(work.begin(), work.begin() + N);
    }

    //Clean up data passed to threads
    delete_threads_data(parm, NUM_THREADS);

    //Destroy lock
    pthread_spin_destroy(&rb_spin_lock);
}

/**
 * Dump out the work load
 */
void print_work() {
    for (unsigned int i = 0; i < work.size(); ++i) {
        std::cerr << work[i] << " ";
    }
    std::cerr << std::endl;
}
#endif
