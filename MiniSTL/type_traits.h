//
// Created by QIHANG SUN on 2023/2/24.
// 本文件用于提取类型信息
//

#ifndef MINISTL_TYPE_TRAITS_H
#define MINISTL_TYPE_TRAITS_H

#include <type_traits>

namespace mystl {
    template <class T, T v>
    struct m_integral_constant {
        static constexpr T value = v;
    };

    template <bool b>
    using m_bool_constant = m_integral_constant<bool, b>;

    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;

    /*****************************************************************************************/
    // type traits

    // is_pair

    // --- forward declaration begin
    template <class T1, class T2>
    struct pair;
    // --- forward declaration end

    template <class T>
    struct is_pair : mystl::m_false_type {};

    template <class T1, class T2>
    struct is_pair<mystl::pair<T1, T2>> : mystl::m_true_type {};

}

#endif //MINISTL_TYPE_TRAITS_H
