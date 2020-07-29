#include <iostream>
#include <thread>
#include "mutex.h"

struct threadType {
    int priority;
    int id;
    gtu::mutex mutex;
    gtu::mutex mutex2;
};

// lock_guard
void threadFunctionLG(struct threadType *t)
{
    try{
        gtu::type mytype;
        mytype.priority=t->priority;
        mytype.id = t->id;

        if(t->mutex.try_lock(t->priority, mytype)) {
            // critical section preparation

            gtu::current = t->id;
            std::lock_guard<gtu::mutex> l(t->mutex);
            std::lock_guard<gtu::mutex> l2(t->mutex2);
            int temp = t->priority;
            t->priority = gtu::highestPriority;

            // end of CS prep

            // CRITICAL SECTION  PS: You can change it with other deadlock cases

            cout << endl;
            cout << "Priority   " << "Counter" << endl;
            for (int i = 0; i < 5; i++) {
                cout << temp << "          " << i << endl;
                if(t->priority < gtu::highestPriority)
                    break;
            }
            t->priority = temp;

            // END OF CRITICAL SECTION

            l.~lock_guard();
            l2.~lock_guard();
            t->mutex.unlock();
            t->mutex2.unlock();
        }
    }
    catch (exception &e){
        cout<<"An exception caught : "<<e.what()<<"\n";
    }
}

// unique_lock
void threadFunctionUL(struct threadType *t)
{
    try{
        gtu::type mytype;
        mytype.priority=t->priority;
        mytype.id = t->id;

        if(t->mutex.try_lock(t->priority, mytype)) {
            // critical section preparation

            gtu::current = t->id;
            std::unique_lock<gtu::mutex> l(t->mutex2);
            std::unique_lock<gtu::mutex> l2(t->mutex);
            int temp = t->priority;
            t->priority = gtu::highestPriority;

            // end of CS prep

            // CRITICAL SECTION  PS: You can change it with other deadlock cases

            cout << endl;
            cout << "Priority   " << "Counter" << endl;
            for (int i = 0; i < 5; i++) {
                cout << temp << "          " << i << endl;
                if(t->priority < gtu::highestPriority)
                    break;
            }
            t->priority = temp;

            // END OF CRITICAL SECTION

            l.unlock();
            l2.unlock();
        }
    }
    catch (exception &e){
        cout<<"An exception was caught : "<< e.what()<<"\n";
    }
}

int main() {

    // created threadType for both of the threads
    // gave each a random priority
    struct threadType t1;
    t1.priority = rand() % 10;
    struct threadType t2;
    t2.priority = rand() % 10;

    int max = t1.priority;

    max = (t2.priority > max) ? t2.priority : max;

    gtu::mutex m;
    gtu::mutex m2;

    gtu::highestPriority = max;

    // two mutexes will be created for testing deadlock case.
    t1.mutex = m;
    t2.mutex = m;

    t1.mutex2 = m2;
    t2.mutex2 = m2;

    thread th1;
    t1.id = 1;
    gtu::type type1;
    type1.priority = t1.priority;
    type1.id = t1.id;
    m.registerThread(type1);
    th1 = thread(threadFunctionLG, &t1);

    thread th2;
    t2.id = 2;
    gtu::type type2;
    type2.priority = t2.priority;
    type2.id = t2.id;
    m.registerThread(type2);
    th2 = thread(threadFunctionUL, &t2);

    th1.join();
    th2.join();

    return 0;
}