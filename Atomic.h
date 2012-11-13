/*
 * Atomic.h
 *
 *  Created on: Nov 10, 2012
 *      Author: ttn14
 */

#ifndef ATOMIC_H_
#define ATOMIC_H_

using namespace std;
/**
 * Implement atomic minimum
 * x = min(x, y);
 * @return true if x got changed
 *         false otherwise
 */
enum {
    FAIL,
    SUCCESS
};

int atomic_compare_replace(long* p_x, long& x, long& y) {

    if (*p_x == x) {
        if (*p_x > y){
            return __sync_bool_compare_and_swap(p_x, x, y);
        }
        return SUCCESS;
    }

    return FAIL;
}

bool atomic_min(long* p_x, long& y) {
    long x;
    do {
        x = *p_x;
    } while (atomic_compare_replace(p_x, x, y) == FAIL);

    return (x != *p_x);
}
#endif /* ATOMIC_H_ */

