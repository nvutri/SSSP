/*
 * Atomic.h
 *
 *  Created on: Nov 10, 2012
 *      Author: ttn14
 */

#ifndef ATOMIC_H_
#define ATOMIC_H_
using namespace std;
bool atomic_min(int& x, int& y){
    if (x > y){
        return __sync_bool_compare_and_swap(&x, x, y);
    }
    return false;
}

#endif /* ATOMIC_H_ */
