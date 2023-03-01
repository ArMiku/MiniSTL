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
    struct has_iterator_cat {   // 判断是否有迭代器类别iterator_category
    private:
        // 区别char类型大小为1
        struct two {char a; char b;};
        template <class U> static two test(...);
        template <class U> static char test(typename U::iterator_category* = 0);
    public:
        // 如果有T中有定义iterator_category类型就返回two 则value就是true，否则返回char则value就是false。
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    // 如果不能隐式转换也就没必要萃取了，
    template <class Iterator, bool>
    struct iterator_traits_impl {};

    // 如果能转换就得到所有迭代器信息。
    template <class Iterator>
    struct iterator_traits_impl<Iterator, true> {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
        typedef typename Iterator::difference_type   difference_type;
    };

    // has_iterator_cat返回值传给iterator_traits_helper第二个参数
    // 如果为false就没必要继续萃取了，如果为true则继承iterator_traits_impl
    template <class Iterator, bool>
    struct iterator_traits_helper {};

    // is_convertible判断Iterator::iterator_category算否可以隐式转换为输入迭代器或输入迭代器
    template <class Iterator>
    struct iterator_traits_helper<Iterator, true>
            : public iterator_traits_impl<Iterator,
                    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
                    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value> {};

    // 萃取迭代器特征
    template <class Iterator>
    struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

    // 针对原生指针的偏特化版本
    template <class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag           iterator_category;
        typedef T                                    value_type;
        typedef T*                                   pointer;
        typedef T&                                   reference;
        typedef ptrdiff_t                            difference_type;
    };

    template <class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag           iterator_category;
        typedef T                                    value_type;
        typedef const T*                             pointer;
        typedef const T&                             reference;
        typedef ptrdiff_t                            difference_type;
    };


    // 萃取某种迭代器
    /*! T类型迭代器能隐式转换为U类型迭代器 */
    template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of
            : public m_bool_constant<std::is_convertible<
            typename iterator_traits<T>::iterator_category, U>::value> {};

    /*! T类型迭代器无法隐式转换为U类型迭代器 */
    template <class T, class U>
    struct has_iterator_cat_of<T, U, false> : public m_false_type {};

    /*! 判断是否为input_iterator_tag类别迭代器 */
    template <class Iter>
    struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

    /*! 判断是否为output_iterator_tag类别迭代器 */
    template <class Iter>
    struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

    /*! 判断是否为forward_iterator_tag类别迭代器 */
    template <class Iter>
    struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

    /*! 判断是否为bidirectional_iterator_tag类别迭代器 */
    template <class Iter>
    struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

    /*! 判断是否为random_access_iterator_tag类别迭代器 */
    template <class Iter>
    struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

    /*! 判断是否为迭代器 所有迭代器都是从input_iterator_tag 和 output_iterator_tag派生来的 */
    template <class Iterator>
    struct is_iterator :
            public m_bool_constant<is_input_iterator<Iterator>::value ||
                                   is_output_iterator<Iterator>::value> {};

    // 萃取某个迭代器的 category
    template <class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }

    // 萃取某个迭代器的 distance_type
    template <class Iterator>
    typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    // 萃取某个迭代器的 value_type
    template <class Iterator>
    typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    /*! 以下函数用于计算迭代器间的距离 */

    // distance 的 input_iterator_tag 的版本
    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    // distance 的 random_access_iterator_tag 的版本
    template <class RandomIter>
    typename iterator_traits<RandomIter>::difference_type
    distance_dispatch(RandomIter first, RandomIter last,
                      random_access_iterator_tag) {
        return last - first;
    }

    template <class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        return distance_dispatch(first, last, iterator_category(first));
    }

    /*! 以下函数用于让迭代器前进 n 个距离 */

    // advance 的 input_iterator_tag 的版本
    template <class InputIterator, class Distance>
    void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
        while (n--)
            ++i;
    }

    // advance 的 bidirectional_iterator_tag 的版本
    template <class BidirectionalIterator, class Distance>
    void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
        if (n >= 0)
            while (n--)  ++i;
        else
            while (n++)  --i;
    }

    // advance 的 random_access_iterator_tag 的版本
    template <class RandomIter, class Distance>
    void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    template <class InputIterator, class Distance>
    void advance(InputIterator& i, Distance n) {
        advance_dispatch(i, n, iterator_category(i));
    }

    /*********************************反向迭代器*********************************/

    // 模板类 : reverse_iterator
    // 代表反向迭代器，使前进为后退，后退为前进
    // 反向迭代器中维护了一个正向迭代器，所有的操作都是正向迭代器的一个适配
    template <class Iterator>
    class reverse_iterator {
    private:
        Iterator current;  // 记录对应的正向迭代器
    public:
        // 反向迭代器的五种相应型别
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type        value_type;
        typedef typename iterator_traits<Iterator>::difference_type   difference_type;
        typedef typename iterator_traits<Iterator>::pointer           pointer;
        typedef typename iterator_traits<Iterator>::reference         reference;

        typedef Iterator                                              iterator_type;
        typedef reverse_iterator<Iterator>                            self;

    public:
        // 构造函数
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type i) :current(i) {}
        reverse_iterator(const self& rhs) :current(rhs.current) {}

    public:
        // 取出对应的正向迭代器
        iterator_type base() const
        { return current; }

        // 重载操作符
        reference operator*() const {
            // 实际对应正向迭代器的前一个位置
            auto tmp = current;
            return *--tmp;
        }

        pointer operator->() const {
            return &(operator*());
        }

        // 前进(++)变为后退(--)
        self& operator++() {
            --current;
            return *this;
        }

        self operator++(int) {
            self tmp = *this;
            --current;
            return tmp;
        }

        // 后退(--)变为前进(++)
        self& operator--() {
            ++current;
            return *this;
        }

        self operator--(int) {
            self tmp = *this;
            ++current;
            return tmp;
        }

        self& operator+=(difference_type n) {
            current -= n;
            return *this;
        }

        self operator+(difference_type n) const {
            return self(current - n);
        }

        self& operator-=(difference_type n) {
            current += n;
            return *this;
        }

        self operator-(difference_type n) const {
            return self(current + n);
        }

        reference operator[](difference_type n) const {
            return *(*this + n);
        }
    };

    // 重载 operator-
    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type
    operator-(const reverse_iterator<Iterator>& lhs,
              const reverse_iterator<Iterator>& rhs) {
        return rhs.base() - lhs.base();
    }

    // 重载比较操作符
    template <class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs) {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs) {
        return rhs.base() < lhs.base();
    }

    template <class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs) {
        return !(lhs == rhs);
    }

    template <class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs,
                   const reverse_iterator<Iterator>& rhs) {
        return rhs < lhs;
    }

    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs) {
        return !(rhs < lhs);
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs) {
        return !(lhs < rhs);
    }

}

#endif //MINISTL_ITERATOR_H
