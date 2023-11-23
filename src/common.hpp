/************************************************************
* @Filename         :chapter3_any.cpp
* @Description      :
* @Date             :2023/03/29 10:00:07
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#pragma once
#include<iostream>
#include<memory>
#include<typeindex>
#include<string>
#include<vector>
#include<tuple>
#include<functional>

using namespace std;

class non_copyable{
 protected:
    non_copyable() = default;
    ~non_copyable() = default;
    non_copyable(const non_copyable&) = delete;             // not allowed copy
    non_copyable& operator=(const non_copyable&) = delete;  // not allowed assign
};


struct Any
{
    Any(void) : m_tpIndex(std::type_index(typeid(void))){}
    Any(const Any& that) : m_ptr(that.Clone()), m_tpIndex(that.m_tpIndex) {}
    Any(Any && that) : m_ptr(std::move(that.m_ptr)), m_tpIndex(that.m_tpIndex) {}

    //auto pointer
    template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type> Any(U && value) : m_ptr(new Derived < typename std::decay<U>::type>(forward<U>(value))),
        m_tpIndex(type_index(typeid(typename std::decay<U>::type))){}

    bool IsNull() const { return !bool(m_ptr); }

    template<class U> bool Is() const
    {
        return m_tpIndex == type_index(typeid(U));
    }

    //Any cast
    template<class U>
    U& AnyCast()
    {
        if (!Is<U>())
        {
            cout << "can not cast " << typeid(U).name() << " to " << m_tpIndex.name() << endl;
            throw bad_cast();
        }

        auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
        return derived->m_value;
    }

    Any& operator=(const Any& a)
    {
        if (m_ptr == a.m_ptr)
            return *this;

        m_ptr = a.Clone();
        m_tpIndex = a.m_tpIndex;
        return *this;
    }

private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;

    struct Base
    {
        virtual ~Base() {}
        virtual BasePtr Clone() const = 0;
    };

    template<typename T>
    struct Derived : Base
    {
        template<typename U>
        Derived(U && value) : m_value(forward<U>(value)) { }

        BasePtr Clone() const
        {
            return BasePtr(new Derived<T>(m_value));
        }

        T m_value;
    };

    BasePtr Clone() const
    {
        if (m_ptr != nullptr)
            return m_ptr->Clone();

        return nullptr;
    }

    BasePtr m_ptr;
    std::type_index m_tpIndex;
};