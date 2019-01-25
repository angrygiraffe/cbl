#pragma once

#include <mutex>
#include <iostream>
#include <typeinfo>

namespace cbl {

template<class T>
class singleton {
 public:
    static T& instance();
 protected:
    singleton() {
      //The initialization of a static data member does not occur unless the static data member is itself used in a way that requires the definition of the static data member to exist.
      //Actively using static member cleaner to initialize it.
      cleaner_.dumb();
    }
 private:
    //
    class cleaner{
     public:
       cleaner() {
           std::cout << "cleaner::cleaner " << typeid(T).name() << std::endl;
       }
       ~cleaner() {
         if(singleton::instance_ != nullptr) {
           delete singleton::instance_;
         }
         std::cout << "cleaner::~cleaner " << typeid(T).name() << std::endl;
       }
       void dumb() const{}
    };
    singleton(const singleton& rhs) = delete;
    singleton& operator=(const singleton& rhs) = delete;

    static T* instance_;
    static std::once_flag init_flag_;
    static cleaner cleaner_;
};

template<class T>
T* singleton<T>::instance_ = nullptr;

template<class T>
std::once_flag singleton<T>::init_flag_;

template<class T>
typename singleton<T>::cleaner singleton<T>::cleaner_;

template<class T>
T& singleton<T>::instance() {
    std::call_once(singleton::init_flag_, [] {
        singleton::instance_ = new T();
    });
    return *instance_;
}

}
