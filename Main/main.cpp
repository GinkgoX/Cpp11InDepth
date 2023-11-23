#include<iostream> 
using namespace std;

// #include"chapter8_singleton.hpp"
// #include"chapter8_visitor.hpp"
// #include"chapter8_commander.hpp"
// #include"chapter8_objectpool.hpp"
// #include"chapter9_threadpool.hpp"
// #include"chapter10_AOP.hpp"
// #include"chapter11_IoC.hpp"
// #include"../src/chapter12_messagebus.hpp"
// #include"../src/chapter13_sqlite.hpp"
#include"../src/chapter15_task.hpp"

int main(){
    // cout << "test AOP in chapter 10" << endl;
    // test_AOP();
    // cout << "test IoC in chapter 11" << endl;
    // test_IoC();
    // cout << "test message bus in chapter 12" << endl;
    // test_message_bus();
    // cout << "test sqlite in chapter 13" << endl;
    // test_sqlite();
    cout << "test task in chapter 15" << endl;
    test_task();
    test_parallel();
    return 0;
}