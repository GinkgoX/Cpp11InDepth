/************************************************************
* @Filename         :chapter3_any.cpp
* @Description      :
* @Date             :2023/03/29 10:00:07
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#include<iostream>
#include<memory>
#include<typeindex>
#include<string>
#include<vector>

using namespace std;    //NOLINT[build/namespaces]

struct my_any {
 public:
    my_any(void) :m_tpIndex(type_index(typeid(void))) {}
    my_any(const my_any& that) :m_ptr(that.clone()), m_tpIndex(that.m_tpIndex) {}
    my_any(my_any&& that) :m_ptr(move(that.m_ptr)), m_tpIndex(that.m_tpIndex) {}
    template<typename U, class = typename
    enable_if<!is_same<typename decay<U>::type, my_any>::value, U>::type>
    my_any(U &&value): //NOLINT[runtime/explicit], for implicit conversion
            m_ptr(new derived<typename decay<U>::type>(forward<U>(value))),
            m_tpIndex(type_index(typeid(typename decay<U>::type))) {}

    bool is_null() const {
        return !static_cast<bool>(m_ptr);
    }

    template<class U>
    bool is() const {
        return m_tpIndex == type_index(typeid(U));
    }

    template<class U>
    U& any_cast() {
        if (!is<U>()) {
            cout << "can not cast " << typeid(U).name() << " to " << m_tpIndex.name() << endl;
            throw bad_cast();
        }
        auto d = dynamic_cast<derived<U>*> (m_ptr.get());
        cout << "type_info: " << typeid(U).name() << " to " << m_tpIndex.name() << endl;
        return d->m_value;
    }

    my_any operator=(const my_any& a) {
        if (m_ptr == a.m_ptr) {
            return *this;
        }
        m_ptr = a.clone();
        m_tpIndex = a.m_tpIndex;
        return *this;
    }

    my_any operator=(my_any&& a) {
        if (m_ptr == std::move(a.m_ptr)) {
            return *this;
        }
        m_ptr = std::move(a.clone());
        m_tpIndex = std::move(a.m_tpIndex);
        return *this;
    }

 private:
    struct base;
    typedef unique_ptr<base> baseptr;
    struct base {
        virtual ~base() {}
        virtual baseptr clone() const = 0;
    };

    template<typename T>
    struct derived :public base {
        template<typename U>
        explicit derived(U&& value) :m_value(forward<U>(value)) {}
        baseptr clone() const {
            return baseptr(new derived<T>(m_value));
        }
        T m_value;
    };

    baseptr clone() const {
        if (m_ptr != nullptr) {
            return m_ptr->clone();
        }
        return nullptr;
    }

    baseptr m_ptr;
    type_index m_tpIndex;
};
// int main() {
//     my_any n;
//     cout << n.is_null() << endl;
//     string s1 = "hello";
//     n = s1;
//     my_any n1 = 1;
//     cout << n1.is<int>() << endl;       // true;
//     int i = n1.any_cast<int>();
//     cout << "i: " << i << endl;
//     string s = n.any_cast<string>();
//     cout << "s: " << s << endl;
// }