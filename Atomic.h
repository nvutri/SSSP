/*
 * Atomic.h
 *
 *  Created on: Nov 10, 2012
 *      Author: ttn14
 */

#ifndef ATOMIC_H_
#define ATOMIC_H_

using namespace std;
enum {
    FAIL,
    SUCCESS
};

/**
 * Make sure that the comparision and replacement happens atomically.
 * @return FAIL: If value of x changed mean while updating
 * @return SUCESSS: If value of x was correctly updated.
 * Or there is no need to do so.
 */
int atomic_compare_replace(long* p_x, long& old_x, long& y) {

    if (*p_x == old_x) {
        if (*p_x > y){
            return __sync_bool_compare_and_swap(p_x, old_x, y);
        }
        return SUCCESS;
    }

    return FAIL;
}

/**
 * Implement atomic minimum
 *
 * x = min(x, y);
 * @return true if x got changed
 *         false otherwise
 */


bool atomic_min(long* p_x, long& y) {
    long old_x;
    do {
        old_x = *p_x;
    } while (atomic_compare_replace(p_x, old_x, y) == FAIL);

    return (old_x != *p_x);
}
#endif /* ATOMIC_H_ */

