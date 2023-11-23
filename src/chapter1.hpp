/*************************************************************************
* @Filename         : chapter1.cpp
* @Description      : 
* @Date             : 2023-03-14 14:58
* @Author           : Maxwell
* @Email            : Maxwell.xie@ovt.com
* @Vesion           : 1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
* ************************************************************************/

#include<iostream>
#include<typeinfo>

using std::cout;
using std::endl;

// 1.1 auto keyword
void test_auto() {
    auto x = 5;                 // int x
    auto pi = new auto(1);      // int* pi
    const auto *v = &x, u = 6;  // const int* v, const int u
    static auto y = 0.0;        // static double y
    auto f = u;                 // int f
    const auto& g = x;          // const int& g
    auto& h = g;                // const int& h
    cout << "auto test: " << endl;
    cout<< "x = " << x << ", pi = " << *pi
        << ", *v = " << *v << ",  u = " << u << ",  y = " << y << endl;
    cout << "g = " << g << ", h = " << h << endl;
}

// 1.2 decltype keyword

class Foo{
 public:
    static const int num = 0;
    int x;
};

void test_decltype() {
    // basic usage
    int x = 1;
    decltype(x) y = 1;          // int y = 1
    decltype(x+y) z = 2;        // int z = 1
    const int& i = x;           // const int i
    decltype(i) j = y;          // const int& j
    const decltype(z)* p = &z;  // const int* p
    decltype(z)* pi = &z;       // int* pi
    decltype(pi)* pp = &pi;     // int** pp

    // advanced usage
    int n = 0;
    volatile const int & x_ = n;
    decltype(n) a = n;          // int a
    decltype(x_) b = n;         // volatile const int&
    decltype(Foo::num) c = n;   // const int c
    Foo foo;
    decltype(foo.x) d = n;      // int d
    cout << "decltype test: " << endl;
    cout << "z = " << z << ", i = " << i << ", j = " << i
        << ", p = " << *p << ", pi = " << *pi << ", pp = " << *pp[0] << endl;
    cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << endl;
}

// decltype for function recall
int& func_int_ref(void);    // lvalue, viewed as address search
int&& func_int_rr(void);    // xvalue
int func_int(void);         // prvalue

const int& func_cint_ref(void);     // lvalue
const int&& func_cint_rr(void);     // rvalue
const int func_cint(void);          // prvalue

const Foo func_cfoo(void);          // prvalue

void test_decltype2() {
    int x = 4;
    decltype(func_int_ref()) a1 = x;    // int&
    decltype(func_int_rr()) b1 = 0;     // int&&
    decltype(func_int()) c1 = 1;        // int

    decltype(func_cint_ref()) d1 = x;   // const int&
    decltype(func_cint_rr()) e1 = 2;    // const int&&
    decltype(func_cint()) f1 = 3;       // int

    decltype(func_cfoo()) f = Foo();       // const Foo
    cout << "a1 = " << a1 << " , b1 = " << b1
        << " , c1 = " << c1 << " , d1 = " << d1 << " , e1 = " << e1 << " , f1 = " << f1 << endl;
    cout <<"decltype(func_int_ref()): " << typeid(a1).name() << endl;
    cout << "decltype(finc_int_rr()): " << typeid(b1).name() << endl;
}

// int main(int argc, char* argv[]) {
//     // test_auto();
//     // test_decltype();
//     test_decltype2();
//     return 0;
// }
