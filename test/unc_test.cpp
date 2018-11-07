/*!
  \file 
  \brief Single Test of uncertain classes using Boost Test Tool.
  \details Class for simple Propagation of Uncertainties
   according to a pure Gaussian model.
*/

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
// Copyright Paul A. Bristow 1998, 2012.

// unc_test.cpp 

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

// #define UNC_TRACE // #define for diagnostic output from uncertain classes.

#define BOOST_TEST_MAIN // Required for int test_main() (must come FIRST).
#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details, for example:
// Linking to lib file: libboost_unit_test_framework-vc141-mt-gd-x64-1_69.lib
// Linking to lib file: libboost_math_c99-vc141-mt-gd-x64-1_69.lib
// Linking to lib file: libboost_math_c99f-vc141-mt-gd-x64-1_69.lib 
// Linking to lib file: libboost_math_c99l-vc141-mt-gd-x64-1_69.lib
// Linking to lib file: libboost_math_tr1-vc141-mt-gd-x64-1_69.lib
// Linking to lib file: libboost_math_tr1f-vc141-mt-gd-x64-1_69.lib
// Linking to lib file: libboost_math_tr1l-vc141-mt-gd-x64-1_69.lib

#include <boost/config.hpp>
#include <boost/cstdlib.hpp> // needed for boost::exit_failure;

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink,
#include <boost/test/floating_point_comparison.hpp> // Extra test tool for FP comparison.
  using boost::unit_test::test_suite;
  using boost::unit_test::unit_test_log;

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
  //using std::lowercase;
  //using std::defaultfloat; // Initial default - neither scientific nor fixed.
  //using std::nofixed;
  //using std::noscientific;
  //using std::noadjust;
  //using std::hexbase;

#include <boost/quan/unc.hpp> // Declaration of Uncertain Classes.

#include <boost/quan/unc_init.hpp>

  // Check using manips output expected string result, for example:
// CHECK(hex << showbase << setw(10) << i, "       0xf")
// CHECK(scientific << setw(20) << d, "       1.234568e+001");
// Note: sets uncertain defaults (& oss has ios defaults too).
// BUT CHECK cannot check the number of chars output using unc_print "used",
// so use CHECK_USED for this.

#define CHECK(manips, result)\
{\
  std::ostringstream oss_check;\
  setUncDefaults(oss_check);\
  oss_check << manips;\
  BOOST_CHECK_EQUAL(oss_check.str(), result);\
}// #define	CHECK(manips, result)
//	BOOST_CHECK_EQUAL(oss_check.str().length(), strlen(result));\
// Temporarily removed because causes too much clutter in log.
// Anyway. if strings are same, then length check is superfluous.

// Also check that the unc used count was correct.
#define	CHECK_USED(manips, result)\
{\
  typedef basic_string <char>::size_type size_type;\
  std::ostringstream oss_check;\
  setUncDefaults(oss_check);\
  oss_check << manips;\
  BOOST_CHECK_EQUAL(oss_check.str(), result);\
}
//	BOOST_CHECK_EQUAL(oss_check.str().length(), static_cast<size_type>(oss_check.iword(usedIndex)));\
// Was temporarily removed because causes too much clutter in log.

// Compare results of reading C string in, for example 1.2 +/-0.01 (8)
#define	CHECK_IN(in_string, value, sd, df, types)\
{\
  uncun r;\
  std::istringstream iss(in_string);\
  setUncDefaults(iss);\
  iss >> r;\
  BOOST_CHECK_CLOSE_FRACTION(r.value(), value, numeric_limits<double>::epsilon());\
  BOOST_CHECK_CLOSE_FRACTION(r.std_dev(), sd, numeric_limits<float>::epsilon());\
  BOOST_CHECK_EQUAL(r.deg_free(), df);\
  BOOST_CHECK_EQUAL(r.types(), types);\
} // #define	CHECK_IN(in, value, sd, df, types)

// CHECK_OUT_IN Output via manips, and read back in, check is same.
#define	 CHECK_OUT_IN(manips, result, value, sd, df, types)\
{\
  typedef basic_string <char>::size_type size_type;\
  std::stringstream ss;\
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

  BOOST_AUTO_TEST_CASE(unc_test_iword)
  { // Uncertain Class iword checks.
// Integrity check on iword begin and end no longer needed in CHECK.
    std::ostringstream oss_iword_check;
    setUncDefaults(oss_iword_check);
    BOOST_CHECK_EQUAL(oss_iword_check.iword(topIndex), indexID); 
    BOOST_CHECK_EQUAL(oss_iword_check.iword(0), indexID); 
    BOOST_CHECK_EQUAL(oss_iword_check.iword(topIndex), indexID); 
    BOOST_CHECK_EQUAL(oss_iword_check.iword(0), indexID);
  } // unc_test_iword

// This test case is automatically registered by using BOOST_AUTO_TEST_CASE.
BOOST_AUTO_TEST_CASE(unc_test)
{ // Uncertain Class tests.
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

  // Custom manipulators:
  //using std::ios::lowercase;
  //using std::ios::defaultfloat; // Initial default - neither scientific nor fixed.
  //using std::ios::nofixed;
  //using std::ios::noscientific;
  //using std::ios::noadjust;
  //using std::ios::hexbase;

 
  unit_test_log.set_threshold_level(boost::unit_test::log_successful_tests);

  string message("Round to std::cout test: " __FILE__ );
#ifdef __TIMESTAMP__
  message += " at " BOOST_STRINGIZE(__TIMESTAMP__);
#endif
#ifdef _MSC_FULL_VER
  message +=  ", MSVC version " BOOST_STRINGIZE(_MSC_FULL_VER) ".";
#else
  message += "."
#endif
  BOOST_TEST_MESSAGE(message);
  BOOST_STATIC_ASSERT_MSG((numeric_limits<double>::is_iec559 == true), "Must be IEC559/IEEE754 floating point");	// IEC559/IEEE754 floating point.
  
  //________________________________________________________________________________________________________________________________
  {
    // Some floating point comparison examples, with two tolerances, just wide enough, and too tight.
  double fp1 = 1.;
  double fp2 = 1.00001;
  // Note that 
  // BOOST_CHECK_CLOSE_FRACTION(fp1, fp2, 0.00001); // just fails,
   // error : in "unc_test": difference{1e-05} between fp1{1} and fp2{1.0000100000000001} exceeds 1.0000000000000001e-05
  // but tolerance 0.001000001 passes.
  BOOST_CHECK_CLOSE_FRACTION(fp1, fp2, 0.0000100001); // Wider tolerance info: test fp1 ~= fp2 passed.
  // unc_test.cpp(211): info: difference{} between fp1{1} and fp2{1.0000100000000001} doesn't exceed 1.0000099999999999e-05
  }

  setUncDefaults(std::cout); // Should set the indexID values too.
  long& zeroid = std::cout.iword(zeroIndex);
  long& topid = std::cout.iword(topIndex);
  BOOST_CHECK_EQUAL(zeroid, indexID); // Check iword init at bottom OK,
  BOOST_CHECK_EQUAL(topid, indexID); // Check iword init at bottom OK,
  BOOST_CHECK_EQUAL(std::cout.iword(zeroIndex), indexID); // Check iword init at bottom OK,
  BOOST_CHECK_EQUAL(std::cout.iword(topIndex), indexID);// and at top too.
  long& uncFlags = std::cout.iword(uncFlagsIndex);

  // Check initial ios flags, precision, fill, width.
  // Expect IOS flags: skipwhite dec, precision 6, fill ' '
  std::streamsize originalWidth = std::cout.width(); // Not useful to restore.
  BOOST_CHECK_EQUAL(std::cout.width(), 0);  // std default precision.
  std::streamsize originalPrecision = std::cout.precision();  // Default precision is 6.
  BOOST_CHECK_EQUAL(std::cout.precision(), 6);  // std default precision is 6.
  long originalFlags = std::cout.flags();  // hex 201 == skipwhite dec.
  BOOST_CHECK_EQUAL(std::cout.flags(), 0x201); 
  BOOST_CHECK_EQUAL(std::cout.flags(), std::ios_base::skipws | std::ios_base::dec);
  BOOST_CHECK_EQUAL(std::cout.fill(), ' '); // fill char is space.
  BOOST_CHECK_EQUAL(std::cout.flags() & ios_base::floatfield, 0);
  BOOST_CHECK_EQUAL(std::cout.flags() & ios_base::adjustfield, 0);
  BOOST_CHECK_EQUAL(std::cout.flags() & ios_base::adjustfield, (int)!(ios_base::left | ios_base::right | ios_base::internal));
  BOOST_CHECK_EQUAL(std::cout.flags() & ios_base::showbase, 0); // no showbase.
  BOOST_CHECK_EQUAL(std::cout.flags() & ios_base::boolalpha, 0); // no boolapha.

  // Basic checks on re-initialisation of ios and unc using.
  void setiosDefaults(std::ostream&); // &
  void setUncDefaults(std::ios_base&);

  std::ostringstream oss;
  std::ios_base::fmtflags init_flags = oss.flags();
  //outFmtFlags(init_flags); // Default flags after initializing stream: skipws & dec.
  setUncDefaults(oss);
  long init_uflags = uFlags(oss); 
  oss.flags(~init_flags); // Alter all the flags.
  //outFmtFlags(oss.flags()); // Show altered flags.
  //outFmtFlags(0xFFFFFF); // Show all flags.
  oss.precision(99);
  BOOST_CHECK_EQUAL(oss.precision(), 99);
  oss.fill('*');
  BOOST_CHECK_EQUAL(oss.fill(), '*');

  //setuFlags(oss,0xFFFFFFFF);
  //showUncFlags(oss);
  // Restore as initialised.
  setiosDefaults(oss);
  setUncDefaults(oss);	
  //outFmtFlags(oss.flags());
  //outUncFlags(oss);
  BOOST_CHECK_EQUAL(oss.rdstate(), ios_base::goodbit);
  BOOST_CHECK_EQUAL(init_flags, oss.flags()); // Check now same as when initialised = skipws & dec.
  BOOST_CHECK_EQUAL(oss.precision(), 6);
  BOOST_CHECK_EQUAL(oss.fill(), ' ');
  BOOST_CHECK_EQUAL(oss.width(), 0);
  BOOST_CHECK_EQUAL(uFlags(oss), init_uflags);
  BOOST_CHECK_EQUAL(oss.iword(uncFlagsIndex), 0);
  BOOST_CHECK_EQUAL(oss.iword(sigDigitsIndex), 3);
  BOOST_CHECK_EQUAL(oss.iword(uncWidthIndex), 10);
  BOOST_CHECK_EQUAL(oss.iword(scaleIndex), 0);
  BOOST_CHECK_EQUAL(oss.iword(setUncSigDigitsIndex), 2);// Default 2 sig digits.

  extern const long indexID;  // random id to check init OK.
  BOOST_CHECK_EQUAL(oss.iword(zeroIndex), indexID);
  BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);

  std::streamsize w = std::cerr.width(6); // Set a width of 6.
  BOOST_CHECK_EQUAL(std::cerr.width(), 6);  // Confirm has been set to 6.
  cerr << endl; // Does NOT 'Use' width.
  BOOST_CHECK_EQUAL(std::cerr.width(), 6);  // Confirm is STILL 6.
  cerr << '\t' << endl; // (\a shows as small square) Does 'Use' width, like << "use" or << 99 
  BOOST_CHECK_EQUAL(std::cerr.width(), 0);// Check width has been reset to zero.

  { // Similar checks for unc type.
    uncun nine94(9.94, 0.1f); // 
    uncun nine95B(_nextafter(9.95, numeric_limits<double>::min()), 0.1f); // Just below 9.95.
    uncun nine95(9.95, 0.1f); // Nearest representable to 9.95.
    uncun nine95A(_nextafter(9.95, numeric_limits<double>::max()), 0.1f); // Just after 9.95.
    uncun nine96(9.96, 0.1f); // 
    CHECK(nine94, "9.94");  // Should round down up to 9.94. 
    CHECK(nine95B, "9.95");  // Should NOT round up to 10. 
    CHECK(nine95, "9.95");  // Should NOT round up to 10.  
    CHECK(nine95A, "9.95");  // Should NOT round up to 10. 
    CHECK(nine96, "9.96");  // Should NOT round up to 10.
  }

  { // Checks on setting all defaults including rounding loss and confidence/alpha.
    setUncDefaults(std::cout);
    // Check the defaults.
    BOOST_CHECK_EQUAL(std::cout.iword(uncFlagsIndex), 0);
    BOOST_CHECK_EQUAL(std::cout.iword(oldUncFlagsIndex), 0);
    BOOST_CHECK_EQUAL(std::cout.iword(sigDigitsIndex), 3);
    BOOST_CHECK_EQUAL(std::cout.iword(oldSigDigitsIndex), 3);
    BOOST_CHECK_EQUAL(std::cout.iword(setSigDigitsIndex), 3);
    BOOST_CHECK_EQUAL(std::cout.iword(uncSigDigitsIndex), 2);
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 2); 
    BOOST_CHECK_EQUAL(std::cout.iword(oldUncSigDigitsIndex), -1); 
    BOOST_CHECK_EQUAL(std::cout.iword(scaleIndex), 0);
    BOOST_CHECK_EQUAL(std::cout.iword(oldScaleIndex), -1);
    BOOST_CHECK_EQUAL(std::cout.iword(setScaleIndex), 0);
    BOOST_CHECK_EQUAL(std::cout.iword(uncWidthIndex), 10);
    BOOST_CHECK_EQUAL(std::cout.iword(oldUncWidthIndex), -1);
    BOOST_CHECK_EQUAL(std::cout.iword(usedIndex), 0);
    BOOST_CHECK_EQUAL(std::cout.iword(oldUncUsedIndex), -1);
    BOOST_CHECK_EQUAL(std::cout.iword(widthIndex), -1);
    BOOST_CHECK_EQUAL(std::cout.iword(oldWidthIndex), -1);
    BOOST_CHECK_EQUAL(std::cout.iword(roundingLossIndex), 50); // Default = 0.05 .
    BOOST_CHECK_EQUAL(std::cout.iword(confidenceIndex), 50000); // Default = 0.05 or 95%.

    // Check that setting values with manipulators gives the expected result.
    std::cout << setRoundingLoss(0.012); // Should store 12 (0.012 * 1000).
    BOOST_CHECK_EQUAL(std::cout.iword(roundingLossIndex), 12); 
    std::cout << setConfidence(0.056); // Should store 56000 because scaled by 1000000.
    BOOST_CHECK_EQUAL(std::cout.iword(confidenceIndex), 56000); 
    BOOST_CHECK_EQUAL(std::cout.iword(uncSigDigitsIndex), 2); // Default = 2
    std::cout << setUncSigDigits(0);  // Zero Sets to default = 2, not zero.
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 2);
    std::cout << setUncSigDigits(1);
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 1);
    std::cout << setUncSigDigits(2);
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 2);
    std::cout << setUncSigDigits(3);
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 3);
    std::cout << setUncSigDigits(4);
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 3); // Limit to 3 significant digits.
    std::cout << setUncSigDigits(-1); // Pass thru a negative argument to show that want an automatic choice based on uncertainty standard deviation.
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), -1);
    std::cout << setRoundingLoss(0.01) ;
    BOOST_CHECK_EQUAL(std::cout.iword(roundingLossIndex), 0.01 * 1000);
    std::cout << setConfidence(0.076) ;
    BOOST_CHECK_EQUAL(std::cout.iword(confidenceIndex), 0.076 * 1000000);
  }


  {
    // Check auto mode of selecting number uncertainty significant digits output.
    // Check effect of 1st digit '1' or '2',
    // Default 1 degree of freedom.
    uncun uu1(12.34, 1.2f);
    CHECK(plusminus << setUncSigDigits(0) << uu1, "12. +/-1.2"); // default so set to need 2 uncSigDigits.
    CHECK(plusminus << setUncSigDigits(+1) << uu1, "12. +/-1."); // +1 so need only 1 digit.
    CHECK(plusminus << setUncSigDigits(+2) << uu1, "12. +/-1.2"); // +1 so need 2 digits.
    CHECK(plusminus << setUncSigDigits(+3) << uu1, "12. +/-1.20"); // +1 so need 3 digits.
    //CHECK(plusminus << setUncSigDigits(+4) << uu1, "12. +/-1.200"); // +1 so need 4 digits.
    //CHECK(plusminus << setUncSigDigits(+3) << uu1, "12. +/-1.200"); // +1 so still need only 4 digits.
    CHECK(plusminus << setUncSigDigits(-1) << uu1, "12. +/-1.2"); // -1 so automatically set from uncertainty.

    uncun uu2(12.34, 2.3f);
    CHECK(plusminus << setUncSigDigits(-1) << uu2, "12. +/-2.3"); // 2.* so need 2 digits.
    uncun uu3(12.34, 3.3f);
    CHECK(plusminus << setUncSigDigits(-1) << uu3, "12. +/-3."); // Just 1 digit is enough.
    uncun uu31(12.34, 0.11f);
    CHECK(adddegfree << plusminus << setUncSigDigits(-1) << uu31, "12.34 +/-0.11 (1)"); // 
    uncun uu32(12.34, 0.23f); 
    CHECK(adddegfree << plusminus << setUncSigDigits(-1) << uu32, "12.3 +/-0.23 (1)"); // 
    uncun uu33(12.34, 0.34f); 
    CHECK(adddegfree << plusminus << setUncSigDigits(-1) << uu33, "12.3 +/-0.3 (1)"); // 
    uncun uu34(12.36, 0.36f); 
    CHECK(adddegfree << plusminus << setUncSigDigits(-1) << uu34, "12.4 +/-0.4 (1)"); // both rounded up.

    // Check specified UncSigDigits.
    CHECK(adddegfree << plusminus << setUncSigDigits(0) << uu34, "12.4 +/-0.36 (1)"); // both rounded up.
    CHECK(adddegfree << plusminus << setUncSigDigits(2) << uu34, "12.4 +/-0.36 (1)"); // both rounded up.
    CHECK(adddegfree << plusminus << setUncSigDigits(3) << uu34, "12.4 +/-0.360 (1)"); // both rounded up.
    CHECK(adddegfree << plusminus << setUncSigDigits(4) << uu34, "12.4 +/-0.360 (1)"); // both rounded up.

    // Check effect of increased degrees of freedom.
    uncun uu4(12.34, 0.1f); // Default 1 degree of freedom.
    uncun uu5(12.34, 0.1f, 20U);
    uncun uu6(12.34, 0.1f, 200U);
    CHECK(adddegfree << plusminus << setUncSigDigits(-1) << uu4, "12.34 +/-0.10 (1)"); // 
    CHECK(adddegfree << plusminus << setUncSigDigits(-1) << uu5, "12.34 +/-0.10 (20)"); // 
    CHECK(adddegfree << plusminus << setUncSigDigits(-1) << uu6, "12.34 +/-0.100 (200)"); // 

    // Check effect of max degrees of freedom.
    uncun udfzero(1.2, 0.01f, 0);
    CHECK(adddegfree << udfzero, "1.200 (0?)");

    uncun udfmax(1.2, 0.01f, std::numeric_limits<unsigned short int>::max());
    CHECK(adddegfree << udfmax, "1.200 (?)");
  }

  { // Check Wimmer Example 5(i) and (ii)

    setUncDefaults(cout);

    uncun uw51(127.835, 15.287f);
    //cout << plusminus << uw51 << endl; // Using default rounding loss.

    //cout << "Default roundingLoss " << std::cout.iword(roundingLossIndex) / 1000. << endl;
    // Default roundingLoss 0.05
    //cout << "Default confidenceIndex " << std::cout.iword(confidenceIndex) / 1000000.  << endl;
    // Default confidenceIndex 0.05
    CHECK(plusminus << uw51, "130. +/-15."); // Using defaults.

    // Wimmer 5(ia) specifies rounding epsilon = 0.01 and confidence alpha = 0.05 & round unc to 2 digits.
    CHECK(plusminus << setUncSigDigits(2) << setConfidence(0.05) << setRoundingLoss(0.01) << uw51, "128. +/-15.");
    // Wimmer 5(ia) calculates the confidence interval using the *rounded* mean and *rounded* sigma.
    //CHECK(plusminus << addlimits << setUncSigDigits(2) << setConfidence(0.05) << setRoundingLoss(0.01) << uw51, "128. +/-15. <98.6, 157.4>");
    // But unclear why it is not better to use the unrounded values?
 //   CHECK(plusminus << addlimits << setUncSigDigits(2) << setConfidence(0.05) << setRoundingLoss(0.01) << uw51, "128. +/-15. <97.90, 157.8>"); // Wimmer 5(i) uses 2 unc digits.
    CHECK(plusminus << addlimits << setUncSigDigits(2) << setConfidence(0.05) << setRoundingLoss(0.01) << uw51, "128. +/-15. <97.9, 157.8>"); // Wimmer 5(i) uses 2 unc digits.
    //  
    // Wimmer 5(ib) specifies rounding epsilon = 0.04 and confidence alpha = 0.05 & round unc to 1 digits.
    //CHECK(plusminus << setUncSigDigits(2) << setRoundingLoss(0.04) << uw51, "130. +/-15."); // Wimmer 5(i) uses 2 unc digits.
    //CHECK(addlimits << plusminus << setUncSigDigits(2) << setRoundingLoss(0.04) << uw51, "130. +/-15. <100.6, 159.4>"); // Wimmer 5(i) uses 2 unc digits.
  //  CHECK(addlimits << plusminus << setUncSigDigits(2) << setRoundingLoss(0.04) << uw51, "130. +/-15. <98.00, 158.0>"); // Using unrounded.
    CHECK(addlimits << plusminus << setUncSigDigits(2) << setRoundingLoss(0.04) << uw51, "130. +/-15. <98., 158.>"); // Using unrounded.

    ////  Wimmer example 5(ii)
    uncun uw52(1.23875, 0.0232f);  
    // Round sigma to one significant digit, but find cannot use epsilon = 0.4, so increase to 0.1,
    // cout << plusminus << setUncSigDigits(1)<< uw52 << endl; // 1.24 +/-0.02
    //
    CHECK(plusminus << setUncSigDigits(1) << setRoundingLoss(0.1) << uw52, "1.24 +/-0.02");
    // Using Wimmer rounded mean and sigma:
    //CHECK(addlimits << plusminus << setUncSigDigits(2) << setRoundingLoss(0.1) << uw52, "1.24 +/-0.02 <1.2008, 1.2792>");
    // Using unrounded for confidence interval.
    CHECK(addlimits << plusminus << setUncSigDigits(1) << setRoundingLoss(0.1) << uw52, "1.24 +/-0.02 <1.193, 1.284>");
    // Since the difference is insignicant and it is troublesome to use the rounded mean and sigma,
    // will use the unrounded values.
  }

  {// Check Wimmer example 6.

    setUncDefaults(cout);

    uncun u6(56.387, (5.67f+5.82f)/2);
    //cout << "mean sigma = " << u6.std_dev() << endl; //   mean sigma = 5.745
    CHECK(addlimits << plusminus << setUncSigDigits(2) << setRoundingLoss(0.05) << setConfidence(0.05) 
    //  << u6, "56. +/-5.7 <44.88, 67.11>"); // Using Wimmer rounded values.
    //    << u6, "56. +/-5.7 <45.10, 67.60>"); // Note wider limits using unrounded values. Previously.
        << u6, "56. +/-5.7 <45.1, 67.6>"); // Note wider limits using unrounded values - note no trailing zeros.

    CHECK(addlimits << plusminus << setUncSigDigits(2) << setRoundingLoss(0.05) << setConfidence(0.01) 
    //  << u6, "56. +/-5.7 <41.3941, 70.6059>"); // Using Wimmer rounded values.
   //   << u6, "56. +/-5.7 <41.60, 71.20>");// Note wider limits using unrounded values and 99% confidence.
        << u6, "56. +/-5.7 <41.6, 71.2>"); // Note no trailing zeros now.
  }
  { // Test one parameter manips to set unc width, scale, sigdigits & unc sigdigits.
    //cout << "\nTest one parameter manips to set unc width, scale, sigdigits & unc sigdigits.\n";
    setUncDefaults(cout);	// Resets stream's format flags to default.
    cout << resetiosflags(ios_base::basefield | ios_base::adjustfield | ios_base::floatfield
    | ios_base::showpos | ios_base::showpoint | ios_base::uppercase | ios_base::showbase )
    << endl;
    setiosDefaults(cout); // &
    setUncDefaults(cout);
    // Test for the default values.
    BOOST_CHECK_EQUAL(std::cout.flags(), ios_base::skipws | ios_base::dec);
    BOOST_CHECK_EQUAL(std::cout.iword(oldUncSigDigitsIndex), -1);
    BOOST_CHECK_EQUAL(std::cout.iword(sigDigitsIndex), 3);  // default = 3.
    BOOST_CHECK_EQUAL(std::cout.iword(uncSigDigitsIndex), 2);  // default = 2.
    BOOST_CHECK_EQUAL(std::cout.iword(scaleIndex), 0);
    BOOST_CHECK_EQUAL(std::cout.iword(widthIndex), -1);
    BOOST_CHECK_EQUAL(std::cout.iword(oldScaleIndex), -1);  // No previous value
    BOOST_CHECK_EQUAL(std::cout.iword(setScaleIndex), 0);
    BOOST_CHECK_EQUAL(std::cout.iword(scaleIndex), 0);

    cout << setSigDigits(4) << endl; // setSigDigits(4) 
    BOOST_CHECK_EQUAL(std::cout.iword(setSigDigitsIndex), 4); 
    BOOST_CHECK_EQUAL(std::cout.iword(oldSigDigitsIndex), 3); // Check saved (default =3).

    BOOST_CHECK_EQUAL(std::cout.iword(oldUncSigDigitsIndex),-1); // Not set yet.
    cout << setSigDigits(2)	<< endl;
    BOOST_CHECK_EQUAL(std::cout.iword(setSigDigitsIndex), 2);
    cout << setUncSigDigits(3) << endl;
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 3);
    cout << setUncSigDigits(2) << endl;
    BOOST_CHECK_EQUAL(std::cout.iword(setUncSigDigitsIndex), 2);
    BOOST_CHECK_EQUAL(std::cout.iword(oldUncSigDigitsIndex), 3);

    cout << setUncSigDigits(2);
    BOOST_CHECK_EQUAL(std::cout.iword(uncSigDigitsIndex), 2);

    cout << setUncWidth(20) << flush; // setUncWidth(10)
    BOOST_CHECK_EQUAL(std::cout.iword(uncWidthIndex), 20);

    cout << setScale(-3) << flush; //  setScale(-3) 
    BOOST_CHECK_EQUAL(std::cout.iword(setScaleIndex), -3);
  }
  { // cout <<	"\nNative method of display of floating point infinity & Not a Number\n";
    // MSVC outputs for non-finite
    //CHECK(numeric_limits<double>::infinity(), "1.#INF");
    //CHECK(-numeric_limits<double>::infinity(), "-1.#INF");
    //CHECK(numeric_limits<float>::quiet_NaN() , "1.#QNAN");
    //CHECK(-numeric_limits<float>::quiet_NaN() , "-1.#IND");
    // 
    CHECK(numeric_limits<double>::infinity(), "inf");
    CHECK(-numeric_limits<double>::infinity(), "-inf");
    CHECK(numeric_limits<float>::quiet_NaN() , "nan");
    CHECK(-numeric_limits<float>::quiet_NaN() , "-nan(ind)");

    // Uncertain type Infinite value - known exactly.
    uncun infinite(numeric_limits<double>::infinity(), 0.f);  // Exact plus infinity.
    CHECK_USED(infinite, "inf");
    CHECK_USED(plusminus << infinite, "inf +/-0");

    uncun infiniteUK(numeric_limits<double>::infinity(), 0.1f);  // unknown plus infinity.
    CHECK_USED(plusminus << infiniteUK, "inf +/-0.10");

    uncun exactNaN(numeric_limits<double>::quiet_NaN(), 0.f); // Exact NaN.
    CHECK_USED(exactNaN, "NaN");
    CHECK_USED(plusminus << exactNaN, "NaN +/-0");

    // NaN with known uncertainty.
    uncun NaNknown(numeric_limits<double>::quiet_NaN(), 0.02f);
    CHECK_USED(NaNknown, "NaN");
    CHECK_USED(plusminus << NaNknown, "NaN +/-0.020");
    uncun NaNknown2(numeric_limits<double>::quiet_NaN(), 0.3f);
    CHECK_USED(plusminus << NaNknown2, "NaN +/-0.30");

    uncun undefined_uncertain = uncun(numeric_limits<double>::quiet_NaN(), 0.1f);
    CHECK_USED(plusminus << undefined_uncertain, "NaN +/-0.10");

    uncun NaNUnknown(numeric_limits<double>::quiet_NaN(), numeric_limits<float>::quiet_NaN()); // NaN of NaN uncertainty.
    CHECK_USED(plusminus << NaNUnknown, "NaN +/-?");

    // Known Values with *unknown uncertainty*.
    // Try to treat uncertainty == NaN as a special case,
    // but get different results for zero and other values.
    // max_digits10 used for zero (for which a case could be made),
    // Unable to see why at present, but other cases may be flawed too.

    // now 0.0000 rather than 0.00000000000000000

    uncun zeroMaybe(0., numeric_limits<float>::quiet_NaN() );
    CHECK_USED(zeroMaybe, "0.00000000000000000");  // 0.00000000000000000

    CHECK_USED(setSigDigits(5) << setsigdigits << zeroMaybe, "0.00000"); // 
    // setsigdigits must override all other options (and also set stddev sigfigs too).

    CHECK_USED(zeroMaybe, "0.00000000000000000");  // 0
    CHECK_USED(plusminus << zeroMaybe, "0.00000000000000000 +/-?");
    CHECK_USED(plusminus << zeroMaybe, "0.00000000000000000 +/-?");
  
    uncun postwoMaybe(+2., numeric_limits<float>::quiet_NaN());  // +1.234 with unknown uncertainty.
    //cout << postwoMaybe << endl;
    CHECK_USED(postwoMaybe, "2.0000000000000000"); 
    uncun posonetwoMaybe(+1.234, numeric_limits<float>::quiet_NaN());  // +1.234 with unknown uncertainty.
    //cout << posonetwoMaybe << endl;
    CHECK_USED(posonetwoMaybe, "1.2340000000000000"); 

    uncun negtwoMaybe(-2.2, numeric_limits<float>::quiet_NaN());  // -2.2 with unknown uncertainty.
    CHECK_USED(negtwoMaybe, "-2.2000000000000002"); 
    CHECK_USED(plusminus << negtwoMaybe, "-2.2000000000000002 +/-?") 
    // Fails here on release version.
    uncun twoMaybe(+2.2, numeric_limits<float>::quiet_NaN());  // +2.2 with unknown uncertainty.
    CHECK_USED(twoMaybe, "2.2000000000000002"); 
    CHECK_USED(plusminus << twoMaybe, "2.2000000000000002 +/-?") 
    uncun infUnknown(9.8, numeric_limits<float>::infinity() );  // value with infinite uncertainty.
    CHECK_USED(infUnknown, "9.8000000000000007"); 
    CHECK_USED(plusminus << infUnknown, "9.8000000000000007 +/-inf");

    BOOST_TEST_MESSAGE("Uncertain Class tests log end. " << __FILE__ << ' ' <<  __TIMESTAMP__ );
  }
}  // Uncertain Class tests. 


/*

Output:










*/