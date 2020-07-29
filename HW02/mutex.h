//
// Created by emire on 14.12.2019.
//

#ifndef HW2_MUTEX_H
#define HW2_MUTEX_H

#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>
#include <atomic>

using namespace std;

namespace gtu {
    struct type{
        int id;
        int priority;
    };
    static atomic<int> current;
    static bool isLocked = false;
    static int highestPriority = 0;
    static vector<type > threads;
    static int subs = 0;

    class mutex : public std::mutex {
    public:

        mutex() noexcept{
        }

        mutex& operator=(mutex const&m){
        }

        ~mutex(){}


        void registerThread(struct type t){
            threads.push_back(t);
        }

        bool checkRegistry(struct type t){
            for(auto value: threads) {
                if (value.id == t.id)
                    return true;
            }
            throw runtime_error("Thread should have been registered before locking!");;
        }

        void lock() {
            while(isLocked)
                ;
            int next = rand() % threads.size();
            if(current == threads.at(next).id)
                isLocked = true;
            else
                while(isLocked)
                    ;
        }

        void unlock() {
            isLocked = false;
        }

        bool try_lock(int priority, struct type t) {
            if(checkRegistry(t)){
                if(!isLocked)
                    return true;
                if(highestPriority <= priority) {
                    return true;
                }
                while(isLocked)
                    ;
                return true;
            }
            return false;
        }


    };
}

#endif //HW2_MUTEX_H
