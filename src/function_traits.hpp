#pragma once
#include<iostream>
#include<memory>
#include<typeindex>
#include<string>
#include<vector>
#include<tuple>
#include<functional>

using namespace std;

template<typename T>
struct function_traits;

template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{
public:
    enum {arity = sizeof...(Args)};
    typedef Ret function_type(Args...);
    typedef Ret return_type;
    using stl_function_type = std::function<function_type>;
    typedef Ret(*pointer)(Args...);

    template<size_t I>
    struct args
    {
        static_assert(I < arity, "index is out of range, index must less than sizeof Args");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

// function pointer
template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)>: function_traits<Ret(Args...)> {};

// std::function
template<typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>>: function_traits<Ret(Args...)>{};

// member function
#define FUNCTION_TRAITS(...)\
    template<typename Ret_Type, typename Class_Type, typename... Args>\
    struct function_traits<Ret_Type(Class_Type::*)(Args...)__VA_ARGS__>:\
    function_traits<Ret_Type(Args...)> {};

FUNCTION_TRAITS();
FUNCTION_TRAITS(const);
FUNCTION_TRAITS(volatile);
FUNCTION_TRAITS(const volatile);

// function object
template<typename Callable>
struct function_traits: function_traits<decltype(&Callable::operator())>{};

// lvalue lambda
template<typename Function>
auto to_function(const Function& lambda) {
    return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
}

// rvalue lambda
template<typename Function>
auto to_function(Function &&lambda) {
    return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
}

// function pointer
template<typename Function>
auto to_function_pointer(const Function &lambda) {
    return static_cast<typename function_traits<Function>::pointer>(lambda);
}
