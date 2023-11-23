/************************************************************
* @Filename         :chapter8_visitor.cpp
* @Description      :
* @Date             :2023/06/08 10:41:15
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/
#pragma once
#include<iostream>
#include<string>

using namespace std;


template<typename... Types>
struct visitor;



template<typename T, typename... Types>
struct visitor<T, Types...>: public visitor<Types...> {
    using visitor<Types...>::visit;
    virtual void visit(const T&) = 0;
};

template<typename T>
struct visitor<T>{
    virtual void visit(const T&) = 0;
};

struct stA;
struct  stB;

struct base {
    typedef visitor<stA, stB> m_visitor;
    virtual void accept(m_visitor&) = 0;
};

struct stA: public base {
    double val;
    void accept(base::m_visitor& v) {
        v.visit(*this);
    }
};

struct stB: public base {
    int val;
    void accept(base::m_visitor& v) {
        v.visit(*this);
    }
};

struct  print_visitor: public base::m_visitor
{
    void visit(const stA& a) {
        cout << "from stA: " << a.val << endl;
    }

    void visit(const stB& b) {
        cout << "from stB: " << b.val << endl;
    }
};

void test_visitor() {
    print_visitor vis;
    stA a;
    a.val = 3.1415926;
    stB b;
    b.val = 3;
    base* bs = &a;
    bs->accept(vis);
    bs = &b;
    bs->accept(vis);
}

// int main() {
//     test();
//     return 0;
// }








