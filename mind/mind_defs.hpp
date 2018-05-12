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

/// \file   mind_defs.hpp
/// \brief  common macro definition for prime library
/// \author Grisha Kirilin
/// \date   22/4/2018

#pragma once

#define WUBBA_LUBBA_DUB_DUB                                       \
    static_assert( __LINE__ < 1000,                               \
                   "Sometimes code is a lot more art than code. " \
                   "A lot of people don't get that. "             \
                   "This module is too large and ugly." );

////////////////////////////////////////////////////////////////////////////////
// Definition for meta-functionss

#define $class  template<class...> class
#define $deduce auto
#define $lambda template lambda
#define $if     if_else_t
#define $def    typedef
#define $type   typedef typename
