/*
 * SpinLock.h
 *
 *  Created on: Nov 9, 2012
 *      Author: ttn14
 */

#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#define NUM_TRIES 10
class SpinLock{
  private:
    bool _locked; // represent the state of this lock
    bool* _lock; // the key to the lock
  public:
    //Init lock to not locked state
    SpinLock() : _locked(false), _lock(&_locked){
    }
    // timeout if neccessary
    bool acquire(float timeout=0){
        //TODO: Change to a while loop
        bool acquired = false;

        while (!acquired) {
            acquired = lock();
        }

        return acquired;
    }
    /**
     * release the acquired lock
     * unlocking
     */
    bool unlock(){
        return __sync_bool_compare_and_swap(_lock, true, false);
    }

    /**
     * locking
     */
    bool lock(){
        return __sync_bool_compare_and_swap(_lock, false, true);
    }

};


#endif /* SPINLOCK_H_ */
