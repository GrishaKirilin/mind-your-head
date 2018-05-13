///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Grisha Kirilin
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/// \file   mind_core.hpp
/// \brief  Algebra of sets for type lists
/// \author Grisha Kirilin
/// \date   8/4/2018

////////////////////////////////////////////////////////////////////////////////
// Includes:

#include "mind_defs.hpp"
#include <utility>

////////////////////////////////////////////////////////////////////////////////
// Primitive Meta Programming Library

namespace mind
{
// base container
template<class... Ts> struct list {};

// container of containers
template<class... Ts> struct maybe {};

////////////////////////////////////////////////////////////////////////////////

namespace impl
{
template<$class NewHead, $class OldHead, class... T>
$deduce apply( OldHead<T...> ) -> NewHead<T...>;
}

template<$class NewHead, class TList>
using apply = decltype( impl::apply<NewHead>( std::declval<TList>() ) );

////////////////////////////////////////////////////////////////////////////////
//
//  JOIN

namespace impl
{
template<$class Head, class... T0, class... T1>
$deduce join( Head<T0...>, Head<T1...> ) -> Head<T0..., T1...>;
}

template<class TList0, class TList1>
using join_t =
    decltype( impl::join( std::declval<TList0>(), std::declval<TList1>() ) );

template<class TList0, class TList1>
struct join { $def join_t<TList0, TList1> type; };

////////////////////////////////////////////////////////////////////////////////
//
//  IS_MEMBER

namespace impl
{
template<class... Ts>
struct is_in
{
    enum { value = false };
};

template<class T0, class T1, class... Ts>
struct is_in<T0, T1, Ts...>
{
    enum
    {
        value = std::is_same<T0, T1>::value
                || ( sizeof ... ( Ts ) > 0 && is_in<T0, Ts...>::value )
    };
};
} // impl

namespace fn
{
template<class T> struct member_of
{
    template<class... Ts> struct lambda
    {
        enum { value = impl::is_in<T, Ts...>::value };
    };
};

template<class TList> struct is_member
{
    template<class T> struct lambda
    {
        enum { value = mind::apply<fn::member_of<T>::$lambda, TList>::value };
    };
};

template<class TList> struct free_of
{
    template<class T> struct lambda
    {
        enum { value = !is_member<TList>::$lambda<T>::value };
    };
};
} // fn

template<class T, class TList>
constexpr bool is_member_v = fn::is_member<TList>::$lambda<T>::value;

template<class TList0, class TList1>
struct is_member { enum { value = is_member_v<TList0, TList1> }; };

////////////////////////////////////////////////////////////////////////////////
//
//  APPEND, PREPEND

namespace impl
{
template<class T, $class Head, class... Ts>
$deduce append( Head<Ts...> ) -> Head<Ts..., T>;

template<class T, $class Head, class... Ts>
$deduce prepend( Head<Ts...> ) -> Head<T, Ts...>;
}

template<class TList, class T>
using append_t = decltype( impl::append<T>( std::declval<TList>() ) );

template<class TList, class T>
using prepend_t = decltype( impl::prepend<T>( std::declval<TList>() ) );

template<class TList, class T> struct append  { $def append_t<TList, T> type; };
template<class TList, class T> struct prepend { $def prepend_t<TList, T> type; };

////////////////////////////////////////////////////////////////////////////////
//
//  IF_ELSE (aka std::conditional)

template<bool, class, class> struct if_else;

template<class Then, class Else>
struct if_else<true, Then, Else>
{
    typedef Then type;
};

template<class Then, class Else>
struct if_else<false, Then, Else>
{
    typedef Else type;
};

template<bool Test, class Then, class Else>
using if_else_t = typename if_else<Test, Then, Else>::type;

////////////////////////////////////////////////////////////////////////////////
//
//  SELECT

namespace impl
{
template<$class Test, class Result, class... Ts>
struct filter
{
    typedef Result type;
};

template<$class Test, class Result, class T, class... Ts>
struct filter<Test, Result, T, Ts...>
{
    $def $if<
        Test<T>::value,
        typename filter<Test, append_t<Result, T>, Ts...>::type,
        typename filter<Test, Result, Ts...>::type
        > type;
};

template<$class Test, $class Head, class... Ts>
struct select_by
{
    $type filter<Test, Head<>, Ts...>::type type;
};

template<$class Test, $class Head, class... Ts>
$deduce select( Head<Ts...> ) -> typename select_by<Test, Head, Ts...>::type;
} // impl

template<$class Test, class TList>
using select_t = decltype( impl::select<Test>( std::declval<TList>() ) );

template<$class Test, class TList>
struct select { $def select_t<Test, TList> type; };

////////////////////////////////////////////////////////////////////////////////
//
//  COMPLEMENT

template<class TList0, class TList1>
using complement_t = select_t<fn::free_of<TList1>::$lambda, TList0>;

template<class TList0, class TList1>
struct complement { $def complement_t<TList0, TList1> type; };

////////////////////////////////////////////////////////////////////////////////
//
//  ALL_TRUE

namespace impl
{
template<$class Test, class... Ts>
struct all_types
{
    enum { value =  true };
};

template<$class Test, class T, class... Ts>
struct all_types<Test, T, Ts...>
{
    enum { value = Test<T>::value && all_types<Test, Ts...>::value };
};
} // impl

namespace fn
{
template<$class Test> struct all
{
    template<class... Ts> struct lambda
    {
        enum { value = impl::all_types<Test, Ts...>::value };
    };
};
} // fn

/*!
\class   `all_true<$class Test, class TList>`
\brief   `value` is `true` if `Test<T>::value` is `true` for all elements
         of `TList`.

\tparam  Test   meta-type with boolean field `value`.
\tparam  TList  list of types.

\details There is an `constexpr` alias `all_true_v`.
         Usage: Use `all_true_v<std::is_signed, foo<T...>>` to test that all
         elements of `foo` are signed types.
*/
template<$class Test, class TList>
struct all_true
{
    enum { value = mind::apply<fn::all<Test>::$lambda, TList>::value };
};

template<$class Test, class TList>
constexpr bool all_true_v = all_true<Test, TList>::value;

////////////////////////////////////////////////////////////////////////////////
//
//  IS_SUBSET

/// \class   `is_subset<class TA, class TB>`
/// \brief   `value` is `true` if a type list `TB` is a subset of `TA`.
///
/// \tparam  TA list of types.
/// \tparam  TB list of types.
///
/// \details There is an `constexpr` alias `is_subset_v`.
///          Usage: Use `is_subset_v<foo<Ts...>, foo<Us...>>` to test
///          that `<Us...>` is a subset of `<Ts...>`.
///
template<class TMajor, class TMinor>
struct is_subset
{
    enum { value = all_true_v<fn::is_member<TMajor>::$lambda, TMinor> };
};

template<class TMinor, class TMajor>
constexpr bool is_subset_v = is_subset<TMajor, TMinor>::value;

////////////////////////////////////////////////////////////////////////////////
//
//  IS_SAME_SET

template<class TList0, class TList1>
constexpr bool is_same_set_v = is_subset_v<TList0, TList1>
                               && is_subset_v<TList1, TList0>;

template<class TList0, class TList1>
struct is_same_set { enum { value = is_same_set_v<TList0, TList1> }; };

////////////////////////////////////////////////////////////////////////////////
//
//  HEAD

namespace impl
{
template<$class Head, class... Ts>
$deduce head( Head<Ts...> ) -> typename Head<>;
}

template<class TList>
using head_t =  decltype( impl::head( std::declval<TList>() ) );

template<class TList> struct head { $def head_t<TList> type; };

////////////////////////////////////////////////////////////////////////////////
//
//  LENGTH

namespace impl
{
template<class... Ts>
struct length
{
    enum { value = sizeof ... ( Ts ) };
};
}

template<class TList>
struct length
{
    enum { value = mind::apply<impl::length, TList>::value };
};

template<class TList>
constexpr size_t length_v = length<TList>::value;

////////////////////////////////////////////////////////////////////////////////
//
//  UNIQUE

namespace impl
{
template<class Result, class... Ts>
struct unique
{
    typedef Result type;
};

template<class Result, class T, class... Ts>
struct unique<Result, T, Ts...>
{
    $def $if<
        !is_member_v<T, Result>,
        typename unique<append_t<Result, T>, Ts...>::type,
        typename unique<Result, Ts...>::type
        > type;
};

template<$class Head, class... Ts>
$deduce unique_of( Head<Ts...> ) -> typename unique<Head<>, Ts...>::type;
} // impl

template<class TList>
using unique_t = decltype( impl::unique_of( std::declval<TList>() ) );

template<class TList> struct unique { $def unique_t<TList> type; };

////////////////////////////////////////////////////////////////////////////////
//
//  UNION_SET

template<class TList0, class TList1>
struct union_set
{
    $def unique_t<join_t<TList0, TList1>> type;
};

template<class TList0, class TList1>
using union_set_t = typename union_set<TList0, TList1>::type;

////////////////////////////////////////////////////////////////////////////////
} // mind
