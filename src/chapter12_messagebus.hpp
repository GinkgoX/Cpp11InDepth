/************************************************************
* @Filename         :chapter12_messagebus.hpp
* @Description      :
* @Date             :2023/07/19 09:09:59
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#pragma once
#include<functional>
#include<unordered_map>
#include<string>
#include<map>
#include"common.hpp"
#include"function_traits.hpp"

using namespace std;

class message_bus: non_copyable 
{
public:
    template<typename F>
    void attach(F&& f, const string& topic="")
    {
        auto func = to_function(std::forward<F>(f));
        add(topic, std::move(func));
    }

    template<typename R>
    void send_request(const string& topic="")
    {
        using function_type = std::function<R()>;
        string message = topic + typeid(function_type).name();
        auto range = m_map.equal_range(message);
        for (m_iter it = range.first; it != range.second; ++it) {
            auto f = it -> second.AnyCast<function_type>();
            f();
        }
    }

    template<typename R, typename... Args>
    void send_request(Args&&... args, const string& topic="")
    {
        using function_type = std::function<R(Args...)>;
        string message = topic + typeid(function_type).name();
        auto range = m_map.equal_range(message);
        for (m_iter it = range.first; it != range.second; ++it) {
            auto f = it -> second.AnyCast<function_type>();
            f(std::forward<Args>(args)...);
        }
    }

    template<typename R, typename... Args>
    void remove(const string& topic="")
    {
        using function_type = std::function<R(Args...)>;
        string message = topic + typeid(function_type).name();
        int count = m_map.count(message);
        auto range = m_map.equal_range(message);
        m_map.erase(range.first, range.second);
    }

private:
    template<typename F>
    void add(const string& topic, F&& f)
    {
        string message = topic + typeid(F).name();
        m_map.emplace(std::move(message), std::forward<F>(f));
    }
    std::multimap<string, Any> m_map;
    typedef std::multimap<std::string, Any>::iterator m_iter;
};

message_bus m_bus;

struct subject
{
public:
    subject() {
        m_bus.attach([this] {driveOK();}, "driveOK");
    }
    void send_request(const string& topic) {
        m_bus.send_request<void, int>(50, topic);
    }
    void driveOK(){
        cout << "driveOK !" << endl;
    }
};

struct car 
{
    car() {
        m_bus.attach([this](int speed){drive(speed);}, "drive");
    }
    void drive(int spped) {
        cout << "car dirve " << endl;
        m_bus.send_request<void>("driveOK");
    }
};

struct bus 
{
    bus() {
        m_bus.attach([this](int speed){drive(speed);}, "drive");
    }
    void drive(int spped) {
        cout << "bus dirve " << endl;
        m_bus.send_request<void>("driveOK");
    }
};

struct truck 
{
    truck() {
        m_bus.attach([this](int speed){drive(speed);});
    }
    void drive(int spped) {
        cout << "truck dirve " << endl;
        m_bus.send_request<void>("driveOK");
    }
};


void test_message_bus() {
    subject m_sub;
    car m_c;
    bus m_b;
    truck m_t;
    const string topic = "drive";
    m_sub.send_request(topic);
    cout << "********" << endl;
    m_sub.send_request("");
}