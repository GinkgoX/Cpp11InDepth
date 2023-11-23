/************************************************************
* @Filename         :chapter8_objpool.hpp
* @Description      :
* @Date             :2023/06/14 10:53:40
* @Author           :Maxwell
* @Email            :Maxwell.xie@ovt.com
* @Version          :1.0
* @ Copyright (c) 2023 Maxwell Xie. All rights reserved
************************************************************/

#include<vector>
#include<memory>

template <typename T>
class obj_pool {
 public:
    obj_pool() {}

    template <typename... Args>
    obj_pool(int num, Args... args) {
        create(num, std::forward<Args>(args)...);
    }
    ~obj_pool() {
        destory();
    }

    class wrapper {
     public:
        template <typename... Args>
        wrapper(Args... args) {
            cout << "create obj: " << this << endl;
            m_obj = std::make_shared<T>(std::forward<Args>(args)...);
            m_is_used = false;
        }
        ~wrapper() {
            cout << "destory obj: " << this << endl;
        }

        void set_used(bool used) {
            m_is_used = used;
        }

        bool is_used() {
            return m_is_used;
        }

        std::shared_ptr<T> get() {
            return m_obj;
        }
     private:
        std::shared_ptr<T> m_obj;
        bool m_is_used;
    };

    template<typename... Args>


 private:
    std::vector<std::shared_ptr<wrapper>> mobjs;
};


























