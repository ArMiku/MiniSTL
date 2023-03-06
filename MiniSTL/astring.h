//
// Created by QIHANG SUN on 2023/3/6.
// 定义了 string, wstring, u16string, u32string 类型
//

#ifndef MINISTL_ASTRING_H
#define MINISTL_ASTRING_H

#include "basic_string.h"

namespace mystl
{

    using string    = mystl::basic_string<char>;
    using wstring   = mystl::basic_string<wchar_t>;
    using u16string = mystl::basic_string<char16_t>;
    using u32string = mystl::basic_string<char32_t>;

}

#endif //MINISTL_ASTRING_H