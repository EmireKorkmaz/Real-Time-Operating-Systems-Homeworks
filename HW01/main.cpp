#include <iostream>
#include <functional>
#include "MyTimer.h"
#include <thread>

using namespace std;
using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = std::function<bool()>;


int main() {
    Millisecs m(3000);
    Timepoint tp = CLOCK::now() + std::chrono::milliseconds(3);
    MyTimer mt;
    mt.registerTimer (tp, [](){
        cout << "time point\n";
    });

    mt.registerTimer (m, [](){
        cout << "milliseconds\n";
    });

    mt.registerTimer (tp, m,[](){
        cout << "time point and milliseconds\n";
    });
    auto f = [](){
        cout << "milliseconds and predicate function\n";
    };
    Timepoint t = CLOCK::now() + std::chrono::seconds(3);
    mt.registerTimer ([t] () -> bool {
                          if((t-CLOCK::now()).count()<0)
                              return true;
                          return false;
                      }
                      , m,f);
    return 0;
}