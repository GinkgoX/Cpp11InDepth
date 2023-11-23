/************************************************************
* @Filename         :chapter8_objectpool.cpp
* @Description      :
* @Date             :2023/06/14 09:39:50
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#include<iostream>
#include<string>
#include<functional>
#include<map>
#include"common.hpp"

using namespace std;

const int MAXOBJECTNUM = 10;

template<typename T>
class object_pool : public non_copyable {
    template<typename... Args>
    using constructor = std::function<std::shared_ptr<T>(Args...)>;
 public:
    template<typename... Args>
    void init(size_t num, Args&&... args) {
        if (num <= 0 || num > MAXOBJECTNUM) {
            throw std::logic_error("object num out of range. ");
        }
        auto construct_name = typeid(constructor<Args...>).name();
        for (size_t i = 0; i < num; i++) {
            m_obj_map.emplace(construct_name, 
                shared_ptr<T>(new T(std::forward<Args>(args)...), 
                    [this, construct_name](T* p){m_obj_map.emplace(std::move(construct_name), std::shared_ptr<T>(p));
                }));
        }
    }

    template<typename... Args>
    std::shared_ptr<T> get() {
        string construct_name = typeid(constructor<Args...>).name();
        auto range = m_obj_map.equal_range(construct_name);
        for (auto it = range.first; it != range.second; ++it) {
            auto ptr = it -> second;
            m_obj_map.erase(it);
            return ptr;
        }
        return nullptr;

    }

 private:
    multimap<string, std::shared_ptr<T>> m_obj_map;
};

struct big_obj {
    big_obj () { cout << "default constructor " << endl; }
    big_obj (int val) { cout << "f(int)" << endl; }
    big_obj (const int& x, const int& y) { cout << "f(x, y)" << endl; }
    void print(const string& str) {
        cout << "print(str): " << str << endl;
    }
};

void print(shared_ptr<big_obj> p, const string& str) {
    if (p) {
        p -> print(str);
    }
}

void test_objectpool() {
    object_pool<big_obj> pool;
    pool.init(2);
    {
        auto p = pool.get();
        print(p, "p");
        auto p2 = pool.get();
        print(p2, "p2");
    }
    auto p = pool.get();
    auto p2 = pool.get();
    print(p, "p");
    print(p2, "p2");
    pool.init(2, 1);
    auto p3 = pool.get<int>();
    print(p3, "p3");
    pool.init(2, 1, 2);
    auto p4 = pool.get<int, int>();
    print(p4, "p4");
    auto p5 = pool.get<int>();
    print(p5, "p5");
}












