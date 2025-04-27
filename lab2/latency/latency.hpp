#include <iostream>
#include <cmath>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include "debug_utils.hpp"

namespace latency {

class latency_helper {
    public:
        std::mutex mutex_;
        std::condition_variable cv;
        bool data_ready = false;
        bool response_ready = false;
        int data = 0;
        std::atomic<bool> done = false;
};

void producer(latency_helper& helper, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::unique_lock<std::mutex> lock(helper.mutex_);
        
        helper.data = i;
        helper.data_ready = true;
        
        helper.cv.notify_one();                
        helper.cv.wait(lock, [&]{ return helper.response_ready; });
        helper.response_ready = false;
    }
    helper.done = true;
    helper.cv.notify_one();
}

void consumer(latency_helper& helper, int& result) {
    while (!helper.done) {
        std::unique_lock<std::mutex> lock(helper.mutex_);
        
        helper.cv.wait(lock, [&]{ return helper.data_ready || helper.done; });
        
        if (helper.done) break;
        
        result = helper.data;
        helper.data_ready = false;
        
        helper.response_ready = true;
        helper.cv.notify_one();
    }
}

double measure_communication_time_thread(latency_helper& helper, int iterations) {
    int result = 0;
    
    auto start_time = time_control::chrono_cur_time ();
    
    std::thread producer_thread(producer, std::ref(helper), iterations);
    std::thread consumer_thread(consumer, std::ref(helper), std::ref(result));
    
    producer_thread.join();
    consumer_thread.join();
    
    auto end_time = time_control::chrono_cur_time ();
    
    auto p_time = std::chrono::duration<double>(end_time - start_time).count();
    return p_time / (iterations * 2); 
}

double measure_communication_time_pipe(int iterations) {
    int buf_read[1];
    int buf_write[1];
    int parent_to_child[2]; // p->c
    int child_to_parent[2]; // c->p

    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        std::cerr << "Pipe was not opened\n";
        exit(1);
    }
    auto start_time = time_control::chrono_cur_time ();
    if (pid_t pid = fork(); pid  == 0) {              
        close(parent_to_child[1]);   
        close(child_to_parent[0]);        
        for (int i = 0; i < iterations; i++) {
            buf_write[0] = getpid(); 
            // std::cout << "Child: " << buf_write[0] << std::endl;
            write(child_to_parent[1], buf_write, sizeof(buf_read));
            read(parent_to_child[0], buf_read, sizeof(buf_read)); 
            // std::cout << "parent msg: " << *buf_read << std::endl;
        }             
        exit(0);
    }

    close(parent_to_child[0]);   
    close(child_to_parent[1]); 
    for (int i = 0; i < iterations; i++) {
        read(child_to_parent[0], buf_read, sizeof(buf_read)); 
        // std::cout << "child msg:" << *buf_read << std::endl;
        buf_write[0] = getpid();   
        // std::cout << "Parent: " << buf_write[0] << std::endl; 
        write(parent_to_child[1], buf_write, sizeof(buf_read));
    }    
    auto end_time = time_control::chrono_cur_time ();
    auto p_time = std::chrono::duration<double>(end_time - start_time).count();
    return p_time / (iterations * 2); 
}

}