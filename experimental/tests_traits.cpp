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

/// \file   tests_traits.cpp
/// \brief  Test experimental type traits
/// \author Grisha Kirilin
/// \date   6/5/2018

////////////////////////////////////////////////////////////////////////////////
// Includes:

#include "mind_traits.hpp"
#include "tests/utest_surrogate.hpp"

////////////////////////////////////////////////////////////////////////////////
// Test classes

struct usr_ctor
{
    usr_ctor( int );
};

struct many_ctor
{
    many_ctor( int );
    many_ctor( int, float );
};

struct def_ctor {};

struct no_ctor
{
    int      val_int;
    short    val_double;
    usr_ctor val_class;
};

////////////////////////////////////////////////////////////////////////////////
// Entry point

int main()
{
    using namespace mind;
    using std::is_same_v;

    unit_test( std::cout )

    .section(
        "Test constructor trait `ctor_trait`",
        is_same_v<maybe<>,                            ctor_trait_t<no_ctor>>,
        is_same_v<maybe<list<>>,                      ctor_trait_t<def_ctor>>,
        is_same_v<maybe<list<int>>,                   ctor_trait_t<usr_ctor>>,
        is_same_v<maybe<list<int, float>, list<int>>, ctor_trait_t<many_ctor>> )

    .section(
        "Test brace constructible trait `has_brace_init_v`",
        has_brace_init_v<no_ctor, int, short, usr_ctor> )

    .section(
        "Test trait for brace list initialization `init_trait_t`",
        is_same_v<maybe<list<int>>,                  init_trait_t<usr_ctor>>,
        is_same_v<maybe<list<int, short, usr_ctor>>, init_trait_t<no_ctor>> )

    .flush_stat();

    return 0;
}

WUBBA_LUBBA_DUB_DUB
