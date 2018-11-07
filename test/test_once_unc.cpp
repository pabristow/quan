/*!
  \file 
  \brief Testing uncertain classes using Boost Test Tool.
  \details Class for simple Propagation of Uncertainties
     according to a pure Gaussian model.
*/

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
// Copyright Paul A. Bristow 1998, 2012.

// unc_tests.cpp 

#ifdef _MSC_VER
#  pragma warning(disable: 4702) // unreachable code
#  pragma warning(disable: 4511) // copy constructor could not be generated.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4521) // alignment of a member was sensitive to packing.
#  pragma warning(disable: 4100) // unreferenced formal parameter.
#  pragma warning(disable: 4701) // local variable may be used without having been initialized.
#  pragma warning(disable: 4121) // alignment of a member was sensitive to packing.
#  pragma warning(disable: 4127) // conditional expression is constant.
#endif

// #define UNC_TRACE // Diagnostic output.

#define BOOST_TEST_MAIN // Required for int test_main() (must come FIRST).
//#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-s-1_49.lib

#include <boost/config.hpp>
#include <boost/cstdlib.hpp> // needed for boost::exit_failure;

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink,
#include <boost/test/floating_point_comparison.hpp> // Extra test tool for FP comparison.
  using boost::unit_test::test_suite;
  using boost::unit_test::unit_test_log;

// Classes for simple propagation of Uncertainties according to a pure Gaussian model.

#include <cmath>   // for <math.h>
#include <iostream>
 // using std::defaultfloat; // Initial default - neither scientific nor fixed.
#include <iomanip>  // for <iomanip.h> for setw, setprecision ...
  using std::setfill;
#include <fstream>  // for <fstream.h> for filing.
  using std::ofstream;
  using std::ifstream;
#include <string>  // for C++ Std strings
  using std::basic_string;
  using std::string;
#include <sstream> // for o & istringstream
  using std::istringstream;
  using std::ostringstream;
  using std::stringstream;
#include <limits>
  using std::numeric_limits;


#include <boost/quan/unc.hpp> // Declaration of Uncertain Classes.

// Check using manips output expected string result, for example:
// CHECK(hex << showbase << setw(10) << i, "       0xf")
// CHECK(scientific << setw(20) << d, "       1.234568e+001");
// Note: sets uncertain defaults (& oss has ios defaults too).
// BUT CHECK cannot check the number of chars output using unc_print "used",
// so use CHECK_USED for this.

#define CHECK(manips, result)\
{\
  ostringstream oss;\
  setUncDefaults(oss);\
  oss << manips;\
  BOOST_CHECK_EQUAL(oss.str(), result);\
}// #define	CHECK(manips, result)
//	BOOST_CHECK_EQUAL(oss.str().length(), strlen(result));\
// Temporarily removed because causes too much clutter in log.
// Anyway. if strings are same, then length check is superfluous.

// Also check that the unc used count was correct.
#define	CHECK_USED(manips, result)\
{\
  typedef basic_string <char>::size_type size_type;\
  ostringstream oss;\
  setUncDefaults(oss);\
  oss << manips;\
  BOOST_CHECK_EQUAL(oss.str(), result);\
}
//	BOOST_CHECK_EQUAL(oss.str().length(), static_cast<size_type>(oss.iword(usedIndex)));\
// Was temporarily removed because causes too much clutter in log.

// Compare results of reading string into a uncun, for example 1.2 +/-0.01 (8)
// with all elements of uncun type.
#define	CHECK_IN(in_string, mean, sd, df, ty)\
{\
  uncun r;\
  std::istringstream iss(in_string);\
  setUncDefaults(iss);\
  iss >> r;\
  BOOST_CHECK_CLOSE_FRACTION(r.value(), mean, numeric_limits<double>::epsilon());\
  BOOST_CHECK_CLOSE_FRACTION(r.std_dev(), sd, numeric_limits<float>::epsilon());\
  BOOST_CHECK_EQUAL(r.deg_free(), df);\
  BOOST_CHECK_EQUAL(r.types(), ty);\
  showUncTypes(r.types());\
} // #define	CHECK_IN(in, value, sd, df, types)

// CHECK_OUT_IN Output via manips, and read back in, check is same.
#define	 CHECK_OUT_IN(manips, result, value, sd, df, types)\
{\
  typedef basic_string <char>::size_type size_type;\
  stringstream ss;\
  setUncDefaults(ss);\
  ss << manips;\
  BOOST_CHECK_EQUAL(ss.str(), result);\
  BOOST_CHECK_EQUAL(ss.str().length(), static_cast<size_type>(ss.iword(usedIndex)));\
  uncun r;\
  ss >> r;\
  BOOST_CHECK_CLOSE_FRACTION(r.value(), value, numeric_limits<double>::epsilon());\
  BOOST_CHECK_CLOSE_FRACTION(r.std_dev(), sd, numeric_limits<float>::epsilon());\
  BOOST_CHECK_EQUAL(r.deg_free(), df);\
  BOOST_CHECK_EQUAL(r.types(), types);\
}// #define	CHECK_OUT_IN(manips, result)

// This test case is automatically registered by using BOOST_AUTO_TEST_CASE.
  using std::istream;
  using std::ostream;
  using std::ios_base;
  using std::char_traits;
  using std::cout;
  using std::cerr;
  using std::cin;
  using std::endl;
  using std::flush;
  using std::ws;
  using std::streamsize;
  using std::boolalpha;
  using std::dec;
  using std::hex;
  using std::showbase;
  using std::fixed;
  using std::scientific;
  using std::right;
  using std::showpos;
  using std::noshowpos;
  using std::noshowbase;
  using std::noshowpoint;
  using std::showpoint;

  using std::setprecision;
  using std::setw;
  using std::resetiosflags;

BOOST_AUTO_TEST_CASE(unc_one_test_basic)
{ // Uncertain Class tests.
  boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages);

  std::string message("One unc test: " __FILE__ );
#ifdef __TIMESTAMP__
  message += " at " BOOST_STRINGIZE(__TIMESTAMP__);
#endif
#ifdef _MSC_FULL_VER
  message +=  ", MSVC version " BOOST_STRINGIZE(_MSC_FULL_VER) ".";
#else
  message += "."
#endif
  BOOST_TEST_MESSAGE(message);

  BOOST_CHECK(numeric_limits<double>::is_iec559 == true);	// IEC559/IEEE754 floating point.

  // Change log level to record warnings & errors.
  // unit_test_log.set_log_threshold(boost::unit_test::log_successful_tests);
  unit_test_log.set_threshold_level(boost::unit_test::log_all_errors);
  //unit_test_log::instance().set_threshold_level(test_suite);
  //unit_test_log::instance().set_threshold_level(messages); // user messages
} 

BOOST_AUTO_TEST_CASE(unc_one_test)
{
  uncun u0(0, 0);  // Exact Zero, using constructor from int zero.
    //BOOST_CHECK(u0.types() & VALUE_ZERO);
    //BOOST_CHECK((u0.types() & VALUE_INTEGER)); // 

    u0.std_dev(0.01f); // Zero, but add known uncertainty.
    BOOST_CHECK_EQUAL(u0.std_dev(), 0.01f);
    BOOST_CHECK(u0.types() & VALUE_ZERO);
    BOOST_CHECK(!(u0.types() & VALUE_INTEGER)); // No longer exact,
    BOOST_CHECK((u0.types() & UNC_KNOWN)); // but unc is known.
    CHECK_USED(plusminus << u0, "0.000 +/-0.010");  // So now show added uncertainty with plusminus.
}
/*
Output:

test_one_unc.vcxproj -> J:\Cpp\quan\MSVC\x64\Debug\test_one_unc.exe
Autorun J:\Cpp\quan\MSVC\x64\Debug\test_one_unc.exe
Running 2 test cases...
Platform: Win32
Compiler: Microsoft Visual C++ version 14.1
STL     : Dinkumware standard library version 650
Boost   : 1.69.0
One unc test: i:\modular-boost\libs\quan\test\test_once_unc.cpp at "Thu Nov  1 12:12:51 2018", MSVC version 191526732.

Test module "Master Test Suite" has passed with:
2 test cases out of 2 passed
6 assertions out of 6 passed

Test case "unc_one_test_basic" has passed with:
1 assertion out of 1 passed

Test case "unc_one_test" has passed with:
5 assertions out of 5 passed


*/