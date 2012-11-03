#ifndef _THREAD_HANDLER_H
#define _THREAD_HANDLER_H
//Thread data container
typedef struct {
    Graph* A;
    int thread_id;
    bool busy;
} thread_parm_t;

typedef thread_parm_t* p_thread_parm_t;


/**
 * Find a free thread
 */
int find_thread(p_thread_parm_t* parm, const int NUM_THREADS){
    //Searching for a free thread

    int thread_id = rand() % NUM_THREADS;
    while (parm[thread_id]->busy){
        thread_id = rand() % NUM_THREADS;
    }
    return thread_id;
}

#endif //! _THREAD_HANLDER_H
