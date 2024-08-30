#ifndef MYTINYSTL_TYPE_TRAITS_H_
#define MYTINYSTL_TYPE_TRAITS_H_

#include <type_traits>

namespace mystl{

// 声明一个模板结构体，模板存在默认值v
template <class T, T v>
struct m_integral_constant{
    static constexpr T value = v;       // 声明静态常量类型为T，值为value，默认被赋值为v
};

// 命名bool类型，参数b的模板m_integral_constant为m_bool_constant
template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;

typedef  m_bool_constant<true> m_true_type;        
typedef  m_bool_constant<false> m_false_type;       

}

#endif