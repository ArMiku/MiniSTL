//
// Created by QIHANG SUN on 2023/2/24.
// 这个文件用于迭代器设计，包含了一些模板结构体与全局函数，
//

#ifndef MINISTL_ITERATOR_H
#define MINISTL_ITERATOR_H

#include <cstddef>

#include "type_traits.h"

namespace mystl {
    // 五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    // iterator 模板
    template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
        typedef Distance    difference_type;
    };

    // iterator traits
    template <class T>
    struct has_iterator_cat {
    private:
        struct two {char a; char b;};
        template <class U> static two test(...);
        template <class U> static char test(typename U::iterator_category* = 0);
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template <class Iterator, bool>
    struct iterator_traits_impl {};

    template <class Iterator>
    struct iterator_traits_impl<Iterator, true> {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
        typedef typename Iterator::difference_type   difference_type;
    };

    template <class Iterator, bool>
    struct iterator_traits_helper {};

    template <class Iterator>
    struct iterator_traits_helper<Iterator, true> : public iterator_traits_helper<Iterator,
            std::is_convertible<typename Iterator::iterator_category, input_iterator_tag> value ||
            std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value> {};

    // 萃取迭代器特征
    template <class Iterator>
    struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};


}

#endif //MINISTL_ITERATOR_H
