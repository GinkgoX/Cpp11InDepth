/************************************************************
* @Filename         :chapter5_thread.cpp
* @Description      :
* @Date             :2023/04/17 18:05:22
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#include<thread>
#include<iostream>
#include<functional>
#include<string>
using namespace std;

void func(int i, double d, const std::string& s) {
    cout << "func: " << i <<", " << d << ", " << s << endl;
}

void test_func() {
    std::cout << "test" << std::endl;
}

// int main() {
//     std::thread t(func, 0, 1.1, "string");
//     std::thread t1(std::bind(func, 1, 0.1, "bind"));
//     test();
//     t.join();
//     t1.join();
//     return 0;
// }



