#ifndef MYTINYSTL_UTIL_H_
#define MYTINYSTL_UTIL_H_

// 一些通用工具的实现，例如move,forward,swap等函数，以及pair

#include <cstddef>
#include "type_traits.h"

namespace mystl {

// move
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

// forward
template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}

template <class Tp>
void swap(Tp& lhs, Tp& rhs)
{
    auto tmp(mystl::move(lhs));
    lhs = mystl::move(rhs);
    rhs = mystl::move(tmp);
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
    for(;first1 != last1; ++first1, (void)++first2){
        mystl::swap(*first1, *first2);
    }

    return first2;
}

template <class Tp, size_t N>
void swap(Tp(&a)[N], Tp(&b)[N])
{
    mystl::swap_range(a, a+N, b);
}

}

#endif