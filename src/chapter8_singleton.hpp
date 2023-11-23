/************************************************************
* @Filename         :chapter8_singleton.cpp
* @Description      :
* @Date             :2023/06/07 15:00:08
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/
#pragma once
#include<iostream>
#include<string>

using namespace std;

template<typename T>
class singleton {
 public:
    template<typename... Args>
    static T* instance(Args&&... args) {
        if (!m_obj) {
            m_obj = new T (std::forward<Args>(args)...);
        }
        return m_obj;
    }

    static T* get_instance() {
        if (!m_obj) {
            throw std::logic_error("instance is empty !");
        }
        return m_obj;
    }

    static void destory_instance() {
        delete m_obj;
        m_obj = nullptr;
    }

 private:
    static T* m_obj;
    singleton();
    virtual ~singleton();
    singleton(const singleton&);
    singleton& operator=(const singleton&);
};

template<typename T>
T* singleton<T>::m_obj = nullptr;

struct A {
    explicit A(const string& str) {
        cout << "A's lvaue instance : " << str << endl;
    }

    explicit A(string&& str) {
        cout << "A's rvalue instance : " << str << endl;
    }
};

struct B {
    explicit B(const string& str) {
        cout << "B's lvaue instance : " << str << endl;
    }

    explicit B(string&& str) {
        cout << "B's rvalue instance : " << str << endl;
    }
};

struct  C {
    C(int x, double y) {
        cout << "C's instance : x= " << x << " , y = " << y << endl;
    }

    void func() {
        cout << "test func " << endl;
    }
};


void test_singleton() {
    string str = "singleton";
    singleton<A>::instance(str);
    singleton<B>::instance(std::move(str));
    singleton<C>::instance(1, 3.14);
    singleton<C>::get_instance()->func();

    singleton<A>::destory_instance();
    singleton<B>::destory_instance();
    singleton<C>::destory_instance();
}


// int main() {
//     test();
//     return 0;
// }






