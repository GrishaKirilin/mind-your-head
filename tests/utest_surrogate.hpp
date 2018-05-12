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

/// \file   utest_surrogate.hpp
/// \brief  Unit test surrogate
/// \author Grisha Kirilin
/// \date   7/5/2018

////////////////////////////////////////////////////////////////////////////////
// Includes:

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// TODO: choose some standard framework.

class unit_test
{
public:

    unit_test( std::ostream &stream )
        : log_stream_{ stream }
    {}

    virtual ~unit_test() {}

    template<class... Args>
    unit_test& section( std::string preface, Args... args )
    {
        log_stream_ << preface << "\n";
        ( log_result( args ), ... );
        log_stream_ << "\n";
        return *this;
    }

    void flush_stat()
    {
        auto passed_cnt = std::count( stat_.begin(), stat_.end(), true );

        log_stream_ << "results: "
                    << passed_cnt << " tests passed, "
                    << ( stat_.size() - passed_cnt ) << " tests failed\n"
                    << std::endl;
        stat_.clear();
    }

protected:

    void log_result( bool passed )
    {
        log_stream_ << ( passed ? "[passed]" : "[failed]" )  << "\n";
        stat_.push_back( passed );
    }

    std::ostream &log_stream_;
    std::vector<bool> stat_; // sic!
};
