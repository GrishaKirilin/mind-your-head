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

/// \file   mind_util.hpp
/// \brief  various helpers to debug meta-programming
/// \author Grisha Kirilin
/// \date   6/5/2018


////////////////////////////////////////////////////////////////////////////////
// Includes:

#include <string_view>

////////////////////////////////////////////////////////////////////////////////
// Print name of type. Found in SO, author is Howard Hinnant.

/*!
\brief   return string view on the name of a type.
\return  compile-time constant string view.
\author  Howard Hinnant
*/
template<class T> constexpr std::string_view type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view( p.data() + 34, p.size() - 34 - 1 );

#elif defined( __GNUC__ )
    string_view p = __PRETTY_FUNCTION__;
#if __cplusplus < 201402
    return string_view( p.data() + 36, p.size() - 36 - 1 );

#else // if __cplusplus < 201402
    return string_view( p.data() + 49, p.find( ';', 49 ) - 49 );

#endif // if __cplusplus < 201402
#elif defined( _MSC_VER )
    string_view p = __FUNCSIG__;
    return string_view( p.data() + 84, p.size() - 84 - 7 );

#endif // ifdef __clang__
}
