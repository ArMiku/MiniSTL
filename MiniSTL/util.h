//
// Created by QIHANG SUN on 2023/2/27.
// 包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等
//

#ifndef MINISTL_UTIL_H
#define MINISTL_UTIL_H

#include <cstddef>
#include "type_traits.h"

namespace mystl {
    // move
    template<class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    // forward
    template <class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T&&>(arg);
    }
}

#endif //MINISTL_UTIL_H
