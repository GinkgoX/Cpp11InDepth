/************************************************************
* @Filename         :chapter8_observer.cpp
* @Description      :
* @Date             :2023/06/07 15:47:55
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#include<iostream>
#include<string>
#include<map>
#include<functional>
#include"common.hpp"

using namespace std;

template<typename Func>
class events:public non_copyable {
 public:
    events() {}
    ~events() {}
    // rvalue reference
    int connect(Func&& f) {
        return assign(f);
    }
    // lvalue reference
    int connect(const Func& f) {
        return assign(f);
    }

    void disconnect(int key) {
        m_connections.erase(key);
    }

    template<typename... Args>
    void notify(Args&&... args) {
        for (auto& it : m_connections) {
            it.second(std::forward<Args>(args)...);
        }
    }

 private:
    int m_id = 0;
    std::map<int, Func> m_connections;

    template<typename F>
    int assign(F&& f) {
        int k = m_id++;
        m_connections.emplace(k, std::forward<F>(f));
        return k;
    }
};

struct sA {
    int a, b;
    void print(int a, int b) {
        cout << "struct A : " <<  a << " , " << b << endl;
    }
};

void print(int a, int b) {
    cout << "print : " << a << " , " << b << endl;
}


void test_observer() {
    events<std::function<void(int, int)>> my_event;
    auto key = my_event.connect(print);

    sA sa;

    // lambda function
    auto lambda_key = my_event.connect(
            [&sa](int a, int b) {
                sa.a = a;
                sa.b = b;
                cout << "struct A : " <<  sa.a << " , " << sa.b << endl;
            });

    // functional object
    std::function<void(int, int)> f = std::bind(&sA::print, &sa, std::placeholders::_1, std::placeholders::_2);
    auto func_key = my_event.connect(f);
    int a = 1, b = 2;
    my_event.notify(a, b);
    my_event.disconnect(key);
    a = 3;
    my_event.notify(a, b);
}

// int main() {
//     test();
//     return 0;
// }


