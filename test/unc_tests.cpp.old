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

// Files made global to avoid nasty memory leak, and loop to end of memory.
const char testInputFilename[] = "unc_test_input.txt"; // Input for tests (fin).
ifstream fin(testInputFilename, ios_base::in);
const char outFilename[] = "unc_test_output.txt"; // output from tests (fout).
ofstream fout(outFilename, ios_base::out); // Use default ios_base::overwrite/replace.
// const char logFilename[] = "unc_log.txt"; // // Boost test log (usual to send to cout).
// ofstream lout(logFilename, ios_base::out); // Use default overwrite/ iso::replace.
const char diagFilename[] = "unc_diag.txt"; // diagnostic log diverted from cerr (dout).
  ofstream dout(diagFilename, ios_base::out); // Use default overwrite/ iso::replace.
//const char testLogFilename[] = "unc_test.log"; // Not used yet.

// typedef basic_string <char>::size_type size_type;
// using basic_string <char>::size_type;

// toString concept works for some purposes,
// but is flawed to combine several manipulators and it needs setUncDefaults.
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

// Integrity check on iword begin and end no longer needed in CHECK.
// BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);\
// BOOST_CHECK_EQUAL(oss.iword(0), indexID);\
// BOOST_CHECK_EQUAL(oss.iword(topIndex), indexID);\
// BOOST_CHECK_EQUAL(oss.iword(0), indexID);\

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

  string message("Round to cout test: " __FILE__ );
#ifdef __TIMESTAMP__
  message += " at " BOOST_STRINGIZE(__TIMESTAMP__);
#endif
#ifdef _MSC_FULL_VER
  message +=  ", MSVC version " BOOST_STRINGIZE(_MSC_FULL_VER) ".";
#else
  message += "."
#endif
  BOOST_MESSAGE(message);

 //   BOOST_CHECK(zeroIndex == indexID); // Should pass?


  // BOOST_CHECK(lout.is_open());
  //   unc_tests.cpp(235): info: check lout.is_open() passed
  // unit_test_log.set_stream(lout); // Switch to log file.
  // BOOST_TEST_MESSAGE(message); 

  BOOST_CHECK(numeric_limits<double>::is_iec559 == true);	// IEC559/IEEE754 floating point.

  // Change log level to record warnings & errors.
  // unit_test_log.set_log_threshold(boost::unit_test::log_successful_tests);
  unit_test_log.set_threshold_level(boost::unit_test::log_all_errors);
  //unit_test_log::instance().set_threshold_level(test_suite);
  //unit_test_log::instance().set_threshold_level(messages); // user messages
  // Prepare to send log to a file instead of cout.

  cout << "\x0F1 Uncertain Class Test output to " << outFilename << ' '
    << __FILE__ << ' ' <<  __TIMESTAMP__ << endl;
  // +- symbol on screen cout = dec 177, hex F1 but shows ~n in files?
  // BUT is messy because in file codeset +- symbol \x0B1 176!

  BOOST_CHECK(fout.is_open());
  // Test output to file ...
  fout << "Test Output from " << __FILE__ << " " << __TIMESTAMP__"\n" << endl;

  // Test diagnostic output to file ...
  //BOOST_CHECK(lout.is_open());
  //lout << "Unc " << logFilename << " opened." << endl;
  BOOST_CHECK(dout.is_open());
  dout << "Unc Diagnostics logged to " << diagFilename << " from " << __FILE__ << " " << __TIMESTAMP__"\n"<< endl;
  cout << "Unc Diagnostics logged to " << diagFilename  << endl; // \x0F1 on screen but ~n in files.
  cerr.rdbuf(dout.rdbuf());	// cerr = dout;  // Switch cerr to diagnostic log.
  // dout << "Diagnostic cerr from " << __FILE__ << " " << __TIMESTAMP__"\n" << endl;
  cerr << "\x0B1 \x0B5 Diagnostic cerr from " << __FILE__ << " " << __TIMESTAMP__"\n" << endl;
  // Greek mu is \x0B5 for files, degree symbol is \x0B0
  // BOOST_CHECK(fin.is_open());  // No input yet?

  BOOST_MESSAGE("Uncertain Class tests log. " << "                                     Expected   Was    Expected");

  // cout.fill('0'); // to get trailing zeros.
  // cout << fixed << setprecision(17) << 12.34 << automatic << endl; // 12.34000000000000000
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

  setUncDefaults(dout); // Should set the indexID values too.
  zeroid = dout.iword(zeroIndex);
  topid = dout.iword(topIndex);
  BOOST_CHECK_EQUAL(zeroid, indexID); // Check iword init at bottom OK,
  BOOST_CHECK_EQUAL(topid, indexID);// & at top too.

  setUncDefaults(std::cout);

  // Check initial ios flags, precision, fill, width.
  // Expect IOS flags: skipwhite dec, precision 6, fill ' '
  std::streamsize originalWidth = fout.width(); // Not useful to restore.
  BOOST_CHECK_EQUAL(fout.width(), 0);  // std default precision.
  std::streamsize originalPrecision = fout.precision();  // Default precision is 6.
  BOOST_CHECK_EQUAL(fout.precision(), 6);  // std default precision is 6.
  long originalFlags = fout.flags();  // hex 201 == skipwhite dec.
  BOOST_CHECK_EQUAL(fout.flags(), 0x201); 
  BOOST_CHECK_EQUAL(fout.flags(), std::ios_base::skipws | std::ios_base::dec);
  BOOST_CHECK_EQUAL(fout.fill(), ' '); // fill char is space.
  BOOST_CHECK_EQUAL(fout.flags() & ios_base::floatfield, 0);
  BOOST_CHECK_EQUAL(fout.flags() & ios_base::adjustfield, 0);
  BOOST_CHECK_EQUAL(fout.flags() & ios_base::adjustfield, (int)!(ios_base::left | ios_base::right | ios_base::internal));
  BOOST_CHECK_EQUAL(fout.flags() & ios_base::showbase, 0); // no showbase.
  BOOST_CHECK_EQUAL(fout.flags() & ios_base::boolalpha, 0); // no boolapha.

  // Basic checks on re-initialisation of ios and unc using.
  void setiosDefaults(ostream&); // &
  void setUncDefaults(ios_base&);

  ostringstream oss;
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

  std::streamsize w = cerr.width(6); // Set a width of 6.
  BOOST_CHECK_EQUAL(cerr.width(), 6);  // Confirm has been set to 6.
  cerr << endl; // Does NOT 'Use' width.
  BOOST_CHECK_EQUAL(cerr.width(), 6);  // Confirm is STILL 6.
  cerr << '\t' << endl; // (\a shows as small square) Does 'Use' width, like << "use" or << 99 
  BOOST_CHECK_EQUAL(cerr.width(), 0);// Check width has been reset to zero.
  } // BOOST_AUTO_TEST_CASE(unc_test_basic)

//#define CPP_TESTS
#ifdef CPP_TESTS
BOOST_AUTO_TEST_CASE(unc_test_stdio)
{  // Examples of C++ std integer output.
  int i1 = 1;
  CHECK(i1, "1");
  CHECK(noshowpoint << i1, "1"); // Explicit NOshowpoint
  CHECK(showpoint << i1, "1"); // Never show point if value is integer!

  int i1234 = 1234;
  CHECK(i1234, "1234");
  CHECK(noshowpoint << i1234, "1234"); // Normal 
  CHECK(showpoint << i1234, "1234"); // NOT "1234." Doesn't show point if integer!
  CHECK(showpos << i1234, "+1234"); // Shows +

  int m1 = -1; // negative variable.
  CHECK(m1, "-1"); //  negative constant. 
  CHECK(hex << m1, "ffffffff"); // 
  int m1234 = -1234;
  CHECK(m1234, "-1234"); // plain negative
  CHECK(noshowpoint << m1234, "-1234"); // Normal 
  CHECK(showpoint << m1234, "-1234"); // NOT "-1234." Doesn't show point if integer!
  CHECK(showpos << m1234, "-1234"); // Makes no difference - always show - sign.
  CHECK(noshowpos << m1234, "-1234"); // Makes no difference - always show - sign.

  int i = 15;
  CHECK(hex << showbase << setw(5) << i, "  0xf"); // 2 spaces + 3 digit chars.
  CHECK(hex << noshowbase << setw(5) << i, "    f"); // 4 spaces + 1 digit char .
  CHECK(hex << left << noshowbase << setw(5) << i, "f    "); // 1 digit char + 4 fill spaces.
  CHECK(right << dec << noshowbase << setw(5) << i, "   15"); // 1 digit char + 3 fill spaces.
  CHECK(setfill('~') << left << dec << noshowbase << setw(5) << i, "15~~~"); //  3 ~ & 1 digit char.
  CHECK(setfill('~') << right << dec << noshowbase << setw(5) << i, "~~~15"); // 1 digit char + 3 ~.

  // Check some examples of built-in double output.

  double zero = 0.;
  double one = 1.;
  double minus1 = -1.;
  double point1 = 0.1;

  CHECK(zero, "0"); // normal defaults, width = 1
  CHECK(showpos << zero, "+0"); // normal defaults, width = 1
  CHECK(setprecision(0) << zero, "0"); // NO decimal point.
  // Check use of showpos and showpoint.
  CHECK(fixed << showpos << showpoint << zero, "+0.000000"); // 1+6 zeros - no precision set, so defaults to 6.
  CHECK(showpoint << setprecision(1) << zero, "0.0"); // normal defaults, width = 7
  CHECK(showpoint << showpos << setprecision(0) << zero, "+0.000000"); // 6 zeros
  CHECK(showpoint << showpos << setprecision(1) << zero, "+0.0"); // normal defaults, width = 1
  CHECK(showpoint << showpos << setprecision(-1) << zero, "+0.000000"); // 6 zeros
  CHECK(showpoint << showpos << setprecision(6) << zero, "+0.000000"); // 6 zeros
  // So setprecision(0) means precision(6) if normal unless fixed when
  CHECK(fixed << showpoint << setfill('~') << setprecision(0) << zero, "0."); // width = 2
  CHECK(fixed << showpos << showpoint << setfill('~') << setprecision(0) << zero, "+0."); // width = 2
  CHECK(fixed << showpos << showpoint << setprecision(0) << zero, "+0."); // width = 3
  CHECK(fixed << showpos << showpoint << setprecision(1) << zero, "+0.0"); // width = 4
  CHECK(fixed << showpos << showpoint << setprecision(2) << zero, "+0.00"); // width = 5
  CHECK(fixed << showpos << showpoint << setprecision(3) << zero, "+0.000"); // width = 6

  CHECK(fixed << setprecision(0) << zero, "0"); // width = 1 
  CHECK(fixed << showpoint << setprecision(0) << zero, "0."); // width = 2
  CHECK(fixed << showpoint << setprecision(1) << zero, "0.0"); // width = 3
  CHECK(fixed << showpoint << setprecision(2) << zero, "0.00"); // width = 4
  CHECK(fixed << showpoint << setprecision(3) << zero, "0.000"); // width = 5

  CHECK(one, "1"); // normal defaults, width = 1
  CHECK(showpoint << one, "1.00000"); // Same as above, using default precision(6)
  // << showpoint means that precision is used.
  CHECK(left << showpoint << setfill('~') << setprecision(6) << one, "1.00000"); // width = 7
  CHECK(left << showpoint << setfill('~') << setprecision(1) << one, "1."); // width = 7
  CHECK(left << showpoint << setfill('~') << setprecision(0) << one, "1.00000"); // NOT "1."
  // So conclude that precision(0) means default is used.
  // Show how precision controls if width is not set & show that no fill char ~ are used):
  CHECK(left << fixed << showpoint << setprecision(4) << one, "1.0000"); // width = 6
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(6) << one, "1.000000"); // width = 8
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(5) << one, "1.00000"); // width = 7
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(4) << one, "1.0000"); // width = 6
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(3) << one, "1.000"); // width = 5
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(2) << one, "1.00"); // width = 4
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(1) << one, "1.0"); // width = 3
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(0) << one, "1."); // width = 2
  CHECK(left << showpoint << setfill('~') << setprecision(0) << one, "1.00000"); // width = 6 + 1 .

  CHECK(fixed << showpoint << setprecision(0) << minus1, "-1."); // width = 3 if value >= 1, precision(0) then 1 digit.
  CHECK(fixed << showpoint << setprecision(1) << minus1, "-1.0"); // width = 4
  CHECK(fixed << showpoint << setprecision(2) << minus1, "-1.00"); // width = 5
  CHECK(fixed << showpoint << setprecision(3) << minus1, "-1.000"); // width = 6

  CHECK(fixed << showpoint << setprecision(0) << point1, "0."); // width = 2 - else if value < 1. then NO precision!
  CHECK(fixed << showpoint << setprecision(1) << point1, "0.1"); // width = 3 
  CHECK(fixed << showpoint << setprecision(2) << point1, "0.10"); // width = 4
  CHECK(fixed << showpoint << setprecision(3) << point1, "0.100"); // width = 5

  double big = 123456.;
  CHECK(big, "123456"); // default 'normal' is not fixed, nor showpoint, nor scientific.
  // Nicholai M Josuttis, The C++ Standard Library, ISBN 0 201 37926 0, page 624
  CHECK(showpoint                    << big, "123456."); // Add single . point.
  CHECK(showpoint << setprecision(0) << big, "123456."); // Same as no setprecision
  CHECK(showpoint << setprecision(1) << big, "1.e+005"); // Cuts down to 1 decimal place and goes exp.
  CHECK(showpoint << setprecision(2) << big, "1.2e+005"); // Cuts down to 1+1 decimal place and goes exp.
  CHECK(showpoint << setprecision(3) << big, "1.23e+005"); // Cuts down to 1+2 decimal place and goes exp.
  CHECK(showpoint << setprecision(4) << big, "1.235e+005"); // Rounds to 1+3 decimal place and goes exp.
  CHECK(showpoint << setprecision(5) << big, "1.2346e+005"); // Rounds to 1+4 decimal place and goes exp.
  CHECK(showpoint << setprecision(6) << big, "123456."); // Using default 6 for setprecision(6).
  CHECK(showpoint                    << big, "123456."); // Same as using default setprecision(6).
  CHECK(showpoint << setprecision(7) << big, "123456.0"); // Precision digits (7) in integral & fractional part.
  CHECK(showpoint << setprecision(17) << big, "123456.00000000000"); // Max 64-bit double setprecision, 18 digits

  // If fixed then precision sets the number of digits in the FRACTIONAL part.
  CHECK(fixed << showpoint << setprecision(0) << big, "123456."); // 
  CHECK(fixed << showpoint << setprecision(1) << big, "123456.0"); //
  CHECK(fixed << showpoint << setprecision(2) << big, "123456.00"); //
  CHECK(fixed << showpoint << setprecision(3) << big, "123456.000"); //

  double fourDigits = 1234.;
  CHECK(fourDigits, "1234"); // default 'normal' aka 'automatic' is not fixed, nor scientific, nor showpoint, nor showpos.
  // Nicholai M Josuttis, The C++ Standard Library, ISBN 0 201 37926 0, page 624
  CHECK(showpoint                    << fourDigits, "1234.00"); // Add single . point.
  CHECK(showpoint << setprecision(0) << fourDigits, "1234.00"); // Same as no setprecision
  CHECK(showpoint << setprecision(1) << fourDigits, "1.e+003"); // Cuts down to 1 decimal place and goes exp.
  CHECK(showpoint << setprecision(2) << fourDigits, "1.2e+003"); // Cuts down to 1+1 decimal place and goes exp.
  CHECK(showpoint << setprecision(3) << fourDigits, "1.23e+003"); // Cuts down to 1+2 decimal place and goes exp.
  CHECK(showpoint << setprecision(4) << fourDigits, "1234."); // Rounds to 1+3 decimal place.
  CHECK(showpoint << setprecision(5) << fourDigits, "1234.0"); // Rounds to 1+4 decimal place.
  CHECK(showpoint << setprecision(6) << fourDigits, "1234.00"); // Using default 6 for setprecision(6).
  CHECK(showpoint                    << fourDigits, "1234.00"); // Same as using default setprecision(6).
  CHECK(showpoint << setprecision(7) << fourDigits, "1234.000"); // Precision digits (7) in integral & fractional part.
  CHECK(showpoint << setprecision(17) << fourDigits, "1234.0000000000000"); // Max 64-bit double setprecision, 18 digits
                                                  
  // If fixed then precision sets the number of digits in the FRACTIONAL part.
  CHECK(fixed << showpoint << setprecision(0) << fourDigits, "1234."); // 
  CHECK(fixed << showpoint << setprecision(1) << fourDigits, "1234.0"); //
  CHECK(fixed << showpoint << setprecision(2) << fourDigits, "1234.00"); //
  CHECK(fixed << showpoint << setprecision(3) << fourDigits, "1234.000"); //

  // Using fixed, showpoint and setprecision to show significant digits, including decimal point and trailing zeros.
  double twelve34 = 12.34;
  CHECK(twelve34, "12.34");
  CHECK(fixed << showpoint << setprecision(0) << twelve34, "12.");
  CHECK(fixed << showpoint << setprecision(1) << twelve34, "12.3");
  CHECK(fixed << showpoint << setprecision(2) << twelve34, "12.34");
  CHECK(fixed << showpoint << setprecision(3) << twelve34, "12.340"); // Adds trailing zero(s)
  CHECK(fixed << showpoint << setprecision(4) << twelve34, "12.3400");
  CHECK(fixed << showpoint << setprecision(5) << twelve34, "12.34000");

  // fill NOT used unless width is set.
  // Forcing + sign with showpos 
  CHECK(left << fixed << showpos << showpoint << setfill('0') << setprecision(0) << one, "+1."); // width = 3
  CHECK(left << fixed << showpoint << setfill('0') << setprecision(0) << minus1, "-1."); // width = 3

  // fill NOT used unless width is set using setw().
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(4) << one, "1.0000"); // no set width, 4 zeros after.
  // Using setfill and showpoint (and setw) to show trailing zeros.
  CHECK(left << fixed << showpoint << setfill('0') << setprecision(4) << setw(8) << one, "1.000000"); // 2 extra fill zeros.
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(4) << setw(8) << one, "1.0000~~"); // 2 extra fill ~.
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(4) << setw(7) << one, "1.0000~"); // 1 extra fill ~
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(4) << setw(6) << one, "1.0000"); // width has no effect.
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(4) << setw(5) << one, "1.0000"); // NO fill.
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(3) << setw(4) << one, "1.000");
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(2) << setw(3) << one, "1.00");
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(1) << setw(3) << one, "1.0");
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(1) << setw(1) << one, "1.0"); // want 1.
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(1) << setw(2)  << zero , "0.0"); // want 0.
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(1) << setw(1)  << zero , "0.0"); // 0.0
  CHECK(left << fixed << noshowpoint << setfill('~') << setprecision(1) << setw(1)  << zero , "0.0");
  CHECK(left << fixed << noshowpoint << setfill('~') << setprecision(0) << setw(1)  << zero , "0");
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(0)  << setw(1)  << zero , "0.");
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(0)  << setw(0)  << zero , "0."); // setw(0) undefined?
  CHECK(left << fixed << showpoint << setfill('~') << setprecision(-1) << one, "1.000000"); // precision < 0 means default = 6

  double d = 1.234567890;
  CHECK(setw(20) << d, "             1.23457"); // precision 6, width 20  is 13 spaces + 7 chars.
  CHECK(fixed << setw(20) << d, "            1.234568"); // width 20  is 12 spaces + 8 chars.
  CHECK(fixed << setw(20) << d, "            1.234568"); // width 20  is 13 spaces + 8 chars.
  CHECK(scientific << setw(20) << d, "       1.234568e+000"); // width 20  is 7 spaces + 13 chars.
  CHECK(scientific << d, "1.234568e+000"); // width reverts automatically to 0.
  CHECK(scientific << setw(20) << right << d, "       1.234568e+000"); // width 20  is 7 spaces + 13 chars.
  CHECK(scientific << setw(20) << left << d, "1.234568e+000       "); // width 20  is 7 spaces + 13 chars.
  CHECK(scientific << setw(20) << internal << d, "       1.234568e+000"); // width 20  is 7 spaces + 13 chars.
  CHECK(scientific << setw(20) << showpos << internal << d, "+      1.234568e+000"); // width 20  is 7 spaces + 13 chars.
  CHECK(scientific << setw(20) << showpos << internal << setfill('0') << d, "+0000001.234568e+000"); // width 20 is 6 zero fills, 0 spaces + 14 chars.
  CHECK(scientific << setw(20) << showpos << internal << setfill('*') << d, "+******1.234568e+000"); // width 20 is 6 * fills, 0 spaces + 14 chars.
  CHECK(scientific << setw(20) << showpos << d, "      +1.234568e+000"); // width 20  is 6 spaces + 14 chars.
  CHECK(scientific << setw(20) << showpos << left << d, "+1.234568e+000      "); // width 20  is 14 chars + 6 spaces.
  CHECK(scientific << setw(20) << noshowpos << setfill('~') << d, "~~~~~~~1.234568e+000"); // Combine scientific with fill
  CHECK(scientific << setw(20) << noshowpos << right << setfill('~') << d,    "~~~~~~~1.234568e+000"); // Combine scientific with fill
  CHECK(scientific << setw(20) << noshowpos << internal << setfill('~') << d, "~~~~~~~1.234568e+000"); // Combine scientific with fill
  CHECK(scientific << setw(20) << noshowpos << left << setfill('~') << d, "1.234568e+000~~~~~~~"); // Combine scientific with fill

  // Padding with fill char.
  CHECK(setw(0) << setfill('~') << i, "15"); // width(0) so no fill.
  CHECK(setw(1) << setfill('~') << i, "15"); // width(1) so no room for fill.
  CHECK(setw(2) << setfill('~') << i, "15"); // width(1) so no room for fill.
  CHECK(setw(3) << setfill('~') << i, "~15"); // width(1) so 1 fill.
  CHECK(setw(5) << setfill('~') << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(setw(5) << setfill('~') << right << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(setw(5) << setfill('~') << internal << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(setw(5) << setfill('~') << left << i, "15~~~"); // width 5 is 2 digits + 3 fill.
  CHECK(setw(5) << setfill('~') << noadjust << i, "~~~15"); // width 5 is 3 fill + 2 digits.
  CHECK(setw(5) << setfill('~') << left << i, "15~~~"); // width 5 is 2 digits + 3 fill.
  CHECK(setfill(' ') << i, "15"); // width goes back to 0 each time.

  // Extra multiple manipulators: spaces, tabs and chars.
  CHECK(spaces(5), "     ");
  CHECK(tabs(5), "\t""\t""\t""\t""\t");
  CHECK(stars(5), "*****");
  CHECK(chars(5, '~'), "~~~~~");
  CHECK(chars(5, ' '), "     ");
} // BOOST_AUTO_TEST_CASE(unc_test_stdio)
#endif // CPP_TESTS
  
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
    BOOST_CHECK_EQUAL(u0.deg_free(), 1);
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
    BOOST_CHECK_EQUAL(z.deg_free(), 1); // deg_free
    BOOST_CHECK(z.types() & VALUE_ZERO); // Is zero.
    BOOST_CHECK(z.types() & VALUE_INTEGER); // Is integer.
    CHECK_USED(z, "0"); // Exact zero.
    CHECK_USED(setw(0) << z, "0"); // Exact zero, default width.
    CHECK_USED(setw(1) << z, "0"); // Tight fit.
    CHECK_USED(setw(2) << z, " 0"); // no adjust == right justify
    CHECK_USED(left << setw(2) << z, "0 "); // Just 1 pad.
    CHECK_USED(left << setw(3) << z, "0  "); // 2 pads.
    CHECK_USED(left << setw(4) << z, "0   "); // 1 digit plus 3 pad.
    CHECK_USED(left << setw(4) << z, "0   "); // 1 digit plus 3 pad, left == default.
    CHECK_USED(right << setw(4) << z, "   0"); // right so 3 pad plus 1 digit.
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
    // so exact (but no longer integer).
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
    uncun exact(1., 1.f, 2, VALUE_EXACT);  // Exact Unity from double with sd and df.
    // This should signal a conflict in the constructor!
    BOOST_CHECK(exact.types() & VALUE_EXACT); // Should still be flagged as exact.
    BOOST_CHECK_EQUAL(exact.value(), 1.); // value
    BOOST_CHECK_EQUAL(exact.std_dev(), 0.f);//  StdDeviation should be over-ridden by VALUE_EXACT.
    // "Value 1 flagged as exact, but uncertainty 1 is not zero!"
    BOOST_CHECK_EQUAL(exact.deg_free(), 0); // deg_free should be overwritten by VALUE_EXACT.
    // "Value 1 flagged as exact, but degfree 2 is not zero!"

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

    uncun imax(numeric_limits<int>::max()); // Exact int max 2147483647.
    BOOST_CHECK(imax.types() & VALUE_INTEGER); // Check IS integer from argument.
    CHECK_USED(imax, "2147483647"); // no . because integer.

    uncun imin(numeric_limits<int>::min()); // Exact int min -2147483648.
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
    // Trailing zeros would imply an uncertainty.

    inch_in_cm.setUncTypes(inch_in_cm.types() & ~VALUE_EXACT); // Flag as NOT exact.
    BOOST_CHECK(!(inch_in_cm.types() & VALUE_EXACT)); // Check exact flag has cleared.
    CHECK_USED(inch_in_cm, "2.54000000000000"); // Max trailing zeros, because sd still == 0.F.

    inch_in_cm.std_dev(0.0012f); // Add some uncertainty.
    BOOST_CHECK(!(inch_in_cm.types() & VALUE_EXACT)); // Check still NOT exact.
    CHECK_USED(inch_in_cm, "2.540"); // One trailing zeros because std dev specified 0.012f.
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
  //outUncTypes(r1.types(), cout);
  istringstream isr1("0"); // Read from integer.
  isr1 >> r1;
  BOOST_CHECK_EQUAL(r1.value(), 0.);
  BOOST_CHECK_CLOSE_FRACTION(r1.value(), 0., std::numeric_limits<double>::epsilon());
  BOOST_CHECK_EQUAL(r1.std_dev(), 0.f);
  BOOST_CHECK_EQUAL(r1.deg_free(), 0);
  BOOST_CHECK_EQUAL(r1.types(), 
    (VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL | UNC_KNOWN | VALUE_EXACT | UNC_NOPLUS | UNC_NOMINUS));

  // Read from real zero 0. with implicit +/- 0.5 == std deviation 0.5.
  CHECK_IN("0.", 0., 0.5f, 0,  (VALUE_ZERO | UNC_KNOWN | UNC_QUAN_DECIMAL)  );
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
  
  {
    uncun r1;
    istringstream isr1("2/3"); // Read from string.
    isr1 >> r1;
    cout << r1.value() << ' ' << r1.std_dev() << ' ' << r1.degFree() << r1.types() << ", " ;
    outUncTypes(r1.types(), cout);
    cout << endl;
  }

    {
    uncun r1;
    istringstream isr1("2/3+/-0.02f"); // Read from string.
    isr1 >> r1;
    cout << r1.value() << ' ' << r1.std_dev() << ' ' << r1.degFree() << r1.types() << ", " ;
    outUncTypes(r1.types(), cout);
    cout << endl;
  }


  // Exponent value and uncertainty.
  CHECK_IN("12.e1 +/-0.1", 12., 0.1f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  CHECK_IN("12. +/-0.1e-1", 12., 0.01f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));
  CHECK_IN("12.e1 +/-0.1e-1", 12., 0.01f, 0, (UNC_KNOWN | UNC_QUAN_DECIMAL | UNC_EXPLICIT));

  /*

  */
  // #define	CHECK_OUT_IN(manips, result, value, sd, df, types)
} //  BOOST_AUTO_TEST_CASE(unc_test_input)

BOOST_AUTO_TEST_CASE(unc_test_unity2)
{
  uncun one(1.); // Exact one from double.
  // Check get and set functions.
  BOOST_CHECK_EQUAL(one.value(), 1.); // value
  BOOST_CHECK_EQUAL(one.std_dev(), 0.f); // std_dev
  BOOST_CHECK_EQUAL(one.deg_free(), 1); // deg_free
  BOOST_CHECK(!(one.types() & VALUE_ZERO));  // Check not zero.
  BOOST_CHECK(!(one.types() & VALUE_INTEGER)); // Check NOT recognised as integer.
  // Because constructed from double should not be integer.
  CHECK_USED(one, "1."); // Sd = 0 so is exact, but not integer.
  CHECK_USED(showpos << one, "+1."); // Show + sign.
  CHECK_USED(plusminus << one, "1. +/-0"); // Do show +/-0 (and .) for NOT integer value.

  one.setUncTypes(VALUE_INTEGER); // Set the integer flag.
  BOOST_CHECK(one.types() & VALUE_INTEGER); // Check IS recognised as integer.
  CHECK_USED(one, "1"); // Implicit so no . wanted. 
  CHECK_USED(noshowpoint << one, "1"); // Explicit no . wanted. 
  //CHECK_USED(showpoint << one, "1."); //  Explicitly want "1." showpoint is ignored for integers.
  CHECK_USED(plusminus << one, "1"); // Don't show +/-0 for _integer_ value, despite plusminus.
  uncun realOne(1.); // is implicitly Exact.
// Construct from doubles: 1., unc 0.0, df 0 uncTypes (0x6420) uncKnown explicit df_exact df_known 
//	Constructed from doubles: m_value 1., m_unc 0.0, m_df 0,  uncTypes (0x64e0) uncKnown noPlus noMinus explicit df_exact df_known 
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
  BOOST_CHECK_EQUAL(realOne.deg_free(), 1); // deg_free
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
  BOOST_CHECK_EQUAL(onetwothree.deg_free(), 1);
  BOOST_CHECK(!(onetwothree.types() & VALUE_ZERO));
  BOOST_CHECK(!(onetwothree.types() & VALUE_INTEGER));
  CHECK_USED(onetwothree, "1.23");
  CHECK_USED(showpos << onetwothree, "+1.23"); // Check showpos works.
  //CHECK_USED(setw(10) << showpos << onetwothree, "+1.23     "); // TODO fails.

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
  uncun nine95B(_nextafter(9.95, numeric_limits<double>::min()), 2.f); // Inexact from double, sd 0.01, df 2 = 3 values.
  uncun nine95A(_nextafter(9.95, numeric_limits<double>::max()), 2.f); // Inexact from double, sd 0.01, df 2 = 3 values.
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
  //cout << big << endl;
  CHECK_USED(big, "1e+039");  // Default exp format, precision 6 total decimal digits.

  uncun bigSI(1e25, 2.f); // Bigger than SI exponent limit.
  CHECK_USED(bigSI, "1e+025");  // // Default exp format, precision 6
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
  CHECK_USED(scientific << noplusminus << setw(20) << left << one2345,   "1.235               "); // 
  CHECK_USED(scientific << plusminus << setw(20) << left << one2345,     "1.235 +/-0.010      "); // 
  CHECK_USED(scientific << plusminus << setw(20) << right << one2345,    "      1.235 +/-0.010"); //
  CHECK_USED(scientific << plusminus << setw(20) << noadjust << one2345, "      1.235 +/-0.010"); // == right adjust.
  CHECK_USED(scientific << plusminus << setw(20) << showpos << one2345,  "     +1.235 +/-0.010"); // == right adjust.
  CHECK_USED(scientific << plusminus << setw(20) << showpos 
                                                << internal << one2345,  "     +1.235 +/-0.010"); // Expect same as right.

  CHECK_USED(scientific << plusminus << setw(20) << noadjust << setfill('~') << one2345, "~~~~~~1.235 +/-0.010"); // expect same as right adjust.
  CHECK_USED(scientific << plusminus << setw(20) << setfill('~') << left << one2345,     "1.235 +/-0.010~~~~~~"); // 
  CHECK_USED(scientific << plusminus << setw(20) << setfill('~') << internal << one2345, "~~~~~~1.235 +/-0.010"); 
  CHECK_USED(scientific << plusminus << setw(20) << setfill('~') << right << one2345,    "~~~~~~1.235 +/-0.010"); // Expect same as right.
  
  // Some bigger values.
  uncun ten2345(10.2345, 0.01f, 2); // Inexact from double, sd 0.01, df 2 = 3 values.
  CHECK_USED(scientific << noplusminus << setw(20) << left << ten2345,   "10.235              "); // 

} // BOOST_AUTO_TEST_CASE(unc_test_manips)

BOOST_AUTO_TEST_CASE(unc_test_setsigfigs)
{ // Tests of fixing sigfigs.
  uncun u0(0, 0);  // Exact Zero, using constructor from int zero.

  fout << "setSigDigits(4) " << setSigDigits(4) << endl; // Check can set setsigdigits,
  BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 4); // and read back correctly.

  fout << "setUncSigDigits(1) " << setUncSigDigits(1) << endl; // Check can set setuncsigdigits,
  BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 1); // and read back correctly.

  CHECK(setSigDigits(4) << u0, "0"); // Not yet setsigdigits, so has no effect.
  // setsigdigits should force to 4 digits set by setSigDigits(4).
  CHECK(setSigDigits(4) << setsigdigits << u0, "0.0000"); // Fails!

  uncun one(1., 0.001f); // 
  CHECK(setSigDigits(3) << setsigdigits << one, "1.00"); //  getting 1.
  CHECK_USED(setSigDigits(-1) << setsigdigits << u0, "0.00000000000000000"); // Invalid sigDigits (show max_digits10).
  CHECK_USED(setSigDigits(0) << setsigdigits << u0, "0.00000000000000000"); // Zero sigDigits (show max_digits10).
  CHECK_USED(setSigDigits(1) << setsigdigits << u0, "0.0"); // Override normal is still exact, so no decimal point.
  CHECK_USED(setSigDigits(2) << setsigdigits << u0, "0.00"); // Is still exact, so no decimal point.
  CHECK_USED(setSigDigits(3) << setsigdigits << u0, "0.000"); // 
  CHECK_USED(setSigDigits(4) << setsigdigits << u0, "0.0000"); //
  CHECK_USED(setSigDigits(1) << setsigdigits << showpoint << u0, "0.0"); // Override normal is still exact, but showpoint demands a decimal point.
  CHECK_USED(setSigDigits(1) << setsigdigits << showpos << u0, "+0.0"); //  Show plus sign.
  CHECK_USED(setSigDigits(1) << setsigdigits << left << setw(6) << u0, "0.0   "); // Trailing spaces.


  uncun u(12.345678, 0.09876F);
  CHECK_USED(plusminus << u, "12.35 +/-0.099");
  CHECK_USED(setSigDigits(6) << setsigdigits << plusminus << u, "12.3457 +/-0.099");
  CHECK_USED(setSigDigits(6) << setsigdigits << plusminus << showpos << u, "+12.3457 +/-0.099");
  CHECK_USED(setUncSigDigits(6) << setuncsigdigits << plusminus << u, "12.35 +/-0.0988");
  // Uncertainty controls value but not uncertainty digits of precision.

} 

BOOST_AUTO_TEST_CASE(unc_test_setscale)
{   // Tests of	unc setscale and autoscale functions.
  // These may not be necessary because Boost.Units provides autoscaling via autoprefix.
  //  uncun int10000(10000); // Should not be autoscaled nor set scaled.
  //  CHECK_USED(int10000, "10000"); // NOT 10.000 k because is neither autoscaled nor set scaled.
  //  CHECK_USED(autoscale << int10000, "10000"); // NOT 10.000 k because is integer.
  //  CHECK_USED(scale	 << int10000, "10000"); // NOT 10.000 k because is integer.
  //  uncun zero(0, 0.000001f); 
  //  CHECK_USED(zero, "0"); 
  //  CHECK_USED(autoscale << zero, "0"); // zero Should not be autoscaled 
  //  CHECK_USED(setScale(6) << scale << zero, "0");  // nor set scaled.

  //  uncun uScaled1(0.00123456);	// Input scaled	by unit	prefix or	symbol.
  //  uncun uScaled2(2345.678);	// 
  //  uncun uScaled3(1.23456);	//
  //  CHECK_USED(autoscale <<	addsisymbol <<	uScaled3, "1.23456 "); // Is trailing space if no symbol right?
  //  CHECK_USED(autoscale <<	addsisymbol <<	uScaled1, "1.23456 m");
  //  CHECK_USED(autoscale <<	addsisymbol <<	uScaled2 , "2.345678 k");
  //  CHECK_USED(autoscale <<	addsiprefix << 	uScaled1, "1.23456 milli");
  //  CHECK_USED(autoscale <<	addsiprefix << 	uScaled2, "2.345678 kilo");
  //  CHECK_USED(autoscale <<	addsiprefix <<	addsisymbol << 	uScaled2, "2.345678 kilo");
  //  // Check get prefix if specify both.
  //  CHECK_USED(noautoscale << scale	<< setScale(3)	<<	addsisymbol <<	 uScaled2, "2.345678 k");
  //  CHECK_USED(scale <<	addsisymbol <<	 uScaled2 , "2345.678 "); // Can't test re-use of previous set scale(3).

  //  CHECK_USED(scale	 << setScale(6)	<<	addsisymbol <<	 uScaled2 , "0.002345678 M");
  //  CHECK_USED(scale	 << setScale(4)	<<	addsisymbol <<	 uScaled2 , "0.2345678 ");
  //  CHECK_USED(scale	 << setScale(2)	<<	addsisymbol <<	 uScaled2 , "23.45678 h");
  //  CHECK_USED(scale	 << setScale(1)	<<	addsisymbol <<	 uScaled2 , "234.5678 da");
  //  CHECK_USED(scale	 << setScale(0)	<<	addsisymbol <<	 uScaled2 , "2345.678 ");
  //  CHECK_USED(scale	 << setScale(-1)	 <<	addsisymbol <<	 uScaled2 , "23456.78 d");
  //  CHECK_USED(scale	 << setScale(-2) 	<<	addsisymbol <<	 uScaled2 , "234567.8 c");
  //  CHECK_USED(scale	 << setScale(-3) 	<<	addsisymbol <<	 uScaled2 , "2345678 m");
  //  CHECK_USED(scale	 << setScale(-4)	 <<	addsisymbol <<	 uScaled2 , "23456780 ");
  //  CHECK_USED(scale	 << setScale(-6)	 <<	addsisymbol <<	 uScaled2 , "2345678000 u"); // Silly scaling!
  //  CHECK_USED(scale	 << setScale(-9)	 <<	addsisymbol <<	 uScaled2 , "2345678000000 n"); // Silly scaling!
  //  CHECK_USED(scale	 << setScale(-6)	 <<	addsisymbol <<	 uScaled1 , "1234.56 u"); // 
  //  CHECK_USED(scale	 << setScale(-3)	 <<	addsisymbol <<	 uScaled1 , "1.23456 m"); // 

  //  CHECK_USED(setSigDigits(2) << setsigdigits << uScaled2 << addsisymbol, "2345.678"); // exact so didn't use setsigdigits
  //  uScaled2.std_dev(0.1f);
  //  CHECK_USED(uScaled2 << addsisymbol, "2345.68");
  //  CHECK_USED(setSigDigits(2) << setsigdigits << uScaled2 << addsisymbol, "2400.");
} //   BOOST_AUTO_TEST_CASE(unc_test_setscale)

BOOST_AUTO_TEST_CASE(unc_test_asym)
{	// Some asymetric uncertainties.
  uncun notMore(1.71, 0.2f, 0, UNC_NOPLUS);  // Only minus.
  BOOST_CHECK((notMore.types() & UNC_NOPLUS));  // 
  CHECK_USED(plusminus << notMore, "1.71 +0/-0.20");
  uncun notLess(1.81, 0.2f, 0, UNC_NOMINUS);  // Only plus.
  BOOST_CHECK((notLess.types() & UNC_NOMINUS));  // 
  CHECK_USED(plusminus << notLess, "1.81 -0/+0.20");
}

BOOST_AUTO_TEST_CASE(unc_test_set_manips)
{
  fout << "\nTest one parameter manips to set unc width, scale, sigdigits & unc sigdigits.\n";
  setUncDefaults(fout);	//†Resets stream's format flags to default.
  fout << resetiosflags(ios_base::basefield | ios_base::adjustfield | ios_base::floatfield
  | ios_base::showpos | ios_base::showpoint | ios_base::uppercase | ios_base::showbase )
  << endl;
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

  fout << "setSigDigits(4) " << setSigDigits(4) << endl;
  BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 4); 

  fout << setSigDigits(2)	<< endl;
  BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 2);
  BOOST_CHECK_EQUAL(fout.iword(oldSigDigitsIndex), 4); // Check save.

  BOOST_CHECK_EQUAL(fout.iword(oldUncSigDigitsIndex),-1); // Default means not set yet.
  fout << setUncSigDigits(5) << endl; // Bigger that makes sense.
  BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 3); // Reduces to max sensible.
  fout << setUncSigDigits(2) << endl; // ISO default.
  BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 2); // Is default.
  BOOST_CHECK_EQUAL(fout.iword(oldUncSigDigitsIndex), 3); // Previous set to 3.

  fout << "setUncWidth(10) " << setUncWidth(20) << flush;
  BOOST_CHECK_EQUAL(fout.iword(uncWidthIndex), 20);

  fout << "setScale(-3) " << setScale(-3) << flush;
  BOOST_CHECK_EQUAL(fout.iword(setScaleIndex), -3);

  fout << "setsigdigits(5) " << setSigDigits(5) << flush;
  BOOST_CHECK_EQUAL(fout.iword(setSigDigitsIndex), 5);

  fout << "setuncsigdigits(1) " << setUncSigDigits(1) << flush;
  BOOST_CHECK_EQUAL(fout.iword(setUncSigDigitsIndex), 1);


} // BOOST_AUTO_TEST_CASE(unc_test_set_manips)

BOOST_AUTO_TEST_CASE(unc_test_more_naninf)
{ // 	"\nNative method of display of floating point infinity & Not a Number\n";
  CHECK(numeric_limits<double>::infinity(), "1.#INF");
  CHECK(-numeric_limits<double>::infinity(), "-1.#INF");
  CHECK(numeric_limits<float>::quiet_NaN() , "1.#QNAN");
  CHECK(-numeric_limits<float>::quiet_NaN() , "-1.#IND");

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
  CHECK_USED(zeroMaybe, "0.00000000000000000");  //  Show max_digits10 because uncertainty is NaN.

  CHECK_USED(plusminus << zeroMaybe, "0.00000000000000000 +/-?");
  
  uncun postwoMaybe(+2., numeric_limits<float>::quiet_NaN());  // +1.234 with unknown uncertainty.
  //cout << postwoMaybe << endl;
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
  uncun infUnknown(9.8, numeric_limits<float>::infinity() );  // Value with infinite uncertainty.
  CHECK_USED(infUnknown, "9.8000000000000007"); 
  CHECK_USED(plusminus << infUnknown, "9.8000000000000007 +/-inf");
} // BOOST_AUTO_TEST_CASE(unc_test_unc_Nan_inf)

BOOST_AUTO_TEST_CASE(unc_test_setsegdigits)
{
  uncun zeroMaybe(0., numeric_limits<float>::quiet_NaN() );
  //CHECK_USED(setSigDigits(7) << setsigdigits << zeroMaybe, "0.0000000"); // TODO
// setSigDigits must override all other options (and also set stddev sigfigs too).
} // BOOST_AUTO_TEST_CASE(unc_test_setsegdigits)

BOOST_AUTO_TEST_CASE(unc_test_coda)
{
  BOOST_MESSAGE("Uncertain Class tests log end. " << __FILE__ << ' ' <<  __TIMESTAMP__ );

  dout << "\n""Unc Diagnostics log end. " << endl; // \x0F1 = +- on screen, but ~n in files!

  cerr.rdbuf(dout.rdbuf()); // Switch diagnostic log back to cerr before close!
  // cerr = fout; in effect.
  dout.close(); // Diagnostics.
  cerr << "\n\nClosed diagnostics file " << diagFilename << ' '<< __TIMESTAMP__ << endl;

  fout << "\nClosed test output "<< __FILE__ << ' ' <<  __TIMESTAMP__ << endl;
  fout.close(); // Test output.
  //lout << logFilename << " end." << endl;

  //unit_test_log.set_stream(cout); // Switch back to cout,
  //lout.close();  // before close in case there is more log output,
  // which is almost certain, else ends unhappily!

  cout << "\nClosed fout "<< __FILE__ << ' ' <<  __TIMESTAMP__ << endl;
//	fin.close();
//	cout << "\nClosed fin "<< __FILE__ << ' ' <<  __TIMESTAMP__ << endl;
  cerr << endl;  // Needed to avoid crash right at end.
}  // BOOST_AUTO_TEST_CASE(unc_test_coda)


/*

// Display of all 8-bit characters, showing differences between cout and fout.
//{for (int i = 1; i < 0xFF; ++i)
  //{ // Output all 256 8-bit chars.
  //	fout << char(i);
  //	if ((i % 64) == 0)
  //	{ // 64 chars per line, 4 lines.
  //		fout << nl;
  //	}
  //}} // for i
  //fout << endl;
  //!"#$%&'()*+,-./0123456789:;<=>?@
  //ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~Ä
  //ÅÇÉÑÖÜáàâäãåçéèêëíìîïñóòôöõúùûü†°¢£§•¶ß®©™´¨≠ÆØ∞±≤≥¥µ∂∑∏π∫ªºΩæø¿
  //¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷◊ÿŸ⁄€‹›ﬁﬂ‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ˜¯˘˙˚¸˝˛

// No minus infinity in numeric_limits,
// but <cfloat> int _fpclass (double) == _FPCLASS_NINF if negative infinity.
// uncun minus_infinite(MINUS_INFINITY, 0.0f);  // exact minus infinity.
// fout << " (MINUS_INFINITY, 0.) " << minus_infinite << endl;
// infinite = uncun(MINUS_INFINITY, 0.1f);  // uncertain plus infinity.
// fout << " (MINUS_INFINITY, 0.1) " << minus_infinite << endl;
if (testMath)
{  
cerr << "Test math functions." << nl;		
// New scope for some unc variables.
uncun a(0), b(0), c(0), d(0),  e(0),  t(0);
fout <<
"\nLibrary functions ceil(), floor(), fabs(),  modf(), frexp(), and fmod()\n"
"all exist primarily for their discontinuities.  All give bad results\n"
"(and warnings from the test class) when used near a discontinuity.\n" 
"Extreme care needed in using these functions with uncertain arguments!\n";
{
// double fmod (double x, double y) returns the floating-point remainder of x / y.
uncun mod(0);
uncun x(0);
mod = fmod(x + 0.5, 1.0);  // Floating point modulus or remainder.
fout << "fmod(" << (x + 0.5) << ", " << 1.0 << ") = " << mod << endl;
mod = fmod(5.5, 0.9 + 0.1 * a);
fout << "fmod(" << 5.5 << ", " << (0.9 + 0.1 * a) << ") = " << mod << endl;
}
{
// unc modf(unc x, double* intPart) 
// modf breaks floating-point value x into fractional & integer parts,
// (each of which has the same sign as x).
// Returns signed fractional portion of x.
// Integer portion is stored as a double floating-point value at intPart.
uncun x(0); // to split.
uncun frac(0.88); // for return.

double intPartdbl;  // for integer part as double.
frac = modf(x, &intPartdbl); // integer part as double.
fout << "modf(" << x << ", intPart) = " << intPartdbl << " & " << frac << endl;
frac = modf(x + 0.5, &intPartdbl);
fout << "modf(" << (x + 0.5) << ", intPart) = " << intPartdbl << " & "<< frac << endl;
}
// double frexp( double x, int *expptr );
// breaks floating-point value (x) into a mantissa (m) and an exponent (n),
// such that the absolute value of 1.0 > m >= 0.5 and x = m*2n.
// Integer exponent n is stored at the location pointed to by expptr. 

int intPart;  // for exponent part of double.
t = frexp(c, &intPart);  // split c to mantissa & exponent.
fout << "frexp(" << c << ", i) = " << t << endl;
t = frexp(c + 0.5, &intPart);
fout << "frexp(" << (c + 0.5) << ", i) = " << t << endl;
t = ceil(c);  // integer (as double) >= double.
fout << "ceil(" << c << ") = " << " " << t << endl;
t = ceil(c + 0.5);
fout << "ceil(" << (c + 0.5) << ") = " << t << endl;
t = floor(c);  // integer (as double) <= double.
fout << "floor(" << c << ") = " << t << endl;
t = floor(c + 0.5);
fout << "floor(" << (c + 0.5) << ") = " << t << endl;

t = fabs(c);  // absolute value of a double.
fout << "fabs(" << c << ") = " << t << endl;
b = -1.2;
t = fabs(b);
fout << "fabs(" << b << ") = " << t << endl;
t = fabs(b + 0.02);
fout << "fabs(" << (b + 0.02) << ") = " << t << endl;
// End scope of uncun a,b,c, ...
} // test Meth

fout << "\nUnits test Output from "
<< __FILE__ << space << __DATE__ << " " << __TIME__ 
<< "\n" << endl;

uncun uGiven;  // Give on input.
uncun uScaled; // Input scaled by unit prefix or symbol.
uncun uSIvalue (0.);  // Copy or converted from non-SI.
string test; // = "  123.45 +/- 0.1 mA (99) ! description";
while(!fin.fail())
{
if (fin) getline(fin, test);
if (test.size() == 0) break;  // to quit.
if (test.size() > 0)
{
fout << "\nINPUT: " << test << endl;  // Echo input to file.
cerr << "\nINPUT: " << test << endl;  // Echo input to console.
istringstream is(test);
is >> uGiven;
if (is.fail() == true || is.eof() == true)
{  // No units given.

fout << uGiven << "(no unit)." << flush;
continue;
} 
bool isSIsymbol = false; // true if is k, m, M, G ...
bool isSIprefix = false;  // true if is kilo, mill, Mega ...
bool isSImultiple = false;  // if SIsymbol or SIprefix
bool isUnit = false;  // if a unit like length, mass ... defined.
bool isSIunit = false;  // true if meter, but false if foot.
string multipleUnit = "";  // "mm", "Mm", "m", "um" "centim", "inches"...
string SIsymbol;  // Examples: k, m, M ...
string SIprefix;  // Examples: "kilo", milli, Mega ...
string SIunitName = ""; // SI unit for type of unit, meter for length.
string addSIsymbol = "";  // Example: m for meter
string unitName = "";   // base unit name, for example meter, inch, oz
string givenName = "";  // name, abbreviation or alias.
double confactor;  // multiply by this to convert non-SI to SI unit.
const unit* u = &length; // unitps[unitno];  // &length, &mass, &current ...
unsigned int SIindex = notOK;  // Index to SIsymbols, SIprefixes & powersOfTen
// == SI maxPowerTen if unity scale factor.
const char** uNames;
const char* unitAbbreviation = "";
unsigned int unitNamesIndex = notOK;
unsigned int givenIndex = 0; // uNames[givenIndex] = given name string

is >> ws; // Needed because whitespace made significant by:
// is.unsetf(ios_base::skipws); or >> noskipws to make all chars significant.
if (is.peek() == '!')
{  // comment follows, so
string comment;
getline(is, comment);  // to end of line.
// No units given.
fout << uGiven << "(no unit)." << flush;
continue;
}

is >> multipleUnit;  // 1 meter, 2.2 mm, 1 inch, 3.4 uin or u"
cerr << "multipleUnit " << multipleUnit << endl;
is >> ws;  // Ensure any trailing whitespace read.

// TODO multiple units like mg/ml also written as mg ml-1???

if (multipleUnit[0] == '?')
{
if (multipleUnit[1] == '?')
{  // Show all units for all types, mass, length, current ...
// fout << "\nshowAllUnits(u, fout); for all units.\n" << endl;
for (int unitno = 0; unitno < unitsCount; unitno++)  // All units.
{
const unit* unit = unitps[unitno];
showAllUnits(unit, fout);
}
continue;  // while more test input.
}
else
{  // Just some examples - or select somehow length, mass etc.
// showAllUnits(&current, fout); // Show possible units for current.
// showAllUnits(&mass, fout);  // Show possible units for mass.
// showAllUnits(&length, fout); // Show all possible units for length.
showAllUnits(u, fout); // Show all possible units for current unit of .
continue;  // while more test input.
} // ?? option all units.
}  // ? option current unit.

// Try to decipher the unit, & SI symbol or prefix multiple, if any.
// unitNamesIndex is one of unitNames like meter, inch, feet ...
unitNamesIndex = findUnit(multipleUnit, u, givenIndex, SIindex);

isUnit = !(unitNamesIndex == notOK);
if (isUnit)  // of specified unit type like mass, length ...
{
uNames = u->unitNames[unitNamesIndex];  // gram, ton, cwt, lb, oz ...
givenName = uNames[givenIndex]; // Name actually given on input.
unitName = uNames[0];  // Base unit name: meter, inch, feet ...
// not abbreviation or alias.
unitAbbreviation = uNames[1];  // Base unit abbreviation: m, in, ft ..
SIunitName = u->SIunitNames[0]; // For example: meter, gram, second ...
addSIsymbol = u->SIunitNames[1];  // m, g, s ...
confactor = u->unitToSIfactors[unitNamesIndex]; // Conversion factor to SI unit.
}
else // isUnit == false
{
cerr << "Unit " << multipleUnit << " is NOT a unit of " << u->unitOf << '.' << endl;
fout << "Unit " << multipleUnit << " is NOT a unit of " << u->unitOf << '.' << endl;
const unit& ur = findAnyUnit(multipleUnit, unitNamesIndex, givenIndex, SIindex);
if (&ur == &unknownUnit  || unitNamesIndex == notOK)
{
fout << "Unit " << multipleUnit << " is NOT a known unit!" << endl;
continue;
}

fout << "But unit " << multipleUnit << " IS a unit of " << ur.unitOf << '.' << endl;
isUnit = (unitNamesIndex != notOK);
if (!isUnit)
{
continue;
}
const unit* uu = u;
u = &ur;
uNames = u->unitNames[unitNamesIndex];  // gram, ton, cwt, lb, oz ...
givenName = uNames[givenIndex]; // Name actually given on input.
unitName = uNames[0];  // Base unit name: meter, inch, feet ...
// not abbreviation or alias.
unitAbbreviation = uNames[1];  // Base unit abbreviation: m, in, ft ..
SIunitName = u->SIunitNames[0]; // For example: meter, gram, second ...
addSIsymbol = u->SIunitNames[1];  // m, g, s ...
confactor = u->unitToSIfactors[unitNamesIndex]; // Conversion factor to SI unit.
}  // is Unit

isSIunit = (unitNamesIndex == 0);  // If meter not inch.
isSImultiple = !(SIindex == notOK); // SI symbol or prefix found.

uScaled = uGiven;  // Assume no conversion factor.
if (isSImultiple)
{  // Scale using SIindex.
uScaled *= powersOfTen[SIindex + maxPowerTen - SImaxPowerTen];
// Works for non-SU units like inch too!
}  //isSImultiple
else
{
SIindex = SImaxPowerTen;  // So SI symbol & prefix = ""
}  // no SImultiple.
SIsymbol = SIsymbols[SIindex]; 
SIprefix = SIprefixes[SIindex];

if (isSIunit)
{  
confactor = u->unitToSIfactors[unitNamesIndex];
if (confactor != 1.)
{  // Internal data error, expect SI unit confactor to be unity!
cerr << "SI unit conversion factor not unity but " << confactor << "!" << endl;
// Ignoring error.
uSIvalue = uScaled;
}
}
else
{ // not SI unit, so convert to SI.
confactor = u->unitToSIfactors[unitNamesIndex];
if (confactor != 0.)
{  // Normal case of valid non-zero conversion factor.
uSIvalue = uScaled * confactor; // Convert to SI unit.
}
else
{  // Special formula needed, for example degrees Celsius.
if (unitName == "Celsius")
{
uSIvalue = uScaled + 273.15;  // To degrees Kelvin.
}
else if (unitName == "Fahrenheit")
{
uSIvalue = (uScaled + 459.67) / 1.8;  // NIST 811 p 49.
} // C or F.
// else other complex conversion identified by (unitName == "???").
} // confactor.
} // SI unit or not.

if (isSIunit)
{
fout << "SI unit of " << u->unitOf << ' '
<< flexform << plusminus << noautoscale  << noSIprefix
<< uGiven << SIprefix << unitName // millimeter
<< " or " << SIsymbol << unitAbbreviation ; // mm
if (isSImultiple)
{
fout << ", SI " // "SI 1.23 meter or m"
<< uScaled << ' ' << SIunitName << " or " << unitAbbreviation;
}
if (fabs(double((uScaled.value()) > 1000.0)) || fabs(double((uScaled.value()) < 1.0)))
{  // Useful to autoscale.
fout << "\nAutoscaled " << plusminus << autoscale  << addSIprefix
<< uScaled << SIunitName
<< " or " << noplusminus << noSIprefix << addSIsymbol << uScaled << addSIsymbol;
}
}
else 
{  // NOT SI unit, so show the value in appropriate SI unit.
fout << "Non-SI unit " << flexform << plusminus << noautoscale
<< uScaled
<< noSIsymbol // To avoid trailing space.
// << '_' // testing
// But want space after "1.0 +/- ?"
<< givenName  // non-SI like feet, fortnight etc.
<< " in SI is " 
<< plusminus << addSIsymbol << noSIprefix << uSIvalue
<< addSIsymbol;// For symbol and abbreviation like mm.
if (
(fabs(double(uSIvalue.value() > 1000.0) ) )
|| fabs(double(uSIvalue.value() < 1.0) ) )
{  // Useful to autoscale.
fout << "\nAutoscaled SI " 
<< plusminus << autoscale
<< addSIprefix << uSIvalue << SIunitName
// For prefix & unit name like millimeter.
<< " or " << noplusminus << noSIprefix << addSIsymbol << uSIvalue << addSIsymbol;
}
}  // if SIunit or not.

is >> noskipws >> ws; // Needed if whitespace made significant by:
// is.unsetf(ios_base::skipws); or noskipws >> to make all chars significant.
is.unsetf(ios_base::skipws); // ==  >> noskipws 
if (is.peek() == '!')
{ // A comment follows, so read and echo it.
string comment;
// is >> comment; // still stops at ws.
getline(is, comment);  // to end of line.
//fout << " Comment: "<< comment << ", length " << comment.length()<< endl;
}
}
}


/*

------ Rebuild All started: Project: unc_tests, Configuration: Debug Win32 ------
Build started 24-Apr-2012 12:20:16.
_PrepareForClean:
  Deleting file "Debug\unc_tests.lastbuildstate".
InitializeBuildStatus:
  Touching "Debug\unc_tests.unsuccessfulbuild".
ClCompile:
  cvt.cpp
  si_units.cpp
  unc.cpp
  unc_print.cpp
  unc_read.cpp
  xiostream.cpp
  unc_tests.cpp
  Generating Code...
Manifest:
  Deleting file "Debug\unc_tests.exe.embed.manifest".
LinkEmbedManifest:
  unc_tests.vcxproj -> J:\Cpp\quan\MSVC\Debug\unc_tests.exe
CustomBuildStep:
  Description: Autorun J:\Cpp\quan\MSVC\Debug\unc_tests.exe
  Running 1 test case...
  Platform: Win32
  Compiler: Microsoft Visual C++ version 10.0
  STL     : Dinkumware standard library version 520
  Boost   : 1.49.0
  ± Uncertain Class Test output to unc_test_output.txt ..\..\libs\quan\test\unc_tests.cpp Fri Mar  9 19:18:39 2012
  Unc Diagnostics logged to unc_diag.txt
../../libs/quan/test/unc_tests.cpp(653): error : in "unc_test": check oss.str() == "0.0000" failed [0 != 0.0000]
  0.00
  Value 1 flagged as exact, but uncertainty 1 is not zero!
  Value 1 flagged as exact, but degfree 2 is not zero!
  uncTypes (0x60e7) zero integer rational uncKnown noPlus noMinus df_exact df_known.0.000000000000000
../../libs/quan/test/unc_tests.cpp(1231): error : in "unc_test": check oss.str() == "0.00000" failed [0.000000000000000 != 0.00000]
  2.00000
  1.23400
  
  Closed fout ..\..\libs\quan\test\unc_tests.cpp Fri Mar  9 19:18:39 2012
C:\Program Files\MSBuild\Microsoft.Cpp\v4.0\Microsoft.CppCommon.targets(183,5): error MSB3073: The command ""J:\Cpp\quan\MSVC\Debug\unc_tests.exe" --result_code=yes --report_level=detailed --catch_system_errors=no  --build_info=yes
C:\Program Files\MSBuild\Microsoft.Cpp\v4.0\Microsoft.CppCommon.targets(183,5): error MSB3073: :VCEnd" exited with code 201.

Build FAILED.

Time Elapsed 00:00:08.71
========== Rebuild All: 0 succeeded, 1 failed, 0 skipped ==========

*/