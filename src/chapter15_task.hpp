#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <future>
#include <algorithm>
#include <vector>
#include <numeric>

using namespace std;

template <typename T>
class Task;

template <typename R, typename... Args>
class Task<R(Args...)>
{
public:
    typedef R return_type;

    template <typename F>
    auto Then(F &&f) -> Task<typename std::result_of<F(R)>::type(Args...)>
    {
        typedef typename std::result_of<F(R)>::type ReturnType;
        auto func = std::move(m_fn);
        return Task<ReturnType(Args...)>([func, &f](Args &&...args){
            std::future<R> lastf = std::async(func, std::forward<Args>(args)...);
            return std::async(f, lastf.get()).get(); 
        });
    }

    Task(std::function<R(Args...)> &&f) : m_fn(std::move(f)) {}
    Task(std::function<R(Args...)> &f) : m_fn(f) {}

    ~Task()
    {
    }

    void Wait()
    {
        std::async(m_fn).wait();
    }

    template <typename... Args>
    R Get(Args &&...args)
    {
        return std::async(m_fn, std::forward<Args>(args)...).get();
    }

    std::shared_future<R> Run()
    {
        return std::async(m_fn);
    }

private:
    std::function<R(Args...)> m_fn;
};

class TaskGroup{
public:
    void Run(Task<void()>&& task) {
        m_void_group.push_back(task.Run());
    }

    template<typename F>
    void Run(F&& f) {
        Run(typename Task<std::result_of<F()>::type()>(std::forward<F>(f)));
    }

    template<typename F, typename... Funs>
    void Run(F&& first, Funs&&... res) {
        Run(std::forward<F>(first));
        Run(std::forward<Funs>(res)...);
    }

    void Wait() {
        for(auto it = m_void_group.begin(); it != m_void_group.end(); ++it) {
            it -> get();
        }
    }

private:
    vector<std::shared_future<void>> m_void_group;
};

template<class Iterator, class Function>
void parallel_foreach(Iterator& begin, Iterator& end, Function& func) {
    auto npart = std::thread::hardware_concurrency();
    auto blocksize = std::distance(begin, end) / npart;
    Iterator last = begin;
    if (blocksize > 0) {
        std::advance(last, (npart - 1) * blocksize);
    } else {
        last = end;
        blocksize = 1;
    }

    std::vector<std::future<void>> futures;
    for (; begin != end; std::advance(begin, blocksize)) {
        futures.emplace_back(std::async([begin, blocksize, &func](){
            std::for_each(begin, begin + blocksize, func);
        }));
    }
    futures.emplace_back(std::async([&begin, &end, &func](){
        std::for_each(begin, end, func);
    }));

    std::for_each(futures.begin(), futures.end(), [](std::future<void>& futuer){futuer.get();});
}

bool check_prime(int x) {
    bool flag = true;
    for (int i = 2; i < x; i++) {
        if (x % i == 0) {
            flag = false;
        }
    }
    if (flag) {
        cout << "prime: " << x << endl;
    }
    return flag;
}

template<typename Range, typename ReduceFunc>
typename Range::value_type parallel_reduce(Range& range, typename Range::value_type& init, ReduceFunc rFunc) {
    return parallel_reduce<Range, ReduceFunc>(range, init, rFunc, rFunc);
}

template<typename Range, typename RangeFunc, typename ReduceFunc>
typename Range::value_type parallel_reduce(Range& range, typename Range::value_type &init, RangeFunc& rangeFunc, ReduceFunc& reduceFunc) {
    auto npart = std::thread::hardware_concurrency();
    auto begin = std::begin(range);
    auto end = std::end(range);
    auto blocksize = std::distance(begin, end) / npart;
    typename Range::iterator last = begin;
    if (blocksize > 0) {
        std::advance(last, (npart - 1) * blocksize);
    } else {
        last = end;
        blocksize = 1;

    }
    typedef typename Range::value_type vType;
    vector<std::future<vType>> futures;
    for (; begin != last; std::advance(begin, blocksize)) {
        futures.emplace_back(std::async([begin, &init, blocksize, &rangeFunc](){
            return rangeFunc(begin, begin + blocksize, init);
        }));
    }
    futures.emplace_back(std::async([&begin, &end, &init, &rangeFunc](){
        return rangeFunc(begin, end, init);
    }));

    vector<vType> results;
    std::for_each(futures.begin(), futures.end(), [&results](std::future<vType>& futuer){
        results.emplace_back(futuer.get());
    });

    return reduceFunc(results.begin(), results.end(), init);
}


void test_task()
{
    // non param in
    cout << "********** test task **********" << endl;
    Task<int()> f([]() { return 0; });
    auto increment = [](int n) { return n + 1; };
    cout << f.Then(increment).Then(increment).Then(increment).Get() << endl;

    // param in
    Task<int()> t([]() {return 32;});
    auto r1 = t.Then([](int result){cout << result << endl;return result+3;}).Then([](int result){cout << result << endl;return result+3;}).Get();
    cout << r1 << endl;
    Task<int(int)> t1([](int i){return i;});
    t1.Then([](int result){return std::to_string(result);}).Then([](const string& str){cout << str << endl; return 0;}).Get(1);
    Task<string(string)> t2([](string str){return str;});
    auto r2 = t2.Then([](const string& str){cout << str.c_str() << endl; return str +" ok";}).Get("task");
    cout << r2 << endl;

    // test group
    cout << "********** test group **********" << endl;
    TaskGroup g;
    std::function<void()> f1 = [] () {cout << "those ";};
    auto f2 = [] () {cout << "are ";};
    g.Run(f1, f2, []() {cout << "tasks." << endl;});
    g.Wait();
}

void test_parallel() {
    cout << "********** test parallel **********" << endl;
    vector<int> v;
    for (int i = 0; i < 10; i++) {
        v.push_back(i + 1);
    }
    // parallel_foreach(v.begin(), v.end(), check_prime);
    int init = 0;
    auto r = parallel_reduce(v, init, [](const vector<int>::iterator& begin, vector<int>::iterator& end, int val) {
        return std::accumulate(begin, end, val);
    });
    cout << "reduce: " << r << endl;
}

