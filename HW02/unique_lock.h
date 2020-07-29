//
// Created by emire on 15.12.2019.
//

#ifndef HW2_UNIQUE_LOCK_H
#define HW2_UNIQUE_LOCK_H

#include "mutex.h"

namespace gtu {
    template <class mutex>
    class unique_lock {
    public:
        mutex mtx;
        unique_lock(mutex m) {
            this->mtx = m;
            gtu::subs++;
            mtx.lock();
        }
        void lock(){
            mtx.lock();
        }

        void unlock(){
            gtu::subs--;
            mtx.unlock();
        }
    };
}

#endif //HW2_UNIQUE_LOCK_H
