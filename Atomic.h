/*
 * Atomic.h
 *
 *  Created on: Nov 10, 2012
 *      Author: ttn14
 */

#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <cstdatomic>
using namespace std;
/**
 * Implement atomic minimum
 * x = min(x, y);
 * @return true if x got changed
 *         false otherwise
 */
//bool atomic_min(long& x, const long& y){
//    long EXPECTED = 0;
//    long MIN_VALUE = -2147483648;
//    /**
//     * Atomic variables
//     * cmp: atomic comparision
//     * changed : atomic boolean
//     */
//
//    atomic_long cmp;
//    atomic_bool changed;
//
//    // cmp = x - y
//    cmp.store(x);
//    cmp.fetch_sub(y);
//    cmp.fetch_and(MIN_VALUE);
//
//    // TODO: check memory operation
//    // if (cmp == 0) -> cmp = y
//    changed.store(cmp.compare_exchange_strong( EXPECTED, y));
//    // if (cmp == MIN_VALUE) -> cmp = x
//    cmp.compare_exchange_strong( MIN_VALUE , x);
//    // x = cmp
//    __sync_bool_compare_and_swap(&x, x, cmp);
//    // return whether x value is changed
//    return changed;
//}


bool atomic_min(long& x, long& y){
    if (x > y){
        return __sync_bool_compare_and_swap(&x, x, y);
//        return true;
    }
    return false;
}

#endif /* ATOMIC_H_ */

