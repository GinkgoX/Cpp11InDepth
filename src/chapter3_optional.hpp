/************************************************************
* @Filename         :chapter3.cpp
* @Description      :
* @Date             :2023/03/23 09:33:30
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#include<type_traits>
#include<iostream>
#include<string>
#include<map>

using std::cout;
using std::endl;
using std::string;
using std::map;

template<typename T>
class Optional{
    using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type; // NOLINT[build/c++11]

 public:
    Optional() : m_hasInit(false) {}
    explicit Optional(const T& v) {
        Create(v);
    }

    explicit Optional(T&& v) : m_hasInit(false) {
        Create(std::move(v));
    }

    ~Optional() {
        Destroy();
    }

    Optional(const Optional& other) : m_hasInit(false) {
        if (other.IsInit())
            Assign(other);
    }

    Optional(Optional&& other) : m_hasInit(false) {
        if (other.IsInit()) {
            Assign(std::move(other));
            other.Destroy();
        }
    }

    Optional& operator=(Optional &&other) {
        Assign(std::move(other));
        return *this;
    }

    Optional& operator=(const Optional &other) {
        Assign(other);
        return *this;
    }

    template<class... Args>
    void emplace(Args&&... args) {
        Destroy();
        Create(std::forward<Args>(args)...);
    }

    bool IsInit() const { return m_hasInit; }

    explicit operator bool() const {
        return IsInit();
    }

    T& operator*() {
        if (IsInit()) {
            return *(reinterpret_cast<T*>(&m_data));
        }

        throw std::logic_error("is not init");
    }

    T const& operator*() const {
        if (IsInit()) {
            return *(reinterpret_cast<const T*>(&m_data));
        }

        throw std::logic_error("is not init");
    }

    bool operator == (const Optional<T>& rhs) const {
        return (!static_cast<bool>(*this)) != (!rhs) ? false
            : (!static_cast<bool>(*this) ? true : (*(*this)) == (*rhs));
    }

    bool operator < (const Optional<T>& rhs) const {
        return !rhs ? false : (!static_cast<bool>(*this) ? true : (*(*this) < (*rhs)));
    }

    bool operator != (const Optional<T>& rhs) {
        return !(*this == (rhs));
    }

 private:
    template<class... Args>
    void Create(Args&&... args) {
        new (&m_data) T(std::forward<Args>
            (args)...);
        m_hasInit = true;
    }

    void Destroy() {
        if (m_hasInit) {
            m_hasInit = false;
            (reinterpret_cast<T*>(&m_data))->~T();
        }
    }

    void Assign(const Optional& other) {
        if (other.IsInit()) {
            Copy(other.m_data);
            m_hasInit = true;
        } else {
            Destroy();
        }
    }

    void Assign(Optional&& other) {
        if (other.IsInit()) {
            Move(std::move(other.m_data));
            m_hasInit = true;
            other.Destroy();
        } else {
            Destroy();
        }
    }

    void Move(data_t&& val) {
        Destroy();
        new (&m_data) T(std::move(*(reinterpret_cast<T*>(&val))));
    }

    void Copy(const data_t& val) {
        Destroy();
        new (&m_data) T(*(reinterpret_cast<const T*>(&val)));
    }

 private:
    bool m_hasInit;
    data_t m_data;
};

class MyClass{
 public:
    MyClass(int a, int b) :x_(a), y_(b) { }
    void print() {
        cout << "x_ = " << x_ << endl;
        cout << "y_ = " << y_ << endl;
    }
 private:
    int x_;
    int y_;
};
void TestOptional() {
    Optional<string> a("ok");
    Optional<string> b("ok");
    Optional<string> c("aa");
    c = a;

    if (c < a)
        cout << '<' << endl;

    if (a == b)
        cout << '=' << endl;

    map<Optional<string>, int> mymap;
    mymap.insert(std::make_pair(a, 1));
    mymap.insert(std::make_pair(c, 2));

    auto it = mymap.find(a);
    cout << it->second << endl;

    Optional<MyClass> d;
    d.emplace(10, 20);
    (*d).print();
}

// int main() {
//     TestOptional();
//     return 0;
// }