/************************************************************
* @Filename         :chapter8_commander.hpp
* @Description      :
* @Date             :2023/06/12 13:59:06
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/
#include<iostream>
#include<string>
#include<functional>
#include<type_traits>

using namespace std;

template<typename R=void>
struct command
{
 private:
    std::function<R()> m_f;

 public:
    // function warpper for callable object
    template<class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
    void warp(F&& f, Args&&... args) {
        m_f = [&] {return f(args...);};
    }

    // function warpper for const member function
    template<class R, class C, class... DArgs, class P, class... Args>
    void warp(R(C::*f)(DArgs...) const, P&& p, Args&&... args) {
        m_f = [&, f] {return (*p.*f)(args...);};
    }
    
    // function warpper for non-const member function
    template<class R, class C, class... DArgs, class P, class... Args>
    void warp(R(C::*f)(DArgs...), P&& p, Args&&... args) {
        m_f = [&, f] {return (*p.*f)(args...);};
    }

    R excute() {
        return m_f();
    }

};

struct stCMD
{
    int m_val;
    int operator() (){return m_val;}
    int operator() (int n) {return m_val + n;}
    int triple0() {
        return m_val * 3;
    }
    int triple(int a) {
        return m_val * 3 + a;
    }
    int triple1() const {
        return m_val * 3;
    }
    const int triple2(int a) const {
        return m_val * 3 + a;
    }

    void triple3() {
        cout << " triple3 " << endl;
    }
};

int add_one(int n) {
    return n + 1;
}

void test_commander() {
    command<int> cmd;
    // general function
    cmd.warp(add_one, 0);
    // lambda expression
    cmd.warp([](int n) {return n+1;}, 1);

    stCMD st = {10};
    int x = 3;

    // member function
    cmd.warp(&stCMD::triple0, &st);
    cmd.warp(&stCMD::triple, &st, x);
    cout << cmd.excute() << endl;
    cmd.warp(&stCMD::triple, &st, 3);

    cmd.warp(&stCMD::triple2, &st, 3);
    cout << cmd.excute() << endl;

}





