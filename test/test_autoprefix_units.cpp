/*!
  \file 
  \brief Testing autoprefix of units with
  uncertain classes using Boost Test Tool.
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

#define BOOST_TEST_MAIN // Required for int test_main() (must come FIRST).
//#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-s-1_49.lib

#include <boost/config.hpp>
#include <boost/cstdlib.hpp> // needed for boost::exit_failure;

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink,
#include <boost/test/floating_point_comparison.hpp> // Extra test tool for FP comparison.
  using boost::unit_test::test_suite;
  using boost::unit_test::unit_test_log;

#include <boost/units/cmath.hpp>
#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/codata/physico-chemical_constants.hpp>
#include <boost/units/systems/si/io.hpp>

// Classes for simple propagation of Uncertainties according to a pure Gaussian model.

#include <cmath>   // for <math.h>
#include <iostream>
  using std::ostream;
  using std::cout;
  using std::ios_base;
  using std::ends;
  using std::endl;
  using std::ios_base;
  using std::hex;
  using std::oct;
  using std::dec;
  using std::boolalpha;
  using std::getline;
  using std::skipws;
  using std::noskipws;
  using std::uppercase;
  using std::showbase;
  using std::showpos;
  using std::left;
  using std::right;
  using std::internal; // Initial default - neither left, right nor internal.
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

#include <boost/quan/xiostream.hpp> // extra iostream manipulators like noshowbase
  using std::lowercase;
  using std::defaultfloat; // Initial default - neither scientific nor fixed.
  using std::nofixed;
  using std::noscientific;
  using std::noadjust;
  using std::hexbase;

#include <boost/quan/unc.hpp> // Declaration of Uncertain Classes.

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


BOOST_AUTO_TEST_CASE(unc_test_basic)
{ // Uncertain Class tests.
  boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages);

  string message("Unc units autoprefix test: " __FILE__ );
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

    // Some floating point comparison examples, with two tolerances, just wide enough, and too tight.
  double fp1 = 1.;
  double fp2 = 1.00001;
  BOOST_CHECK_CLOSE(fp1, fp2, 0.00100001); // Wide tolerance info: test fp1 ~= fp2 passed.
  // difference between fp1{1} and fp2{1.00001} exceeds 0.001% (0.00001 * 100 = 0.001%)
  // Note that BOOST_CHECK_CLOSE(fp1, fp2, 0.001); // just fails, but 0.001000001 passes.

  // BOOST_CHECK_CLOSE(fp1, fp2, 0.0000000001); // Tight tolerance so should fail [1 !~= 1.00001 (+/-1e-010)].
  // error in "call_test_main": difference between fp1{1} and fp2{1.00001} exceeds 1e-010%

  } // BOOST_AUTO_TEST_CASE(unc_test_basic)

BOOST_AUTO_TEST_CASE(unc_test_units)
 {
    int t = 12;
    uncun twelveI(t); // Exact 12 from integer variable.
    BOOST_CHECK(twelveI.types() & VALUE_INTEGER); // Check IS integer from variable.
    CHECK_USED(twelveI, "12"); // no . because integer.

    using namespace boost::units;
    using namespace boost::units::si;

    // Using just length.
    quantity<length> l1(9876.5432*meters);
    //cout << l1 << endl; //   9876.54 m
    CHECK_USED(l1, "9876.54 m"); // 
    //cout << engineering_prefix << l1 << endl; // 9.87654 km
    CHECK_USED(engineering_prefix << l1, "9.87654 km"); // 
    //cout << plusminus << engineering_prefix << l1 << endl; // 9.87654 km
    CHECK_USED(plusminus << engineering_prefix << l1, "9.87654 km"); // No uncertainty expected as none exists.

    // Uncertain length with standard deviation and degrees of freedom.
    quantity<length, uncun > tenthslen(uncun(0.9876, 0.0321F, 3)*meters);
    CHECK_USED(tenthslen, "0.99 m"); // No plusminus.
    quantity<length, uncun > unitslen(uncun(9.8760, 0.321F, 3)*meters);
    CHECK_USED(unitslen, "9.9 m"); // No plusminus.
    quantity<length, uncun > tenslen(uncun(98.760, 3.21F, 3)*meters);
    CHECK_USED(tenslen, "99. m"); // No plusminus.
    quantity<length, uncun > hundredslen(uncun(987.60, 32.1F, 3)*meters);
    CHECK_USED(hundredslen, "990. m"); // No plusminus.

    quantity<length, uncun > megalen(uncun(9876543.21, 321000.1F, 3)*meters);
    CHECK_USED(engineering_prefix << megalen, "9.9 Mm"); // No plusminus.

    quantity<length, uncun > teralen(uncun(9.87654321e12, 3.21e11F, 3)*meters);
    CHECK_USED(engineering_prefix << plusminus << teralen, "9.9 +/-0.32 Tm"); // No plusminus.

     quantity<length, uncun > biglen(uncun(9876.0, 321.0F)*meters);
    // cout << biglen << std::endl; // 9900. m
    CHECK_USED(biglen , "9900. m"); // No plusminus.
    CHECK_USED(engineering_prefix << biglen , "9.9 km"); // No plusminus.
    // Test plusminus alone.
    // std::cout << no_prefix  << plusminus; // 9900. +/-300. m
    CHECK_USED(plusminus << biglen , "9900. +/-320. m"); // expected.
    // engineering prefix and plusminus.
    // std::cout << engineering_prefix << plusminus << biglen << std::endl; //  9.9 +/-0.3 km
    CHECK_USED(plusminus << engineering_prefix << biglen , "9.9 +/-0.32 km");
    CHECK_USED(addnoisyDigit << plusminus << engineering_prefix << biglen , "9.88 +/-0.32 km");

    CHECK_USED(adddegfree << plusminus << engineering_prefix << biglen , "9.9 +/-0.32 (1) km");
    CHECK_USED(addlimits << plusminus << engineering_prefix << biglen , "9.9 +/-0.32 <9.250, 10.51> km");
    // Set confidence to 0.01 or 99% (from default of 0.05 or 95%) to narrow confidence interval.
    CHECK_USED(setConfidence(0.01) << addlimits << plusminus << engineering_prefix << biglen , "9.9 +/-0.32 <9.050, 10.70> km");
    // 
    CHECK_USED(adddegfree << addlimits << plusminus << engineering_prefix << biglen , "9.9 +/-0.32 <9.250, 10.51> (1) km");

    quantity<length, uncun> tinylen(uncun(0.009876, 0.0000321F)*meters);

    CHECK_USED(engineering_prefix << tinylen , "9.88 mm"); // No plusminus.
    // Test plusminus alone.
    // std::cout << no_prefix  << plusminus; // 9900. +/-300. m
    CHECK_USED(plusminus << tinylen , "0.00988 +/-0.000032 m"); // expected.
    // engineering prefix and plusminus.
    // std::cout << engineering_prefix << plusminus << tinylen << std::endl; //  9.9 +/-0.3 km
    CHECK_USED(plusminus << engineering_prefix << tinylen , "9.88 +/-0.032 mm");
    CHECK_USED(addnoisyDigit << plusminus << engineering_prefix << tinylen , "9.876 +/-0.032 mm");

    CHECK_USED(adddegfree << plusminus << engineering_prefix << tinylen , "9.88 +/-0.032 (1) mm");
    CHECK_USED(addlimits << plusminus << engineering_prefix << tinylen , "9.88 +/-0.032 <9.813, 9.939> mm");
    // Set confidence to 0.01 or 99% (from default of 0.05 or 95%) to narrow confidence interval.
    CHECK_USED(setConfidence(0.01) << addlimits << plusminus << engineering_prefix << tinylen , "9.88 +/-0.032 <9.793, 9.959> mm");
    // 
    CHECK_USED(adddegfree << addlimits << plusminus << engineering_prefix << tinylen ,
      "9.88 +/-0.032 <9.813, 9.939> (1) mm");

    // Control rounding loss from default.
    CHECK_USED(setRoundingLoss(0.002) << adddegfree << addlimits << plusminus << engineering_prefix << tinylen ,
      "9.876 +/-0.032 <9.813, 9.939> (1) mm");
    
    // This produces (two) warnings thus:
    // Epsilon 0.001 is too small for gamma rounded/unrounded ratio 0.996885, threshold is -0.00115918 for gaussian distribution.
    // Cannot compute a rounding digit index m because epsilon 0.001 is too small!
    // Note shows lots of trailing zeros because is impossible to make 'correct' rounding.
    //CHECK_USED(setRoundingLoss(0.001) << adddegfree << addlimits << plusminus << engineering_prefix << tinylen ,
    //  "9.87600000000000 +/-0.032 <9.813, 9.939> (1) mm");

} // BOOST_AUTO_TEST_CASE(unc_test_units)

/*

Output

 Unc units autoprefix test: test_autoprefix_units.cpp at "Tue Jul 17 11:10:18 2012", MSVC version 160040219.
  9876.54 m
  9.87654 km
  quantity<length, uncun > biglen(uncun(9876.0, 321.0)*meters); autoscaled = 9.9 km


  */