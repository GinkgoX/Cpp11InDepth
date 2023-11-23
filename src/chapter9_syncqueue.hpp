#include<iostream>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<list>

using namespace std;

template<typename T>
class sync_queue {
 public:
   sync_queue(int max):m_max_size(max), m_need_stop(false) {}
   void put(const T& x) {
      add(x);
   }

   void put(T&& x) {
      add(std::forward<T>(x));
   }

   void take(std::list<T>& list) {
      std::unique_lock<std::mutex> locker(m_mutex);
      m_not_empty.wait(locker, [this] {return m_need_stop || not_empty();});
      if(m_need_stop) {
         return;
      }
      list = std::move(m_queue);
      m_not_full.notify_one();
   }

   void take(T& t) {
      std::unique_lock<std::mutex> locker(m_mutex);
      m_not_empty.wait(locker, [this] { return m_need_stop || not_empty();});
      if(m_need_stop) {
         return;
      }
      t = m_queue.front();
      m_queue.pop_front();
      m_not_full.notify_one();
   }

   void stop() {
      {
         std::lock_guard<std::mutex> locker(m_mutex);
         m_need_stop = true;
      }
      m_not_empty.notify_all();
      m_not_full.notify_all();
   }

   bool empty() {
      std::lock_guard<std::mutex> locker(m_mutex);
      return m_queue.empty();
   }

   bool full() {
      std::lock_guard<std::mutex> locker(m_mutex);
      return m_queue.size() == m_max_size;
   }

   size_t size() {
      std::lock_guard<std::mutex> locker(m_mutex);
      return m_queue.size();
   }

   int count() {
      return m_queue.size();
   }

 private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_not_empty;
    std::condition_variable m_not_full;
    int m_max_size;
    bool m_need_stop;

    bool not_full() const {
      bool full = m_queue.size() >= m_max_size;
      if(full) {
         cout << "buffer is full, need wait ... " << endl;
      }
      return !full;
    }

    bool not_empty() const {
      bool empty = m_queue.empty();
      if(empty) {
         cout << "buffer is empty, need wait ..., async thred: "<< this_thread::get_id() << endl;
      }
      return !empty;
    }

    template<typename F>
    void add(F&& x) {
      std::unique_lock<std::mutex> locker(m_mutex);
      m_not_full.wait(locker, [this] {return m_need_stop || not_full();});
      if(m_need_stop) {
         return;
      }
      m_queue.push_back(std::forward<F>(x));
      m_not_empty.notify_one();
    }
}; 















