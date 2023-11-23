/************************************************************
* @Filename         :chapter10_AOP.hpp
* @Description      :
* @Date             :2023/07/07 16:12:21
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#include<iostream>
#include<string>
#include<ctime>
#include"common.hpp"

using namespace std;

#define HAS_MEMBER(member)\
template<typename T, typename... Args>struct has_member_##member\
{\
private:\
        template<typename U> static auto check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
    template<typename U> static std::false_type check(...);\
public:\
    enum{value = std::is_same<decltype(check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(foo)
HAS_MEMBER(before)
HAS_MEMBER(after)

template<typename Func, typename... Args>
class aspect: public non_copyable {
public:
    aspect(Func&& func):m_func(std::forward<Func>(func)) { };

    template<typename T>
    typename std::enable_if<has_member_before<T, Args...>::value && has_member_after<T, Args...>::value>::type invoke(Args&&... args, T&& aspect) {
        aspect.before(std::forward<Args>(args)...);
        m_func(std::forward<Args>(args)...);
        aspect.after(std::forward<Args>(args)...);
    }

    template<typename T>
    typename std::enable_if<has_member_before<T, Args...>::value && !has_member_after<T, Args...>::value>::type invoke(Args&&... args, T&& aspect) {
        aspect.before(std::forward<Args>(args)...);
        m_func(std::forward<Args>(args)...);
    }

    template<typename T>
    typename std::enable_if<!has_member_before<T, Args...>::value && has_member_after<T, Args...>::value>::type invoke(Args&&... args, T&& aspect) {
        m_func(std::forward<Args>(args)...);
        aspect.after(std::forward<Args>(args)...);
    }

    template<typename Head, typename... Tail>
    void invoke(Args&&... args, Head&& head_aspect, Tail&&... tail_aspect) {
        head_aspect.before(std::forward<Args>(args)...);
        invoke(std::forward<Args>(args)..., std::forward<Tail>(tail_aspect)...);
        head_aspect.after(std::forward<Args>(args)...);
    }

private:
    Func m_func;
};

template<typename T> using identity_t = T;

template<typename... AP, typename... Args, typename Func>
void invoke(Func&& f, Args&&... args) {
    aspect<Func, Args...> ap(std::forward<Func>(f));
    ap.invoke(std::forward<Args>(args)..., identity_t<AP>()...);
}

class time_elapse {
public:
    void before(int i) {
        cout << i << endl;
    }
    void after(int i) {
        cout << "time elapsed: " << i << endl;
    }
};

class logger {
public:
    void before(int i) {
        cout << "entering " << endl;
    }
    void after(int i) {
        cout << "leavling " << endl;
    }
};

void foo(int a) {
    cout << "real HT function: " << a << endl;
}


void test_AOP() {
    invoke<logger, time_elapse>(&foo, 1);
    cout << "----------------------" << endl;
    invoke<time_elapse, logger>(&foo, 1);
}

















