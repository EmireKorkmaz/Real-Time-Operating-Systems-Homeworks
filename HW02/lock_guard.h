//
// Created by emire on 13.12.2019.
//

#ifndef HW2_LOCK_GUARD_H
#define HW2_LOCK_GUARD_H

namespace gtu {
    #include "mutex.h"

    template< class mutex >
    class lock_guard {
    public:
        lock_guard(mutex m){
            this->mtx = m;
            gtu::subs++;
            mtx.lock();
        }

        ~lock_guard() {
            gtu::subs--;
            mtx.unlock();
        }

    private:
        mutex mtx;
    };
}


#endif //HW2_LOCK_GUARD_H
