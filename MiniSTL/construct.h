//
// Created by QIHANG SUN on 2023/2/24.
// construct : 负责对象的构建
// destroy   : 负责对象的析构
//

#ifndef MINISTL_CONSTRUCT_H
#define MINISTL_CONSTRUCT_H

#include <new>

#include "type_traits.h"
#include "iterator.h"

namespace mystl {

    // construct begin

    // 无参构造
    template <class T>
    void construct(T* ptr) {
        ::new((void*) ptr) T();
    }

    // 单参构造
    template <class T1, class T2>
    void construct(T1* ptr, const T2& value) {
        ::new((void*) ptr) T1(value);
    }

    // 多参构造
    template <class T, class... Args>
    void construct(T* ptr, Args&&... args) {
        ::new ((void*) ptr) T(std::forward<Args>(args)...);
    }
    // construct end

    // destroy begin
    template <class T>
    void destroy_one(T*, std::true_type) {}

    // 接受一个指针调用析构函数
    template <class T>
    void destroy_one(T* pointer, std::false_type) {
        if(pointer != nullptr) {
            pointer -> ~T();
        }
    }

    // 如果元素的数值型别(value type)无non-trivial destructor•
    template <class ForwardIter>
    void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

    // 如果元素的数值型别(value type)有non-trivial destructor
    template <class ForwardIter>
    void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
        for(; first != last; first++)
            destroy(&*first);
    }

    template <class T>
    void destroy(T* pointer) {
        destroy_one(pointer, std::is_trivially_destructible<T>{});
    }

    template <class ForwardIter>
    void destroy(ForwardIter first, ForwardIter last) {
        destroy_cat(first, last, std::is_trivially_destructible<
                typename iterator_traits<ForwardIter>::value_type>{});
    }
    // destroy end
}

#endif //MINISTL_CONSTRUCT_H