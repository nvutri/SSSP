#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H

//Thread data container
typedef struct thread_parm_t thread_parm_t;
typedef thread_parm_t* p_thread_parm_t;

struct thread_parm_t{
    Graph* A;
    int thread_id;
    int left;  // left range of nodes
    int right; // right range of nodes

    //Work List Container
    typedef std::priority_queue<int, std::vector<int>, CompareNode> work_type;
    work_type work_list; // threads' local work_list

    //Delta To Limit Work load on
    int DELTA_MAX;
    //Access to other threads data
    p_thread_parm_t *parm;

    //Thread Busy flag
    bool busy;
};



/**
 * Allocate memory for passing parameters to the threads
 * Free and initialize them
 */
void create_threads_storage(p_thread_parm_t* parm, const int NUM_THREADS){
    for (int t = 0; t < NUM_THREADS; ++t) {
        parm[t] = new thread_parm_t;
    }
}

/**
 * Free the threads data
 */
void delete_threads_data(p_thread_parm_t* parm, const int NUM_THREADS){
    for (int thread_id = 0; thread_id < NUM_THREADS; ++thread_id)
        delete (parm[thread_id]);

}

/**
 * Thread barrier
 */
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
