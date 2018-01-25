#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <cassert>
#include <type_traits>

template <typename T>
class optional_movable {
    bool is_exist;
    char storage[sizeof(T)];

public:
    optional_movable(): is_exist(false) { }

    optional_movable(optional_movable const &other) = delete;

    optional_movable(optional_movable &&other): is_exist(other.is_exist) {
        if (is_exist) {
            new (get_ptr()) T(std::move(*other.get_ptr()));
        }
    }

    optional_movable(T const &obj): is_exist(true) {
        new (get_ptr()) T(obj);
    }

    optional_movable(T &&obj): is_exist(true) {
        new (get_ptr()) T(std::move(obj)    );
    }

    optional_movable& operator=(optional_movable &&other) {
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
        return !is_exist;
    }

    operator bool() {
        return is_exist;
    }

    ~optional_movable() {
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

template <typename T>
class optional_copyable {
    bool is_exist;
    char storage[sizeof(T)];

public:
    optional_copyable(): is_exist(false) { }

    optional_copyable(optional_copyable const &other): is_exist(other.is_exist) {
        if (is_exist) {
            new (get_ptr()) T(*other.get_ptr());
        }
    }

    optional_copyable(optional_copyable &&other): is_exist(other.is_exist) {
        if (is_exist) {
            new (get_ptr()) T(std::move(*other.get_ptr()));
        }
    }

    optional_copyable(T const &obj): is_exist(true) {
        new (get_ptr()) T(obj);
    }

    optional_copyable(T &&obj): is_exist(true) {
        new (get_ptr()) T(std::move(obj)    );
    }

    optional_copyable& operator=(optional_copyable const &other) {
        if (is_exist) {
            (*get_ptr()).T::~T();
        }
        is_exist = other.is_exist;
        if (is_exist) {
            new (get_ptr()) T(*other.get_ptr());
        }
        return *this;
    }

    optional_copyable& operator=(optional_copyable &&other) {
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
        return !is_exist;
    }

    operator bool() {
        return is_exist;
    }

    ~optional_copyable() {
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


template<typename T>
using optional =
    std::conditional_t<std::is_copy_constructible<T>::value,
                       optional_copyable<T>,
                       optional_movable<T>>;
#endif //OPTIONAL_H
