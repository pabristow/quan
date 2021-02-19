/*!
  \file
  \brief Testing uncertain classes input functions using Boost Test Tool.
  \details Class for simple Propagation of Uncertainties
     according to a pure Gaussian model.
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

// These test cases are automatically registered by using BOOST_AUTO_TEST_CASE.
BOOST_AUTO_TEST_CASE(unc_test_basic)
{ // Uncertain Class tests.
 // boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages);

  std::string message = "unc_test_basic";

  BOOST_TEST_MESSAGE(message);

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

  //BOOST_CHECK(fout.is_open());
  //// Test output to file ...
  //fout << "Test Output from " << __FILE__ << " " << __TIMESTAMP__"\n" << std::endl;
  //std::cout << "\x0F1 Uncertain Class Test output to " << outFilename << ' '
  //  << __FILE__ << ' ' <<  __TIMESTAMP__ << std::endl;
  // +- symbol on screen std::cout = dec 177, hex F1 but shows ~n in files?
  // BUT is messy because in file codeset +- symbol \x0B1 176! octal \361

  // Test diagnostic output to file ...
  ////BOOST_CHECK(lout.is_open());
  ////lout << "Unc " << logFilename << " opened." << std::endl;

  //BOOST_CHECK(dout.is_open());
  //dout << "Unc Diagnostics logged to " << diagFilename << " from " << __FILE__ << " " << __TIMESTAMP__".\n"<< std::endl;
 // std::cout << "Unc Diagnostics logged to " << diagFilename  << std::endl; // \x0F1 on screen but ~n in files.
//  std::cerr.rdbuf(dout.rdbuf()); // cerr = dout;  // Switch cerr to diagnostic log.
  // dout << "Diagnostic cerr from " << __FILE__ << " " << __TIMESTAMP__"\n" << std::endl;
//  std::cerr << "\x0B1 \x0B5 Diagnostic cerr from " << __FILE__ << " " << __TIMESTAMP__".\n" << std::endl;
  // Greek mu is \x0B5 for files, degree symbol is \x0B0
  // BOOST_CHECK(fin.is_open());  // No input yet?

//  BOOST_TEST_MESSAGE("Uncertain Class tests log. " << "                                     Expected   Was    Expected");

  // std::cout.fill('0'); // to get trailing zeros.
  // std::cout << std::fixed << std::setprecision(17) << 12.34 << std::defaultfloat << std::endl; // 12.34000000000000000
  // gives 17 digits (2 digits plus 15 trailing zeros AFTER decimal point).

  //________________________________________________________________________________________________________________________________

    // Some floating point comparison examples, with two tolerances, just wide enough, and too tight.
    double fp1 = 1.;
    double fp2 = 1.00001;
    BOOST_CHECK_CLOSE(fp1, fp2, 0.00100001); // Wide tolerance info: test fp1 ~= fp2 passed.
    // difference between fp1{1} and fp2{1.00001} exceeds 0.001% (0.00001 * 100 = 0.001%)
    // Note that BOOST_CHECK_CLOSE(fp1, fp2, 0.001); // just fails, but 0.001000001 passes.

    // BOOST_CHECK_CLOSE(fp1, fp2, 0.0000000001); // Tight tolerance so should fail [1 !~= 1.00001 (+/-1e-010)].
    // error in "call_test_main": difference between fp1{1} and fp2{1.00001} exceeds 1e-010%

    //std::stringstream log;
    //std::ostringstream oss;
    //setiosDefaults(oss);
    //outFmtFlags(oss.flags(), log, ".\n");
    //std::cout << log.str() << std::endl;


    ////outUncFlags(oss );
    //BOOST_CHECK_EQUAL(oss.rdstate(), ios_base::goodbit);
   
    {
      // 
     std::stringstream log;
     std::stringstream ossu;
      setUncDefaults(ossu);
      outUncValues(ossu, log); // Output all the uncnames and uncvalues to log.
     // std::cout << log.str() << std::endl;
      std::string s = log.str();
      BOOST_CHECK_EQUAL(log.str(), s);
      //std::cout << log.str()[0];
      std::string uncflagwords =
        "UncValues: uncFlags 0, setSigDigits 3, uncWidth 10, uncSetWidth -1, uncScale 0, uncSetScale 0, uncUsed 0, uncOldFlags 0, uncOldUncWidth -1, uncOldScale -1, uncSigDigits 2, uncoldSigDigits -1, oldUncUsed -1, oldStdDevSigDigits -1, setUncSigDigits 2, roundingLossIndex 0.05, confidenceIndex 0.05.\n";
      // Before call of setUncDefaults(ossu);
      // "UncValues: uncFlags 0, setSigDigits 0, uncWidth 0, uncSetWidth 0, uncScale 0, uncSetScale 0, uncUsed 0, uncOldFlags 0, uncOldUncWidth 0, uncOldScale 0, uncSigDigits 0, uncoldSigDigits 0, oldUncUsed 0, oldStdDevSigDigits 0, setUncSigDigits 0, roundingLossIndex 0, confidenceIndex 0.\n";

      BOOST_CHECK_EQUAL(log.str(), uncflagwords);
      BOOST_CHECK_EQUAL(log.str().compare(uncflagwords), 0);
    }
  } // BOOST_AUTO_TEST_CASE(unc_test_basic)


BOOST_AUTO_TEST_CASE(quan1_test_1)
{ // Some examples of testing uncertain class.
  {
    ostringstream oss;
    setUncDefaults(oss);
    uncun u0(0., 0.F);
    oss << u0;
    BOOST_CHECK_EQUAL(oss.str(), "0");
  }
  {
    ostringstream oss;
    setUncDefaults(oss);
    uncun u2(2., 0.F);
    oss << u2;
    BOOST_CHECK_EQUAL(oss.str(), "2.");
  }
  {
    ostringstream oss;
    setUncDefaults(oss);
    uncun u123(1.23, 0.01F);
    oss << plusminus << u123;
    BOOST_CHECK_EQUAL(oss.str(), "1.230 +/-0.010");
  }
} // BOOST_AUTO_TEST_CASE(quan1_test_1)


BOOST_AUTO_TEST_CASE(quan1_test_2)
{ // Tests on display of uncertain flags and uncertain types.

  unsigned short int uncFlags = plusMinus | noisyDigit;
  //cout << showUncFlags(uncFlags) << endl;
  CHECK(showUncFlags(uncFlags), "uncFlags (48) add_+/- add_noisy.");
  unsigned short int uncTypes = UNC_QUAN_DECIMAL | UNC_KNOWN;
  //cout << showUncTypes(uncTypes) << endl; // uncTypes: (120) uncKnown quantize10
  CHECK(showUncTypes(uncTypes), "uncTypes (0x120) uncKnown quantize10.");
  unsigned short defType = UNC_KNOWN | UNC_EXPLICIT | DEG_FREE_EXACT | DEG_FREE_KNOWN; //  0x6420
  //cout << showUncTypes(defType) << endl;
  CHECK(showUncTypes(defType), "uncTypes (0x6420) uncKnown explicit df_exact df_known.");
} // BOOST_AUTO_TEST_CASE(quan1_test_2)




/*
* 
0.05 (9)
1.1 +/-0.1 1.2 1.0 1.1+/-0.001
1.25 0.97

*/