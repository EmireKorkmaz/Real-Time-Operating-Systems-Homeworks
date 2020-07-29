//
// Created by emire on 20.11.2019.
//

#ifndef HW1_MYTIMER_H
#define HW1_MYTIMER_H

#include <thread>
#include <condition_variable>
#include "ITimer.h"

using namespace std;
using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using cond = std::condition_variable;
using mut = std::mutex;

class MyType {
public:

    Timepoint tp;
    Millisecs period;
    TTimerCallback cb;
    TPredicate pred;
};


class MyTimer : public ITimer {
public:

    MyTimer() : t(&MyTimer::thread_function, this){
    }
    ~MyTimer(){
        t.join();
    }
    virtual void registerTimer(const Timepoint &tp, const TTimerCallback &cb){
        MyType m;
        m.tp = tp;
        m.cb = cb;
        array[0]=m;
    }
    virtual void registerTimer(const Millisecs & period, const TTimerCallback &cb){
        MyType m;
        m.period=period;
        m.cb = cb;
        array[1]=m;
    }
    virtual void registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb){
        MyType m;
        m.period=period;
        m.tp = tp;
        m.cb = cb;
        array[2]=m;
    }
    virtual void registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb){
        MyType m;
        m.period=period;
        m.cb = cb;
        m.pred = pred;
        array[3]=m;
    }
    void thread_function(){

        for(;;){
            if(i==0){
                i=1;
                Timepoint tp = array[0].tp;
                for(;;){
                    array[0].cb();

                    if((tp-CLOCK::now()).count()<0) {
                        cout << "The given time has ended." << endl;
                        break;
                    }
                }
                continue;
            }
            else if(i==1){
                i=2;
                Millisecs period = array[1].period;
                mut m;
                cond cv;
                std::unique_lock<std::mutex> lock(m);
                Timepoint t = CLOCK::now() + period;

                for(;;){
                    array[1].cb();
                    if (cv.wait_for(lock,period)==std::cv_status::timeout) {
                        std::cout << "Waits for " << period.count() << " ms" << std::endl;
                    }
                }
            }
            else if(i==2){
                i=3;
                Millisecs period = array[2].period;
                Timepoint tp = array[2].tp;
                mut m;
                cond cv;
                std::unique_lock<std::mutex> lock(m);
                Timepoint t = CLOCK::now() + period;
                chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

                for(;;){
                    if((tp-CLOCK::now()).count()<0) {
                        cout << "The given time has ended." << endl;
                        break;
                    }
                    if (cv.wait_for(lock,period)==std::cv_status::timeout) {
                        std::cout << "Waits for " << period.count() << " ms" << std::endl;
                    }
                }
                continue;
            }
            else if(i==3){
                i=4;
                i=2;
                Millisecs period = array[3].period;
                mut m;
                cond cv;
                std::unique_lock<std::mutex> lock(m);
                Timepoint t = CLOCK::now() + period;

                for(;;){
                    if(array[3].pred()) {
                        cout << "Predicate function's condition has been met.\n";
                        break;
                    }
                    array[3].cb();
                    if (cv.wait_for(lock,period)==std::cv_status::timeout) {
                        std::cout << "Waits for " << period.count() << " ms" << std::endl;
                    }
                }
            }

        }
    }

private:
    MyType array[4];
    int i=0;
    std::thread t;
};

#endif //HW1_MYTIMER_H
