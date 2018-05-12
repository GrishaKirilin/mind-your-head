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

/// \file   tests.cpp
/// \brief  Test for the core of `mind` library
/// \author Grisha Kirilin
/// \date   8/4/2018

////////////////////////////////////////////////////////////////////////////////
// Includes:

#include "utest_surrogate.hpp"
#include "mind_core.hpp"
#include "mind_util.hpp"

#include <iostream>
#include <iomanip>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// Entry point

template<class... T> struct foo {};
template<class... T> struct bar {};

int main()
{
    using namespace mind;
    using std::is_same_v;
    using std::is_floating_point;
    using std::is_signed;

    if ( auto value = 3; value > 0 ) {
        std::cout << "local value " << value << "\n";
    }

    unit_test( std::cout )

    .section(
        "Test meta-meta function `apply`",
        is_same_v<bar<float>, mind::apply<bar, foo<float>>> )

    .section( "Test meta function `join_t`",
              is_same_v<foo<int, float>, join_t<foo<int>, foo<float>>> )

    .section( "Test meta predicate `is_member_v`",
              !is_member_v<float, foo<>>,
              is_member_v<float, foo<float>>,
              is_member_v<float, foo<int, float, double>>,
              !is_member_v<short, foo<int, float, double>> )

    .section( "Test meta function `append_t`",
              is_same_v<bar<float>, append_t<bar<>, float>>,
              is_same_v<foo<int, float>, append_t<foo<int>, float>> )

    .section( "Test meta function `prepend_t`",
              is_same_v<append_t<foo<>, int>, prepend_t<foo<>, int>>,
              is_same_v<foo<int, float>, prepend_t<foo<float>, int>> )

    .section( "Test meta function `select_t`",
              is_same_v<foo<float>,
                        mind::select_t<is_floating_point, foo<int, float>>>,
              is_same_v<foo<int, float>,
                        mind::select_t<is_signed, foo<int, float>>> )

    .section( "Test meta function `complement_t`",
              is_same_v<
                  foo<int, float>,
                  mind::complement_t<foo<int, float, double>, foo<double>>> )

    .section( "Test meta function `union_set_t`",
              is_same_v<foo<int, float, double>,
                        mind::union_set_t<foo<int, float>, foo<float, double>>> )

    .section( "Test meta-meta predicate `all_true_v`",
              !mind::all_true_v<is_floating_point, foo<int, float>>,
              mind::all_true_v<is_signed, foo<int, float>> )

    .section( "Test meta predicate `is_subset_v`",
              !is_subset_v<foo<double>, foo<int, float>>,
              is_subset_v<foo<double, float>, foo<int, float, double>>,
              is_subset_v<foo<double, int>, foo<int, float, double>> )

    .section( "Test meta predicate `is_same_set_v`",
              !is_same_set_v<foo<double>, foo<int, float>>,
              is_same_set_v<foo<double, int, float>, foo<int, float, double>>,
              is_same_set_v<foo<int, double, float>, foo<int, float, double>> )

    .section( "Test meta function `head_t`",
              is_same_set_v<foo<>, head_t<foo<>>>,
              is_same_set_v<foo<>, head_t<foo<float>>>,
              is_same_set_v<bar<>, head_t<bar<int, float>>> )

    .section( "Test meta function `unique_t`",
              is_same_set_v<foo<>, unique_t<foo<>>>,
              is_same_set_v<foo<float>, unique_t<foo<float>>>,
              is_same_set_v<bar<int>, unique_t<bar<int, int>>>,
              is_same_set_v<bar<int, float>, unique_t<bar<int, int, float>>> )

    .flush_stat();

    return 0;
}

WUBBA_LUBBA_DUB_DUB
