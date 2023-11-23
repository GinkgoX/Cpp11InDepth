/************************************************************
* @Filename         :chapter11_IoC.hpp
* @Description      :
* @Date             :2023/07/12 09:08:57
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/
#pragma once
#include<functional>
#include<unordered_map>
#include"common.hpp"

using namespace std;

class ioc_container: public non_copyable {
public:
    ioc_container(void) {}
    ~ ioc_container(void) {}

    template<class T, typename Depend, typename... Args>
    typename std::enable_if<!std::is_base_of<T, Depend>::value>::type register_type(const string& key) {
        std::function<T* (Args...)> function = [] (Args... args) {return new T(new Depend(args...)); };
        register_type(key, function);
    }

    template<class T, typename Depend, typename... Args>
    typename std::enable_if<std::is_base_of<T, Depend>::value>::type register_type(const string& key) {
        std::function<T* (Args...)> function = [] (Args... args) {return new Depend(args...); };
        register_type(key, function);
    }

    template<class T, typename... Args>
    void register_simple(const string& key) {
        std::function<T* (Args...)> function = [] (Args... args) {return new T(args...); };
        register_type(key, function);
    }

    template<class T, typename... Args>
    T* resolve(const string& key, Args... args) {
        auto iter = ioc_map.find(key);
        if (iter == ioc_map.end()) {
            return nullptr;
        }
        Any resolver = iter -> second;
        std::function<T* (Args...)> function = resolver.AnyCast<std::function<T* (Args...)>>();
        return function(args...);
    }

    template<class T, typename... Args>
    std::shared_ptr<T> resolve_shared(const string& key, Args... args) {
        T* t = resolve<T>(key, args...);
        return std::shared_ptr<T>(t);
    }

private:
    void register_type(const string& key, Any ctor) {
        if (ioc_map.find(key) != ioc_map.end()) {
            throw std::invalid_argument("this key already exist!");
        }
        ioc_map.emplace(key, ctor);
    }

    unordered_map<string, Any> ioc_map;
};


struct base
{
    virtual void func() {cout << "func" << endl;}
    virtual ~base() {}
};

struct derivedB: public base
{
    derivedB(int a, double b): m_a(a), m_b(b) {

    }
    void func() override {
        cout << "add: " << m_a + m_b << endl;
    }
private:
    int m_a;
    double m_b;
};

struct derivedC: public base
{
};

struct A
{
    A(base* ptr): m_ptr(ptr) {}
    void func() {
        m_ptr ->func();
    }
    ~A() {
        if(!m_ptr) {
            delete m_ptr;
            m_ptr = nullptr;
        }
    }
private:
    base* m_ptr;
};

struct interface_ioc
{
    virtual void func() = 0;
    virtual ~interface_ioc() {};
};

struct derivedE: public interface_ioc
{
    void func() override {
        cout << "func in derivedE" << endl;
    }
};

struct derivedF: public interface_ioc
{
    void func() override {
        cout << "func in derivedF" << endl;
    }
};




void test_IoC() {
    ioc_container ioc;
    // register for inherence
    ioc.register_type<A, derivedC>("C");
    auto c = ioc.resolve_shared<A>("C");
    c ->func();
    ioc.register_type<A, derivedB, int, double>("B");
    auto b = ioc.resolve_shared<A>("B", 1, 2.0);
    b -> func();

    // register for interface
    ioc.register_type<interface_ioc, derivedE>("E");
    ioc.register_type<interface_ioc, derivedF>("F");
    auto pb = ioc.resolve_shared<interface_ioc>("E");
    pb->func();
    pb = ioc.resolve_shared<interface_ioc>("F");
    pb->func();
}



