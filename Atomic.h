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

bool atomic_min(long& x, long& y){
    if (x > y){
        return __sync_bool_compare_and_swap(&x, x, y);
    }
    return false;
}

#endif /* ATOMIC_H_ */

