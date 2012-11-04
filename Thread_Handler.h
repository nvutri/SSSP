#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H
//Thread data container
typedef struct {
    Graph* A;
    int thread_id;
    int left;  // left range of nodes
    int right; // right range of nodes
} thread_parm_t;

typedef thread_parm_t* p_thread_parm_t;

/**
 * Allocate memory for passing parameters to the threads
 * Free and initialize them
 */
void init_threads(p_thread_parm_t* parm, const int MAX_THREADS, Graph& A){
    for (int t = 0; t < MAX_THREADS; ++t) {
        parm[t] = new thread_parm_t;
        parm[t]->A = &A;
        parm[t]->left = 0;
        parm[t]->right = 0;
    }
}

void join_threads(pthread_t* threads, const int NUM_THREADS){
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id)
        pthread_join(threads[thread_id], NULL);
}

/**
 * Find a free thread
 */
//int find_thread(p_thread_parm_t* parm, const int NUM_THREADS){
//    //Searching for a free thread
//
//    int thread_id = rand() % NUM_THREADS;
//    while (parm[thread_id]->busy){
//        thread_id = rand() % NUM_THREADS;
//    }
//    return thread_id;
//}

#endif //! _THREAD_HANLDER_H
