/*!
  \file
  \brief Testing uncertain classes using Boost Test Tool.
  \details Class for simple Propagation of Uncertainties
     according to a pure Gaussian model.

    see also test_rounding.cpp for tests of the rounding algorithms used.
*/

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
// Copyright Paul A. Bristow 1998, 2012, 2021.

// unc_tests.cpp
// #define BOOST_QUAN_UNC_TRACE // Diagnostic output.

#include <boost/config.hpp>
#include <boost/cstdlib.hpp> // needed for boost::exit_failure;

#define BOOST_TEST_MAIN // Required for int test_main() (must come FIRST).
//#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-s-1_49.lib

#include <boost/test/included/unit_test.hpp> //
//#include <boost/core/lightweight_test.hpp >  does not support BOOST_AUTO_TEST_CASE' so can't use without big changes.


//#include <boost/test/tools/floating_point_comparison.hpp> // Extra test tool for FP comparison.
 // using boost::unit_test::test_suite;
//  using boost::unit_test::unit_test_log;
// unit_test_log caused trouble with GCC and MSYS so removed for now.

#include <boost/math/special_functions/next.hpp>
//     using boost::math::nextafter;

// Classes for simple propagation of Uncertainties according to a pure Gaussian model.

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

#include <boost/quan/xiostream.hpp> // extra iostream manipulators like noshowbase
  using std::defaultfloat; // Initial default - neither scientific nor fixed.
 // using std::nofixed;
//  using std::noscientific;
//  using std::noadjust;
//  using std::hexbase;

#include <boost/quan/unc.hpp> // Declaration of Uncertain Classes.

// Files made global to avoid nasty memory leak, and loop to end of memory.
const char testInputFilename[] = "unc_test_input.txt"; // Input for tests (fin).
ifstream fin(testInputFilename, ios_base::in);
const char outFilename[] = "unc_test_output.txt"; // output from tests (fout).
ofstream fout(outFilename, ios_base::out); // Use default ios_base::overwrite/replace.
// const char logFilename[] = "unc_log.txt"; // // Boost test log (usual to send to cout).
// ofstream lout(logFilename, ios_base::out); // Use default overwrite/ iso::replace.
//const char diagFilename[] = "unc_diag.txt"; // diagnostic log diverted from cerr (dout).
 // ofstream dout(diagFilename, ios_base::out); // Use default overwrite/ iso::replace.
//const char testLogFilename[] = "unc_test.log"; // Not used yet.

// typedef basic_string <char>::size_type size_type;
// using basic_string <char>::size_type;

// toString concept works for some purposes,
// but is flawed to combine several manipulators and it needs setUncDefaults.
// Check using manips output expected string result, for example:
// CHECK(std::hex << std::showbase << std::setw(10) << i, "       0xf")
// CHECK(std::scientific << std::setw(20) << d, "       1.234568e+001");
// Note: sets uncertain defaults (& oss has ios defaults too).
// BUT CHECK cannot check the number of chars output using unc_print "used", so can use CHECK_USED for this.

using namespace boost::quan;

#define CHECK(manips, result)\
{\
  std::ostringstream oss;\
  setUncDefaults(oss);\
  oss << manips;\
  BOOST_CHECK_EQUAL(oss.str(), result);\
}// #define CHECK(manips, result)

// Also check that the unc used count was correct.
#define CHECK_USED(manips, result)\
{\
  typedef basic_string <char>::size_type size_type;\
  std::ostringstream oss;\
  setUncDefaults(oss);\
  oss << manips;\
  BOOST_CHECK_EQUAL(oss.str(), result);\
}
//  BOOST_CHECK_EQUAL(oss.str().length(), static_cast<size_type>(oss.iword(usedIndex)));\
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
// BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);\
// BOOST_CHECK_EQUAL(oss.iword(0), indexID);\
// BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);\
// BOOST_CHECK_EQUAL(oss.iword(0), indexID);\

// This test case is automatically registered by using BOOST_AUTO_TEST_CASE.

BOOST_AUTO_TEST_CASE(unc_test_basic)
{ // Uncertain Class tests.
 // boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages);

//  string message("Round to cout test: " __FILE__ );
//#ifdef __TIMESTAMP__
//  message += " at " BOOST_STRINGIZE(__TIMESTAMP__);
//#endif
//#ifdef _MSC_FULL_VER
//  message +=  ", MSVC version " BOOST_STRINGIZE(_MSC_FULL_VER) ".";
//#else
//  message += "."
//#endif
 // BOOST_TEST_MESSAGE(message);

 //   BOOST_CHECK(zeroIndex == indexID); // Should pass?
  // BOOST_CHECK(lout.is_open());
  //   unc_tests.cpp(235): info: check lout.is_open() passed
  // unit_test_log.set_stream(lout); // Switch to log file.
  // BOOST_TEST_MESSAGE(message);

  BOOST_CHECK(std::numeric_limits<double>::is_iec559 == true); // Must be IEC559/IEEE754 floating-point.

  // Change log level to record warnings & errors.
  // unit_test_log.set_log_threshold(boost::unit_test::log_successful_tests);
 // unit_test_log.set_threshold_level(boost::unit_test::log_all_errors);
  //unit_test_log::instance().set_threshold_level(test_suite);
  //unit_test_log::instance().set_threshold_level(messages); // user messages
  // Prepare to send log to a file instead of std::cout.


  BOOST_CHECK(fout.is_open());
  // Test output to file ...
  fout << "Test Output from " << __FILE__ << " " << __TIMESTAMP__"\n" << std::endl;
  std::cout << "\x0F1 Uncertain Class Test output to " << outFilename << ' '
    << __FILE__ << ' ' <<  __TIMESTAMP__ << std::endl;
  // +- symbol on screen std::cout = dec 177, hex F1 but shows ~n in files?
  // BUT is messy because in file codeset +- symbol \x0B1 176! octal \361

  // Test diagnostic output to file ...
  //BOOST_CHECK(lout.is_open());
  //lout << "Unc " << logFilename << " opened." << std::endl;

  //BOOST_CHECK(dout.is_open());
  //dout << "Unc Diagnostics logged to " << diagFilename << " from " << __FILE__ << " " << __TIMESTAMP__".\n"<< std::endl;
 // std::cout << "Unc Diagnostics logged to " << diagFilename  << std::endl; // \x0F1 on screen but ~n in files.
//  std::cerr.rdbuf(dout.rdbuf()); // cerr = dout;  // Switch cerr to diagnostic log.
  // dout << "Diagnostic cerr from " << __FILE__ << " " << __TIMESTAMP__"\n" << std::endl;
//  std::cerr << "\x0B1 \x0B5 Diagnostic cerr from " << __FILE__ << " " << __TIMESTAMP__".\n" << std::endl;
  // Greek mu is \x0B5 for files, degree symbol is \x0B0
  // BOOST_CHECK(fin.is_open());  // No input yet?

  BOOST_TEST_MESSAGE("Uncertain Class tests log. " << "                                     Expected   Was    Expected");

  // std::cout.fill('0'); // to get trailing zeros.
  // std::cout << std::fixed << std::setprecision(17) << 12.34 << std::defaultfloat << std::endl; // 12.34000000000000000
  // gives 17 digits (2 digits plus 15 trailing zeros AFTER decimal point).

  //________________________________________________________________________________________________________________________________
  {
    // Some floating point comparison examples, with two tolerances, just wide enough, and too tight.
  double fp1 = 1.;
  double fp2 = 1.00001;
  BOOST_CHECK_CLOSE(fp1, fp2, 0.00100001); // Wide tolerance info: test fp1 ~= fp2 passed.
  // difference between fp1{1} and fp2{1.00001} exceeds 0.001% (0.00001 * 100 = 0.001%)
  // Note that BOOST_CHECK_CLOSE(fp1, fp2, 0.001); // just fails, but 0.001000001 passes.

  // BOOST_CHECK_CLOSE(fp1, fp2, 0.0000000001); // Tight tolerance so should fail [1 !~= 1.00001 (+/-1e-010)].
  // error in "call_test_main": difference between fp1{1} and fp2{1.00001} exceeds 1e-010%
  }

  setUncDefaults(fout); // Should set the indexID values too.
  long& zeroid = fout.iword(zeroIndex);
  long& topid = fout.iword(topIndex);
  BOOST_CHECK_EQUAL(zeroid, indexID); // Check iword init at bottom OK,
  BOOST_CHECK_EQUAL(topid, indexID); // Check iword init at bottom OK,
  BOOST_CHECK_EQUAL(fout.iword(zeroIndex), indexID); // Check iword init at bottom OK,
  BOOST_CHECK_EQUAL(fout.iword(topIndex), indexID);// and at top too.
  long& uncFlags = fout.iword(uncFlagsIndex);

 // setUncDefaults(dout); // Should set the indexID values too.
  //zeroid = dout.iword(zeroIndex);
  //topid = dout.iword(topIndex);
  //BOOST_CHECK_EQUAL(zeroid, indexID); // Check iword init at bottom OK,
  //BOOST_CHECK_EQUAL(topid, indexID);// & at top too.

  setUncDefaults(std::cout);

  //// Check initial ios flags, precision, fill, width.
  //// Expect IOS flags: skipwhite dec, precision 6, fill ' '
  //std::streamsize originalWidth = fout.width(); // Not useful to restore.
  //BOOST_CHECK_EQUAL(fout.width(), 0);  // std default precision.
  //std::streamsize originalPrecision = fout.precision();  // Default precision is 6.
  //BOOST_CHECK_EQUAL(fout.precision(), 6);  // std default precision is 6.
  //long originalFlags = fout.flags();  // hex 201 == skipwhite dec.
  //BOOST_CHECK_EQUAL(fout.flags(), 0x201);
  //BOOST_CHECK_EQUAL(fout.flags(), std::ios_base::skipws | std::ios_base::dec);
  //BOOST_CHECK_EQUAL(fout.fill(), ' '); // fill char is space.
  //BOOST_CHECK_EQUAL(fout.flags() & ios_base::floatfield, 0);
  //BOOST_CHECK_EQUAL(fout.flags() & ios_base::adjustfield, 0);
  //BOOST_CHECK_EQUAL(fout.flags() & ios_base::adjustfield, (int)!(ios_base::left | ios_base::right | ios_base::internal));
  //BOOST_CHECK_EQUAL(fout.flags() & ios_base::showbase, 0); // no showbase.
  //BOOST_CHECK_EQUAL(fout.flags() & ios_base::boolalpha, 0); // no boolapha.

  // Basic checks on re-initialisation of ios and unc using.
//  void setiosDefaults(std::ostream&); // &

 // void boost::quan::setUncDefaults(std::ios_base&);

  std::ostringstream oss;
  std::ios_base::fmtflags init_flags = oss.flags();
  //outFmtFlags(init_flags); // Default flags after initializing stream: skipws & dec.
  boost::quan::setUncDefaults(oss);
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
  boost::quan::setUncDefaults(oss);
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

  BOOST_CHECK_EQUAL(oss.iword(zeroIndex), indexID); // Magic number used to check that initialization has taken place.
  BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);

  std::streamsize w = std::cerr.width(6); // Set a width of 6.
  BOOST_CHECK_EQUAL(std::cerr.width(), 6);  // Confirm has been set to 6.
  std::cerr << std::endl; // Does NOT 'Use' width.
  BOOST_CHECK_EQUAL(std::cerr.width(), 6);  // Confirm is STILL 6.
  std::cerr << '\t' << std::endl; // (\a shows as small square) Does 'Use' width, like << "use" or << 99
  BOOST_CHECK_EQUAL(std::cerr.width(), 0);// Check width has been reset to zero.
  } // BOOST_AUTO_TEST_CASE(unc_test_basic)

BOOST_AUTO_TEST_CASE(unc_test_std_rounding)
{ // Show 'C++ Standard' rounding of 9.95 and precision(6) is NOT "10."
  {
    // Repeated with unc types later.
    double nine95 = 9.95;
    double nine94 = 9.94;
    double nine96 = 9.96;
    CHECK(nine95, "9.95");  // Should NOT round up to 10 - default precision is 6.
    CHECK(nine94, "9.94");  // Should NOT down up to 9.9
    CHECK(nine96, "9.96");  // Should NOT round up to 10.

    CHECK(setprecision(2) << nine95, "9.9");  // Should round up to 10.
    CHECK(setprecision(2) << nine94, "9.9");  // Should down up to 9.9 .
    CHECK(setprecision(2) << nine96, "10");  // Should round up to 10 .
    CHECK(showpoint << setprecision(2) << nine96, "10.");  // Should round up to 10.
    CHECK(fixed << noshowpoint << setprecision(2) << nine95, "9.95");
    CHECK(fixed << showpoint << setprecision(2) << nine95, "9.95");
  }
  { // Similar checks for unc type.
    using boost::math::float_next;
    using boost::math::nextafter;

    uncun nine94(9.94, 0.1f); //
    uncun nine95B(boost::math::nextafter(9.95, (std::numeric_limits<double>::min)()), 0.1f); // Just below 9.95.
    uncun nine95(9.95, 0.1f); // Nearest representable to 9.95.)
    uncun nine95A(boost::math::nextafter(9.95, (std::numeric_limits<double>::max)()), 0.1f); // Just after 9.95.
    uncun nine96(9.96, 0.1f); //
    CHECK(nine94, "9.94");  // Should round down up to 9.94.
    CHECK(nine95B, "9.95");  // Should NOT round up to 10.
    CHECK(nine95, "9.95");  // Should NOT round up to 10.
    CHECK(nine95A, "9.95");  // Should NOT round up to 10.
    CHECK(nine96, "9.96");  // Should NOT round up to 10.
  }
}// BOOST_AUTO_TEST_CASE(unc_test_std_rounding)


 BOOST_AUTO_TEST_CASE(unc_test_zeros)
 { // Uncertain reals - constructor and get/set functions.
   { // (Start a new scope for all uncs to allow reuse of names like one23...)
    uncun zd(0);  // Integer Zero, using constructor from int zero.
    unsigned short f = zd.uncFlags();
    BOOST_CHECK(zd.types() & VALUE_ZERO); // Should be flagged as zero.
    BOOST_CHECK(zd.types() & VALUE_EXACT); // Should be flagged as exact because sd == 0.
    BOOST_CHECK(zd.types() & VALUE_INTEGER); // and integer.
    CHECK(zd, "0"); // Expect just 0 because is exact integer zero.

    uncun u0(0.); // Construct from double zero (default unc, df, & flags)
    BOOST_CHECK_EQUAL(u0.value(), 0.);
    BOOST_CHECK_EQUAL(u0.std_dev(), 0.f);
    BOOST_CHECK_EQUAL(u0.deg_free(), 0);
    BOOST_CHECK(u0.types() & VALUE_ZERO); // Should still be flagged as zero.
    CHECK(u0, "0"); // Expect 0 because is exact and isInteger zero because zero uncertainty.

   }
   {
   uncun u0(0, 0);  // Exact Zero, using constructor from int zero.
    //BOOST_CHECK(u0.types() & VALUE_ZERO);
    //BOOST_CHECK((u0.types() & VALUE_INTEGER)); //

    u0.std_dev(0.01f); // Zero, but add known uncertainty.
    BOOST_CHECK_EQUAL(u0.std_dev(), 0.01f);
    BOOST_CHECK(u0.types() & VALUE_ZERO);
    BOOST_CHECK(!(u0.types() & VALUE_INTEGER)); // No longer exact,
    BOOST_CHECK((u0.types() & UNC_KNOWN)); // but unc is known.
    CHECK_USED(plusminus << u0, "0.000 +/-0.010");  // So now show added uncertainty.

    u0.std_dev(0.1f); // Increase std deviation.
    BOOST_CHECK(u0.types() & VALUE_ZERO); // Should still be flagged as zero.
    BOOST_CHECK(!(u0.types() & VALUE_INTEGER)); // Made uncertain, so no longer exact.
    CHECK_USED(u0, "0.00"); // So now add decimal point.
    CHECK_USED(plusminus << u0, "0.00 +/-0.10"); // So now show increased uncertainty.

    u0.std_dev(1.f); // Increase std deviation more.
    BOOST_CHECK(u0.types() & VALUE_ZERO); // Should still be flagged as zero.
    BOOST_CHECK(!(u0.types() & VALUE_INTEGER)); // Made uncertain so no longer exact.
    CHECK_USED(u0, "0.0"); // Add decimal point.
    CHECK_USED(plusminus << u0, "0.0 +/-1.0"); // Add uncertainty.

    uncun z(0);  // Exact Zero, using constructor from integer.
    BOOST_CHECK_EQUAL(z.value(), 0.); // value
    BOOST_CHECK_EQUAL(z.std_dev(), 0.f);// std_dev
    BOOST_CHECK_EQUAL(z.deg_free(), 0); // deg_free zero because one 'observation'.
    BOOST_CHECK(z.types() & VALUE_ZERO); // Is zero.
    BOOST_CHECK(z.types() & VALUE_INTEGER); // Is integer.
    CHECK_USED(z, "0"); // Exact zero.
    CHECK_USED(std::setw(0) << z, "0"); // Exact zero, default width.
    CHECK_USED(std::setw(1) << z, "0"); // Tight fit.
    CHECK_USED(std::setw(2) << z, " 0"); // no adjust == right justify
    CHECK_USED(std::left << setw(2) << z, "0 "); // Just 1 pad.
    CHECK_USED(std::left << setw(3) << z, "0  "); // 2 pads.
    CHECK_USED(std::left << setw(4) << z, "0   "); // 1 digit plus 3 pad.
    CHECK_USED(std::left << setw(4) << z, "0   "); // 1 digit plus 3 pad, left == default.
    CHECK_USED(std::right << setw(4) << z, "   0"); // right so 3 pad plus 1 digit.
   }
  } //  BOOST_AUTO_TEST_CASE(unc_test_zeros)

 BOOST_AUTO_TEST_CASE(unc_test_unity)
 {
    uncun unity(1); // Exact unity constructed from integer.
    BOOST_CHECK(unity.types() & VALUE_INTEGER); // Check IS recognised as integer.
    CHECK_USED(unity, "1");  // No decimal point for integer.
// CHECK_USED(showpoint << unity, "1."); // DO show decimal point for integer value but only if showpoint.
// But this would imply all the setprecision (6) trailing zeros!  showpoint is MAD!
    BOOST_CHECK(unity.types() & VALUE_EXACT); // And is exact (sd == 0).
    CHECK_USED(plusminus << unity, "1"); // Do NOT show +/-0 for integer value.

    unity.types(VALUE_INTEGER); // Clear integer flag.
    BOOST_CHECK((unity.types() & VALUE_INTEGER) == false); // Check integer flag has been cleared.
    BOOST_CHECK(unity.types() & VALUE_EXACT); // Check is still exact (sd == 0).

    CHECK_USED(unity, "1."); // Should have decimal point because sd == 0,
    // so exact (but no longer VALUE_INTEGER == true).
    CHECK_USED(showpoint << unity, "1."); // Regardless of showpoint or not.

    CHECK_USED(plusminus << unity, "1. +/-0"); // Do show +/-0 for exact value (no longer integer).
    // Show "+/-0" if requested for real exact, for example: "2.54 +/-0".
    // But NOT for integer, so, for example, just "1" and never "1 +/-0".

    uncun ud(1., 0.f);  // Exact Unity from double.
    BOOST_CHECK(!(ud.types() & VALUE_ZERO)); // Should NOT be flagged as zero.
    BOOST_CHECK(ud.types() & VALUE_EXACT); // Should be flagged as zero.
    BOOST_CHECK(!(ud.types() & VALUE_INTEGER)); // but NOT integer.

    uncun negud(-1., 0.f);  // Exact Minus negative Unity from double.
    BOOST_CHECK(!(negud.types() & VALUE_ZERO)); // Should NOT be flagged as zero.
    BOOST_CHECK(negud.types() & VALUE_EXACT); // Should be flagged as zero.
    uncun u0(0.);
    u0.std_dev(0.001f);
    CHECK_USED(u0, "0.0000");
    u0.std_dev(0.0001f);
    CHECK_USED(u0, "0.00000"); //
    u0.std_dev(0.00001f);
    CHECK_USED(u0, "0.000000"); //
    u0.std_dev(0.000001f);
    CHECK_USED(u0, "0.0000000"); //
    u0.std_dev(0.000000000000001f);
    CHECK_USED(u0,"0.00000000000000"); // All 15 guaranteed decimal digits.
    u0.std_dev(0.0000000000000001f);
    CHECK_USED(u0, "0.00000000000000"); // All 17 significant decimal digits.
    //  uncun exact(1., 1.f, 2, VALUE_EXACT);  // Exact Unity from double with std_dev and deg_free.
    // This should signal a conflict in the constructor!
    // Uncertain warning : value 1 is flagged as uncTypeFlags == VALUE_EXACT, but uncertainty 1 is not zero!
    // Uncertain warning : value 1 flagged as uncTypeFlags == VALUE_EXACT, but degfree 2 is not zero!
    // BOOST_CHECK(exact.types() & VALUE_EXACT); // Should still be flagged as exact.
    // BOOST_CHECK_EQUAL(exact.value(), 1.); // value
    // BOOST_CHECK_EQUAL(exact.std_dev(), 0.f);//  StdDeviation should be over-ridden by VALUE_EXACT,
   // BOOST_CHECK_EQUAL(exact.deg_free(), 0); // deg_free should be overwritten by VALUE_EXACT, hence warning.

    uncun iminus1(-1); // Exact minus 1 from integer -1 value.
    BOOST_CHECK(iminus1.types() & VALUE_INTEGER); // Check IS recognised as integer.
    BOOST_CHECK(iminus1.types() & VALUE_EXACT); // Check IS recognised as integer.
    BOOST_CHECK(iminus1.types() & UNC_NOPLUS); // Check IS recognised as no > 0.
    uncun iminus10(-10); // Exact minus 10 from integer -1 value.
    BOOST_CHECK(iminus10.types() & VALUE_INTEGER); // Check IS recognised as integer.
    BOOST_CHECK(iminus10.types() & UNC_NOPLUS); // Check IS recognised as not > 0.
    BOOST_CHECK(iminus10.types() & UNC_NOMINUS); // Check IS recognised as not < 0.
    CHECK_USED(iminus10, "-10");  // Must be negative integer.

    uncun uminus1(-1.); // NOT Exact minus 1 from double -1. value.
    BOOST_CHECK(!(uminus1.types() & VALUE_INTEGER)); // Check IS recognised as integer.
    CHECK_USED(uminus1, "-1.");  // Must be negative double, not integer.
    // These fail separating 1 and .
    //CHECK_USED(left << setw(10)  << uminus1, "-1.       ");  //
    //CHECK_USED(right << setw(10) << uminus1, "       -1.");  //

 }   // BOOST_AUTO_TEST_CASE(unc_test_unity)

 BOOST_AUTO_TEST_CASE(unc_test_1234)
 {
    int t = 12;
    uncun twelveI(t); // Exact 12 from integer variable.
    BOOST_CHECK(twelveI.types() & VALUE_INTEGER); // Check IS integer from variable.
    CHECK_USED(twelveI, "12"); // no . because integer.
//    CHECK_USED(noplusminus << showpoint << twelveI, "12."); // Show decimal point for integer value only if showpoint.
    CHECK_USED(plusminus << twelveI, "12"); // Do NOT show +/-0 for integer value.
    twelveI.types(VALUE_INTEGER); // Clear integer flag.
    BOOST_CHECK(!(twelveI.types() & VALUE_INTEGER)); // Check integer flag has cleared.
    CHECK_USED(twelveI, "12."); // Should have decimal point because sd = 0 so should be exact like 2.54.

    uncun twelve3(12.3, 0.026f); // Explicit sd.
    CHECK_USED(noplusminus << twelve3, "12.30"); // OK
    CHECK_USED(plusminus << twelve3, "12.30 +/-0.026");
    //CHECK_OUT_IN(plusminus << twelve3, "12.30 +/-0.05", 12.3, 0.025f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT) ));

    uncun imax((std::numeric_limits<int>::max)()); // Exact int max 2147483647.
    BOOST_CHECK(imax.types() & VALUE_INTEGER); // Check IS integer from argument.
    CHECK_USED(imax, "2147483647"); // no . because integer.

    uncun imin((std::numeric_limits<int>::min)()); // Exact int min -2147483648.
    BOOST_CHECK(imin.types() & VALUE_INTEGER); // Check IS integer from argument.
    CHECK_USED(imin, "-2147483648"); // no . because integer.
    uncun twelvede(1234.); // double from double variable, default stddev, so implicitly exact.
    BOOST_CHECK(!(twelvede.types() & VALUE_INTEGER)); // Check is NOT integer from variable.
    BOOST_CHECK((twelvede.types() & VALUE_EXACT)); // Check IS flagged as exact.
    CHECK_USED(twelvede, "1234."); // . because from double, even though exact.  // 1.234 no trailing zeros.
 } // BOOST_AUTO_TEST_CASE(unc_test_1234)

 BOOST_AUTO_TEST_CASE(unc_test_exact)
 { // Exact values
    // 1. inch Autoscaled SI 25. +/- 13. millimeter or 25. mm
    // 1.0 inch Autoscaled SI 25.4 +/- 1.3 millimeter or 25.4 mm
    // 1.00 Autoscaled SI 25.4 +/- 0.13 millimeter or 25.4 mm
    // 1.000 Autoscaled SI 25.400 +/- 0.013 millimeter or 25.400 mm

    uncun inch_in_cm(2.54); // Exact, with decimal fraction part, implicitly created from double.
    BOOST_CHECK((inch_in_cm.types() & VALUE_EXACT)); // Check is known to be exact.
    CHECK_USED(inch_in_cm, "2.54"); // No trailing zeros, because exact.
    // Trailing zeros would imply some uncertainty.
    inch_in_cm.setUncTypes(inch_in_cm.types() & ~VALUE_EXACT); // Flag as NOT exact.
    BOOST_CHECK(!(inch_in_cm.types() & VALUE_EXACT)); // Check exact flag has cleared.
    // CHECK(inch_in_cm, "2.54000000000000"); // Max trailing zeros, because sd uncertainty is still == 0.F.
    CHECK(inch_in_cm, "2.54");
    inch_in_cm.std_dev(0.0012f); // Add some uncertainty.
    BOOST_CHECK(!(inch_in_cm.types() & VALUE_EXACT)); // Check still NOT exact.
    CHECK_USED(inch_in_cm, "2.540"); // One trailing zeros because uncertaintu (std dev) specified as 0.012f.
    inch_in_cm.types(VALUE_EXACT); // Make Exact - Explicitly. - Doesn't work
    inch_in_cm.std_dev(0.f); // No uncertainty.
    BOOST_CHECK((inch_in_cm.types() & VALUE_EXACT)); // Check is now known to be exact.
    CHECK_USED(inch_in_cm, "2.54"); // Explicitly exact, so no trailing zeros.
} //  BOOST_AUTO_TEST_CASE(unc_test_exact)


  BOOST_AUTO_TEST_CASE(unc_test_12345)
  {
    uncun one234(1234.321, 1000.f); // double from double variable, but NOT exact.
    BOOST_CHECK(!(one234.types() & VALUE_INTEGER)); // Check NOT integer from variable.
    BOOST_CHECK(!(one234.types() & VALUE_EXACT)); // Check is NOT flagged as exact.
    CHECK_USED(one234, "1200."); // Rounded to zero before decimal point shows uncertainty.)
    one234.std_dev(100.f);
    CHECK_USED(one234, "1230."); // No trailing zeros show implicit uncertainty.
    one234.std_dev(10.f);
    CHECK_USED(one234, "1234."); // because from double and not exact, fractional decimals show uncertainty.
    one234.std_dev(1.f);
    CHECK_USED(one234, "1234.3"); // because from double and not exact, fractional decimals show uncertainty.
    one234.std_dev(0.1f);
    CHECK_USED(one234, "1234.32"); // because from double and not exact, fractional decimals show uncertainty.
    one234.std_dev(0.01f);
    CHECK_USED(one234, "1234.321"); // because from double and not exact, trailing zero shows uncertainty.

    uncun twelve34(12.34, 0.10f); // double from double variable, but NOT exact.
    BOOST_CHECK(!(twelve34.types() & VALUE_INTEGER)); // Check NOT integer from variable.
    BOOST_CHECK(!(twelve34.types() & VALUE_EXACT)); // Check is flagged NOT exact.
    CHECK_USED(twelve34, "12.34"); // because from double and not exact, trailing zeros show uncertainty.
    twelve34.std_dev(0.01f);
    CHECK_USED(twelve34, "12.340"); // trailing zeros show uncertainty.
    twelve34.std_dev(0.001f);
    CHECK_USED(twelve34, "12.3400"); // trailing zeros show uncertainty.
    twelve34.std_dev(0.0001f);
    CHECK_USED(twelve34, "12.34000"); // trailing zeros show uncertainty.
    twelve34.std_dev(0.00001f);
    CHECK_USED(twelve34, "12.340000"); // trailing zeros show uncertainty.
    // Now increase std above the step 1.9999

    twelve34.std_dev(1.f);
    CHECK_USED(twelve34, "12.3"); //
    CHECK_USED(addnoisyDigit << twelve34, "12.34"); // 1 more sig digit because requested.

    twelve34.std_dev(2.f);
    CHECK_USED(twelve34, "12."); // 1 less sig digit because over the 1.9999 stdDev step.
    CHECK_USED(addnoisyDigit << twelve34, "12.3"); // 1 more sig digit because requested.

    uncun twelve(12., 0.25f); // Implicit from input of 12. is +|- 0.5
    // so for 0.95 probability, to get std deviation halve 0.5 to 0.25.
    CHECK_USED(twelve, "12.0"); // Implicit noplusminus.
    CHECK_USED(twelve << noplusminus, "12.0"); // Explicit NO plusminus.
    CHECK_USED(plusminus << twelve, "12.0 +/-0.25"); // Explicit plusminus.

    uncun twoPoint54(2.54, 0.f); // Real and exact.
    BOOST_CHECK( (twoPoint54.types() & VALUE_EXACT)); // Check IS flagged as exact.
    BOOST_CHECK(!(twoPoint54.types() & VALUE_INTEGER)); // Check NOT flagged as integer.
    CHECK_USED(noplusminus << twoPoint54, "2.54"); // Check no added +/- .
    CHECK_USED(plusminus << twoPoint54, "2.54 +/-0"); // Check do get added +/-

    uncun onepoint2(1.2);
    CHECK_USED(onepoint2, "1.2");  // Exact.
    onepoint2.std_dev(1.f);
    CHECK_USED(onepoint2, "1.2");
    onepoint2.std_dev(0.1f);
    CHECK_USED(onepoint2, "1.20");
    onepoint2.std_dev(0.01f);
    CHECK_USED(onepoint2, "1.200");
    onepoint2.std_dev(0.001f);
    CHECK_USED(onepoint2, "1.2000");
    onepoint2.std_dev(0.005f);
    CHECK_USED(onepoint2, "1.200");
} //  BOOST_AUTO_TEST_CASE(unc_test_12345)

BOOST_AUTO_TEST_CASE(unc_test_input)
{
  // zero integer rational uncKnown noPlus noMinus at end of read.
  uncun r1;
  // std::cout << "default uncertain constructed types are : "; outUncTypes(r1.types(), std::cout); std::cout << std::endl;
  // default uncertain constructed types are : uncTypes (0x60e7) zero integer rational uncKnown noPlus noMinus df_exact df_known.
  istringstream isr1("0"); // Read from integer.
  isr1 >> r1;
  BOOST_CHECK_EQUAL(r1.value(), 0.);
  BOOST_CHECK_CLOSE_FRACTION(r1.value(), 0., std::numeric_limits<double>::epsilon());
  BOOST_CHECK_EQUAL(r1.std_dev(), 0.f);
  BOOST_CHECK_EQUAL(r1.deg_free(), 0);
  BOOST_CHECK_EQUAL(r1.types(),
    (VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL | UNC_KNOWN | VALUE_EXACT | UNC_NOPLUS | UNC_NOMINUS));

  // Read from real zero 0. with implicit +/- 0.5 == std deviation 0.5.
  CHECK_IN("0.", 0., 0.5f, 0,  (VALUE_ZERO | UNC_KNOWN | UNC_QUAN_DECIMAL) );
  // Read from real unit 1. with implicit +/- 0.5 == std deviation 0.5.
  CHECK_IN("1.", 1., 0.5f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL));
  // Read from real . with implicit +/- 0.5 == std deviation 0.5.
  CHECK_IN("12.", 12., 0.5f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL));
  // Read from real with implicit +/- 0.5 == std deviation 0.5.
  CHECK_IN("123.", 123., 0.5f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL));
  // Read from real with implicit +/- 0.05 == std deviation 0.05.
  CHECK_IN("0.1", 0.1, 0.05f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL));
  // Read from real with implicit +/- 0.05 == std deviation 0.05.
  CHECK_IN("0.12", 0.12, 0.005f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL));
  // Read from real with implicit +/- 0.05 == std deviation 0.05.
  CHECK_IN("0.01", 0.01, 0.005f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL));
  // Read from real with implicit +/- 0.05 == std deviation 0.005.
  CHECK_IN("0.001", 0.001, 0.0005f, 0,  (UNC_KNOWN | UNC_QUAN_DECIMAL));
  // Read from real with implicit +/- 0.05 == std deviation 0.005.

  // Read from real with explicit +/-.
  CHECK_IN("12. +/-0.1", 12., 0.1f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  // Read from real with -/+ instead of +/-.
  CHECK_IN("12. +/-0.1", 12., 0.1f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  // Read real zero, but +/1.
  CHECK_IN("0. +/-0.1", 0., 0.1f, 0, (VALUE_ZERO | UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  // Read real zero, but -/+.
  CHECK_IN("0. -/+0.1", 0., 0.1f, 0, (VALUE_ZERO | UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));

  // Read from real zero, plus only.
  CHECK_IN("0. +0.01", 0.,  0.01f, 0, (VALUE_ZERO | UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT | UNC_NOMINUS));
  // Read from real, minus only.
  CHECK_IN("0. -0.2", 0., 0.2f, 0, (VALUE_ZERO | UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT | UNC_NOPLUS));

  CHECK_IN("12. +|-0.1 (19)", 12., 0.1f, 19, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT | DEG_FREE_KNOWN | DEG_FREE_EXACT));
  CHECK_IN("12. +\\-0.1 (19)", 12., 0.1f, 19, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT | DEG_FREE_KNOWN | DEG_FREE_EXACT));
  CHECK_IN("12. +/+0.1 (19)", 12., 0.1f, 19, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT | DEG_FREE_KNOWN | DEG_FREE_EXACT | UNC_NOMINUS ));
  CHECK_IN("12. -/-0.1 (19)", 12., 0.1f, 19, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT | DEG_FREE_KNOWN | DEG_FREE_EXACT | UNC_NOPLUS));
  // Extra layout chars.
  CHECK_IN("12.   +/-0.1", 12., 0.1f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  CHECK_IN("12.+/-  0.1", 12., 0.1f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));

  CHECK_IN("12.  +/-  0.1", 12., 0.1f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));

  // Integer (and hence rational too)
  CHECK_IN("12 +/-0.1", 12., 0.1f, 0, (VALUE_INTEGER | VALUE_RATIONAL | UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT ));
  // Rational - not implemented yet.
  //CHECK_IN("2/3", 2/3, 0.1f, 0, (VALUE_RATIONAL | UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT ));
  //CHECK_IN("2/3 +/-0.1", 2/3, 0.1f, 0, (VALUE_RATIONAL | UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT ));

  //{
  //  uncun r123;
  //  istringstream isr123("2/3"); // Read from string.
  //  isr123 >> r123;
  //  std::cout << "isr123(\"2 / 3\") is " << r123.value() << ' ' << r123.std_dev() << ' ' << r123.degFree() << r123.types() << ", \n" ;
  //  outUncTypes(r123.types(), std::cout);
  //  std::cout << "\n"<< std::endl;
  //  // isr123("2 / 3") is 0 0 0231,
  //  // uncTypes(0xe7) zero integer rational uncKnown noPlus noMinus.
  //}

  //{
  //  uncun r12302;
  //  istringstream isr12302("2/3+/-0.02f"); // Read from string.
  //  isr12302 >> r12302;
  //  std::cout << "isr12302(\"2/3 + / -0.02f\") is " << r12302.value() << ' ' << r12302.std_dev() << ' ' << r12302.degFree() << r12302.types() << ", " ;
  //  // std::cout << "\n";
  //  outUncTypes(r12302.types(), std::cout);
  //  std::cout << std::endl;
  //  // isr12302("2/3 + / -0.02f") is 0 0.02 017702, uncTypes (0x4526) integer rational uncKnown quantize10 explicit df_known.
  //}

  // Exponent value and uncertainty.
  CHECK_IN("12.e1 +/-0.1", 12., 0.1f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  CHECK_IN("12. +/-0.1e-1", 12., 0.01f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  CHECK_IN("12.e1 +/-0.1e-1", 12., 0.01f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));

  // #define  CHECK_OUT_IN(manips, result, value, sd, df, types)
} //  BOOST_AUTO_TEST_CASE(unc_test_input)

BOOST_AUTO_TEST_CASE(unc_test_unity2)
{
  uncun one(1.); // Exact one from double.
  // Check get and set functions.
  BOOST_CHECK_EQUAL(one.value(), 1.); // value
  BOOST_CHECK_EQUAL(one.std_dev(), 0.f); // std_dev
  BOOST_CHECK_EQUAL(one.deg_free(), 0); // deg_free 0 because one 'observation'.
  BOOST_CHECK(!(one.types() & VALUE_ZERO));  // Check not zero.
  BOOST_CHECK(!(one.types() & VALUE_INTEGER)); // Check NOT recognised as integer.
  // Because constructed from double should not be integer.
  CHECK_USED(one, "1."); // Sd = 0 so is exact, but not integer.
  // CHECK_USED(std::showpos << one, "+1."); // Show + sign.  TODO not passing on the showpos requirement?
  CHECK_USED(plusminus << one, "1. +/-0"); // Do show +/-0 (and .) for NOT integer value.

  one.setUncTypes(VALUE_INTEGER); // Set the integer flag.
  BOOST_CHECK(one.types() & VALUE_INTEGER); // Check IS recognised as integer.
  CHECK_USED(one, "1"); // Implicit so no . wanted.
  CHECK_USED(noshowpoint << one, "1"); // Explicit no . wanted.
  //CHECK_USED(showpoint << one, "1."); //  Explicitly want "1." showpoint is ignored for integers.
  CHECK_USED(plusminus << one, "1"); // Don't show +/-0 for _integer_ value, despite plusminus.
  uncun realOne(1.); // is implicitly Exact.
// Construct from doubles: 1., unc 0.0, df 0 uncTypes (0x6420) uncKnown explicit df_exact df_known
//  Constructed from doubles: m_value 1., m_unc 0.0, m_df 0,  uncTypes (0x64e0) uncKnown noPlus noMinus explicit df_exact df_known
  BOOST_CHECK(!(realOne.types() & VALUE_INTEGER)); // Check NOT recognised as integer.
  CHECK_USED(plusminus << realOne, "1. +/-0"); // explicit plusminus, but NOT integer, so show +/-
  realOne.std_dev(0.1f); // Change to inexact
  BOOST_CHECK_EQUAL(realOne.std_dev(), 0.1f); // sd = 0.1.
  BOOST_CHECK(!(realOne.types() & VALUE_EXACT)); // Check NOT now recognised as exact because sd no longer zero.
  CHECK_USED(plusminus << realOne, "1.00 +/-0.10");
  // Output sd 0.20, stdDevsigDigits 2
  realOne.std_dev(0.2f);
  CHECK_USED(plusminus << realOne, "1.0 +/-0.20");
  realOne.std_dev(0.3f);
  CHECK_USED(plusminus << realOne, "1.0 +/-0.30");
  realOne.std_dev(0.01f);
  CHECK_USED(plusminus << realOne, "1.000 +/-0.010");
  realOne.std_dev(0.0101f);
  CHECK_USED(plusminus << realOne, "1.000 +/-0.010");

  realOne.std_dev(0.05f);
  CHECK_USED(plusminus << realOne, "1.00 +/-0.050");
  realOne.std_dev(0.001f);
  CHECK_USED(plusminus << realOne, "1.0000 +/-0.0010");
  realOne.std_dev(0.075f);
  CHECK_USED(plusminus << realOne, "1.00 +/-0.075"); // Note rounding because df = 1
  BOOST_CHECK_EQUAL(realOne.deg_free(), 0); // deg_free 0 because one observation.
  realOne.deg_free(11); // Increase degfree.
  BOOST_CHECK_EQUAL(realOne.deg_free(), 11); // Check has increased.
  CHECK_USED(plusminus << realOne, "1.00 +/-0.075"); // Note NO rounding because df = 10
  realOne.deg_free(101); // and more
  BOOST_CHECK_EQUAL(realOne.deg_free(), 101); //
  CHECK_USED(plusminus << realOne, "1.00 +/-0.075"); // Note NO rounding because df = 100
  realOne.deg_free(1001);
  BOOST_CHECK_EQUAL(realOne.deg_free(), 1001); //
  CHECK_USED(plusminus << realOne, "1.00 +/-0.075"); // Note NO rounding because df = 1000
} //  BOOST_AUTO_TEST_CASE(unc_test_unity2)

BOOST_AUTO_TEST_CASE(unc_test_123)
{
  uncun onetwothree(1.23); // Exact from double,
  BOOST_CHECK_EQUAL(onetwothree.value(), 1.23);
  BOOST_CHECK_EQUAL(onetwothree.std_dev(), 0.f);
  BOOST_CHECK_EQUAL(onetwothree.deg_free(), 0);
  BOOST_CHECK(!(onetwothree.types() & VALUE_ZERO));
  BOOST_CHECK(!(onetwothree.types() & VALUE_INTEGER));
  CHECK_USED(onetwothree, "1.23");
  // CHECK_USED(std::showpos << onetwothree, "+1.23"); // Check std::showpos works.  TODO decide if it really show show?
  //CHECK_USED(setw(10) << std::showpos << onetwothree, "+1.23     "); // TODO fails.

  uncun minusone(-1.); // Exact minus one from double.
  CHECK_USED(minusone, "-1.");
  uncun minusonetwothree(-1.23); // Exact negative from double,
  CHECK_USED(minusonetwothree, "-1.23");

  uncun one2345(1.2345, 0.01f, 2); // Inexact from double, sd 0.01, df 2 values.
  BOOST_CHECK_EQUAL(one2345.value(), 1.2345); // Check contructor.
  BOOST_CHECK_EQUAL(one2345.std_dev(), 0.01f);
  BOOST_CHECK_EQUAL(one2345.deg_free(), 2); // Now 2 means 2 values.
  BOOST_CHECK(!(one2345.types() & VALUE_ZERO));
  BOOST_CHECK(!(one2345.types() & VALUE_INTEGER));
  CHECK_USED(one2345, "1.235"); // Round-up 4 sigDigits because sd 0.01
  // Check that > 10 values means that more significant digits are used.
  one2345.deg_free(10); // 0 is one value, so 10 means 11 values.
  one2345.std_dev(0.01234f); // Check rounding of std deviation.
  BOOST_CHECK_EQUAL(one2345.std_dev(), 0.01234f);
  BOOST_CHECK_EQUAL(one2345.deg_free(), 10); // 0 is one value, so 10 means 11 values.
  CHECK_USED(plusminus << one2345, "1.235 +/-0.012");  // Not +/-0.0123 despite sd < 0.02
  one2345.deg_free(10); // 1 value.
} //   BOOST_AUTO_TEST_CASE(unc_test_123)

BOOST_AUTO_TEST_CASE(unc_test_nines)
{

using boost::math::nextafter;
  uncun nine95B(boost::math::nextafter(9.95, (std::numeric_limits<double>::min)()), 2.f); // Inexact from double, sd 0.01, df 2 = 3 values.
  uncun nine95A(boost::math::nextafter(9.95, (std::numeric_limits<double>::max)()), 2.f); // Inexact from double, sd 0.01, df 2 = 3 values.
  uncun nine95(9.95, 2.f); // Inexact from double, sd 0.01, df 2 = 3 values.
  uncun nine94(9.94, 2.f); // Inexact from double, sd 0.01, df 2 = 3 values.
  uncun nine96(9.96, 2.f); // Inexact from double, sd 0.01, df 2 = 3 values.
}

BOOST_AUTO_TEST_CASE(unc_test_rounding)
{
  // Uncertain values.
   uncun one2345(1.2345, 0.01f, 2); // Inexact from double, sd 0.01, df 2 = 3 values.
    one2345.std_dev(0.0049f);
    CHECK_USED(plusminus << one2345, "1.235 +/-0.0049"); // 3 significant value, but only 1 sd digits.
    one2345.std_dev(0.005f);
    CHECK_USED(plusminus << one2345, "1.235 +/-0.0050"); //
    one2345.std_dev(0.009f);
    CHECK_USED(plusminus << one2345, "1.235 +/-0.0090"); //
    one2345.std_dev(0.01f);
    CHECK_USED(plusminus << one2345, "1.235 +/-0.010"); //
    one2345.std_dev(0.011f);
    CHECK_USED(plusminus << one2345, "1.235 +/-0.011"); //
    one2345.std_dev(0.02f);
    CHECK_USED(plusminus << one2345, "1.23 +/-0.020");  // Round up to 1.24, 2 significant value digits.
    one2345.std_dev(0.05f);
    CHECK_USED(plusminus << one2345, "1.23 +/-0.050");  // 1 significant sd digit.
    one2345.std_dev(0.06f);
    CHECK_USED(plusminus << one2345, "1.23 +/-0.060");  //
    one2345.std_dev(0.07f);
    CHECK_USED(plusminus << one2345, "1.23 +/-0.070");  //
    one2345.std_dev(0.08f);
    CHECK_USED(plusminus << one2345, "1.23 +/-0.080");  //
    one2345.std_dev(0.09f);
    CHECK_USED(plusminus << one2345, "1.23 +/-0.090");  //
    one2345.std_dev(0.095f);
    CHECK_USED(plusminus << one2345, "1.23 +/-0.095");  //

    one2345.std_dev(0.01f);
    // without noscientific << causes std dev shows in exp format:  1.235 +/-1.00e-002
    CHECK_USED(scientific << noplusminus << one2345, "1.235");
    CHECK_USED(scientific << plusminus << one2345, "1.235 +/-0.010");  // OK note not +/-0.010  because < 0.02.
    one2345.std_dev(0.019f);
    CHECK_USED(scientific << plusminus << one2345, "1.23 +/-0.019");  // Note still 2 significant digits.
    one2345.std_dev(0.02f);
    CHECK_USED(scientific << plusminus << one2345, "1.23 +/-0.020");  // Note step change to 1 significant digits at 1.999.
    one2345.std_dev(0.09f);
    CHECK_USED(scientific << plusminus << one2345, "1.23 +/-0.090");  // Note sd 1 significant digit.
    one2345.std_dev(0.1f);
    CHECK_USED(scientific << plusminus << one2345, "1.23 +/-0.10");  // OK note not +/-0.01  because < 0.02.
    one2345.std_dev(0.19f);
    CHECK_USED(scientific << plusminus << one2345, "1.2 +/-0.19");  // Note still 2 significant digits.
    one2345.std_dev(0.2f);
    CHECK_USED(scientific << plusminus << one2345, "1.2 +/-0.20");  // Note step change to 1 significant digits at 1.999.
    one2345.std_dev(0.9f);
    CHECK_USED(scientific << plusminus << one2345, "1.2 +/-0.90");  // Note 1 significant digit.
    CHECK_USED(scientific << plusminus << addnoisyDigit << one2345, "1.23 +/-0.90");  // Note 1 more significant digits for value & sd.
} //   BOOST_AUTO_TEST_CASE(unc_test_rounding)

BOOST_AUTO_TEST_CASE(unc_test_big)
{
  uncun bigish(1e14, 2e12f); // Not bigger than maxdigits10 limit 1 and 15 zeros, small unc.
  CHECK_USED(bigish, "100000000000000.");  //
  uncun bigish2(1.23456789e14, 2e12f); //
  CHECK_USED(bigish2, "123000000000000.");  //  Show some non-zero digits.
  uncun bigish3(1.23456789e14, 1e6f); //
  CHECK_USED(bigish3, "123456789000000.");  // Show many non-zero digits.

  // Check on too big values.
  uncun big(1e39, 2e36f); // Bigger than exponent limit.
  //std::cout << big << std::endl;
  CHECK_USED(big, "1e+39");  // Default exp format, precision 6 total decimal digits.

  uncun bigSI(1e25, 2.f); // Bigger than SI exponent limit.
  CHECK_USED(bigSI, "1e+25");  // // Default exp format, precision 6
  // Some very big integers and exacts.
  uncun million = uncun(1000000);  // Exact integer million 1000000
  CHECK_USED(million, "1000000"); // integer.
  uncun billion = uncun(1000000000);  // Exact integer billion 1000000000
  CHECK_USED(billion, "1000000000"); // integer.

  // Exact but not integer.
  uncun i10e3 = uncun(1.e3, 0.0f);  // Exact real thousand 1000,
  CHECK_USED(i10e3, "1000."); // 1000. exact, but not integer.

  uncun i10e6 = uncun(1.e6, 0.f);  // Exact real million 1000000.
  CHECK_USED(i10e6, "1000000."); // exact.

  uncun i10e6u = uncun(1.e6, 0.000001f);  // Nearly exact real million 1000000.
  CHECK_USED(i10e6u,  "1000000.0000000"); //
  uncun i10e61 = uncun(1000000.123, 0.f);  // Exact real million 1000000.123.
  CHECK_USED(i10e61, "1000000.123"); // exact.
  uncun i10e62 = uncun(1000000.123, 0.000001f);  // Nearly Exact real million 1000000
  CHECK_USED(i10e62, "1000000.1230000"); // exact.
  uncun i10e9 = uncun(1.0e9, 0.0f);  // Exact billion 1000000000.
  CHECK_USED(i10e9, "1000000000.");

  uncun i10e99 = uncun(987654321., 0.0f);  // Exact 9 billion 987654321.
  CHECK_USED(i10e99, "987654321.");

} // BOOST_AUTO_TEST_CASE(unc_test_big)

BOOST_AUTO_TEST_CASE(unc_test_manips)
{
  // Examples of scientific and unc values including setw.
  uncun one2345(1.2345, 0.01f, 2); // Inexact from double, sd 0.01, df 2 = 3 values.
  one2345.std_dev(0.01f);
  CHECK_USED(std::scientific << noplusminus << std::setw(20) << std::left << one2345,   "1.235               "); //
  CHECK_USED(std::scientific << plusminus << std::setw(20) << std::left << one2345,     "1.235 +/-0.010      "); //
  CHECK_USED(std::scientific << plusminus << std::setw(20) << std::right << one2345,    "      1.235 +/-0.010"); //
  //CHECK_USED(scientific << plusminus << setw(20) << noadjust << one2345, "      1.235 +/-0.010"); // == right adjust.
  CHECK_USED(std::scientific << plusminus << std::setw(20) << std::showpos << one2345,  "     +1.235 +/-0.010"); // == right adjust.
  CHECK_USED(std::scientific << plusminus << std::setw(20) << std::showpos
                                                << internal << one2345,  "     +1.235 +/-0.010"); // Expect same as right.

 // CHECK_USED(scientific << plusminus << setw(20) << noadjust << setfill('~') << one2345, "~~~~~~1.235 +/-0.010"); // expect same as right adjust.
  CHECK_USED(std::scientific << plusminus << std::setw(20) << std::setfill('~') << std::left << one2345,     "1.235 +/-0.010~~~~~~"); //
  CHECK_USED(std::scientific << plusminus << std::setw(20) << std::setfill('~') << std::internal << one2345, "~~~~~~1.235 +/-0.010");
  CHECK_USED(std::scientific << plusminus << std::setw(20) << std::setfill('~') << std::right << one2345,    "~~~~~~1.235 +/-0.010"); // Expect same as right.

  // Some bigger values.
  uncun ten2345(10.2345, 0.01f, 2); // Inexact from double, sd 0.01, df 2 = 3 values.
  CHECK_USED(std::scientific << noplusminus << std::setw(20) << std::left << ten2345,   "10.235              "); //

} // BOOST_AUTO_TEST_CASE(unc_test_manips)

//BOOST_AUTO_TEST_CASE(unc_test_setsigfigs)
//{ // Tests of fixing sigfigs.
//  uncun u0(0, 0);  // Exact Zero, using constructor from int zero.
//
//  fout << "setSigDigits(4) " << setSigDigits(4) << std::endl; // Check can set setsigdigits,
//  BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 4); // and read back correctly.
//
//  fout << "setUncSigDigits(1) " << setUncSigDigits(1) << std::endl; // Check can set setuncsigdigits,
//  BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 1); // and read back correctly.
//
//  CHECK(setSigDigits(4) << u0, "0"); // Not yet setsigdigits, so has no effect.
//  // setsigdigits should force to 4 digits set by setSigDigits(4).
//  CHECK(setSigDigits(4) << setsigdigits << u0, "0.0000"); // Fails!
//
//  uncun one(1., 0.001f); //
//  CHECK(setSigDigits(3) << setsigdigits << one, "1.00"); //  getting 1.
//  CHECK_USED(setSigDigits(-1) << setsigdigits << u0, "0.00000000000000000"); // Invalid sigDigits (show max_digits10).
//  CHECK_USED(setSigDigits(0) << setsigdigits << u0, "0.00000000000000000"); // Zero sigDigits (show max_digits10).
//  CHECK_USED(setSigDigits(1) << setsigdigits << u0, "0.0"); // Override normal is still exact, so no decimal point.
//  CHECK_USED(setSigDigits(2) << setsigdigits << u0, "0.00"); // Is still exact, so no decimal point.
//  CHECK_USED(setSigDigits(3) << setsigdigits << u0, "0.000"); //
//  CHECK_USED(setSigDigits(4) << setsigdigits << u0, "0.0000"); //
//  CHECK_USED(setSigDigits(1) << setsigdigits << showpoint << u0, "0.0"); // Override normal is still exact, but showpoint demands a decimal point.
//  CHECK_USED(setSigDigits(1) << setsigdigits << std::showpos << u0, "+0.0"); //  Show plus sign.
//  CHECK_USED(setSigDigits(1) << setsigdigits << left << setw(6) << u0, "0.0   "); // Trailing spaces.
//
//
//  uncun u(12.345678, 0.09876F);
//  CHECK_USED(plusminus << u, "12.35 +/-0.099");
//  CHECK_USED(setSigDigits(6) << setsigdigits << plusminus << u, "12.3457 +/-0.099");
//  CHECK_USED(setSigDigits(6) << setsigdigits << plusminus << std::showpos << u, "+12.3457 +/-0.099");
//  CHECK_USED(setUncSigDigits(6) << setuncsigdigits << plusminus << u, "12.35 +/-0.0988");
//  // Uncertainty controls value but not uncertainty digits of precision.
//
//}

BOOST_AUTO_TEST_CASE(unc_test_setscale)
{   // Tests of unc setscale and autoscale functions.
  // These may not be necessary because Boost.Units provides autoscaling via autoprefix.
  //  uncun int10000(10000); // Should not be autoscaled nor set scaled.
  //  CHECK_USED(int10000, "10000"); // NOT 10.000 k because is neither autoscaled nor set scaled.
  //  CHECK_USED(autoscale << int10000, "10000"); // NOT 10.000 k because is integer.
  //  CHECK_USED(scale   << int10000, "10000"); // NOT 10.000 k because is integer.
  //  uncun zero(0, 0.000001f);
  //  CHECK_USED(zero, "0");
  //  CHECK_USED(autoscale << zero, "0"); // zero Should not be autoscaled
  //  CHECK_USED(setScale(6) << scale << zero, "0");  // nor set scaled.

  //  uncun uScaled1(0.00123456); // Input scaled by unit prefix or symbol.
  //  uncun uScaled2(2345.678); //
  //  uncun uScaled3(1.23456);  //
  //  CHECK_USED(autoscale << addsisymbol <<  uScaled3, "1.23456 "); // Is trailing space if no symbol right?
  //  CHECK_USED(autoscale << addsisymbol <<  uScaled1, "1.23456 m");
  //  CHECK_USED(autoscale << addsisymbol <<  uScaled2 , "2.345678 k");
  //  CHECK_USED(autoscale << addsiprefix <<  uScaled1, "1.23456 milli");
  //  CHECK_USED(autoscale << addsiprefix <<  uScaled2, "2.345678 kilo");
  //  CHECK_USED(autoscale << addsiprefix <<  addsisymbol <<  uScaled2, "2.345678 kilo");
  //  // Check get prefix if specify both.
  //  CHECK_USED(noautoscale << scale << setScale(3)  <<  addsisymbol <<   uScaled2, "2.345678 k");
  //  CHECK_USED(scale << addsisymbol <<   uScaled2 , "2345.678 "); // Can't test re-use of previous set scale(3).

  //  CHECK_USED(scale   << setScale(6) <<  addsisymbol <<   uScaled2 , "0.002345678 M");
  //  CHECK_USED(scale   << setScale(4) <<  addsisymbol <<   uScaled2 , "0.2345678 ");
  //  CHECK_USED(scale   << setScale(2) <<  addsisymbol <<   uScaled2 , "23.45678 h");
  //  CHECK_USED(scale   << setScale(1) <<  addsisymbol <<   uScaled2 , "234.5678 da");
  //  CHECK_USED(scale   << setScale(0) <<  addsisymbol <<   uScaled2 , "2345.678 ");
  //  CHECK_USED(scale   << setScale(-1)   << addsisymbol <<   uScaled2 , "23456.78 d");
  //  CHECK_USED(scale   << setScale(-2)  <<  addsisymbol <<   uScaled2 , "234567.8 c");
  //  CHECK_USED(scale   << setScale(-3)  <<  addsisymbol <<   uScaled2 , "2345678 m");
  //  CHECK_USED(scale   << setScale(-4)   << addsisymbol <<   uScaled2 , "23456780 ");
  //  CHECK_USED(scale   << setScale(-6)   << addsisymbol <<   uScaled2 , "2345678000 u"); // Silly scaling!
  //  CHECK_USED(scale   << setScale(-9)   << addsisymbol <<   uScaled2 , "2345678000000 n"); // Silly scaling!
  //  CHECK_USED(scale   << setScale(-6)   << addsisymbol <<   uScaled1 , "1234.56 u"); //
  //  CHECK_USED(scale   << setScale(-3)   << addsisymbol <<   uScaled1 , "1.23456 m"); //

  //  CHECK_USED(setSigDigits(2) << setsigdigits << uScaled2 << addsisymbol, "2345.678"); // exact so didn't use setsigdigits
  //  uScaled2.std_dev(0.1f);
  //  CHECK_USED(uScaled2 << addsisymbol, "2345.68");
  //  CHECK_USED(setSigDigits(2) << setsigdigits << uScaled2 << addsisymbol, "2400.");
} //   BOOST_AUTO_TEST_CASE(unc_test_setscale)

BOOST_AUTO_TEST_CASE(unc_test_asym)
{ // Some asymmetric uncertainties.
  uncun notMore(1.71, 0.2f, 0, UNC_NOPLUS);  // Only minus.
  BOOST_CHECK((notMore.types() & UNC_NOPLUS));  //
  CHECK_USED(plusminus << notMore, "1.71 +0/-0.20");
  uncun notLess(1.81, 0.2f, 0, UNC_NOMINUS);  // Only plus.
  BOOST_CHECK((notLess.types() & UNC_NOMINUS));  //
  CHECK_USED(plusminus << notLess, "1.81 -0/+0.20");
}

BOOST_AUTO_TEST_CASE(unc_test_set_manips)
{ // Test all the uncertain one parameter manips to set unc width, scale, sigdigits & unc sigdigits.
  fout << "\nTest one parameter manips to set unc width, scale, sigdigits & unc sigdigits.\n";
  setUncDefaults(fout); // Resets stream's format flags to default.
  fout << resetiosflags(ios_base::basefield | ios_base::adjustfield | ios_base::floatfield
  | ios_base::showpos | ios_base::showpoint | ios_base::uppercase | ios_base::showbase )
  << std::endl;
  setiosDefaults(fout); // &
  setUncDefaults(fout);

  BOOST_CHECK_EQUAL(fout.flags(), ios_base::skipws | ios_base::dec);
  BOOST_CHECK_EQUAL(fout.iword(oldUncSigDigitsIndex), -1);
  BOOST_CHECK_EQUAL(fout.iword(sigDigitsIndex), 3);  // default
  BOOST_CHECK_EQUAL(fout.iword(scaleIndex), 0);
  BOOST_CHECK_EQUAL(fout.iword(widthIndex), -1);
  BOOST_CHECK_EQUAL(fout.iword(oldScaleIndex),-1);
  BOOST_CHECK_EQUAL(fout.iword(setScaleIndex), 0);
  BOOST_CHECK_EQUAL(fout.iword(scaleIndex), 0);

  //fout << "setSigDigits(4) " << setSigDigits(4) << std::endl;
  //BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 4);

  //fout << setSigDigits(2) << std::endl;
  //BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 2);
  //BOOST_CHECK_EQUAL(fout.iword(oldSigDigitsIndex), 4); // Check save.

  //BOOST_CHECK_EQUAL(fout.iword(oldUncSigDigitsIndex),-1); // Default means not set yet.
  //fout << setUncSigDigits(5) << std::endl; // Bigger that makes sense.
  //BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 3); // Reduces to max sensible.
  //fout << setUncSigDigits(2) << std::endl; // ISO default.
  //BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 2); // Is default.
  //BOOST_CHECK_EQUAL(fout.iword(oldUncSigDigitsIndex), 3); // Previous set to 3.

  //fout << "setUncWidth(10) " << setUncWidth(20) << flush;
  //BOOST_CHECK_EQUAL(fout.iword(uncWidthIndex), 20);

  //fout << "setScale(-3) " << setScale(-3) << flush;
  //BOOST_CHECK_EQUAL(fout.iword(setScaleIndex), -3);

  //fout << "setsigdigits(5) " << setSigDigits(5) << flush;
  //BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 5);

  //fout << "setuncsigdigits(1) " << setUncSigDigits(1) << flush;
  //BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 1);


} // BOOST_AUTO_TEST_CASE(unc_test_set_manips)

BOOST_AUTO_TEST_CASE(unc_test_more_naninf)
{ //  "\nNative method of display of floating point infinity & Not a Number\n";
//#ifdef BOOST_MSVC
  //CHECK(numeric_limits<double>::infinity(), "1.#INF");
  //CHECK(-numeric_limits<double>::infinity(), "-1.#INF");
  //CHECK(numeric_limits<float>::quiet_NaN() , "1.#QNAN");
  //CHECK(-numeric_limits<float>::quiet_NaN() , "-1.#IND");
//#else if (BOOST_CLANG || BOOST_GCC)
  CHECK(numeric_limits<double>::infinity(), "inf");
  CHECK(-numeric_limits<double>::infinity(), "-inf");
  CHECK(numeric_limits<float>::quiet_NaN(), "nan");

 // CHECK(-numeric_limits<float>::quiet_NaN(), "-nan(ind)");  // nan for gcc.
//#endif

  // Uncertain type Infinite value - known exactly.
  uncun infinite(numeric_limits<double>::infinity(), 0.f);  // Exact plus infinity.
  CHECK_USED(infinite, "inf");
  CHECK_USED(plusminus << infinite, "inf +/-0");
  CHECK_USED(std::showpos << infinite, "+inf");
  CHECK_USED(std::showpos << plusminus << infinite, "+inf +/-0");

  uncun infiniteUK(numeric_limits<double>::infinity(), 0.1f);  // unknown plus infinity.
  CHECK_USED(plusminus << infiniteUK, "inf +/-0.10");
  CHECK_USED(std::showpos << plusminus << infiniteUK, "+inf +/-0.10");

  uncun exactNaN(numeric_limits<double>::quiet_NaN(), 0.f); // Exact NaN
  CHECK_USED(exactNaN, "NaN");
  CHECK_USED(plusminus << exactNaN, "NaN +/-0");

  // NaN with known uncertainty.
  uncun NaNknown(numeric_limits<double>::quiet_NaN(), 0.01f);
  CHECK_USED(NaNknown, "NaN");
  CHECK_USED(plusminus << NaNknown, "NaN +/-0.010");
  uncun NaNknown2(numeric_limits<double>::quiet_NaN(), 0.1f);
  CHECK_USED(plusminus << NaNknown2, "NaN +/-0.10");

  uncun undefined_uncertain = uncun(numeric_limits<double>::quiet_NaN(), 0.1f);
  CHECK_USED(plusminus << undefined_uncertain, "NaN +/-0.10");

  uncun NaNUnknown(numeric_limits<double>::quiet_NaN(), numeric_limits<float>::quiet_NaN()); // NaN of NaN uncertainty.
  CHECK_USED(plusminus << NaNUnknown, "NaN +/-?");

  // Values with unknown uncertainty.
  // Try to treat stdDev==NaN as a special case,
  // but get different results for zero and other values.
  // max_digits10 used for zero (for which a case could be made),
  // default precision 6 for others.
  // Unable to see why at present, but other cases may be flawed too.

} // BOOST_AUTO_TEST_CASE(unc_test_naninf)

BOOST_AUTO_TEST_CASE(unc_test_unc_Nan_inf)
{
  uncun zeroMaybe(0., numeric_limits<float>::quiet_NaN() );
  //CHECK_USED(zeroMaybe, "0.0000000000000000");  //  Show std::max_digits10 because uncertainty is NaN.
  //CHECK_USED(plusminus << zeroMaybe, "0.0000000000000000 +/-?");

  uncun postwoMaybe(+2., numeric_limits<float>::quiet_NaN());  // +1.234 with unknown uncertainty.
  //std::cout << postwoMaybe << std::endl;
  uncun posonetwoMaybe(+1.234, numeric_limits<float>::quiet_NaN());  // +1.234 with unknown uncertainty.
  //std::cout << posonetwoMaybe << std::endl;
  CHECK_USED(posonetwoMaybe, "1.2340000000000000");

  uncun negtwoMaybe(-2.2, numeric_limits<float>::quiet_NaN());  // -2.2 with unknown uncertainty.
  CHECK_USED(negtwoMaybe, "-2.2000000000000002");
  CHECK_USED(plusminus << negtwoMaybe, "-2.2000000000000002 +/-?")
  // Fails here on release version.
  uncun twoMaybe(+2.2, numeric_limits<float>::quiet_NaN());  // +2.2 with unknown uncertainty.
  CHECK_USED(twoMaybe, "2.2000000000000002");
  CHECK_USED(plusminus << twoMaybe, "2.2000000000000002 +/-?")
  uncun infUnknown(9.8, numeric_limits<float>::infinity() );  // Value with infinite uncertainty.
  CHECK_USED(infUnknown, "9.8000000000000007");
  CHECK_USED(plusminus << infUnknown, "9.8000000000000007 +/-inf");
} // BOOST_AUTO_TEST_CASE(unc_test_unc_Nan_inf)

// Output of Confidence limits/interval tests.
BOOST_AUTO_TEST_CASE(unc_test_ci_outputs)
{
  uncun ud1(1.23, 0.56F, 7); // Uncertain type.

  CHECK(ud1, "1.2"); //
  CHECK(plusminus << ud1, "1.2 +/-0.56"); // From "plusminus << ud1" Got  expected 1.2 +/-0.56 == 1.2 +/-0.56 : true
  CHECK(addlimits << ud1, "1.2 <0.82, 1.64>"); // From "addlimits << ud1" Got  expected 1.2 <0.82, 1.64> == 1.2 <0.82, 1.64> : true
  CHECK(plusminus << addlimits << ud1, "1.2 +/-0.56 <0.82, 1.64>");
  CHECK(addlimits << plusminus << ud1, "1.2 +/-0.56 <0.82, 1.64>");
  CHECK(scientific << plusminus << addlimits << adddegfree << ud1, "1.2 +/-0.56 <0.82, 1.64> (7)");
  CHECK(plusminus << addlimits << adddegfree << ud1, "1.2 +/-0.56 <0.82, 1.64> (7)");
  CHECK(addlimits << ud1, "1.2 <0.82, 1.64>");
  CHECK(plusminus << ud1, "1.2 +/-0.56");
} // BOOST_AUTO_TEST_CASE(unc_test_ci_outputs)


BOOST_AUTO_TEST_CASE(unc_test_setsegdigits)
{
  uncun zeroMaybe(0., numeric_limits<float>::quiet_NaN() );
  //CHECK_USED(setSigDigits(7) << setsigdigits << zeroMaybe, "0.0000000"); // TODO
// setSigDigits must override all other options (and also set stddev sigfigs too).
} // BOOST_AUTO_TEST_CASE(unc_test_setsegdigits)

BOOST_AUTO_TEST_CASE(unc_test_coda)
{
  BOOST_TEST_MESSAGE("Uncertain Class tests log end. " << __FILE__ << ' ' <<  __TIMESTAMP__ );

  //dout << "\n""Unc Diagnostics log end. " << std::endl; // \x0F1 = +- on screen, but ~n in files!

  //std::cerr.rdbuf(dout.rdbuf()); // Switch diagnostic log back to cerr before close!
  //// cerr = fout; in effect.
  //dout.close(); // Diagnostics.
  //std::cerr << "\n\nClosed diagnostics file " << diagFilename << ' '<< __TIMESTAMP__ << std::endl;

  fout << "\nClosed test output "<< __FILE__ << ' ' <<  __TIMESTAMP__ << std::endl;
  fout.close(); // Test output.
  //lout << logFilename << " end." << std::endl;

  //unit_test_log.set_stream(std::cout); // Switch back to std::cout,
  //lout.close();  // before close in case there is more log output,
  // which is almost certain, else ends unhappily!

 // std::cout << "\nClosed fout "<< __FILE__ << ' ' <<  __TIMESTAMP__ << std::endl;
//  fin.close();
//  std::cout << "\nClosed fin "<< __FILE__ << ' ' <<  __TIMESTAMP__ << std::endl;
  std::cerr << std::endl;  // Needed to avoid crash right at end.
}  // BOOST_AUTO_TEST_CASE(unc_test_coda)

