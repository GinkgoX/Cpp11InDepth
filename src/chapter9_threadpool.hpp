#include<list>
#include<thread>
#include<memory>
#include<atomic>
#include"chapter9_syncqueue.hpp"

using Task = std::function<void()>;

const int max_tasks = 10;
class threadpool
{
private:
    std::list<std::shared_ptr<std::thread>> m_groups;
    sync_queue<Task> m_queue;
    atomic_bool m_runing;
    std::once_flag m_flag;

    void start(int threads) {
        m_runing = true;
        for(int i = 0; i < threads; i++) {
            m_groups.push_back(std::make_shared<std::thread>(&threadpool::run, this));
        }
    }

    void run() {
        while (m_runing) {
            std::list<Task> list;
            m_queue.take(list);

            for(auto& task: list) {
                if (!m_runing) {
                    return;
                }
                task();
            }
        }
    }

    void stop_groups() {
        m_queue.stop();
        m_runing = false;
        for(auto thread: m_groups) {
            if(thread) {
                thread -> join();
            }
        }
        m_groups.clear();
    }

public:
    threadpool(int threads = std::thread::hardware_concurrency()):m_queue(max_tasks){
        start(threads);
    }

    ~threadpool() {
        stop();
    }

    void stop() {
        std::call_once(m_flag, [this] {stop_groups();});
    }

    void add_task(Task&& task) {
        m_queue.put(std::forward<Task>(task));
    }

    void add_task(const Task& task) {
        m_queue.put(task);
    }

};


void test_threadpool() {
    threadpool pool(2);
    std::thread thd1([&pool] {
        for (int i = 0; i < 3; i++) {
            auto thdID = this_thread::get_id();
            pool.add_task([thdID] {cout << "thread #1 : " << thdID << endl;});
        }
    });

    std::thread thd2([&pool] {
        for (int i = 0; i < 3; i++) {
            auto thdID = this_thread::get_id();
            pool.add_task([thdID] {cout << "thread #2 : " << thdID << endl; });
        }
    });

    this_thread::sleep_for(std::chrono::seconds(2));
    getchar();
    pool.stop();
    thd1.join();
    thd2.join();
}