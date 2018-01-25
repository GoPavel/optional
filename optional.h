#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <cassert>

template <typename T>
class optional;



template <typename T>
class optional{
    bool is_exist;
    char storage[sizeof(T)];

public:
    optional(): is_exist(false) { }

    optional(optional const &other): is_exist(other.is_exist) {
        if (is_exist) {
            new (get_ptr()) T(*other.get_ptr());
        }
    }

    optional(optional &&other): is_exist(other.is_exist) {
        if (is_exist) {
            new (get_ptr()) T(std::move(*other.get_ptr()));
        }
    }

    optional(T const &obj): is_exist(true) {
        new (get_ptr()) T(obj);
    }

    optional(T &&obj): is_exist(true) {
        new (get_ptr()) T(std::move(obj)    );
    }

    optional& operator=(optional const &other) {
        if (is_exist) {
            (*get_ptr()).T::~T();
        }
        is_exist = other.is_exist;
        if (is_exist) {
            new (get_ptr()) T(*other.get_ptr());
        }
        return *this;
    }

    optional& operator=(optional &&other) {
        if (is_exist) {
            (*get_ptr()).T::~T();
        }
        is_exist = other.is_exist;
        if(is_exist) {
            new (get_ptr()) T(std::move(*other.get_ptr()));
        }
        return *this;
    }

    T& operator*() {
        assert(is_exist);
        return *get_ptr();
    }

    T* operator->() {
        assert(is_exist);
        return get_ptr();
    }

    bool is_empty() {
        return is_exist;
    }

    operator bool() {
        return is_exist;
    }

    ~optional() {
        if (is_exist) {
            (*get_ptr()).T::~T();
        }
    }
private:

    T* get_ptr() {
        return reinterpret_cast<T *>(storage);
    }

    T const* get_ptr() const {
        return reinterpret_cast<T const *>(storage);
    }
};

#endif //OPTIONAL_H
