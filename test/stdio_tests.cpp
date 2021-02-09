/*!
\file
\brief Testing stdio using Boost Test Tool.
\details analogous to test for uncertain Classes in unc_tests.cpp.
*/

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
// Copyright Paul A. Bristow 1998, 2012, 2021.

// stdio_tests.cpp

#include <boost/config.hpp>
#include <boost/cstdlib.hpp> // needed for boost::exit_failure;
//#include <boost/test/minimal.hpp>
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

#include <boost/test/tools/floating_point_comparison.hpp> // Extra test tool for FP comparison.

#include <cmath>   // for <math.h>
#include <iostream>
using std::ends;
using std::endl;
using std::ios_base;
using std::cin;
using std::cout;
using std::flush;
using std::ws;
using std::boolalpha;
using std::dec;
using std::hex;
using std::oct;
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
using std::boolalpha;
using std::getline;
using std::skipws;
using std::noskipws;
using std::uppercase;
using std::showbase;
using std::showpos;
using std::internal; // Initial default - neither left, right nor internal.
using std::noshowpoint;
using std::streamsize;
using std::char_traits;

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

void setUncDefaults(std::ostream&){return;}

void showUncTypes(int) { return; };

#define CHECK(manips, result)\
{\
  std::ostringstream oss;\
  setUncDefaults(oss);\
  oss << manips;\
  BOOST_CHECK_EQUAL(oss.str(), result);\
} // #define CHECK(manips, result)

//  BOOST_CHECK_EQUAL(oss.str().length(), strlen(result));
// Temporarily removed because causes too much clutter in log.
// Anyway, if strings are same, then length check is superfluous.

// Also check that the unc used count was correct.
#define CHECK_USED(manips, result)\
{\
  typedef basic_string <char>::size_type size_type;\
  std::ostringstream oss;\
  setUncDefaults(oss);\
  oss << manips;\
  BOOST_CHECK_EQUAL(oss.str(), result);\
}
//  BOOST_CHECK_EQUAL(oss.str().length(), static_cast<size_type>(oss.iword(usedIndex)));
// Was temporarily removed because causes too much clutter in log.

// Compare results of reading string into a uncun, for example 1.2 +/-0.01 (8)
// with all elements of uncun type.
#define CHECK_IN(in_string, mean, sd, df, ty)\
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
} // #define  CHECK_IN(in, value, sd, df, types)

// CHECK_OUT_IN Output via manips, and read back in, check is same.
#define  CHECK_OUT_IN(manips, result, value, sd, df, types)\
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
}// #define CHECK_OUT_IN(manips, result)

// Integrity check on iword begin and end no longer needed in CHECK.
/*
BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);\
BOOST_CHECK_EQUAL(oss.iword(0), indexID);\
BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);\
BOOST_CHECK_EQUAL(oss.iword(0), indexID);\
*/


BOOST_AUTO_TEST_CASE(test_init)
{
  int current_time = 0; // real call is required here

  BOOST_TEST_MESSAGE("Testing initialization :");
  BOOST_TEST_MESSAGE("Current time:" << current_time);
}

// This is used by unit_test/minimal.hpp
// but was too minimal, and deprecated.
//int test_main(int, char* [])             // note the name!
//{
//  int i1 = 1;
//  CHECK(i1, "1");
//} // int test_main(int, char* [])




BOOST_AUTO_TEST_CASE(unc_test_stdio)
{  // Examples of C++ std integer output.
  int i1 = 1;
  CHECK(i1, "1");
  CHECK(noshowpoint << i1, "1"); // Explicit NOshowpoint
  CHECK(std::showpoint << i1, "1"); // Never show point if value is integer!

  int i1234 = 1234;
  CHECK(i1234, "1234");
  CHECK(noshowpoint << i1234, "1234"); // Normal
  CHECK(std::showpoint << i1234, "1234"); // NOT "1234." Doesn't show point if integer!
  CHECK(std::showpos << i1234, "+1234"); // Shows +

  int m1 = -1; // negative variable.
  CHECK(m1, "-1"); //  negative constant.
  CHECK(hex << m1, "ffffffff"); //
  int m1234 = -1234;
  CHECK(m1234, "-1234"); // plain negative
  CHECK(std::noshowpoint << m1234, "-1234"); // Normal
  CHECK(std::showpoint << m1234, "-1234"); // NOT "-1234." Doesn't show point if integer!
  CHECK(std::showpos << m1234, "-1234"); // Makes no difference - always show - sign.
  CHECK(std::noshowpos << m1234, "-1234"); // Makes no difference - always show - sign.

  int i = 15;
  CHECK(std::hex << std::showbase << std::setw(5) << i, "  0xf"); // 2 spaces + 3 digit chars.
  CHECK(std::hex << std::noshowbase << std::setw(5) << i, "    f"); // 4 spaces + 1 digit char .
  CHECK(std::hex << std::left << std::noshowbase << std::setw(5) << i, "f    "); // 1 digit char + 4 fill spaces.
  CHECK(std::right << dec << noshowbase << std::setw(5) << i, "   15"); // 1 digit char + 3 fill spaces.
  CHECK(std::setfill('~') << std::left << std::dec << std::noshowbase << setw(5) << i, "15~~~"); //  3 ~ & 1 digit char.
  CHECK(std::setfill('~') << std::right << std::dec << std::noshowbase << setw(5) << i, "~~~15"); // 1 digit char + 3 ~.

  // Check some examples of built-in double output.

  double zero = 0.;
  double one = 1.;
  double minus1 = -1.;
  double point1 = 0.1;

  CHECK(zero, "0"); // normal defaults, width = 1
  CHECK(std::showpos << zero, "+0"); // normal defaults, width = 1
  CHECK(std::setprecision(0) << zero, "0"); // NO decimal point.
  // Check use of std::showpos and showpoint.
  CHECK(std::fixed << std::showpos << std::showpoint << zero, "+0.000000"); // 1+6 zeros - no precision set, so defaults to 6.
  //CHECK(std::showpoint << std::setprecision(1) << zero, "0."); // normal defaults.
  //CHECK(std::showpoint << std::showpos << std::setprecision(0) << zero, "+0."); // no trailing zeros.but Clang 0.0
  //CHECK(std::showpoint << std::showpos << std::setprecision(1) << zero, "+0."); // normal defaults, width = 1 but Clang 0.0
  //CHECK(std::showpoint << std::showpos << std::setprecision(-1) << zero, "+0.00000"); // MSVC GCC 5+1 = 6 zeros but clang 6+1 zeros
  //CHECK(std::showpoint << std::showpos << std::setprecision(6) << zero, "+0.00000"); // MSVC GCC  5+1=6 zeros, but Clang 6+1
  // So setprecision(0) means precision(6) if normal unless fixed when
  CHECK(std::fixed << std::showpoint << std::setfill('~') << std::setprecision(0) << zero, "0."); // width = 2
  CHECK(std::fixed << std::showpos << std::showpoint << std::setfill('~') << std::setprecision(0) << zero, "+0."); // width = 2
  CHECK(std::fixed << std::showpos << std::showpoint << std::setprecision(0) << zero, "+0."); // width = 3
  CHECK(std::fixed << std::showpos << std::showpoint << std::setprecision(1) << zero, "+0.0"); // width = 4
  CHECK(std::fixed << std::showpos << std::showpoint << std::setprecision(2) << zero, "+0.00"); // width = 5
  CHECK(std::fixed << std::showpos << std::showpoint << std::setprecision(3) << zero, "+0.000"); // width = 6

  CHECK(std::fixed << std::setprecision(0) << zero, "0"); // width = 1
  CHECK(std::fixed << std::showpoint << std::setprecision(1) << zero, "0.0"); // width = 3
  CHECK(std::fixed << std::showpoint << std::setprecision(2) << zero, "0.00"); // width = 4
  CHECK(std::fixed << std::showpoint << std::setprecision(3) << zero, "0.000"); // width = 5

  CHECK(one, "1"); // normal defaults, width = 1
  CHECK(std::showpoint << one, "1.00000"); // Same as above, using default precision(6)
  // << std::showpoint means that precision is used.
  CHECK(std::left << std::showpoint << std::setfill('~') << std::setprecision(6) << one, "1.00000"); // width = 7
  CHECK(std::left << std::showpoint << std::setfill('~') << std::setprecision(1) << one, "1."); // width = 7
  CHECK(std::left << std::showpoint << std::setfill('~') << std::setprecision(0) << one, "1."); //
  // So conclude that precision(0) means default is used.
  // Show how precision controls if width is not set & show that no fill char ~ are used):
  CHECK(std::left << fixed << std::showpoint << std::setprecision(4) << one, "1.0000"); // width = 6
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(6) << one, "1.000000"); // width = 8
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(5) << one, "1.00000"); // width = 7
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(4) << one, "1.0000"); // width = 6
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(3) << one, "1.000"); // width = 5
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(2) << one, "1.00"); // width = 4
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(1) << one, "1.0"); // width = 3
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(0) << one, "1."); // width = 2
  CHECK(std::left << std::showpoint << std::setfill('~') << std::setprecision(0) << one, "1."); //

  CHECK(std::fixed << std::showpoint << std::setprecision(0) << minus1, "-1."); // width = 3 if value >= 1, precision(0) then 1 digit.
  CHECK(std::fixed << std::showpoint << std::setprecision(1) << minus1, "-1.0"); // width = 4
  CHECK(std::fixed << std::showpoint << std::setprecision(2) << minus1, "-1.00"); // width = 5
  CHECK(std::fixed << std::showpoint << std::setprecision(3) << minus1, "-1.000"); // width = 6

  CHECK(std::fixed << std::showpoint << std::setprecision(0) << point1, "0."); // width = 2 - else if value < 1. then NO precision!
  CHECK(std::fixed << std::showpoint << std::setprecision(1) << point1, "0.1"); // width = 3
  CHECK(std::fixed << std::showpoint << std::setprecision(2) << point1, "0.10"); // width = 4
  CHECK(std::fixed << std::showpoint << std::setprecision(3) << point1, "0.100"); // width = 5

  double big = 123456.;
  CHECK(big, "123456"); // default 'normal' is not fixed, nor showpoint, nor scientific.
  // Nicholai M Josuttis, The C++ Standard Library, ISBN 0 201 37926 0, page 624
  CHECK(std::showpoint << big, "123456."); // Add single . point.
  CHECK(std::showpoint << std::setprecision(0) << big, "1.e+05"); // Not now same as no setprecision.
  CHECK(std::showpoint << std::setprecision(1) << big, "1.e+05"); // Cuts down to 1 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(2) << big, "1.2e+05"); // Cuts down to 1+1 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(3) << big, "1.23e+05"); // Cuts down to 1+2 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(4) << big, "1.235e+05"); // Rounds to 1+3 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(5) << big, "1.2346e+05"); // Rounds to 1+4 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(6) << big, "123456."); // Using default 6 for setprecision(6).
  CHECK(std::showpoint << big, "123456."); // Same as using default setprecision(6).
  CHECK(std::showpoint << std::setprecision(7) << big, "123456.0"); // Precision digits (7) in integral & fractional part.
  CHECK(std::showpoint << std::setprecision(17) << big, "123456.00000000000"); // Max 64-bit double setprecision, 18 digits

  // If fixed then precision sets the number of digits in the FRACTIONAL part.
  CHECK(std::fixed << std::showpoint << std::setprecision(0) << big, "123456."); //
  CHECK(std::fixed << std::showpoint << std::setprecision(1) << big, "123456.0"); //
  CHECK(std::fixed << std::showpoint << std::setprecision(2) << big, "123456.00"); //
  CHECK(std::fixed << std::showpoint << std::setprecision(3) << big, "123456.000"); //

  double fourDigits = 1234.;
  CHECK(fourDigits, "1234"); // default 'normal' aka 'automatic' is not fixed, nor scientific, nor showpoint, nor std::showpos.
  // Nicholai M Josuttis, The C++ Standard Library, ISBN 0 201 37926 0, page 624
  CHECK(std::showpoint << fourDigits, "1234.00"); // Add single . point.
  CHECK(std::showpoint << std::setprecision(0) << fourDigits, "1.e+03"); // Same as no setprecision
  CHECK(std::showpoint << std::setprecision(1) << fourDigits, "1.e+03"); // Cuts down to 1 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(2) << fourDigits, "1.2e+03"); // Cuts down to 1+1 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(3) << fourDigits, "1.23e+03"); // Cuts down to 1+2 decimal place and goes exp.
  CHECK(std::showpoint << std::setprecision(4) << fourDigits, "1234."); // Rounds to 1+3 decimal place.
  CHECK(std::showpoint << std::setprecision(5) << fourDigits, "1234.0"); // Rounds to 1+4 decimal place.
  CHECK(std::showpoint << std::setprecision(6) << fourDigits, "1234.00"); // Using default 6 for setprecision(6).
  CHECK(std::showpoint << fourDigits, "1234.00"); // Same as using default setprecision(6).
  CHECK(std::showpoint << std::setprecision(7) << fourDigits, "1234.000"); // Precision digits (7) in integral & fractional part.
  CHECK(std::showpoint << std::setprecision(17) << fourDigits, "1234.0000000000000"); // Max 64-bit double setprecision, 18 digits

  // If fixed then precision sets the number of digits in the FRACTIONAL part.
  CHECK(std::fixed << std::showpoint << std::setprecision(0) << fourDigits, "1234."); //
  CHECK(std::fixed << std::showpoint << std::setprecision(1) << fourDigits, "1234.0"); //
  CHECK(std::fixed << std::showpoint << std::setprecision(2) << fourDigits, "1234.00"); //
  CHECK(std::fixed << std::showpoint << std::setprecision(3) << fourDigits, "1234.000"); //

  // Using fixed, showpoint and setprecision to show significant digits, including decimal point and trailing zeros.
  double twelve34 = 12.34;
  CHECK(twelve34, "12.34");
  CHECK(std::fixed << std::showpoint << std::setprecision(0) << twelve34, "12.");
  CHECK(std::fixed << std::showpoint << std::setprecision(1) << twelve34, "12.3");
  CHECK(std::fixed << std::showpoint << std::setprecision(2) << twelve34, "12.34");
  CHECK(std::fixed << std::showpoint << std::setprecision(3) << twelve34, "12.340"); // Adds trailing zero(s)
  CHECK(std::fixed << std::showpoint << std::setprecision(4) << twelve34, "12.3400");
  CHECK(std::fixed << std::showpoint << std::setprecision(5) << twelve34, "12.34000");

  // fill NOT used unless width is set.
  // Forcing + sign with std::showpos
  CHECK(std::left << fixed << std::showpos << std::showpoint << std::setfill('0') << std::setprecision(0) << one, "+1."); // width = 3
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('0') << std::setprecision(0) << minus1, "-1."); // width = 3

  // fill NOT used unless width is set using setw().
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(4) << one, "1.0000"); // no set width, 4 zeros after.
  // Using setfill and showpoint (and setw) to show trailing zeros.
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('0') << std::setprecision(4) << setw(8) << one, "1.000000"); // 2 extra fill zeros.
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(4) << setw(8) << one, "1.0000~~"); // 2 extra fill ~.
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(4) << setw(7) << one, "1.0000~"); // 1 extra fill ~
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(4) << setw(6) << one, "1.0000"); // width has no effect.
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(4) << setw(5) << one, "1.0000"); // NO fill.
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(3) << setw(4) << one, "1.000");
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(2) << setw(3) << one, "1.00");
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(1) << setw(3) << one, "1.0");
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(1) << setw(1) << one, "1.0"); // want 1.
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(1) << setw(2) << zero, "0.0"); // want 0.
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(1) << setw(1) << zero, "0.0"); // 0.0
  CHECK(std::left << std::fixed << std::noshowpoint << std::setfill('~') << std::setprecision(1) << setw(1) << zero, "0.0");
  CHECK(std::left << std::fixed << std::noshowpoint << std::setfill('~') << std::setprecision(0) << setw(1) << zero, "0");
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(0) << setw(1) << zero, "0.");
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(0) << setw(0) << zero, "0."); // setw(0) undefined?
  CHECK(std::left << std::fixed << std::showpoint << std::setfill('~') << std::setprecision(-1) << one, "1.000000"); // precision < 0 means default = 6

  double d = 1.234567890;
  CHECK(std::setw(20) << d, "             1.23457"); // precision 6, width 20  is 13 spaces + 7 chars.
  CHECK(std::fixed << setw(20) << d, "            1.234568"); // width 20  is 12 spaces + 8 chars.
  CHECK(std::fixed << setw(20) << d, "            1.234568"); // width 20  is 13 spaces + 8 chars.
  CHECK(std::scientific << setw(20) << d, "        1.234568e+00"); // width 20  is 7 spaces + 13 chars.
  CHECK(std::scientific << d, "1.234568e+00"); // width reverts automatically to 0.
  CHECK(std::scientific << setw(20) << std::right << d, "        1.234568e+00"); // width 20  is 7 spaces + 13 chars.
  CHECK(std::scientific << setw(20) << std::left << d, "1.234568e+00        "); // width 20  is 7 spaces + 13 chars.
  CHECK(std::scientific << setw(20) << std::internal << d, "        1.234568e+00"); // width 20  is 7 spaces + 13 chars.
  CHECK(std::scientific << setw(20) << std::showpos << internal << d, "+       1.234568e+00"); // width 20  is 7 spaces + 13 chars.
  CHECK(std::scientific << setw(20) << std::showpos << internal << std::setfill('0') << d, "+00000001.234568e+00"); // width 20 is 6 zero fills, 0 spaces + 14 chars.
  CHECK(std::scientific << setw(20) << std::showpos << internal << std::setfill('*') << d, "+*******1.234568e+00"); // width 20 is 6 * fills, 0 spaces + 14 chars.
  CHECK(std::scientific << setw(20) << std::showpos << d, "       +1.234568e+00"); // width 20  is 6 spaces + 14 chars.
  CHECK(std::scientific << setw(20) << std::showpos << std::left << d, "+1.234568e+00       "); // width 20  is 14 chars + 6 spaces.
  CHECK(std::scientific << setw(20) << std::noshowpos << std::setfill('~') << d, "~~~~~~~~1.234568e+00"); // Combine scientific with fill
  CHECK(std::scientific << setw(20) << std::noshowpos << std::right << std::setfill('~') << d, "~~~~~~~~1.234568e+00"); // Combine scientific with fill
  CHECK(std::scientific << setw(20) << std::noshowpos << std::internal << std::setfill('~') << d, "~~~~~~~~1.234568e+00"); // Combine scientific with fill
  CHECK(std::scientific << setw(20) << std::noshowpos << std::left << std::setfill('~') << d, "1.234568e+00~~~~~~~~"); // Combine scientific with fill

  // Padding with fill char.
  CHECK(std::setw(0) << std::setfill('~') << i, "15"); // width(0) so no fill.
  CHECK(std::setw(1) << std::setfill('~') << i, "15"); // width(1) so no room for fill.
  CHECK(std::setw(2) << std::setfill('~') << i, "15"); // width(1) so no room for fill.
  CHECK(std::setw(3) << std::setfill('~') << i, "~15"); // width(1) so 1 fill.
  CHECK(std::setw(5) << std::setfill('~') << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(std::setw(5) << std::setfill('~') << std::right << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(std::setw(5) << std::setfill('~') << std::internal << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(std::setw(5) << std::setfill('~') << std::left << i, "15~~~"); // width 5 is 2 digits + 3 fill.
 // CHECK(std::setw(5) << std::setfill('~') << noadjust << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(std::setw(5) << std::setfill('~') << std::left << i, "15~~~"); // width 5 is 2 digits + 3 fill.
  CHECK(setfill(' ') << i, "15"); // width goes back to 0 each time.

  // Extra multiple manipulators: spaces, tabs and chars.
  // Not now implemented or used.
  //CHECK(spaces(5), "     ");
  //CHECK(tabs(5), "\t""\t""\t""\t""\t");
  //CHECK(stars(5), "*****");
  //CHECK(chars(5, '~'), "~~~~~");
  //CHECK(chars(5, ' '), "     ");
} // BOOST_AUTO_TEST_CASE(unc_test_stdio)

/*


GCC and MSVC no errors

I:/Cpp/quan/stdio_tests/stdio_tests.cpp(204): error: in "unc_test_stdio": check oss.str() == "0." has failed [0.0 != 0.]
I:/Cpp/quan/stdio_tests/stdio_tests.cpp(205): error: in "unc_test_stdio": check oss.str() == "+0." has failed [+0.0 != +0.]
I:/Cpp/quan/stdio_tests/stdio_tests.cpp(206): error: in "unc_test_stdio": check oss.str() == "+0." has failed [+0.0 != +0.]
I:/Cpp/quan/stdio_tests/stdio_tests.cpp(207): error: in "unc_test_stdio": check oss.str() == "+0.00000" has failed [+0.000000 != +0.00000]
I:/Cpp/quan/stdio_tests/stdio_tests.cpp(208): error: in "unc_test_stdio": check oss.str() == "+0.00000" has failed [+0.000000 != +0.00000]



*/


