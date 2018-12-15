#pragma once

#include <mutex>

namespace cbl {

//Sinlgeton base class.
//Example:
//define:
//  class Foo: class cbl::singleton<Foo> {
//    friend class cbl::singleton<Foo>; //add if constructor is private
//    ...
//usage:
//  Foo::ingleton().bar();
class singleton {
 public:
    static T& instance();
 protected:
    singleton() {
        //The initialization of a static data member does not occur unless the static data member is itself used in a way that requires the definition of the static data member to exist.
        //So do an use to initialization static memeber cleaner_.
        cleaner_.dumb();
    }
 private:
    //Cleaner class is responsible for calling singleton destruct when programme exit.
    class cleaner {
     public:
        ~cleaner() {
            if(singleton::instance_ != nullptr) {
                delete singleton::instance_;
            }
        }
        void dumb() const {}
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
