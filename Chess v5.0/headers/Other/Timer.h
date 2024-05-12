#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
    Timer() : start_time(std::chrono::steady_clock::now()) {}
    ~Timer() {
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        std::cout << "Timer elapsed: " << duration.count() << " seconds" << std::endl;
    }
};


#endif // !TIMER_H



