#ifndef MYTINYSTL_ITERATOR_H_
#define MYTINYSTL_ITERATOR_H_

#include <cstddef>
#include "type_traits.h"

namespace mystl{

// C++ 五种迭代器类型
struct input_iterator_tag {};                                                    // 输入迭代器
struct output_iterator_tag {};                                                   // 输出迭代器
struct forward_iterator_tag : public input_iterator_tag {};                      // 前向迭代器
struct bidirectional_iterator_tag : public forward_iterator_tag {};              // 双向迭代器
struct random_access_iterator_tag : public bidirectional_iterator_tag {};        // 随机迭代器


// iterator模板,最基础，最底层的迭代器
template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator{
    typedef     Category    iterator_category;
    typedef     T           value_type;
    typedef     Distance    difference_type;
    typedef     Pointer     pointer;
    typedef     Reference   reference;
};


// typename关键字的意思是告诉编译器，后面的U::iterator_category是一个类型，而不是一个函数
// 检查是否存在嵌套类型U::iterator_category
template <class T>
struct has_iterator_cat{
private:   
    struct two {char a; char b;};       // 定义一个结构体，大小为两个字节
    template <class U> static two test(...);        // 定义一个静态函数test，test接受所有参数，并且返回类型为two
    template <class U> static char test(typename U::iterator_category* = 0);        // 定义静态方法，接受U::iterator_category类型指针，默认参数为0
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);       // 如果T是iterator_category类型指针，则返回char，value为true，反之返回two，大小为2
};

// 模板特化
template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
    typedef typename Iterator::iterator_category        iterator_category;
    typedef typename Iterator::value_type               value_type;
    typedef typename Iterator::difference_type          difference_type;
    typedef typename Iterator::pointer                  pointer;
    typedef typename Iterator::reference                reference;
};


// 辅助模板，根据传入参数类型，来使得后面的模板可以继承自不同的类型
// 借助模板特化判断一种迭代器是否可以转换为其他类型
template <class Iterator, bool>
struct iterator_traits_helper {};

// 如果可以转换为输入或者输出迭代器，则继承自iterator_traits_impl<Iterator,true>,否则继承iterator....<Iterator,false>
template <class Iterator>
struct iterator_traits_helper<Iterator, true> : 
    public iterator_traits_impl<Iterator, 
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value> {

};

// 萃取迭代器
template <class Iterator>
struct iterator_traits :
    public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

// 普通指针特化
template <class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef ptrdiff_t                   difference_type;
};


// 普通const指针特化
template <class T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
    typedef ptrdiff_t                   difference_type;
};


// 迭代器T是否可以转换为U，如果可以则value值会是true,否则是false
template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of : 
    public m_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category, U>::value>
{
};

// has_iterator_cat_of偏特化,false
template <class T, class U>
struct has_iterator_cat_of<T,U, false> : public m_false_type {};
 
template <class Iter>
struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator : 
    public m_bool_constant<is_input_iterator<Iterator>::value || is_output_iterator<Iterator>::value>
{
};

// 返回迭代器的tag,例如input_iterator_tag
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category iterator_category (const Iterator&){
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();          // 返回一个默认结构体的构造对象
}

// 萃取迭代器的difference_type
template <class Iterator>
typename iterator_traits<Iterator>::difference_type* distance_type (const Iterator&){
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取迭代器value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}



}
#endif
