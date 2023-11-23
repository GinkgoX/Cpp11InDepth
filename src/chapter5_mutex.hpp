/************************************************************
* @Filename         :chapter5_mutex.cpp
* @Description      :
* @Date             :2023/04/18 14:09:39
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/
#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>

std::mutex g_lock;

void func() {
    g_lock.lock();
    std::cout <<  "enter thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "leaving thread " << std::this_thread::get_id() << std::endl;
    g_lock.unlock();
}

// int main() {
//     std::thread t1(func);
//     std::thread t2(func);
//     std::thread t3(func);
//     t1.join();
//     t2.join();
//     t3.join();
//     return 0;
// }











