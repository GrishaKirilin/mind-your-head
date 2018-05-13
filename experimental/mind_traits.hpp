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

/// \file   mind_traits.hpp
/// \brief  experimental type traits
/// \author Grisha Kirilin
/// \date   6/5/2018


////////////////////////////////////////////////////////////////////////////////
// Includes:

#include <type_traits>
#include <utility>

#include "mind_core.hpp"

namespace mind
{
////////////////////////////////////////////////////////////////////////////////
// helper types

template<class T> struct identity { T operator()(); };

////////////////////////////////////////////////////////////////////////////////
// Sow-Reap : found in twiiter of Kris Jusiak

template<class T, auto Id> struct sow
{
    friend auto reap( sow );

    template<class U> struct seed
    {
        friend auto reap( sow )
        {
            return identity<U>{} ( );
        }
    };

    template<class U,
             class = std::enable_if_t<!std::is_same_v<T, U>>,
             class = decltype( seed<U>{} )
             >
    operator U &();
};

////////////////////////////////////////////////////////////////////////////////
// Constructor trait

// *INDENT-OFF*
namespace impl
{
template<class T, auto... Ids>
auto ctor_trait( std::index_sequence<Ids...> )
{
    using std::make_index_sequence;
    using std::is_constructible_v;

    if constexpr ( (sizeof...( Ids ) == 0) && is_constructible_v<T> )
    {
        return maybe<list<>>{};
    }
    else if constexpr (sizeof...( Ids ) == 0)
    {
        return maybe<>{};
    }
    else if constexpr ( is_constructible_v<T, sow<T, Ids>...> )
    {
        return join_t<
            maybe<list<decltype( reap( sow<T, Ids>{} ) )...>>,
            decltype( ctor_trait<T>( make_index_sequence<sizeof...( Ids ) - 1>{} ) )
            >{};
    }
    else
    {
        return ctor_trait<T>( make_index_sequence<sizeof...( Ids ) - 1>{} );
    }
}
}
// *INDENT-ON*

template<class T, size_t MaxArgs = 20>
using ctor_trait_t =
    decltype( impl::ctor_trait<T>( std::make_index_sequence<MaxArgs>{} ) );

////////////////////////////////////////////////////////////////////////////////
// Has direct list initialization

// *INDENT-OFF*
namespace impl
{
template <class T, class = void, class... Args>
struct has_brace_init : std::false_type {};

template <class T, class... Args>
struct has_brace_init<
    T,
    std::void_t<decltype( T{ std::declval<Args>()... } )>,
    Args...> : std::true_type {};
}
// *INDENT-ON*

template<class T, class... Args>
using has_brace_init = impl::has_brace_init<T, void, Args...>;

template<class T, class... Args>
constexpr bool has_brace_init_v = has_brace_init<T, Args...>::value;

////////////////////////////////////////////////////////////////////////////////
// Trait initialization list

// *INDENT-OFF*
namespace impl
{
template<class T, auto... Ids>
auto init_trait( std::index_sequence<Ids...> )
{
    if constexpr ( has_brace_init_v<T, sow<T, Ids>...> )
        return maybe<list<decltype( reap( sow<T, Ids>{} ) )...>>{};
    else if constexpr ( sizeof...( Ids ) == 0 )
        return maybe<> {};
    else
        return init_trait<T>( std::make_index_sequence<sizeof...( Ids ) - 1>{} );
}
}
// *INDENT-ON*

template<class T, size_t MaxLength = 20>
using init_trait_t =
    decltype( impl::init_trait<T>( std::make_index_sequence<MaxLength>{} ) );
}
