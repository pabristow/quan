/*! \file
\brief  Test Meas2 class.
*/

// TestQuan1.cpp
// Copyright Paul A. Bristow 2012

// Test of meas2 - pair of measurements using uncertainty and statistical tests.

// see QuanOne word document for description.
#ifdef _MSC_VER
#  pragma warning(disable : 4996) //  _CRT_SECURE_NO_WARNINGS.
#  pragma warning(disable : 4702) // Unreachable code.
#endif

// #include <boost/test/minimal.hpp> // Boost minimal test
#define BOOST_TEST_MAIN // Required for int test_main() (must come FIRST).
#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-s-1_49.lib
#define BOOST_SYSTEM_STATIC_LINK 1

#include <boost/config.hpp>
#include <boost/cstdlib.hpp> // needed for boost::exit_failure;
#include <boost/chrono/chrono.hpp>
#include "boost/date_time/local_time/local_time.hpp"

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink,
#include <boost/test/floating_point_comparison.hpp> // Extra test tool for FP comparison.
  using boost::unit_test::test_suite;
  using boost::unit_test::unit_test_log;

#include <cmath>   // for sqrt, exp ...
#include <iostream> // 
#include <iomanip>  // for  setw, setprecision ...
#include <fstream>  // for filing.
#include <string>  // for C++ Std strings.
#include <sstream> // for stringstreams.
#include <vector>  // for vectors.
#include <functional>  // for less...
#include <algorithm>  // for sort, copy, swap ...
#include <numeric> // for accumulate... 
#include <locale> 
#include <ctime>  // C time.h defines time_t time(time_t* timer);
#include <cassert> // for testing only.

#include <boost/quan/unc.hpp> // Declaration of Uncertain Classes.
#include <boost/quan/meas.hpp> // uncertain plus order.

using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::getline;
using std::ios;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::hex;
using std::ws;
using std::skipws;
using std::noskipws;
using std::string;
using std::istringstream;
using std::ostringstream;
using std::boolalpha;
using std::uppercase;
using std::scientific;
using std::setprecision;
using std::resetiosflags;
using std::fixed;
using std::vector;
using std::iterator;
using std::allocator;
using std::abs;
using std::copy;
using std::sort;
using std::accumulate;
using std::transform;
using std::less;
using std::swap;
using std::locale;
using std::use_facet;
using std::ctype;
using std::numpunct;

time_t timeNow(); // return C time(0);
std::ostream& operator<< (std::ostream& os, time_t t); 

// Macros to test output strings and inputusing unc and meas classes.
// Note uses setUncDefaults(oss); to initialise unc iostream data.
// Example: CHECK(plusminus << u123, "1.234 +/-0.02");
#define CHECK(manips, result)\
{\
  ostringstream oss;\
  setUncDefaults(oss);\
  oss << manips;\
  BOOST_CHECK_EQUAL(oss.str(), result);\
}// #define	CHECK(manips, result)


std::string versions()
{
  std::stringstream mess;
  mess << "Program: " __FILE__  << "\n";
#ifdef __TIMESTAMP__
  mess << __TIMESTAMP__;
#endif
  mess << "\nBuildInfo:\n" "  Platform " << BOOST_PLATFORM;
  mess << "\n  Compiler " BOOST_COMPILER;
#ifdef _MSC_FULL_VER
  mess << "\n  MSVC version "<< BOOST_STRINGIZE(_MSC_FULL_VER) << ".";
#endif
  mess << "\n  STL " BOOST_STDLIB;
  mess << "\n  Boost version " << BOOST_VERSION/100000 << "." << BOOST_VERSION/100 % 1000 << "." << BOOST_VERSION % 100 << endl;
  return mess.str();
} // std::string versions()


char const outFilename[] = "meas2_out.txt";
char const logFilename[] = "meas2_log.txt";
char const testInputFilename[] = "meas2_in.txt";

char const* Ctime(time_t* t); // Timestamp as string "Tue Apr 24 16:57:31 2001""\n"

void msdelay(unsigned int d)
{// Delay for at least d millisecond:
    auto go = boost::chrono::steady_clock::now() + boost::chrono::milliseconds(d);
    while (boost::chrono::steady_clock::now() < go);
}

BOOST_AUTO_TEST_CASE(meas2_test_0)
{ 
  //std::cout << versions() << std::endl;
  BOOST_TEST_MESSAGE("meas2 test");

  BOOST_TEST_MESSAGE(versions());
  BOOST_CHECK(std::numeric_limits<double>::is_iec559 == true);
  // IEE745 is assumed by code.
} // BOOST_AUTO_TEST_CASE(quan1_test_0)


BOOST_AUTO_TEST_CASE(meas2_test_1)
{	// Some examples of testing uncertainy class without using macro CHECK.



} // BOOST_AUTO_TEST_CASE(meas2_test_1)


/*

Output:

  Description: Autorun J:\Cpp\quan\MSVC\Debug\meas2_test.exe
  Running 2 test cases...
  meas2 test
  Program: test_meas2_basic.cpp
  Tue Jul 24 16:48:47 2012
  BuildInfo:
    Platform Win32
    Compiler Microsoft Visual C++ version 10.0
    MSVC version 160040219.
    STL Dinkumware standard library version 520
    Boost version 1.50.0
  
  Test case meas2_test_1 did not check any assertions
  
  *** No errors detected



*/