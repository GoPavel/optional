#ifndef OPTIONAL_H
#define OPTIONAL_H
#include <cassert>
#include <type_traits>

template <typename T>
class optional_base {
protected:
    bool is_exist;
    std::aligned_storage_t<sizeof(T), alignof(T)> storage;

public:
    optional_base(): is_exist(false) { }

    optional_base(optional_base const &other): is_exist(other.is_exist) {
        if (is_exist) {
            new (get_ptr()) T(*other.get_ptr());
        }
    }

    optional_base(optional_base &&other): is_exist(other.is_exist) {
        if (is_exist) {
            new (get_ptr()) T(std::move(*other.get_ptr()));
        }
    }

    optional_base(T const &obj): is_exist(true) {
        new (get_ptr()) T(obj);
    }

    optional_base(T &&obj): is_exist(true) {
        new (get_ptr()) T(std::move(obj));
    }

    optional_base& operator=(optional_base const &other) {
        if (is_exist) {
            (*get_ptr()).T::~T();
        }
        is_exist = other.is_exist;
        if (is_exist) {
            new (get_ptr()) T(*other.get_ptr());
        }
        return *this;
    }

    optional_base& operator=(optional_base &&other) {
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

    ~optional_base() {
        if (is_exist) {
            (*get_ptr()).T::~T();
        }
    }
protected:

    T* get_ptr() {
        return &reinterpret_cast<T&>(storage);
    }

    T const* get_ptr() const {
        return &reinterpret_cast<T const &>(storage);
    }

};

template <typename T>
class optional_movable : public optional_base<T> {

public:
    optional_movable(): optional_base<T>() { }

    optional_movable(optional_movable const &other) = delete;

    optional_movable(optional_movable &&other): optional_base<T>(std::move(other)) { }

    optional_movable(T &&obj): optional_base<T>(std::move(obj)) {}

    optional_movable& operator=(optional_movable &&other) {
        optional_base<T>::operator=(std::move(other));
        return *this;
    }
};

template <typename T>
class optional_copyable : public optional_base<T> {
public:
    optional_copyable(): optional_base<T>() { }

    optional_copyable(optional_copyable const &other): optional_base<T>(other) { }

    optional_copyable(optional_copyable &&other): optional_base<T>(std::move(other)) { }

    optional_copyable(T &&obj): optional_base<T>(std::move(obj)) {}

    optional_copyable(T const &obj): optional_base<T>(obj) { }

    optional_copyable& operator=(optional_copyable const &other) {
        optional_base<T>::operator=(other);
        return *this;
    }

    optional_copyable& operator=(optional_copyable &&other) {
        optional_base<T>::operator=(std::move(other));
        return *this;
    }
};

template<typename T>
using optional =
    std::conditional_t<std::is_copy_constructible<T>::value,
                       optional_copyable<T>,
                       optional_movable<T>>;
#endif //OPTIONAL_H
