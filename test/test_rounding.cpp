/*!
  \file
  \brief Tests of Common rounding.
  \details 
    http://www.diycalculator.com/popup-m-round.shtml#A3
    all rounding types, including round-to-half and asymmetric and symmetric versions.\n
    http://www.chem1.com/acad/webtext/pre/mm3.html
     "The purpose in rounding off is to avoid expressing a value to a greater degree of precision
     than is consistent with the uncertainty in the measurement."\n
     "Observed values should be rounded off to the number of digits
     that most accurately conveys the uncertainty in the measurement."
  \date Oct 09, May 12
  \author Paul A. Bristow
*/

//   \file test_rounding_cout.hpp

// Copyright Paul A. Bristow 2009, 2012.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)  // Boost.Test
  
#define BOOST_TEST_MAIN
// #define BOOST_LIB_DIAGNOSTIC "on" // Report Boost.Test library file details.

#include <boost/static_assert.hpp>

#include <boost/test/unit_test.hpp>
  using boost::unit_test::test_suite;
  using boost::unit_test::unit_test_log;

using boost::unit_test_framework::unit_test_log;
using boost::unit_test_framework::log_level;

#include <boost/quan/rounding.hpp>

#include <iostream>
  using std::cout;
  using std::cerr;
  using std::cin;
  using std::endl;
  using std::ends;
  using std::dec;
  using std::hex;
  using std::fixed;
  using std::left;
  using std::right;
  using std::showpoint;
  using std::defaultfloat;
  using std::scientific;
  using std::boolalpha;
  using std::showpos;
  using std::noshowpos;

#include <iomanip>
  using std::setprecision;
  using std::setw;
#include <string>
  using std::string;

#include <fstream>  // for fstream
  using std::fstream;
  using std::ofstream;

#include <sstream> // stream
  using std::ostringstream;
  using std::string;
  using std::basic_string;
  using std::ios_base;

#include <limits>
  using std::numeric_limits;

BOOST_STATIC_ASSERT (std::numeric_limits<double>::is_iec559); // Assume IEEE 754 ONLY.
// _STATIC_ASSERT (numeric_limits<double>::is_iec559); // and MS STATIC assert.

 // const unsigned int maxdigits10 = 2 + std::numeric_limits<double>::digits * 3010/10000;
  const double eps = std::numeric_limits<double>::epsilon();
  const int digits10 = std::numeric_limits<double>::digits10;

  const double tol = 0.001; // Fractional tolerance for  BOOST_CHECK_CLOSE_FRACTION.

BOOST_AUTO_TEST_CASE(round_test_0)
{ 
  //BOOST_CHECK_EQUAL(round_ms(12345, 1), "12345."); // 3 significant digits, so rounder digit is '8', and m is +1
}

BOOST_AUTO_TEST_CASE(round_test_1)
{ // Round to cout tests.

  string message("Round test: " __FILE__ );
#ifdef __TIMESTAMP__
  message += " at " BOOST_STRINGIZE(__TIMESTAMP__);
#endif
#ifdef _MSC_FULL_VER
  message +=  ", MSVC version " BOOST_STRINGIZE(_MSC_FULL_VER) ".";
#else
  message += "."
#endif

  BOOST_MESSAGE(message);
  BOOST_MESSAGE("double maxdigits10 is " << maxdigits10);
    cout << "std::numeric_limits<double>::max_exponent = " << std::numeric_limits<double>::max_exponent 
    << ", \nstd::numeric_limits<double>::max_exponent10 = " << std::numeric_limits<double>::max_exponent10 
    << ",\nstd::numeric_limits<double>::max_exponent10 -1 = "<< std::numeric_limits<double>::max_exponent10 -1 << ". " << endl;
    //  std::numeric_limits<double>::max_exponent = 1024, 
    //  std::numeric_limits<double>::max_exponent10 = 308,
    //  std::numeric_limits<double>::max_exponent10 -1 = 307. 
#ifdef _MSC_VER
    BOOST_CHECK_EQUAL(maxdigits10, 17U);
#endif

  // Experiments to see if a special meaning can be assigned to BOTH fixed and scientific.
  // BUT the std::hex display option added with TR1
  // means CANNOT use this mechanism.
  // #define _IOShexfloat	0x3000	// added with TR1.
  // Last top Bit 15 also appears used, so MUST use uncflags for controlling scaling etc.

  //std::ios_base::fmtflags fs = cout.flags();
  //cout << hex << "cout.flags() " << (cout.flags() & std::ios_base::floatfield) << endl;
  //cout << "std::ios::fixed "<< std::ios::fixed  << fixed << endl;
  //cout << hex << "cout.flags() " << (cout.flags() & std::ios_base::floatfield) << endl;
  //cout << "std::ios::scientific "<< std::ios::scientific << scientific << endl;
  //cout << hex << "cout.flags() " << (cout.flags() & std::ios_base::floatfield) << endl;
 
  //cout.setf(std::ios::fixed | std::ios::scientific, std::ios_base::floatfield);
  //cout.setf(std::ios::fixed | std::ios::scientific);
  //cout << hex << "cout.flags() = " << (cout.flags() & std::ios_base::floatfield) << endl;
  // so need a extra 'special' manipulator to set BOTH scientific AND fixed.
} //  BOOST_AUTO_TEST_CASE(round_test_1)



BOOST_AUTO_TEST_CASE(round_f_test)
{
  // Removed as assume can do scaling with Boost,Units autoscaling.
  //{ // Test unc scaling to scientific and engineering powers.
  //    scaled = true;
  //    BOOST_CHECK_EQUAL(round_f(123.4567, 3), "123."); // No trailing space.
  //    BOOST_CHECK_EQUAL(round_f(1234.567, 3), "1.23 k");
  //    BOOST_CHECK_EQUAL(round_f(12345678.9, 3), "12.3 M");
  //    BOOST_CHECK_EQUAL(round_f(123456789., 4), "123.5 M");
  //    BOOST_CHECK_EQUAL(round_f(1234567890., 6), "1.23457 G");
  //    scaled = false;
  //}
  { // Test round_f.
    // round_f is fixed format.
    double v = 1.23456; // Value to test rounded output.
    BOOST_CHECK_EQUAL(round_f(v, 0), ""); // With message "Trying to output zero significant digits!".
    BOOST_CHECK_EQUAL(round_f(v, -1), ""); // With message "Trying to output -1 significant digits!".
    BOOST_CHECK_EQUAL(round_f(v, 1), "1."); // 
    BOOST_CHECK_EQUAL(round_f(v, 2), "1.2"); // 
    BOOST_CHECK_EQUAL(round_f(v, 3), "1.23"); // 
    BOOST_CHECK_EQUAL(round_f(v, 4), "1.235"); // 
    BOOST_CHECK_EQUAL(round_f(v, 5), "1.2346"); // 
    BOOST_CHECK_EQUAL(round_f(v, 6), "1.23456"); // 
    BOOST_CHECK_EQUAL(round_f(v, 7), "1.234560"); // 
    BOOST_CHECK_EQUAL(round_f(v, 8), "1.2345600"); // 
    BOOST_CHECK_EQUAL(round_f(v, 9), "1.23456000"); // 
    BOOST_CHECK_EQUAL(round_f(v, 10), "1.234560000"); // 
    BOOST_CHECK_EQUAL(round_f(v, 11), "1.2345600000"); // 
    BOOST_CHECK_EQUAL(round_f(v, 12), "1.23456000000"); // 
    BOOST_CHECK_EQUAL(round_f(v, 13), "1.234560000000"); // 
    BOOST_CHECK_EQUAL(round_f(v, 14), "1.2345600000000"); // Maximum significant digits -1
    BOOST_CHECK_EQUAL(round_f(v, 15), "1.23456000000000"); // 
    // Message is  "Maximum significant digits is 15" - and several repeats.
    BOOST_CHECK_EQUAL(round_f(v, 16), "1.23456000000000"); //  Maximum significant digits is 15
    BOOST_CHECK_EQUAL(round_f(v, 17), "1.23456000000000"); // Maximum significant digits is 15
    BOOST_CHECK_EQUAL(round_f(v, std::numeric_limits<double>::digits10), "1.23456000000000"); // 
    BOOST_CHECK_EQUAL(round_f(v, maxdigits10), "1.23456000000000"); // Maximum significant digits is 15 (should be std::numeric_limits<double>::maxdigits10).

    v = 1.23456789012345678901234567890; // Check that all the digits are used, and rounded at digit 15
    BOOST_CHECK_EQUAL(round_f(v, maxdigits10), "1.23456789012346"); // Maximum significant digits is 15 (should be std::numeric_limits<double>::maxdigits10).

    v = 0.12345678901234567890; // Value < unity.
    BOOST_CHECK_EQUAL(round_f(v, 1), "0.1"); // 
    BOOST_CHECK_EQUAL(round_f(v, 2), "0.12"); // 
    BOOST_CHECK_EQUAL(round_f(v, 3), "0.123"); // 
    BOOST_CHECK_EQUAL(round_f(v, digits10-10), "0.12346"); // 5
    BOOST_CHECK_EQUAL(round_f(v, 13), "0.1234567890123"); // 13
    BOOST_CHECK_EQUAL(round_f(v, digits10 -2), "0.1234567890123"); // 13
    BOOST_CHECK_EQUAL(round_f(v, 14), "0.12345678901235"); // 14
    BOOST_CHECK_EQUAL(round_f(v, digits10 -1), "0.12345678901235"); // 14
    BOOST_CHECK_EQUAL(round_f(v, maxdigits10), "0.123456789012346"); // Maximum significant digits is 15 (should be std::numeric_limits<double>::maxdigits10).
  
    v = 0.0012345678901234567890; // Value much less than unity.
    BOOST_CHECK_EQUAL(round_f(v, 1), "0.001"); // 
    BOOST_CHECK_EQUAL(round_f(v, 2), "0.0012"); // 
    BOOST_CHECK_EQUAL(round_f(v, 3), "0.00123"); // 
    BOOST_CHECK_EQUAL(round_f(v, 4), "0.001235"); // 
    BOOST_CHECK_EQUAL(round_f(v, 10), "0.001234567890"); // 
    BOOST_CHECK_EQUAL(round_f(v, 11), "0.0012345678901"); // 
    BOOST_CHECK_EQUAL(round_f(v, 12), "0.00123456789012"); // 
    BOOST_CHECK_EQUAL(round_f(v, 13), "0.001234567890123"); // 
    BOOST_CHECK_EQUAL(round_f(v, 14), "0.0012345678901235"); // 
    BOOST_CHECK_EQUAL(round_f(v, digits10), "0.00123456789012346"); // 
    BOOST_CHECK_EQUAL(round_f(v, maxdigits10), "0.00123456789012346"); // 

    v = 12345678901234567890.0; // Value > max_digits10 digits.
    BOOST_CHECK_EQUAL(round_f(v, 4),           "12350000000000000000.");
    BOOST_CHECK_EQUAL(round_f(v, 10),          "12345678900000000000.");
    BOOST_CHECK_EQUAL(round_f(v, maxdigits10), "12345678901234600000."); 

    v = 0.0000000012345678901234567890; // Value < max_digits10 digits.
    BOOST_CHECK_EQUAL(round_f(v, 4), "0.000000001235"); // 

    // Value max and min.
    v = (std::numeric_limits<double>::max)(); // 1.797693134862316e+308
    BOOST_CHECK_EQUAL(round_f(v, 4), "179800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000."); // 
    v = (std::numeric_limits<double>::min)();// 2.2250738585072014e-308
    BOOST_CHECK_EQUAL(round_f(v, 4), "0.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002225"); // 
  }
} //BOOST_AUTO_TEST_CASE(round_f_test)


BOOST_AUTO_TEST_CASE(round_e_test)  // Test round_e.
{ // round_e is scientific or exponential format.
  {
    double d = 1.1234012340123401234; // No carry examples.

    BOOST_CHECK_EQUAL(round_e(d, 1), "1.");
    BOOST_CHECK_EQUAL(round_e(d, 2), "1.1");
    BOOST_CHECK_EQUAL(round_e(d, 3), "1.12"); 
    BOOST_CHECK_EQUAL(round_e(d, 4), "1.123"); // 4 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 5), "1.1234"); // 5 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 6), "1.12340"); //6 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, digits10-1), "1.1234012340123"); // 14 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, digits10),   "1.12340123401234"); // 15 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, maxdigits10), "1.12340123401234"); // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, maxdigits10+1), "1.12340123401234");  // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, maxdigits10+2), "1.12340123401234");  // 15 sigdigits, because reached digits10.
  }

  {
    double d = 1.12345678901234567890; // 

  //cout << "round_e(d, 1); " << round_e(d, 1);  // 1.
  //cout << "round_e(d, 2); " << round_e(d, 2); // 1.1
  //cout << "round_e(d, 3); " << round_e(d, 3); // 1.12
  //cout << "round_e(d, 4); "<< round_e(d, 4); // 1.123
  //cout << "round_e(d, 5); "<< round_e(d, 5); // 1.1234
  //cout << "round_e(d, 6); "<< round_e(d, 6); // 1.12340
  //cout << "round_e(d, digits10); "<< round_e(d, digits10); // 
  //cout << "round_e(d, digits10); "<< round_e(d, maxdigits10); //

  BOOST_CHECK_EQUAL(round_e(d, 1), "1."); // 1 digits.
  BOOST_CHECK_EQUAL(round_e(d, 2), "1.1"); // 2 digits.
  BOOST_CHECK_EQUAL(round_e(d, 3), "1.12"); // 3 digits.
  BOOST_CHECK_EQUAL(round_e(d, 4), "1.123"); // 4 digits.
  BOOST_CHECK_EQUAL(round_e(d, 5), "1.1235"); // 5 digits.
  BOOST_CHECK_EQUAL(round_e(d, 6), "1.12346"); // 6 digits.

  BOOST_CHECK_EQUAL(round_e(d, 11), "1.1234567890"); // 11 digits.
  BOOST_CHECK_EQUAL(round_e(d, 12), "1.12345678901"); // 12 digits.
  BOOST_CHECK_EQUAL(round_e(d, 13), "1.123456789012"); // 13 digits.
  BOOST_CHECK_EQUAL(round_e(d, 14), "1.1234567890123"); // 14 digits.
  BOOST_CHECK_EQUAL(round_e(d, 15), "1.12345678901235"); // 15 digits.
  BOOST_CHECK_EQUAL(round_e(d, 16), "1.12345678901235"); // 15 digits, because reached digits10.
  BOOST_CHECK_EQUAL(round_e(d, 17), "1.12345678901235");  // 15 digits, because reached digits10.
  BOOST_CHECK_EQUAL(round_e(d, 18), "1.12345678901235");  // 15 digits, because reached digits10.
  }
  {
    double d = 0.99999999999999999; // Round to before decimal point.
    // cout << setprecision(4) << showpoint << d << endl; // 1.000
    BOOST_CHECK_EQUAL(round_e(d, 1), "1."); // 1 digit.
    BOOST_CHECK_EQUAL(round_e(d, 2), "1.0"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "1.00"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "1.000"); // 4 digits.
    // ... 
    BOOST_CHECK_EQUAL(round_e(d, 15), "1.00000000000000"); // 15 digits.
    BOOST_CHECK_EQUAL(round_e(d, 16), "1.00000000000000"); // 15 digits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 17), "1.00000000000000");  // 15 digits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 18), "1.00000000000000");  // 15 digits, because reached digits10.
  }

  {
    double d = 19.999999999999999; // Actually 2.00000000000000
    BOOST_CHECK_EQUAL(round_e(d, 1), "2.e1"); // 1 digit.
    BOOST_CHECK_EQUAL(round_e(d, 2), "2.0e1"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "2.00e1"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "2.000e1"); // 4 digits.
    //... 
    BOOST_CHECK_EQUAL(round_e(d, 15), "2.00000000000000e1"); // 15 digits.
    BOOST_CHECK_EQUAL(round_e(d, 16), "2.00000000000000e1"); // 15 digits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 17), "2.00000000000000e1");  // 15 digits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 18), "2.00000000000000e1");  // 15 digits, because reached digits10.
  }
  { 
    double d = 19.99; // Roundable.
    BOOST_CHECK_EQUAL(round_e(d, 1), "2.e1"); // 1 digit.
    BOOST_CHECK_EQUAL(round_e(d, 2), "2.0e1"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "2.00e1"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "1.999e1"); // 4 digits.
    BOOST_CHECK_EQUAL(round_e(d, 5), "1.9990e1"); // 5 digits, last is trailing significant zero.
    //... 
    BOOST_CHECK_EQUAL(round_e(d, 15), "1.99900000000000e1"); // 15 digits.
    BOOST_CHECK_EQUAL(round_e(d, 16), "1.99900000000000e1"); // 15 digits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 17), "1.99900000000000e1");  // 15 digits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 18), "1.99900000000000e1");  // 15 digits, because reached digits10.
  }
    { // Positive exponent.
    double d = 9.9e+2; // 990
    //cout << setprecision(1) << fixed << d << endl; //  990.0
    //cout << setprecision(1) << scientific << d << endl; // 9.9e+002
    //cout << setprecision(0) << scientific << d << endl; // 9.9e+002
    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e3"); // 1 digit, rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e2"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.90e2"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.900e2"); // 4 digits.
  }

  { // Big positive exponent.
    double d = 9.9e+20; // 990
    //cout << setprecision(1) << fixed << d << endl; //  990000000000000000000.0
    //cout << setprecision(1) << scientific << d << endl; // 9.9e+020
    //cout << setprecision(0) << scientific << d << endl; // 9.9e+020
    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e21"); // 1 digit, rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e20"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.90e20"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.900e20"); // 4 digits.
  }

    { // Very big positive exponent.
    double d = 9.9e+123; // 
    //cout << setprecision(1) << fixed << d << endl; //  990000000000000000000.0
    //cout << setprecision(1) << scientific << d << endl; // 9.9e+020
    //cout << setprecision(0) << scientific << d << endl; // 9.9e+020
    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e124"); // 1 digit, rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e123"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.90e123"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.900e123"); // 4 digits.
  }

  { // Negative exponent.
    double d = 9.9e-2;
    //cout << setprecision(1) << fixed << d << endl; // 0.1
    //cout << setprecision(1) << scientific << d << endl; // 9.9e-002
    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e-1"); // 1 digit, rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e-2"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.90e-2"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.900e-2"); // 4 digits.
  }

  { // Big negative exponent.
    double d = 9.9e-20;
    //cout << setprecision(1) << fixed << d << endl; // 0.1
    //cout << setprecision(1) << scientific << d << endl; // 9.9e-002
    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e-19"); // 1 digit, rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e-20"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.90e-20"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.900e-20"); // 4 digits.
  }
  { // Very big negative exponent.
    double d = 9.9e-234;
    //cout << setprecision(1) << fixed << d << endl; // 0.1
    //cout << setprecision(1) << scientific << d << endl; // 9.9e-002
    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e-233"); // 1 digit, rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e-234"); // 2 digits.
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.90e-234"); // 3 digits.
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.900e-234"); // 4 digits.
  }
  { // Negative values.
    double d = -1.;
    BOOST_CHECK_EQUAL(round_e(d, 1), "-1."); // 1 digit, not rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "-1.0"); // 2 digit, not rounded.
    BOOST_CHECK_EQUAL(round_e(d, 3), "-1.00"); // 2 digit, not rounded.
  }

  { // Problematic rounding.
    // 1.5 is stored as 1.49999999999999, but should round up to 1.5, not down to 1.4.
    double d = 1.5;
    BOOST_CHECK_EQUAL(round_e(d, 1), "2."); // 1 digit, not rounded.
    BOOST_CHECK_EQUAL(round_e(d, 2), "1.5"); // 2 digit, not rounded.
    BOOST_CHECK_EQUAL(round_e(d, 3), "1.50"); // 2 digit, not rounded.
  }

  { // Misc
    double d = 1.2945678901234567890e6;
    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e6");
    BOOST_CHECK_EQUAL(round_e(d, 2), "1.3e6");
    BOOST_CHECK_EQUAL(round_e(d, 3), "1.29e6");
    BOOST_CHECK_EQUAL(round_e(d, 4), "1.295e6"); 
    BOOST_CHECK_EQUAL(round_e(d, 11), "1.2945678901e6"); // 11 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 12), "1.29456789012e6"); // 12 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 13), "1.294567890123e6"); // 13 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 14), "1.2945678901235e6"); // 14 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 15), "1.29456789012346e6"); // 15 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 16), "1.29456789012346e6"); // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 17), "1.29456789012346e6");  // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 18), "1.29456789012346e6");  // 15 sigdigits, because reached digits10.

    d = 987.654; // 9.87654e2

    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e3");
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e2");
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.88e2");
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.877e2"); 
    BOOST_CHECK_EQUAL(round_e(d, 5), "9.8765e2"); 
    BOOST_CHECK_EQUAL(round_e(d, 6), "9.87654e2"); 
    BOOST_CHECK_EQUAL(round_e(d, 7), "9.876540e2"); // 11 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 11), "9.8765400000e2"); // 11 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 12), "9.87654000000e2"); // 12 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 13), "9.876540000000e2"); // 13 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 14), "9.8765400000000e2"); // 14 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 15), "9.87654000000000e2"); // 15 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 16), "9.87654000000000e2"); // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 17), "9.87654000000000e2");  // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 18), "9.87654000000000e2");  // 15 sigdigits, because reached digits10.


     d = 0.0987654; // 9.87654e-2

    BOOST_CHECK_EQUAL(round_e(d, 1), "1.e-1");
    BOOST_CHECK_EQUAL(round_e(d, 2), "9.9e-2");
    BOOST_CHECK_EQUAL(round_e(d, 3), "9.88e-2");
    BOOST_CHECK_EQUAL(round_e(d, 4), "9.877e-2"); 
    BOOST_CHECK_EQUAL(round_e(d, 5), "9.8765e-2"); 
    BOOST_CHECK_EQUAL(round_e(d, 6), "9.87654e-2"); 
    BOOST_CHECK_EQUAL(round_e(d, 7), "9.876540e-2"); // 7 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 11), "9.8765400000e-2"); // 11 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 12), "9.87654000000e-2"); // 12 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 13), "9.876540000000e-2"); // 13 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 14), "9.8765400000000e-2"); // 14 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 15), "9.87654000000000e-2"); // 15 sigdigits.
    BOOST_CHECK_EQUAL(round_e(d, 16), "9.87654000000000e-2"); // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 17), "9.87654000000000e-2");  // 15 sigdigits, because reached digits10.
    BOOST_CHECK_EQUAL(round_e(d, 18), "9.87654000000000e-2");  // 15 sigdigits, because reached digits10.
  }

  { // Non-finites.
    double d = std::numeric_limits<double>::quiet_NaN();
    BOOST_CHECK_EQUAL(round_e(d, 1), "NaN");
    BOOST_CHECK_EQUAL(round_e(-d, 1), "-NaN");

    d = std::numeric_limits<double>::infinity();
    BOOST_CHECK_EQUAL(round_e(d, 1), "Inf");
    BOOST_CHECK_EQUAL(round_e(-d, 1), "-Inf");
  }
} // BOOST_AUTO_TEST_CASE(round_e_test) 

BOOST_AUTO_TEST_CASE(cdf_quantile_test)
  { // Test the calculation of CDF and quantiles for triangular and uniform distributions.
    static const double sqrt_6 = 2.4494897427831780981972840747058913919659474804966; // sqrt(6)

    boost::math::triangular tdist(-sqrt_6, 0., +sqrt_6); // Wimmer page 23, below eq 6.
    double alpha = 0.005;
    double qlo = quantile(complement(tdist, alpha));
    double qhi = quantile(tdist, alpha);
    //cout << "quantile(complement(tdist, alpha));  " << qlo << ", quantile(tdist, alpha) " << qhi << endl;
    //  quantile(complement(tdist, alpha));  2.20454, quantile(tdist, alpha) -2.20454
    //cout << "cdf( quantile(complement(tdist, alpha)) = "<< cdf_tri(qlo) << ", cdf(quantile(tdist, alpha) = " << cdf_tri(qhi) << endl;
    //   cdf( quantile(complement(tdist, alpha)) = 0.995, cdf(quantile(tdist, alpha) = 0.005
    BOOST_CHECK_CLOSE_FRACTION(cdf_tri(qhi), alpha, 0.001); 
    BOOST_CHECK_CLOSE_FRACTION(cdf_tri(qlo), 1. - alpha, 0.001); 

    double q01l = quantile_tri(1. - alpha);
    double q01h = quantile_tri(alpha);

    //cout << "quantile_tri(1. - alpha) " << q01l << ", quantile_tri(alpha); " << q01h << endl;
    //   quantile_tri(1. - alpha) 2.20454, quantile_tri(alpha); -2.20454

    double c01h = cdf_tri(q01h);
    BOOST_CHECK_CLOSE_FRACTION(c01h, alpha, 0.02); // Fails with 0.01, OK with 0.02
    // difference{0.292893} between c01h{0.0070710678118655057} and alpha{0.01} exceeds 0.01

    double c01l = cdf_tri(q01l);
    BOOST_CHECK_CLOSE_FRACTION(c01l, 1. - alpha, 0.005);  // OK at 0.005
    //cout << "cdf_tri(quantile_tri(alpha)) " << c01l << ",cdf_tri(quantile_tri(1. - alpha)) " << c01h << endl;
    //  cdf_tri(quantile_tri(alpha)) 0.995,cdf_tri(quantile_tri(1. - alpha)) 0.005
    BOOST_CHECK_EQUAL(cdf_tri(-3.), 0); // check > sqrt_6 limits.
    BOOST_CHECK_EQUAL(cdf_tri(+3.), 1.);

    boost::math::uniform udist(-sqrt_3, +sqrt_3); // Wimmer page 3, below eq 5.
  // Loop back tests.
  //cout << "loopback boost::math cdf(z) "<< endl;
    for (double d = -2.4; d < +2.4; d += 0.2)
    {
     // cout << d << ' '<< cdf(tdist, d) << ' ' << quantile(tdist, cdf(tdist, d)) << ' '<< d - quantile(tdist, cdf(tdist, d)) << endl;
      //BOOST_CHECK_CLOSE_FRACTION(d, quantile(tdist, cdf(tdist, d)), 0.01); fails for d near zero
      BOOST_CHECK_SMALL(d - quantile(tdist, cdf(tdist, d)), 100 * eps);
    }

    //cout << "loopback boost::math quantile(alpha))" << endl;
    for (double a = 0.; a <= 1.; a += 0.1)
    {
      //cout << a << ' ' << quantile(tdist, a) << ' ' << cdf(tdist, quantile(tdist, a)) << ' ' << a - cdf(tdist, quantile(tdist, a)) << endl;
      BOOST_CHECK_CLOSE_FRACTION(a, cdf(tdist, quantile(tdist, a)), 0.01);
    }

    //cout << "loopback triangular cdf(z)" << endl;
    for (double d = -2.4; d < +2.4; d += 0.2)
    {
      //cout << d << ' '<< cdf_tri(d) << ' ' << quantile_tri(cdf_tri(tdist, d)) << ' '<< d - quantile_tri(cdf_tri(tdist, d)) << endl;
      //BOOST_CHECK_CLOSE_FRACTION(d, quantile_tri(cdf(tdist, d)), 0.01); // fails d near zero
      BOOST_CHECK_SMALL(d - quantile_tri(cdf(tdist, d)), 100 * eps);
    }

    //cout << "loopback triangular quantile" << endl;
    for (double a = 0.; a <= 1.; a += 0.1)
    {
      //cout << a << ' ' << quantile_tri(a) << ' ' << cdf_tri(quantile_tri(a)) << ' ' << a - cdf_tri(quantile_tri(a)) << endl;
      BOOST_CHECK_CLOSE_FRACTION(a, cdf_tri(quantile_tri(a)), 0.01);
    }
    //cout << "loopback uniform quantile" << endl;
    for (double a = 0.; a <= 1.; a += 0.1)
    {
      //cout << a << ' ' << quantile_uni(a) << ' ' << cdf_uni(quantile_uni(a)) << ' ' << a - cdf_uni(quantile_uni(a)) << endl;
      BOOST_CHECK_CLOSE_FRACTION(a, cdf_uni(quantile_uni(a)), 0.01);
    }
    //cout << "loopback uniform cdf_uni(z)" << endl;
    for (double d = -sqrt_3; d < +sqrt_3; d += 0.2)
    {
      //cout << d << ' '<< cdf(d) << ' ' << quantile_tri(cdf(tdist, d)) << ' '<< d - quantile_tri(cdf(tdist, d)) << endl;
      //BOOST_CHECK_CLOSE_FRACTION(d, quantile_tri(cdf(tdist, d)), 0.01); // fails d near zero
      BOOST_CHECK_SMALL(d - quantile_uni(cdf(udist, d)), 100 * eps);
    }    
    //  Compare Wimmer and Boost::math uniform cdf
    for (double d = -sqrt_3; d < +sqrt_3; d += 0.2)
    {
      BOOST_CHECK_SMALL((cdf_uni(d) - cdf(udist, d)), 100 * eps);
    }    
    //  Compare Wimmer and Boost::math uniform quant
    for (double a = 0.; a <= 1.; a += 0.1)
    {
      BOOST_CHECK_SMALL((quantile_uni(a) - quantile(udist, a)), 100 * eps);
    }    
  } // BOOST_AUTO_TEST_CASE(cdf_quantile_test)

BOOST_AUTO_TEST_CASE(round_ms_test)
{  // Test round_ms function.
  //template<typename = FPT> std::string round_ms(FPT v, signed int m); // Round v to digit m.

  double v = (std::numeric_limits<double>::max)();
  BOOST_CHECK_EQUAL(round_ms(v, std::numeric_limits<double>::max_exponent10), "0."); // 1.797693134862316 and 308
  BOOST_CHECK_EQUAL(round_ms(-v, std::numeric_limits<double>::max_exponent10), "0."); // -1.797693134862316 and 308
  BOOST_CHECK_EQUAL(round_ms(v, std::numeric_limits<double>::max_exponent10 -1), "200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000."); // -1.797693134862316 and 307
  BOOST_CHECK_EQUAL(round_ms(-v, std::numeric_limits<double>::max_exponent10 -1), "-200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000."); // -1.797693134862316 and 307

  BOOST_CHECK_EQUAL(round_ms(v, 308), "0."); // 1.797693134862316 and 308
  BOOST_CHECK_EQUAL(round_ms(-v, 308), "0."); // -1.797693134862316 and 308
  BOOST_CHECK_EQUAL(round_ms(v, 307), "200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000."); // -1.797693134862316 and 308
  BOOST_CHECK_EQUAL(round_ms(-v, 307), "-200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000."); // -1.797693134862316 and 308

  v = (std::numeric_limits<double>::min)();
  // cout <<setprecision (17) << v << endl; // 2.2250738585072014e-308 
  BOOST_CHECK_EQUAL(round_ms(v, std::numeric_limits<double>::min_exponent10), "0."); //  2.22507385850720 and -308
  BOOST_CHECK_EQUAL(round_ms(v, std::numeric_limits<double>::min_exponent10+1), "0."); //  2.22507385850720 and -307 ?? is 0. expected?
  BOOST_CHECK_EQUAL(round_ms(v, std::numeric_limits<double>::min_exponent10+2), "0."); //  2.22507385850720 and -306 ?? is 0. expected?

  // Zero (positive).
  BOOST_CHECK_EQUAL(round_ms(0., 0), "0."); // 
  BOOST_CHECK_EQUAL(round_ms(0., +1), "0."); // 
  BOOST_CHECK_EQUAL(round_ms(0., -1), "0."); // 
  BOOST_CHECK_EQUAL(round_ms(0., +2), "0."); // 
  BOOST_CHECK_EQUAL(round_ms(0., -2), "0.0"); // 
  BOOST_CHECK_EQUAL(round_ms(0., -3), "0.00"); // 
  // Zero (negative).
  BOOST_CHECK_EQUAL(round_ms(-0., +2), "0."); // 
  BOOST_CHECK_EQUAL(round_ms(-0., +1), "0."); // 
  BOOST_CHECK_EQUAL(round_ms(-0., 0), "0."); // 
  BOOST_CHECK_EQUAL(round_ms(-0., -1), "-0."); // 
  BOOST_CHECK_EQUAL(round_ms(-0., -3), "-0.00"); // 
  BOOST_CHECK_EQUAL(round_ms(-3278.35876, -3), "-3278.36"); // 

  // Infinity plus and minus.
  BOOST_CHECK_EQUAL(round_ms(+std::numeric_limits<double>::infinity(), +2), "inf");
  BOOST_CHECK_EQUAL(round_ms(-std::numeric_limits<double>::infinity(), +1), "-inf");
  // NaN, note sign bit of NaN is recognized.
  BOOST_CHECK_EQUAL(round_ms(+std::numeric_limits<double>::quiet_NaN(), +2), "NaN");
  BOOST_CHECK_EQUAL(round_ms(-std::numeric_limits<double>::quiet_NaN(), +1), "-NaN");

  BOOST_CHECK_EQUAL(round_ms(123.45678901234567890, -2),"123.5") ; //
  // Wimmer example 2 page 1660
  BOOST_CHECK_EQUAL(round_ms(3278.35876, -3), "3278.36"); // 
  BOOST_CHECK_EQUAL(round_ms(159.21, +1), "200."); //
  BOOST_CHECK_EQUAL(round_ms(1121.85, +3), "0."); //
  // Some more examples.
  BOOST_CHECK_EQUAL(round_ms(1111.1111, +3), "0."); //
  BOOST_CHECK_EQUAL(round_ms(1111.1111, +2), "1000."); //
  BOOST_CHECK_EQUAL(round_ms(1111.1111, +1), "1100."); //
  BOOST_CHECK_EQUAL(round_ms(1111.1111, +0), "1110."); //
  BOOST_CHECK_EQUAL(round_ms(1111.1111, -1), "1111."); //
  BOOST_CHECK_EQUAL(round_ms(1111.1111, -2), "1111.1"); //
  BOOST_CHECK_EQUAL(round_ms(1111.1111, -3), "1111.11"); //

  BOOST_CHECK_EQUAL(round_ms(9999.9999, +4), "0."); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, +3), "0."); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, +2), "10000."); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, +1), "10000."); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, +0), "10000."); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, -1), "10000."); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, -2), "10000.0"); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, -3), "10000.00"); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, -4), "10000.000"); //
  BOOST_CHECK_EQUAL(round_ms(9999.9999, -5), "9999.9999"); //

  BOOST_CHECK_EQUAL(round_ms(1703.12345679, -7), "1703.123457"); // David A. Pimentel 6 digits after decimal point example 2 (so m = -7).

  BOOST_CHECK_EQUAL(round_ms(0.152, -3), "0.15");
  BOOST_CHECK_EQUAL(round_ms(0.155, -3), "0.16");
  BOOST_CHECK_EQUAL(round_ms(0.159, -3), "0.16");
  // With option to use shorter form with no zero preceeding decimal point.
  //BOOST_CHECK_EQUAL(round_ms(0.152, -3), ".15");
  //BOOST_CHECK_EQUAL(round_ms(0.155, -3), ".16");
  //BOOST_CHECK_EQUAL(round_ms(0.159, -3), ".16");

  BOOST_CHECK_EQUAL(round_ms(0.159, 0), "0.");
  BOOST_CHECK_EQUAL(round_ms(0.159, -1), "0.");
  BOOST_CHECK_EQUAL(round_ms(0.159, -2), "0.2");
  BOOST_CHECK_EQUAL(round_ms(0.159, -3), "0.16");
  BOOST_CHECK_EQUAL(round_ms(0.159, -4), "0.159");

  BOOST_CHECK_EQUAL(round_ms(0.01234567, -2), "0.");
  BOOST_CHECK_EQUAL(round_ms(0.01234567, -3), "0.01");
  BOOST_CHECK_EQUAL(round_ms(0.01234567, -4), "0.012");
  BOOST_CHECK_EQUAL(round_ms(0.0001234567, -6), "0.00012");

  // Examples from http://www.chem1.com/acad/webtext/pre/mm3.html
  BOOST_CHECK_EQUAL(round_ms(34.216, -2), "34.2"); // 3 significant digits, so rounder digit is '1', and m is -2
  BOOST_CHECK_EQUAL(round_ms(6.252, -2), "6.3"); // 2 significant digits, so rounder digit is '5', and m is -2
  BOOST_CHECK_EQUAL(round_ms(39.99, -2), "40.0"); // 3 significant digits, so rounder digit is '9', and m is -2
  BOOST_CHECK_EQUAL(round_ms(85381., 1), "85400."); // 3 significant digits, so rounder digit is '8', and m is +1
  BOOST_CHECK_EQUAL(round_ms(0.04597, -5), "0.0460"); // 3 significant digits, so rounder digit is '7', and m is -5

  BOOST_CHECK_EQUAL(round_ms(1.23, -2),"1.2"); // round to -1, so m = -2 round down, no carry
  BOOST_CHECK_EQUAL(round_ms(1.25, -2),"1.3"); // round -1, so m = -2, round up no carry.
  BOOST_CHECK_EQUAL(round_ms(1.95, -2),"2.0"); // round -1, so m = -2, round up, and carry.

  // Some very large values.
  BOOST_CHECK_EQUAL(round_ms(1.95e+6, +2),"1950000."); // 
  BOOST_CHECK_EQUAL(round_ms(1.95e+9, +2),"1950000000."); // 
  BOOST_CHECK_EQUAL(round_ms(1.95e+12, +2),"1950000000000."); // 
  BOOST_CHECK_EQUAL(round_ms(1.95e+15, +2),"1950000000000000."); // 
  BOOST_CHECK_EQUAL(round_ms(1.95e+18, +2),"1950000000000000000."); // 
  BOOST_CHECK_EQUAL(round_ms(1.95e+24, +2),"1950000000000000000000000."); // 
  // numeric_limits<double>::max() = 1.797693134862316 E 308
  BOOST_CHECK_EQUAL(round_ms(numeric_limits<double>::max(), 305),"180000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000."); // 
  BOOST_CHECK_EQUAL(round_ms(numeric_limits<double>::min(), -307),"0."); 

  // Some small values.
  BOOST_CHECK_EQUAL(round_ms(1.95e-6, -8),"0.0000020");
  BOOST_CHECK_EQUAL(round_ms(-1.95e-6, -8),"-0.0000020");
  BOOST_CHECK_EQUAL(round_ms(-1.95e-12, -14),"-0.0000000000020");
} // BOOST_AUTO_TEST_CASE(round_ms_test)

BOOST_AUTO_TEST_CASE(Wimmer_3)
{ // Test by comparion with Wimmer example 3, page 1660.
  // Calculate confidence interval.
  //std::pair<double, double> conf_interval(double value, double unc, double df, double alpha);
  std::pair<double, double> p(97.8725, 157.7975);
  // cout << p << endl; // <97.8725, 157.798>
  p = conf_interval(127.835, 15.287);  // Default to 1 degree of freedom, alpha 0.05, 95% confidence & gaussian.
  BOOST_CHECK_CLOSE_FRACTION(p.first, 97.8725, tol);
  BOOST_CHECK_CLOSE_FRACTION(p.second, 157.798, tol); // 
  // 2nd example for 99%, alpha = 0.01:
  p = conf_interval(127.835, 15.287, 1.,  0.01);  // 99% confidence.
  BOOST_CHECK_CLOSE_FRACTION(p.first, 88.4583, tol);
  BOOST_CHECK_CLOSE_FRACTION(p.second, 167.2117, tol); // 
  // End Test by comparion with Wimmer example 3, page 1660.
} // BOOST_AUTO_TEST_CASE(Wimmer_3)

BOOST_AUTO_TEST_CASE(Wimmer_4)
{ // Wimmer example 4  - more than one measurement.
  // std::pair<double, double> conf_interval(double mean, double unc, double df = 1., double alpha = 0.05, distribution_type distrib = gaussian)
  // Uncertainty 1.215349 given is now sigma/sqrt(N or df),
  // so need to multiply by sqrt(2).
  std::pair<double, double> p;
  p = conf_interval(13.8049, 1.215349 * sqrt(2.), 2, 0.05, gaussian); // 95% confidence, and default gaussian.
  BOOST_CHECK_CLOSE_FRACTION(p.first, 11.42281596, tol);
  BOOST_CHECK_CLOSE_FRACTION(p.second, 16.18698404, tol);
  p = conf_interval(13.8049, 1.215349 * sqrt(2.), 2);  // 95% confidence, and default gaussian.
  BOOST_CHECK_CLOSE_FRACTION(p.first, 11.42281596, tol);
  BOOST_CHECK_CLOSE_FRACTION(p.second, 16.18698404, tol);
// Similarly for 
  p = conf_interval(13.8049, 1.215349 * sqrt(2.), 2, 0.01);  // df =2, 99% confidence, and default gaussian.
  BOOST_CHECK_CLOSE_FRACTION(p.first, 10.67416098, tol);
  BOOST_CHECK_CLOSE_FRACTION(p.second, 16.93563902, tol); // 
  // End Wimmer example 4
} // BOOST_AUTO_TEST_CASE(Wimmer_4)

BOOST_AUTO_TEST_CASE(Wimmer_5_1)
{ // Wimmer example 5(i) e = 0.01
  //int round_m(double epsilon, double unc, unsigned int sigdigits, distribution_type t);

  // Mean is 127.835, and standard deviation sigma is 15.287.
  double sigma = 15.287;
  // Round sigma to 2 significant decimal digits.
  double sigma_star = round_sig(sigma, 2); // 2 for ISO rule.
  BOOST_CHECK_CLOSE_FRACTION(sigma_star, 15., tol); // Sigma rounded.

  double epsilon = 0.01; // Allow 1% loss from rounding.
  int m = round_m(epsilon, sigma, 2U, gaussian); // Calculate rounding digit, using sigma.
  BOOST_CHECK_EQUAL(m, -1); // Rounding digit is 1st after decimal point, 0.d.
  // Repeat using defaults.
  m = round_m(epsilon, sigma); // Calculate rounding digit, using sigma.
  BOOST_CHECK_EQUAL(m, -1); // Rounding digit is 1st after decimal point, 0.d.

  double g = gamma(sigma_star, sigma);
  BOOST_CHECK_CLOSE_FRACTION(g, 0.98123, tol); // Check gamma = 0.98123 given.

  double d = delta(epsilon, g, gaussian); 
  BOOST_CHECK_CLOSE_FRACTION(d, 0.0537, tol); // Wimmer only quotes 0.05.
  d = 0.05 * sigma_star / (5 * g); // Round as Wimmer to 0.05
  double md = log10(d);
  BOOST_CHECK_CLOSE_FRACTION(md, -0.815, tol);
  BOOST_CHECK_CLOSE_FRACTION(floor(md), -1, tol); // m = -1, so rounding digit is 0.d.
  // Using full accuracy 0.0537 also gives m = -1 as calculated by round_m above.
  
  double x = 127.835; // Value to be rounded using m = -1.
  BOOST_CHECK_EQUAL(round_ms(x, m),"128."); // Round mean to a string.
  BOOST_CHECK_EQUAL(round_ms(x, round_m(0.01, 15.287, 2U, gaussian)),"128.");

// void out_value_df_limits(double mean, double unc, int degfree = 1, std::ostream& os = cout)
  ostringstream oss;
  out_value_df_limits(127.835, 15.287, 1, oss);
  // out_value_df_limits(123.835, 15.287); 128. +/- 15 <97.9, 157.8>
  // cout << oss.str() << endl;
  BOOST_CHECK_EQUAL(oss.str(), "128. +/- 15 <97.9, 157.8>");
} // BOOST_AUTO_TEST_CASE(Wimmer_5_1)
  
 BOOST_AUTO_TEST_CASE(Wimmer_5_2) 
 { // Wimmer example 5(ii)

   double sigma = 0.0232;
   double sigma_star = round_sig(sigma, 1); // Why only one significant digit for rounding?
   BOOST_CHECK_CLOSE_FRACTION(sigma_star, 0.02, tol);

   double g = gamma(sigma_star, sigma);
   BOOST_CHECK_CLOSE_FRACTION(g, 0.86207, tol); // Check gamma =  0.86207 given.

   // Wimmer says epsilon = 0.04 will fails from table 1.
   // Epsilon 0.05 (95%) gamma_0.05 is 0.90175 which is > 0.86207,
   // Epsilon 0.1 (90%) gamma_0.1 is 0.81271 which is < 0.86207 so OK.
   double epsilon = 0.001; // Expect to fail if only use one digit for rounding sigma.
   int m = round_m(epsilon, sigma); // Calculate rounding digit, using sigma.
   BOOST_CHECK_EQUAL(m, -9999);
   // Message: Cannot return a rounding m because epsilon 0.001 is too small!
   epsilon = 0.1;  // Should be OK.
   m = round_m(epsilon, sigma); // Calculate rounding digit, using sigma.
   BOOST_CHECK_EQUAL(m, -3);

   double x = 1.23875;  // Final part of example.
   epsilon = 0.1;  // Should be OK.
   string s = round_ms(x, m);
   BOOST_CHECK_EQUAL(s, "1.24");
   std::pair<double, double> ci =
     conf_interval(x, sigma_star, 1., 0.05);
   //cout << "conf_interval(x, sigma, 1., 0.05);" << ci << endl;
   BOOST_CHECK_CLOSE_FRACTION(ci.first, 1.2008, tol * 2); // 1.1995
   BOOST_CHECK_CLOSE_FRACTION(ci.second, 1.2792, tol); // 1.27795
   // Not sure why this isn't exactly same as Wimmer.
 }

BOOST_AUTO_TEST_CASE(round_ue_test)
{ // Test using round_ue 
  // Round using value and uncertainty (and controlling sigdigits of uncertainty).
  // std::string round_ue(double v, double unc, double epsilon = 0.01, unsigned int sigdigits = 2U)
  BOOST_CHECK_EQUAL(round_ue(127.835, 15.287, 0.01, 2U),"128.");  // explicit 2 uncertain digits.
  BOOST_CHECK_EQUAL(round_ue(127.835, 15.287, 0.01),"128."); // Default unc sigdigit (2)s.
  BOOST_CHECK_EQUAL(round_ue(127.835, 15.287),"128."); // Default unc sigdigits and default epsilon.

  // Wimmer example 5(i b) e = 0.04
  BOOST_CHECK_EQUAL(round_ue(127.835, 15.287, 0.04),"130."); // Increase e to 0.04 to allow more rounding loss.
  // Wimmer example 5(ii a) e = 0.05 fails.
  //BOOST_CHECK_NE(round_ue(1.23875, 0.0232, 0.05, 1U),"1.24"); // Fails correctly with message:
  // Epsilon 0.05 is too small for gamma rounded/unrounded ratio 0.862069
  // Cannot return a rounding m because epsilon is too small!
  // Wimmer example 5(ii b) e = 0.1
  BOOST_CHECK_EQUAL(round_ue(1.23875, 0.0232, 0.1, 1U),"1.24"); // 

  { // Wimmer example 6 - case (ii) known confidence interval estimate for sigma.

    // P{lo_sigma < sigma < hi_sigma} = 1 - phi
  
    // So we know 0.98 confidence interval for unc (sigma) is <5.67, 5.82>.
    // phi = 1- 0.98 = 0.02
    // unc conf phi = 0.02, round conf eps = 0.05, combined eta = eps - phi = 0.05 - 0.02 = 0.03.
    double nu = 0.03;

    std::pair<double, double> u(5.67, 5.82); // Known sigma 0.98 confidence interval estimate.
    // and gamma = 5.67/5.82 = 0.97423
    //cout << "uncs ratio " << gamma(5.67, 5.82) << endl; //  uncs ratio 0.974227
    BOOST_CHECK_CLOSE_FRACTION(gamma(5.67, 5.82), 0.97423, tol); // unc ratio.
    // cout << "delta " << delta(nu, gamma(5.67, 5.82)) << endl; //  delta 0.261833
    BOOST_CHECK_CLOSE_FRACTION(delta(nu, gamma(5.67, 5.82)), 0.26, tol*10); // delta = 0.26
    // cout << log10(0.26 * 5.82 /5) << endl; // expect -0.519074
    BOOST_CHECK_CLOSE_FRACTION(log10(0.26 * 5.82 / 5), -0.519, tol); 
    // m < -0.519 so rounding x.
    int m = static_cast<int>(floor(-0.519));
    //cout << "floor(-0.519) " << m << endl; // floor(-0.519) -1
    BOOST_CHECK_EQUAL(m, -1);
    double x = 56.387; // Measured value.
    BOOST_CHECK_EQUAL(round_ms(x, m), "56."); // Rounded measured value.
    double x_star = 56.; // Rounded mean x.

    double alpha = 0.05; // 95% confidence, for 0.05_proper (1-alpha) = 0.95 confidence interval.
    using boost::math::normal;
    normal dist; // Normal distribution with zero mean and unit standard deviation.
    //double t1 = quantile(dist, alpha / 2);
    //cout << t1 << endl; // = -1.95996 so can use 
    double t = quantile(complement(dist, alpha /2 ));
    // cout << t << endl; // 1.95996 for 0.05
    BOOST_CHECK_CLOSE_FRACTION(t, 1.95996, tol);
    // Wimmer rounds to t = 1.96; but tests pass OK with either value.

    std::pair<double, double> p; // Confidence interval.
    p.first = x_star - 5.67 * t; // lower.
    p.second = x_star + 5.67 * t; // higher.
    // cout << p << endl; //  <44.887, 67.113>
    BOOST_CHECK_CLOSE_FRACTION(p.first, 44.8868, tol); // Wimmer values.
    BOOST_CHECK_CLOSE_FRACTION(p.second, 67.1132, tol);
    
    // Wimmer second example using tighter alpha.
    x_star = 56.; // Rounded mean.
    alpha = 0.01; // 99% confidence for 0.05 proper 0.99 confidence interval estimate.
    normal dist01; // Normal distribution with zero mean and unit standard deviation.
    t = quantile(complement(dist01, alpha / 2));
    // cout << t << endl; // 1.95996
    // Confidence interval.
    p.first = x_star - 5.67 * t;
    p.second = x_star + 5.67 * t;
    // cout << "Alpha 0.01 " << p << endl; //  Alpha 0.01 <41.395, 70.605>
    BOOST_CHECK_CLOSE_FRACTION(p.first, 41.391, tol); 
    BOOST_CHECK_CLOSE_FRACTION(p.second, 70.6059, tol); 
    // Note this 0.995 confidence requirement means that the interval is wider.
  }//End Wimmer example 6 - case (ii) known confidence interval estimate for sigma.
    
  { // Wimmer p 1662, example 7 - Repeated measurements with known variance.
    using namespace boost::math;
    int n = 10; // number of measurements is 10.
    double x = 87.93911210729208; //  Mean of 10 measurements.
    double variance = 0.5; // known variance sigma^2 is 0.5.
    double sigma = sqrt(variance); // and known variance sigma^2 is 0.5.
    double sigma_star = sigma / sqrt(static_cast<double>(n)); // Why divide by sqrt(10)?
    BOOST_CHECK_CLOSE_FRACTION(sigma_star, 0.223606797, tol/10.); 
    double sigma_rounded = round_sig(sigma_star, 1); // Round to 1 sig digit to get 0.2.
    // cout << "unc_rounded " << unc_rounded << endl; // unc_rounded 0.2
    BOOST_CHECK_CLOSE_FRACTION(sigma_rounded, 0.2, tol); // 
    BOOST_CHECK_EQUAL(sigma_rounded, 0.2); // unc 0.1 properly rounded.
    ostringstream oss;
    oss << setprecision(3) << sigma_rounded; // expect 0.2 whatever precision (< digits10)
    BOOST_CHECK_EQUAL(oss.str(), "0.2"); // sigma 0.1_properly rounded.

    double gam = sigma_rounded / sigma_star; // 0.2 / 0.223606797 = 0.89442
    BOOST_CHECK_CLOSE_FRACTION(gam, 0.89442, tol);  
    // Using eq 24 gives delta(0.1, 0.89442, max) = 0.43
    double del = delta(0.1, gam); // eps = 0.1 for mere 90% confidence.
    BOOST_CHECK_CLOSE_FRACTION(del, 0.43, 0.05); 
    // According to eq 12, m < -1.716, 
    // so we ought to round x mean to order m = -2, (rounding digit is 0.0d)
    // to get the 0.1 properly-rounded result of format dd.d, 
    // and the 0.1-proper (1-alpha) confidence interval estimate for mean,
    // for alpha = 0.01,
    // confidence interval is 87.9 +/- 0.2 and (87.3848, 88.4152).

    double r = log10(del * sigma_rounded / (5 * gam));
    //cout << "r = " << r << endl; //   r = -1.71409
    BOOST_CHECK_CLOSE_FRACTION(r, -1.716, tol *5 );
    int m = static_cast<int>(floor(log10(del * sigma_rounded / (5 * gam)))); // Wimmer eq 12 p 1661.
    BOOST_CHECK_EQUAL(m, -2);
    // Expect mean 87.9 +/- 0.2
    BOOST_CHECK_EQUAL(round_ms(x, m), "87.9"); // 0.1 properly rounded.
    // Have already got unc_rounded so display "+/- 0.2".
    double rounded_x = 87.9;

    // Estimate confidence interval.
    double alpha = 0.01;
    //boost::math::normal dist(rounded_x, variance/(n-1));
    //double t = quantile(complement(dist, alpha/2));
    //cout << "quantile(complement(dist, alpha/2)); = " << t << endl; // for 0.1 = 1.64485, 0.01 2.57583
    // This should be the upper limit.
    // quantile(complement(dist, alpha/2)); = 88.1213  
    // dist(x, variance/n); gives quantile(complement(dist, alpha/2)); = 88.0679
    // dist(rounded_x, variance/n); gives quantile(complement(dist, alpha/2)); = 88.0288
    // dist(rounded_x, variance/(n-1)); gives quantile(complement(dist, alpha/2)); = 88.0431
    // So none of these are quite right - expect upper = 88.4152

    normal dist01;
    double t = quantile(complement(dist01, alpha/2));
    std::pair<double, double> p; // Confidence interval.
    // Note use of rounded values to get exactly the same result as Wimmer.
    p.first = rounded_x - sigma_rounded * t;
    p.second = rounded_x + sigma_rounded * t;
    // cout << "lower " << p.first << ", upper " << p.second << endl; //   lower 87.3848, upper 88.4152
    BOOST_CHECK_CLOSE_FRACTION(p.first, 87.3848, tol);  // These confidence intervals are given.
    BOOST_CHECK_CLOSE_FRACTION(p.second, 88.4152, tol); 

    // So in summary:
    // void out_value_limits(double mean, double unc, std::pair<double, double> ci, int m, std::ostream& os = cout);
    // std::cout << "out_value_limits(x, sigma_star, p, m, std::cout); ";
    // out_value_limits(x, sigma_star, p, m, std::cout); 
    // std::cout << std::endl;
    //  out_value_limits(x, sigma_star, p, m, std::cout); 87.9 +/- 0.22 <87.38, 88.42> 

    alpha = 0.1;  // 90% confidence for mean.
    t = quantile(complement(dist01, alpha / 2));
    //cout << t << endl; // 1.64485
    p.first = rounded_x - sigma_star * t;
    p.second = rounded_x + sigma_star * t;
    // cout << "Alpha 0.1 " << p << endl; // Alpha 0.1 <87.5322, 88.2678>
    BOOST_CHECK_CLOSE_FRACTION(p.first, 87.573, tol);  // These confidence intervals not given,
    BOOST_CHECK_CLOSE_FRACTION(p.second, 88.27, tol);  // but they look plausible.

    alpha = 0.05;  // 95% confidence for mean (not given by Wimmer).
    t = quantile(complement(dist01, alpha / 2));
    // cout << t << endl; // 1.95996
    p.first = rounded_x - sigma_star * t;
    p.second = rounded_x + sigma_star * t;
    //cout << "Alpha 0.05 " << p << endl; // 87.5009 == 87.5 // 88.3774 == 88.3 rounded.
    // Alpha 0.05 <87.4617, 88.3383>
    BOOST_CHECK_CLOSE_FRACTION(p.first, 87.3848, tol); 
    BOOST_CHECK_CLOSE_FRACTION(p.second, 88.4152, tol); 
    BOOST_CHECK_EQUAL(round_ms(p.first, m), "87.5"); // 0.1 properly rounded.
    BOOST_CHECK_EQUAL(round_ms(p.second, m), "88.3"); // 0.1 properly rounded.

    alpha = 0.01;  // 99% confidence for mean (Wimmer).
    t = quantile(complement(dist01, alpha / 2));
    // cout << t << endl; // 2.57583
    p.first = rounded_x - sigma_star * t;
    p.second = rounded_x + sigma_star * t;
    // cout << "Alpha 0.01 " << p << endl;// 87.3631 == 87.3,  88.5151 == 88.5
    // Alpha 0.01 <87.324, 88.476>
    BOOST_CHECK_CLOSE_FRACTION(p.first, 87.3848, tol); // but Wimmer gives 87.3848 not 88.5151.
    BOOST_CHECK_CLOSE_FRACTION(p.second, 88.4152, tol); // but Wimmer gives 88.4152 not 88.5151.
    BOOST_CHECK_EQUAL(round_ms(p.first, m), "87.3"); // 0.1_properly rounded.
    BOOST_CHECK_EQUAL(round_ms(p.second, m), "88.5"); // 0.1_properly rounded.
    //cout << "Mean " << x << " == " << round_ms(x, m) << " +/- " << sigma_rounded
    //  << " <" << round_ms(p.first, m) << ", " << round_ms(p.second, m) << ">" << endl;
    //Mean 87.9391 == 87.9 +/- 0.2 <87.3, 88.5>

  }// End Wimmer p 1662, example 7 - Repeated measurements with known variance.
} // BOOST_AUTO_TEST_CASE(round_m_test)

BOOST_AUTO_TEST_CASE(Wimmer_triangular_test)
{  // Wimmer Triangular distribution example 3, page 27.
  double x = 127.835;
  double unc = 15.287;
  double unc_rounded = round_sig(unc, 2); // round to 2 sig digit - ISO rule.
  // cout << "unc_rounded " << unc_rounded << endl; // unc_rounded 15
  BOOST_CHECK_CLOSE_FRACTION(unc_rounded, 15., 0.01);
  ostringstream oss;
  oss << setprecision(3) << unc_rounded; // expect 0.2 whatever precision (< digits10)
  BOOST_CHECK_EQUAL(oss.str(), "15"); // unc 15. properly rounded.
  double gam = unc_rounded / unc;
  BOOST_CHECK_CLOSE_FRACTION(gam, 0.98123, 0.01); 
  // double delta(double epsilon, double gamma, distribution_type distrib = normal);
  double del = delta(0.01, gam, triangular); // eps = 0.01 for high 99% confidence.
  BOOST_CHECK_CLOSE_FRACTION(del, 0.0561, 0.01);

  int m = round_m(0.01, unc, 2, triangular);
  BOOST_CHECK_EQUAL(m, -1);
  BOOST_CHECK_EQUAL(round_ms(x, m), "128.");
  // ostringstream oss2; // Maybe quicker to construct a new string than erase old one?
  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  oss << setprecision(3) << unc_rounded; // expect 15 whatever precision (< digits10)
  BOOST_CHECK_EQUAL(oss.str(), "15"); // unc 15. properly rounded.
  std::pair<double, double> ci;
  //std::pair<double, double> conf_interval(double value, double unc, double df = 1., double alpha = 0.05, distribution_type distrib);
  // value should be the rounded value 130, not the 'exact' double value 127.835.
  // unc should also be the rounded value, 15, not the 'exact' double value 15.287.
  // I don't understand the logic of calculating the confidence intervals with rounded data?
  using boost::lexical_cast;
  double vr = lexical_cast<double>(round_ms(x, m)); // = 128.
  ci = conf_interval(vr, unc_rounded, 1, 0.05, triangular); // alpha = 0.05, example 3 1st 0.01-proper 0.95 confidence interval <99.5, 156.5>. 
  // cout << "conf_interval(vr, unc, 1, 0.05, triangular); " << ci << endl;
  //  conf_interval(vr, unc, 1, 0.05, triangular); <99.4735, 156.527>
  BOOST_CHECK_CLOSE_FRACTION(ci.first, 99.5, tol); // 98.9 
  BOOST_CHECK_CLOSE_FRACTION(ci.second, 156.5, tol); // 157.1

  // Round confidence intervals to 1 decimal digit 
  //double cilo =  lexical_cast<double>(round_ms(ci.first, m-1)); // = 98.9
  //cout << "ci lo " << cilo << endl; //   ci lo 99.5
  //double cihi =  lexical_cast<double>(round_ms(ci.second, m-1)); // = 156.5
  //cout << "ci hi " << cihi << endl; //  ci hi 156.5

  //out_confidence_interval(ci, m, cout); // <99.5, 157>
  //cout << endl;

  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  out_confidence_interval(ci, m, oss); // <99.5, 157>
  BOOST_CHECK_EQUAL(oss.str(), "<99.5, 157>");

  //out_value_limits(x, unc, ci, m); //  128. +/- 15 <99.5, 156.5>
  //cout << endl;
  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  out_value_limits(x, unc, ci, m, oss); //  128. +/- 15 <99.5, 156.5>
  BOOST_CHECK_EQUAL(oss.str(), "128. +/- 15 <99.5, 156.5>");

  // Now for 2nd example, consider eps = 0.05 - only 95% rather than 99% confidence of rounding loss.
  del = delta(0.05, gam, triangular); // eps = 0.05 for lower 95% confidence.
  BOOST_CHECK_CLOSE_FRACTION(del, 0.4931, tol);
  m = round_m(0.05, unc, 2, triangular);
  BOOST_CHECK_EQUAL(m, 0); // round to order m == 0
  BOOST_CHECK_EQUAL(round_ms(x, m), "130.");
  // Show confidence intervals for various alpha, 0.01 to 0.1:
  ci = conf_interval(x, unc, 1, 0.01, triangular);
  //cout << "conf_interval(x, unc, 1, 0.01, triangular); " << ci << endl;
  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  oss << setprecision(6) << ci;
  BOOST_CHECK_EQUAL(oss.str(), "<94.1342, 161.536>");

  ci = conf_interval(x, unc, 1, 0.02, triangular);
  //cout << "conf_interval(x, unc, 1, 0.02, triangular); " << ci << endl;
  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  oss << setprecision(6) << ci;
  BOOST_CHECK_EQUAL(oss.str(), "<95.6852, 159.985>");

  ci = conf_interval(x, unc, 1, 0.05, triangular);
  //cout << "conf_interval(x, unc, 1, 0.05, triangular); " << ci << endl;
  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  oss << setprecision(6) << ci;
  BOOST_CHECK_EQUAL(oss.str(), "<98.7627, 156.907>");

  ci = conf_interval(x, unc, 1, 0.1, triangular);
  //cout << "conf_interval(x, unc, 1, 0.1, triangular); " << ci << endl;
  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  oss << setprecision(6) << ci;
  BOOST_CHECK_EQUAL(oss.str(), "<102.231, 153.439>");

  //conf_interval(x, unc, 1, 0.01, triangular); <94.1342, 161.536>
  //conf_interval(x, unc, 1, 0.02, triangular); <95.6852, 159.985>
  //conf_interval(x, unc, 1, 0.05, triangular); <98.7627, 156.907>
  //conf_interval(x, unc, 1, 0.1, triangular); <102.231, 153.439>
  // Showing significantly wider interval as require less confidence (1-alpha) from 99% down to 90%.

  // Check with Wimmer 0.05 proper 0.95 confidence interval is <101.5, 158.5>
  vr = lexical_cast<double>(round_ms(x, m));
  ci = conf_interval(vr, unc_rounded, 1, 0.05, triangular);
  BOOST_CHECK_CLOSE_FRACTION(ci.first, 101.5, tol);
  BOOST_CHECK_CLOSE_FRACTION(ci.second, 158.5, tol);
} // BOOST_AUTO_TEST_CASE(Wimmer_triangular_test)

BOOST_AUTO_TEST_CASE(Wimmer_uniform_test)
{ // Wimmer Uniform distribution - no example but use same data as triangular.
  //cout << "Uniform distribution " << endl;
  double x = 127.835;
  double unc = 15.287;
  double unc_rounded = round_sig(unc, 2); // round to 2 sig digit - why?
  // cout << "unc_rounded " << unc_rounded << endl; // 15
  BOOST_CHECK_CLOSE_FRACTION(unc_rounded, 15., tol);
  ostringstream oss;
  oss << setprecision(3) << unc_rounded; // expect 0.2 whatever precision (< digits10)
  BOOST_CHECK_EQUAL(oss.str(), "15"); // unc 15. properly rounded.
  double gam = unc_rounded / unc;
  BOOST_CHECK_CLOSE_FRACTION(gam, 0.98123, 0.01); 
  // double delta(double epsilon, double gamma, distribution_type distrib = normal);
  // Compare all deltas for the three distributions.
  BOOST_CHECK_CLOSE_FRACTION(delta(0.01, gam),  0.053705908361491229, 0.01);
  BOOST_CHECK_CLOSE_FRACTION(delta(0.01, gam, triangular), 0.056070697653393911, 0.01);
  BOOST_CHECK_CLOSE_FRACTION(delta(0.01, gam, uniform), -1, 0.01);
  // cout << "delta(0.01, gam, gaussian) = " << delta(0.01, gam) << ' ' << delta(0.01, gam, triangular) << ' ' << delta(0.01, gam, uniform) << endl;
  // "Epsilon 0.01 is too small for gamma rounded/unrounded ratio 0.98122587819716089, threshold is 0.98999999999999999for uniform distribution."
  double del = delta(0.01, gam, uniform); // eps = 0.01 for high 99% confidence.
  BOOST_CHECK_CLOSE_FRACTION(del, -1., 0.01); // but is too high, so return -1 
  // "Epsilon 0.01 is too small for gamma rounded/unrounded ratio 0.981226, threshold is 0.99."
  del = delta(0.04, gam, uniform); // eps = 0.01 for not so high 96% confidence.
  BOOST_CHECK_CLOSE_FRACTION(del, 0.106046331775506, tol);
  int m = round_m(0.04, unc, 2, uniform);
  BOOST_CHECK_EQUAL(m, -1);
  BOOST_CHECK_EQUAL(round_ms(x, m), "128.");
  // ostringstream oss2; // Maybe quicker to construct a new string than erase old one?
  oss.clear(); // Clears only clear stream fail or error bits!
  oss.str(""); // Erases the previous str (oss.str().erase() does NOT change std::string - because it only acts on a copy C string?)
  oss << setprecision(3) << unc_rounded; // expect 15 whatever precision (< digits10)
  BOOST_CHECK_EQUAL(oss.str(), "15"); // unc 15. properly rounded.
  //std::pair<double, double> conf_interval(double value, double unc, double df = 1., double alpha = 0.05, distribution_type distrib);
  // value should be the rounded value 130, not the 'exact' double value 127.835.
  // unc should also be the rounded value, 15, not the 'exact' double value 15.287.
  // I don't understand the logic of calculating the confidence intervals with rounded data?
  using boost::lexical_cast;
  double vr = lexical_cast<double>(round_ms(x, m)); // = 128.
  std::pair<double, double> ci = conf_interval(vr, unc_rounded, 1, 0.05, uniform); // alpha = 0.05, example 3b 1st 0.04-proper 0.95 confidence interval <99.5, 156.5>. 
  //cout << "conf_interval(vr, unc, 1, 0.05, uniform); " << ci << endl;
  //  conf_interval(vr, unc, 1, 0.05, uniform); <103.318, 152.682>
  {
    std::ostringstream os;
    os << ci;
    BOOST_CHECK_EQUAL(os.str(), "<103.318, 152.682>");
  }
  BOOST_CHECK_CLOSE_FRACTION(ci.first, 103.32, tol); //
  BOOST_CHECK_CLOSE_FRACTION(ci.second, 152.68, tol); // 
  //cout << "out_value_limits(x, unc, ci, m) = " ;
  //out_value_limits(x, unc, ci, m);
  //cout << endl; // 
  // out_value_limits(x, unc, ci, m) = 128. +/- 15 <103.3, 152.7>
  {
    std::ostringstream os;
    out_value_limits(x, unc, ci, m, os); // Output value, uncertainty and confidence limits.
    BOOST_CHECK_EQUAL(os.str(), "128. +/- 15 <103.3, 152.7>");
    // Need also degrees of freedom.

  }
} // BOOST_AUTO_TEST_CASE(Wimmer_uniform_test)

BOOST_AUTO_TEST_CASE(Sephton_C_rounding_test)
{  // Sephton's example of counter-intuitive C/C++ rounding.
    double d = 0.15; 
    //cout << setprecision(17) << "0.15 precision 17 is " << d << endl; // Double is 0.14999999999999999
    //cout << setprecision(1) << "0.15 precision 1 is " << d << endl; // 0.15 precision 1 is 0.1
    // so internally stored value is nearer to 1., so rounds down to 0.1, perhaps to viewers' surprise!
    BOOST_CHECK_EQUAL(round_e(d, 0),""); // No significant digits! might decide to throw an exception?
    BOOST_CHECK_EQUAL(round_e(d, 1),"2.e-1"); // 1 significant digit.
    BOOST_CHECK_EQUAL(round_e(d, 2),"1.5e-1"); 
    BOOST_CHECK_EQUAL(round_e(d, 3),"1.50e-1");
    BOOST_CHECK_EQUAL(round_e(d, 4),"1.500e-1");

    d = 1.25; //
    //cout << setprecision(17) << "1.25 precision 17 is " << d << endl; // Double is 
    //cout << setprecision(2) << "1.25 precision 2 is " << d << endl; // 1.25 precision 2 is 1.3
    //cout << "printf(""%.1f"", 1.25); = "; printf("%.1f\n", 1.25); cout << endl; // printf(%.1f, 1.25); = 1.3
    d = 0.15; // Sephton's pathological example showing that
    // output digit string is 'correct' (0.15) for precision(numeric_limits<double>::digits10)
    // and this rounds to 0.2, as expected.
    // Imprecision 0.14999999999999999 only appears at numeric_limits<double>::maxdigits10.
    // So by starting with the digit10 precision string, we should never get the unexpected
    // and unwanted rounding down to 0.1.
    for (int i = 1; i <= maxdigits10; i++)
    {
      //cout << i << ' ' << showpoint << setprecision(i) << d << ' ' << round_e(d, i) << endl;
      //1 0.1 2.e-001
      //2 0.15 1.5e-001
      //3 0.150 1.50e-001
      //4 0.1500 1.500e-001
      //5 0.15000 1.5000e-001
      //6 0.150000 1.50000e-001
      //7 0.1500000 1.500000e-001
      //8 0.15000000 1.5000000e-001
      //9 0.150000000 1.50000000e-001
      //10 0.1500000000 1.500000000e-001
      //11 0.15000000000 1.5000000000e-001
      //12 0.150000000000 1.50000000000e-001
      //13 0.1500000000000 1.500000000000e-001
      //14 0.15000000000000 1.5000000000000e-001
      //15 0.150000000000000 1.50000000000000e-001
      //16 0.1500000000000000 1.50000000000000e-001
      //17 0.14999999999999999 1.50000000000000e-001
    }
} // BOOST_AUTO_TEST_CASE(Sephton_C_rounding_test) Sephton's example of counter-intuitive C/C++ rounding.

 BOOST_AUTO_TEST_CASE(round_n_test) // Round number v *at* d th decimal place.
 {
  // Test round_1.
  BOOST_CHECK_CLOSE_FRACTION(round_1(0.52), 0.5, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_1(0.55), 0.6, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_1(0.59), 0.6, 2 * eps);

    // Test round_2.
  BOOST_CHECK_CLOSE_FRACTION(round_2(0.152), 0.15, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_2(0.155), 0.16, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_2(0.159), 0.16, 2 * eps);

  // Test round_3.
  BOOST_CHECK_CLOSE_FRACTION(round_3(0.1552), 0.155, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_3(0.1555), 0.156, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_3(0.1559), 0.156, 2 * eps);

  // Test round_n.
  BOOST_CHECK_CLOSE_FRACTION(round_to_n(0.159, 1), 0.2, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_to_n(0.159, 2), 0.16, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_to_n(0.159, 3), 0.159, 2 * eps);

  // Test round_sig. v rounded to n significant decimal digits.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(1703.12345679, 6), 1703.12, 2 * eps); //  David A. Pimentel 6 significant digits example 1.
  BOOST_CHECK_CLOSE_FRACTION(round_nth(1703.12345679, 6), 1703.123457, 2 * eps); //  David A. Pimentel 6 digits after decimal point example 2.

  // Value 15.284.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.284, 0), 0., 2 * eps); //  By definition, zero significant digits returns zero.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.284, 1), 20., 2 * eps); // Expect rounded up to 20.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.284, 2), 15., 2 * eps); // Expect rounded to 15.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.284, 3), 15.3, 2 * eps);  // Round up .28 to .3.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.284, 4), 15.28, 2 * eps); // Round .284 to .28.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.284, 5), 15.284, 2 * eps); // Unchanged.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.284, 5), 15.2840, 2 * eps); // Unchanged but with one trailing zero.

  // Quite big values, but not more than digits10. 
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 0), 0., 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 1), 100000., 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 2), 120000., 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 3), 123000, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 4), 123500., 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 5), 123460.0, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 6), 123457.0, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 7), 123456.8, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 8), 123456.79, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(123456.789, 9), 123456.789, 2 * eps);

  BOOST_CHECK_CLOSE_FRACTION(round_sig(15.287, 2), 15., 2 * eps);
  // Wimmer rounding examples, p 1659, 1 significant digit rounding.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(127.37, 1), 100., 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(0.0983, 1), 0.1, 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(52.83, 1), 50., 2 * eps);
  // Wimmer rounding examples, p 1660, 3 significant digit rounding.
  BOOST_CHECK_CLOSE_FRACTION(round_sig(1582.738, 3), 1580., 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(985.438, 3), 985., 2 * eps);
  BOOST_CHECK_CLOSE_FRACTION(round_sig(0.00987624, 3), 0.00988, 2 * eps);

  //cout << round(15.287) << ' ' << round(15.287)/15.287 << endl; // 0.981226
  //cout << round(15.587) << endl;
  //cout << round(15.1f) << endl;
 } //  BOOST_AUTO_TEST_CASE(round_n_test)


 BOOST_AUTO_TEST_CASE(Wimmer_delta)
 {
  //double d0 = delta(0.01, 15./15.287);
  //double d1 = delta(0.01, 0.98123);
  //double d2 = delta(0.04, 0.98123);
  //double d3 = delta(0.005, 0.99736);
  //double d4 = delta(0.1, 0.89442);
  //cout << d1 << endl; // 0.053779
  //cout << d2 << endl; // 0.352144
  //cout << d3 << endl; // 0.12
  //cout << d4 << endl; // 0.431895

  //double d5 = delta(0.05, 0.86207);
  // Epsilon 0.05 is too small for gamma rounded/unrounded ratio 0.86207

  //cout << d5 << endl; // -1.#IND

  // Values in Table 1 Threshold value of gamma rounded for epsilon.
  // If gamma rounded ratio > threshold, cannot get epsilon-properly rounded result.
  //double d005 = delta(0.005, 0.98972); // fails.
  //double d005p = delta(0.005, 0.99871);  // is OK
  //double d01 = delta(0.01, 0.97954);
  //double d02 = delta(0.02, 0.95951);
  //double d03 = delta(0.03, 0.93987);
  //double d04 = delta(0.04, 0.92063);
  //double d05 = delta(0.05, 0.90175);
  //double d1 = delta(0.1, 0.81271);
  //Epsilon 0.05 is too small for gamma rounded/unrounded ratio 0.86207
  //Epsilon 0.005 is too small for gamma rounded/unrounded ratio 0.98972
  //Epsilon 0.01 is too small for gamma rounded/unrounded ratio 0.97954
  //Epsilon 0.02 is too small for gamma rounded/unrounded ratio 0.95951
  //Epsilon 0.03 is too small for gamma rounded/unrounded ratio 0.93987
  //Epsilon 0.04 is too small for gamma rounded/unrounded ratio 0.92063
  //Epsilon 0.05 is too small for gamma rounded/unrounded ratio 0.90175
  //Epsilon 0.1 is too small for gamma rounded/unrounded ratio 0.81271


  //cout << d005p << endl;
  //cout << d005 << endl;
  //cout << d01 << endl;
  //cout << d02 << endl;
  //cout << d03 << endl;
  //cout << d04 << endl;
  //cout << d05 << endl;
  //cout << d1 << endl;

  // Wimmer et al claim "equation 24 approximation error is less than 0.0123 for epsilon 0.005 to 0.1".
  // Check that this is true for these examples from the text.
  // Perhaps should be abs (difference) < 0.0123 ?
   BOOST_CHECK((delta(0.01, 0.98123) - 0.05) < 0.0123);  
   BOOST_CHECK((delta(0.04, 0.98123) - 0.35) < 0.0123);  
   BOOST_CHECK((delta(0.005, 0.99736) - 0.12) < 0.0123);  
   BOOST_CHECK((delta(0.1, 0.89442) - 0.43) < 0.0123);  
}    // BOOST_AUTO_TEST_CASE(Wimmer_delta)

BOOST_AUTO_TEST_CASE(Wimmer_gamma_test)   // Test Wimmer gamma = rounded /unrounded
{
  BOOST_CHECK_CLOSE_FRACTION(gamma(1.8, 1.8457), 0.97524, 0.01); 
} // BOOST_AUTO_TEST_CASE(Wimmer_gamma_test) 

BOOST_AUTO_TEST_CASE(round_m_test2)
{
    ostringstream oss;
    double u = 15.287;
    //cout << round_1(u) << ' ' << round_2(u) << ' ' << round_3(u) << ' ' << round_nth(u, 4) << endl;
    // 15.3 15.29 15.287 15.287
    oss << round_1(u);
    BOOST_CHECK_EQUAL(oss.str(), "15.3");
    oss.str(""); oss.clear();

    double t = 0.15; // printf unwanted round down case.
   // cout << round_1(t) << ' ' << round_2(t) << ' ' << round_3(t) << ' ' << round_nth(t, 4) << endl;
    // 0.2 0.15 0.15 0.15
    // Note the desired 0.2! Using this rounding algorithm, unlike printf 0.1.
    //   0.2 0.15 0.15 0.15
    oss << round_1(t);
    BOOST_CHECK_EQUAL(oss.str(), "0.2");
    oss.str(""); oss.clear();
    oss << round_2(t);
    BOOST_CHECK_EQUAL(oss.str(), "0.15");
    oss.str(""); oss.clear();
    oss << round_nth(t, 1);
    BOOST_CHECK_EQUAL(oss.str(), "0.2");

    BOOST_CHECK_EQUAL(round_m(0.01, 15.287, 2U), -1); // Example 5 (i a), page 1662.
    BOOST_CHECK_EQUAL(round_m(0.04, 15.287, 2U), 0); // Example 5 (i b)
    BOOST_CHECK_NE(round_m(0.05, 0.0232, 1U), -3); // Example 5 (ii a) epsilon <= 0.05 should fail, and does.   
   // Cannot return a rounding m because epsilon 0.05 is too small!
    BOOST_CHECK_EQUAL(round_m(0.1, 0.0232, 1U), -3); // Example 5 (ii b), should pass.
  
  // 0.053779018871944452 and 0.05
  BOOST_CHECK_EQUAL(round_e(0.0000000012345678901234567, 1),"1.e-9"); // round 1 sig digit.
  //BOOST_CHECK_EQUAL(round(0.12345678901234567, 0),"1."); // round 0 - at decimal
  //BOOST_CHECK_EQUAL(round(1.2345678901234567, 0),"1."); // round 0 - at decimal
  //BOOST_CHECK_EQUAL(round(123456789.01234567, 0),"1."); // round 0 - at decimal
  //BOOST_CHECK_EQUAL(round(12345678901234567., 0),"1."); // round 0 - at decimal
  //BOOST_CHECK_EQUAL(round(12345678901234567890123456789., 0),"1."); // round 0 - at decimal

  //double d = 1.2945678901234567890;
  //cout << scientific << setprecision(0) << d << endl; //default 6
  //cout << scientific << setprecision(1) << d << endl; // 1.2
  //cout << scientific << setprecision(2) << d << endl; // 1.23
  //cout << scientific << setprecision(3) << d << endl; // 1.235
  //cout << scientific << setprecision(15) << d << endl; // 1.294567890123457e+000 
  //cout << scientific << setprecision(16) << d << endl; // 1.2945678901234567e+000
  //cout << scientific << setprecision(17) << d << endl; // 1.29456789012345670e+000
  //cout << scientific << setprecision(18) << d << endl; // 1.294567890123456700e+000
} // BOOST_AUTO_TEST_CASE(round_m_test) 

//  cout << endl;

/*

------ Build started: Project: test_rounding, Configuration: Debug Win32 ------
Build started 04-May-2012 18:12:45.
InitializeBuildStatus:
  Creating "Debug\test_rounding.unsuccessfulbuild" because "AlwaysCreate" was specified.
ClCompile:
  test_rounding.cpp
ManifestResourceCompile:
  All outputs are up-to-date.
Manifest:
  All outputs are up-to-date.
LinkEmbedManifest:
  All outputs are up-to-date.
  test_rounding.vcxproj -> J:\Cpp\quan\MSVC\Debug\test_rounding.exe
CustomBuildStep:
  Description: Autorun "J:\Cpp\quan\MSVC\Debug\\test_rounding.exe"
  Running 17 test cases...
  Platform: Win32
  Compiler: Microsoft Visual C++ version 10.0
  STL     : Dinkumware standard library version 520
  Boost   : 1.50.0
  Round test: ..\libs\quan\test\test_rounding.cpp at "Fri May  4 18:12:45 2012", MSVC version 160040219.
  double maxdigits10 is 17
  std::numeric_limits<double>::max_exponent = 1024, 
  std::numeric_limits<double>::max_exponent10 = 308,
  std::numeric_limits<double>::max_exponent10 -1 = 307. 
  Trying to output zero significant digits!
  Trying to output -1 significant digits!
  Maximum significant digits is 15
  Maximum significant digits is 15
  Maximum significant digits is 15
  Maximum significant digits is 15
  Maximum significant digits is 15
  Maximum significant digits is 15
  Maximum significant digits is 15
  Epsilon 0.001 is too small for gamma rounded/unrounded ratio 0.991379, threshold is -0.00666462 for gaussian distribution.
  Cannot compute a rounding m because epsilon 0.001 is too small!
  Epsilon 0.01 is too small for gamma rounded/unrounded ratio 0.981226, threshold is 0.99 for uniform distribution.
  Epsilon 0.01 is too small for gamma rounded/unrounded ratio 0.981226, threshold is 0.99 for uniform distribution.
  Trying to display 0 significant decimal digits!
  Cannot return a rounding m because epsilon 0.05 is too small!
  
  Test suite "Master Test Suite" passed with:
    544 assertions out of 544 passed
    17 test cases out of 17 passed
  
    Test case "round_test_1" passed with:
      1 assertion out of 1 passed
  
    Test case "round_f_test" passed with:
      48 assertions out of 48 passed
  
    Test case "round_e_test" passed with:
      126 assertions out of 126 passed
  
    Test case "cdf_quantile_test" passed with:
      134 assertions out of 134 passed
  
    Test case "round_ms_test" passed with:
      80 assertions out of 80 passed
  
    Test case "Wimmer_3" passed with:
      4 assertions out of 4 passed
  
    Test case "Wimmer_4" passed with:
      6 assertions out of 6 passed
  
    Test case "Wimmer_5_1" passed with:
      10 assertions out of 10 passed
  
    Test case "Wimmer_5_2" passed with:
      7 assertions out of 7 passed
  
    Test case "round_ue_test" passed with:
      36 assertions out of 36 passed
  
    Test case "Wimmer_triangular_test" passed with:
      20 assertions out of 20 passed
  
    Test case "Wimmer_uniform_test" passed with:
      15 assertions out of 15 passed
  
    Test case "Sephton_C_rounding_test" passed with:
      5 assertions out of 5 passed
  
    Test case "round_n_test" passed with:
      38 assertions out of 38 passed
  
    Test case "Wimmer_delta" passed with:
      4 assertions out of 4 passed
  
    Test case "Wimmer_gamma_test" passed with:
      1 assertion out of 1 passed
  
    Test case "round_m_test2" passed with:
      9 assertions out of 9 passed
  
FinalizeBuildStatus:
  Deleting file "Debug\test_rounding.unsuccessfulbuild".
  Touching "Debug\test_rounding.lastbuildstate".

Build succeeded.

Time Elapsed 00:00:04.20
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========


*/

