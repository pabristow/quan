// TestQuan1.cpp
// Test unc fully first.
// Compiles and run 8 Aug but results wrong in unc
// Using atime seems to work but is messy - use Boost posixTime?

// Test of Quan1 - univariate using uncertainty and statistical tests.

// see QuanOne word document for description.
#ifdef _MSC_VER
#  pragma warning(disable : 4189) // 'argument' : conversion from 'time_t' to 'unsigned short', possible loss of data
#  pragma warning(disable : 4996) //  _CRT_SECURE_NO_WARNINGS.
#  pragma warning(disable : 4702) // Unreachable code.
#endif

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

#include "unc.hpp" // Declaration of Uncertain Classes.
#include "meas.hpp" // uncertain plus order.
#include "to_string.hpp" // 

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

char const outFilename[] = "Quan1Out.txt";
char const logFilename[] = "Quan1Log.txt";
char const testInputFilename[] = "Quan1In.txt";

// This block of definitions MUST be positioned before main.
// 14 indexes of long iwords allocated by calls of ios_base.xalloc();
// 1st call of xalloc() returns 0 so ios_base.iword(0) used for magic id,
// 2nd call of xalloc() returns 1 so ios_base.iword(1) used for uncFlags,
// 3rd calls returns 2, so iosword(2) used for sigDigits ...
// Order of assignment must ensure these match enum uncindex (if used).
const long zeroIndex = ios_base::xalloc(); // 1st iword[0] to hold a 'magic' id.

const long uncFlagsIndex = ios_base::xalloc(); // long& uncFlags = iword(1)
const long oldUncFlagsIndex = ios_base::xalloc(); // long& olduncFlags = iword(2)
const long sigDigitsIndex = ios_base::xalloc(); // sigDigits = iword(3)
const long oldSigDigitsIndex = ios_base::xalloc(); // oldsigDigits = iword(4)
const long setSigDigitsIndex = ios_base::xalloc(); // setsigDigits = iword(5)
const long stdDevSigDigitsIndex = ios_base::xalloc(); // stdDevSigDigits = iword(6)
const long oldStdDevSigDigitsIndex = ios_base::xalloc(); // oldStdDevSigDigits = iword(7)
const long ScaleIndex = ios_base::xalloc(); // scale = iword(8)
const long oldScaleIndex = ios_base::xalloc(); // oldScale = iword(9)
const long setScaleIndex = ios_base::xalloc(); // setScale = iword(10)
const long uncWidthIndex = ios_base::xalloc(); // uncWidth = iword(11)
const long oldUncWidthIndex = ios_base::xalloc(); // oldUncwidth = iword(12)
const long oldUncSetWidthIndex = ios_base::xalloc(); // oldUncSetWidth = iword(13)
const long usedIndex = ios_base::xalloc(); // used = iword(14)
const long oldUncUsedIndex = ios_base::xalloc(); // oldUsed = iword(15)
const long widthIndex = ios_base::xalloc(); // width = iword(16)
const long oldWidthIndex = ios_base::xalloc(); // oldWidth = iword(17)

const long topIndex = ios_base::xalloc(); // long& topIndex = iword(18] == iword(0] check!

extern const long indexID;  // 'Magic' value defined in unc.cpp.

bool isIndexed = true; // Above indexes have been initialised.

char const* Ctime(time_t* t); // Timestamp as string "Tue Apr 24 16:57:31 2001""\n"

// #include <boost/test/minimal.hpp> // Boost minimal test
#define BOOST_TEST_MAIN // Required for int test_main() (must come FIRST).
#define BOOST_LIB_DIAGNOSTIC "on"// Show library file details.
// Linking to lib file: libboost_unit_test_framework-vc100-mt-s-1_49.lib

#include <boost/config.hpp>
#include <boost/cstdlib.hpp> // needed for boost::exit_failure;

#include <boost/test/unit_test.hpp> // Enhanced for unit_test framework autolink,
#include <boost/test/floating_point_comparison.hpp> // Extra test tool for FP comparison.
  using boost::unit_test::test_suite;
  using boost::unit_test::unit_test_log;

    // Direct use of std predicate function less using
  // TEMPLATE STRUCT less in <functional>
  // Used below for URealUncorr and Meas which both have operator< defined.
  // if (std::less<double>()(l, h))
  // if (std::less<URealUncorr>()(l, h))
  // if (std::less<Meas>()(lm, hm))

  //template<typename Type>
  //struct less
  //  : public binaryfunction<Type, Type, bool>
  //{	// functor for operator<
  //  bool operator()(const Type& Left, const Type& Right) const
  //  {	// apply operator< to operands
  //    return (Left < Right);
  //  } // operator<
  //};


BOOST_AUTO_TEST_CASE(quan1_test_0)
{ 
  string message("Quan1 test: " __FILE__ );
#ifdef __TIMESTAMP__
  message += " at " BOOST_STRINGIZE(__TIMESTAMP__);
#endif
#ifdef _MSC_FULL_VER
  message +=  ", MSVC version " BOOST_STRINGIZE(_MSC_FULL_VER) ".";
#else
  message += "."
#endif
  BOOST_MESSAGE(message);
  BOOST_CHECK(std::numeric_limits<double>::is_iec559 == true);
  // IEE745 is assumed by code.

  setUncDefaults(cout); 
  BOOST_CHECK(isIndexed); // Should pass.
  BOOST_CHECK(cout.iword(zeroIndex) == indexID); // Should pass.
  BOOST_CHECK(cout.iword(topIndex) == indexID); // Should pass.
  cout.iword(0) = indexID; // 1st iword[0] to hold a 'magic' id.

  // Integrity check on iword begin and end no longer needed in CHECK.
  {
    ostringstream oss;
    setUncDefaults(oss);
    BOOST_CHECK(oss.iword(zeroIndex)== indexID);
    BOOST_CHECK(oss.iword(0) == indexID);
    BOOST_CHECK(oss.iword(topIndex)== indexID);
    BOOST_CHECK(oss.iword(0) == indexID);
  }
} // BOOST_AUTO_TEST_CASE(quan1_test_0)

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

BOOST_AUTO_TEST_CASE(quan1_test_1)
  {	// Some examples of testing uncertaint class without using macro CHECK.
  {
    ostringstream oss;
    setUncDefaults(oss);
    URealUncorr u0(0., 0.F);
    oss << u0;
    BOOST_CHECK_EQUAL(oss.str(),"0");
  }
  {
    ostringstream oss;
    setUncDefaults(oss);
    URealUncorr u2(2., 0.F);
    oss << u2;
    BOOST_CHECK_EQUAL(oss.str(), "2.");
    //cout << "URealUncorr u2(2., 0.F) == " << u2 << endl;
  }
  {
    ostringstream oss;
    setUncDefaults(oss);
    URealUncorr u123(1.23, 0.01F);
    oss << plusminus << u123;
    BOOST_CHECK_EQUAL(oss.str(),"1.230 +/-0.02");
    // cout << "URealUncorr u123(1.23., 0.01F) == " << plusminus << u123 << endl;
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


BOOST_AUTO_TEST_CASE(quan1_test_3)
{

  // +/- symbol = dec 177, hex F1  messy!! fout is different to cout!
  ofstream fout(outFilename, ios::out); // use default overwrite/ iso::replace.
  BOOST_CHECK(fout);
  //if (!fout)
  //{  // failed to open OK.
  //  cerr << "File " << outFilename << " failed to open!" << endl;
  //  return notOK;
  //}
   cout << "Quan1 Test output to "	<< outFilename << space <<  __TIMESTAMP__ << endl;

  ifstream fin(testInputFilename, ios::in);
  BOOST_CHECK(fin.is_open());
  //if (!fin.is_open())
  //{
  //  cerr << "File " << testInputFilename << " failed to open!" << endl;
  //  return notOK;
  //}
  cout << "Quan1 input from " <<  testInputFilename << space << __TIMESTAMP__  << endl;
  

  if (false)
  {// Test of comparison operators and predicates for UReals now in unc.h. 
    // Rely on unc tests.
    URealUncorr u0(0.,0.F); // Exact integral zero gave problems before, now OK - all zero.
    cout << noplusminus << endl; 
    cout << "URealUncorr u0(0.,0.F) == " << u0 << endl;  // plain 0
    cout << plusminus << flush;
    cout << "URealUncorr u0(0.,0.F) == " << u0 << endl; // still plain 0 as expected.
    cout << noplusminus << u0 << endl; // still plain 0 as expected.
    cout << plusminus  << u0 << endl; // 9.89 < 9.99 < 10.1
    cout << addlimits << plusminus << u0 << endl; // 9.89 < 9.99 +/-0.10 < 10.1
    cout << uppercase << boolalpha << endl;
    cout << showformat << endl; // FormatFlags (0x4205)  skipws, uppercase, dec, boolalpha.
    // 	ostream& showuncflags(ostream& os);
    cout << showuncflags << endl; // uncFlags (0x808)  add +/-, add limits.
    // void outUncFlags(unsigned short int uncFlags = std::cout.iword(uncFlagsIndex), ostream& os = std::cerr);
    //outUncFlags(unsigned short(cout.iword(uncFlagsIndex)), cout); // Using defaults above.
    cout << endl;
  }


  /* to_string doesn't appear to work - causes bad_alloc - come back to this?
  if(false)// Test to_string, and some specializations.
  {
    double dd = 3.333333333333333333333333333333;
    std::string xx;
    xx = to_string<double>(dd);
    cout << xx << endl; // to_string default is precision(10) not std default 6.
    CHECK(xx, "3.333333333"); // Expect precision 10 digits.
    xx = to_string<bool>(true); 
    cout << xx << endl; // to_string default is boolalpha.
    CHECK(xx, "true"); // Expect boolalpha so "true" not "1".

    URealUncorr uu(1.6, 0.1f);
    setUncDefaults(cout);
    xx = to_string<URealUncorr>(uu);
    cout << xx << endl;
    CHECK(xx, "1.5 < 1.6 +/-0.10 < 1.7"); // Expect addlimits & plusminus.

    //const char* cs = "test";
    //xx = to_string("test");
    //CHECK(xx, "test");
    //URealUncorr hhh(1.6, 0.1f); // even at 2 standard deviations.
    //string hs = "to_string<URealUncorr>(hhh)  " + to_string<URealUncorr>(hhh) +" is cool!";
    //cout << hs << endl;
  } // tostring
  */
  
  // Check operators which only use value taking no account of uncertainty.
  URealUncorr l(1.1, 0.1f); // Two test values l and h whose confidence intervals DO overlap.
  URealUncorr h(1.1, 0.1f);
  URealUncorr ll(1.1, 0.1f); // Test values ll and hh whose confidence intervals DON'T overlap at one sd.
  URealUncorr hh(1.4, 0.1f);
  URealUncorr lll(1.1, 0.1f); // Test values lll and hhh whose confidence intervals DON'T overlap,
  URealUncorr hhh(1.6, 0.1f); // even at 2 standard deviations.

  cout 	<< "top of lower " << l.getValue() << " = " << l.getValue() + l.getStdDeviation() << endl
    << "bottom of higher " << h.getValue() << " = " << h.getValue() - h.getStdDeviation() << endl;
  // If linearly combining SDs adding or subtracting two values, use sqrt sum squares or hypot.
  BOOST_CHECK_GE((l.getValue() + l.getStdDeviation()), (h.getValue() - h.getStdDeviation()));

  cout << "sqrt_sumsquares sds = " << hypot(h.getStdDeviation(), l.getStdDeviation()) << nl;
  // But that isn't relevant when comparing means - see below NR in C .

  cout << "Difference of values in max sds between " << plusminus << h << " and " << l << " == "
   << (h.getValue() - l.getValue())/ std::max(l.getStdDeviation(), h.getStdDeviation()) << endl; // max

  cout << "Difference in sds between " << h << " and " << l << " == "
   << (h.getValue() - l.getValue())/ hypot(h.getStdDeviation(), l.getStdDeviation())  << endl; // hypot

  cout << "Difference in sds between " << hh << " and " << ll << " == "
   << (hh.getValue() - ll.getValue())/ hypot(hh.getStdDeviation(), ll.getStdDeviation())  << endl; // hypot

  cout << "Difference in sds between " << hhh << " and " << lll << " == "
   << (hhh.getValue() - lll.getValue())/ hypot(hhh.getStdDeviation(), lll.getStdDeviation())  << endl; // hypot

  // NR in C 13.4.3 p 512 comparing means with unequal variance, degrees of freedom and uncertainty.
  double meandiff = hh.getValue() - ll.getValue();
  URealUncorr meandiffu;
  meandiffu = ( hh - ll);
  cout << "Means "  << plusminus << ll << tab << hh << ", Difference of means is " << meandiffu << endl;
  cout << "SDs " << ll.getStdDeviation() << tab << hh.getStdDeviation() << endl;
  double cdf = hypot(hh.getStdDeviation(), ll.getStdDeviation());
  cout << "hypot sds " << cdf << nl; // Cruder combined uncertainty - wrong?
  double dfll = ll.getDegFree() +1; // N1
  double dfhh = hh.getDegFree() +1; // N2
  cout << "Degrees of freedom " << dfll << tab << dfhh << nl;
  double sell = (ll.getStdDeviation() * ll.getStdDeviation() )/ (ll.getDegFree() +1);
  double sehh = (hh.getStdDeviation() * hh.getStdDeviation() )/ (hh.getDegFree() +1);
  double se = sqrt(sell + sehh);
  cout << "Standard errors " << sell << tab << sehh << ", mean se " << (sell + sehh) * 0.5 << " sqrt() " << se << nl;

  double varll = ll.getStdDeviation() * ll.getStdDeviation();
  double varhh = hh.getStdDeviation() * hh.getStdDeviation();
  cout << "Variances " << varll << tab << varhh << nl;
  double varlln = varll / dfll ; // variances/N
  double varhhn = varhh / dfhh;
  cout << "variance/df " << varlln << tab << varhhn << nl;
  double dftop = (varlln + varhhn) * (varlln + varhhn); // top of 13.4.4
  cout << "df top " << dftop << endl;
  double dfbot = (varlln * varlln)/ dfll + (varhhn * varhhn) / dfhh; // bottom of 13.4.4
  cout << "df bot " << dfbot << endl;
  double df = dftop/dfbot;
  cout << "df " << df << nl; // Final 13.4.4 degrees of freedom (not integer).

  cout << "sqrt(varlln + varhhn) " << sqrt(varlln + varhhn) << endl;

  double st = meandiff/ sqrt(varlln + varhhn); // Student's t 13.4.3
  cout << "Student's t " << st << ", df " << df << nl;

//	double prob = betai(0.5 * df, 0.5, df/(df + st * st));
//	cout << "Probability that are different is " << prob << endl;


  if (l < h)
  { // Uses URealUncorr::operator<  - only uses central value, no uncertainty.
    cout << noplusminus << l << " is  < " << h  << endl; // 0.90 < 1.1 < 1.3  is < 1.0 < 1.2 < 1.4
  }
  if (l == h)
  { // Uses URealUncorr::operator==  - only uses central value, no uncertainty.
    cout << " l == h " << endl;
  }
  if (l != h)
  { // Uses URealUncorr::operator!=  - only uses central value, no uncertainty.
    cout << " l != h " << endl;
  }


 
  double ld = 1.2;
  double hd = 3.4;
  if (std::less<double>()(ld, hd))
  {
    cout << "(less<double>(" << ld << ", " << hd << " )) " << endl;
  }
  if (std::less<URealUncorr>()(l, h))
  {
    cout << "(std::less(l, h)) " << " l < h " << endl;
  }
  if (lessAbs<URealUncorr>()(l, h))
  {
    cout << "(lessAbs(l, h)) " << " l < h " << endl;
  }
  cout << "Test member predicate function " << plusminus << l << " < " << h  << endl;
  cout << plusminus << "(lessUnc(l, h)) ";
  if (URealUncorr::lessU(l, h))
  { // 
    cout << " l < h " << endl;
  }	
  else
  { // Not less so check if 
    if (URealUncorr::moreU(l, h))
    { // greater
      cout << " l > h " << endl;
    }
    else
    { // or approximate equal.
      cout << " l ~= h " << endl;
    }
  }
  cout << " l " << l << ((URealUncorr::equalU(l, h) ? " ~= " : " != ")) << " h " << h << endl;
  cout << " ll " << ll << ((URealUncorr::equalU(ll, hh) ? " ~= " : " != ")) << " hh " << hh << endl;
  cout << " l " << l << ((URealUncorr::equalU2(l, h) ? " ~= " : " != ")) << " h " << h << endl;
  cout << " ll " << ll << ((URealUncorr::equalU2(ll, hh) ? " ~= " : " != ")) << " hh " << hh << endl;

  cout << "Testing static memberfunction predicate lessU<isCorrelated>" << nl;
  //	bool lessU(const UReal<isCorrelated>& l, const UReal<isCorrelated>& r) 

  if (UReal<false>::lessU(ll, hh) )
  { // Not using typedef for UReal<false>::
    cout << ll << " is < " << hh << endl;
  }	
  else
  {
    cout << ll << " is >= " << hh << endl;
  }
  // if (UReal<true>::lessU(ll, hh) ) // fails because ll and hh are correlated.
  // and anyway is not really implemented yet for correlated,
  // but would use the same code silently.
   UReal<true> lc(1.); 	UReal<true> hc(2.);
  // Two correlated reals.

  if (UReal<true>::lessU(lc, hc) )
  { // Full UReal<true>::
    cout << " lc < hc " << endl;
  }	
  // global bool lessUnc not longer implemented for UReal<false> or <true>
  // and will fail to compile if  "if (lessUnc(lc, hc) ... " is used.	
  //

  if (URealUncorr::lessU(ll, hh) )
  { // Full URealUncorr:: qualification is essential - uses typedef for URealUncorr.
    cout << ll << " is < " << hh << endl;
  }	
  else
  {
    cout << ll << " is >= " << hh << endl;
  }
  if (URealUncorr::lessU2(ll, hh) )
  { // Two sd version.
    cout << ll << " is << " << hh << endl;
  }	
  else
  {
    cout << ll << " is >= " << hh << endl;
  }
  
  if (URealUncorr::lessU2(lll, hhh) )
  { // Two sd version.
    cout << lll << " is << " << hhh << endl;
  }	
  else
  {
    cout << lll << " is >= " << hhh << endl;
  }

  if (URealUncorr::lessU(0., hh) )
  { // Left most argument needs conversion from double.
    cout << " 0. < " << hh << endl;
  }	
  else
  {
    cout << " 0. >= " << hh << nl;
  }

  const URealUncorr vh(99.);
  if (URealUncorr::lessU(ll, vh) )
  { // right most argument needs conversion from double.
    cout <<  ll << " is < " << vh << endl;
  }	
  else
  {
    cout <<  ll << " is >= " << vh << endl;
  }

//	typedef UReal<false>::lessU UlessU;  doesn't work
  // Check moreU
  if (URealUncorr::moreU(ll, vh) )
  { // right most argument needs conversion from double.
    cout <<  ll << " is > " << vh << endl;
  }	
  else
  {
    cout <<  ll << " is <= " << vh << endl;
  }

  if (URealUncorr::equalU(l, h) )
  { // right most argument needs conversion from double.
    cout << l << " is ~= " << h << endl;
  }	
  else
  {
    cout << l << " is !~= " << h << endl;
  }


//	time_t* nowp = &now;
//	cout << " now = "  << localtime(&now) << endl; //  Outputs 004BCDD0 address of struct* tm?
//	cout << Ctime(nowp) << endl; // outputs: "Fri Jul 21 15:31:41 2000" = asctime(localtime(now))
//	cout << asctime(localtime(&now)) << endl; // outputs: "Fri Jul 21 15:31:41 2000""\n" = asctime(localtime(now))

 
    URealUncorr u0(0., 0.F);

    Meas zz; // Default constructor.
    //cout << "Meas zz; = " << zz << endl;
    CHECK(zz, "0"); 

    zz = 1.;
    //cout << "zz = 1; == " << zz << endl;
    CHECK(zz, "1"); 

    Meas z0(0.); // Construct from double zero.
    //cout << "Meas z0(0.) == " << z0 << endl;
    CHECK(z0, "0"); 

    Meas z0i(0); // Construct from int zero.
    //cout << "Meas z0i(0) == " << z0i << endl;
    CHECK(z0i, "0"); 

    Meas one(1.);

    ptime now =  ptime(day_clock::local_day());
    Meas m0(u0, "m0", now); // Using now defaults.
    // CHECK(m0, "m0 0 @ 15:42:45"); // Doesn't work because time now keeps changing!
    Meas m01(u0, "m01", 2); // Using unity value defaults.
    //cout << m01 << endl;
    CHECK(m01, "m01 0 # 2");


    //Meas(URealUncorr u, string id, time_t ti);  // <<<<<<<<<<<<<<<<< test these 
    //Meas(URealUncorr u, string id, unsigned short int);// <<<<<<<<<<<<<<<<< test these 

    Meas zm1(-1.); // negative double value to allow check of abs function below. 
    Meas z1m(-1.); // negative double value to check ==
    BOOST_CHECK(zm1 == z1m);
    BOOST_CHECK(zm1 != z0);

    //bool c = (zz < z0); // Check operator<
    BOOST_CHECK(z0 < zz); // Check operator<
    BOOST_CHECK(zz > z0); // Check operator>
    //cout << zz <<  " < " << z0 << " is " << boolalpha << c << endl;
    //c = (zm1 < 0 );
    BOOST_CHECK(zm1 < 0 );
    // BOOST_CHECK(0 > zm1); // No operator >

    //cout << zm1 <<  " < " << z0 << " is " << boolalpha << c << endl;
    //bool c = (zm1 < Meas(0) );
    BOOST_CHECK(zm1 < Meas(0) );
    // cout << zm1 <<  " < " << Meas(0) << " is " << boolalpha << c << endl;
    BOOST_CHECK(Meas(0) > zm1 );

    Meas mone = -one;
    //cout << "-one == " << mone << endl;
    CHECK(mone, "-1");
    //cout << "Meas zm1(-1.) == " << zm1 << endl;
    CHECK(zm1, "-1");
  
    Meas z1 = fabs(zm1); // double to check abs function.
    BOOST_CHECK(z1 == 1 );
    Meas z11 = abs(zm1); // double to check abs function.
    BOOST_CHECK(z11 == 1 );
    //cout << "abs(zm1) = std::abs(zm1) = " << abs(zm1) << endl; // Should lookup type Meas, and does!
    //cout << "abs<Meas>(zm1) == " << abs<Meas>(zm1) << endl; // explicit Type Meas fails!
    //cout << "abs(zm1).m_value == " << abs(zm1.m_value) << endl;		

  //Meas lmNow(l, "low", now);
  //cout << "Meas lmNow(l, \"low\", now) == " << lmNow << endl; 
  //Meas hmNow(h, "hi", 2);
  //cout << "Meas hmNow(h, \"hi\", 2); == " << hmNow << endl; 
  //time_t nnow =   now + 90; // Add 90 seconds to time for Meas hi.
  //Meas nmNow(h, "hi", nnow); // Show time has advanced.
  //cout << "Meas nmNow(h, \"hi\", nnow); == " << nmNow << endl; // Meas nmNow(h, "hi", nnow); == hi 1.2 @ 22:59:49
  // Some comparisons.


  Meas lm(l, "low", 1);
  Meas hm(h, "high", 2);


  if (lm < hm)
  { // Using bool operator< (Meas&)
    cout << "(lm < hm)  "<< lm << " < " << hm << endl;
  }				
  if (std::less<Meas>()(lm, hm))
  { // Using std functor less which uses operator<
    cout << "less(lm, hm) " << lm << " < " << hm << endl;
  }
  if (lessAbs<Meas>()(lm, hm))
  { // Using Meas functor lessAbs
    cout << "lessAbs<Meas>()(lm, hm) " << lm << " < " << hm << endl;
  }
  if (lessAbs<Meas>()(lm, hm))
  { // Using Meas functor lessAbs again 
    cout << "lessAbs<Meas>()(lm, hm) " << lm << " < " << hm << endl;
  }
  if (Meas::lessAbsM(lm, hm))
  { // Compare absolute value within uncertainty using Meas::lessAbsM function.
    cout << "lessAbsM(lm, hm) " << lm << " < " << hm << endl;
  }
    

  int vCount = 0;
  vector<URealUncorr> vs;
  vector<Meas> vms;
  vs.reserve(20); // More efficient than repeated push_backs.
  cout << "Space reserved for " << vs.capacity() << " values." << endl;
  
  string line; // = "  123.45 +/- 0.1 mA (99) ! description";
  int lineCount = 0;
  int valueCount = 0;
  while(!fin.fail())
  {
    URealUncorr v;
    Meas vm;
    if (fin)
    { // More to get.
      getline(fin, line); // 1.1 1.2 1.0 1.1 ...
    }
    if (line.size() == 0) break;  // Blank line (key return) to quit.
    if (line.size() > 0)
    { // Not a blank line.
      ++lineCount;
      fout << "Input line: " << line << endl;  // Echo input line to file.
      istringstream is(line);
      do
      {
        is >> v; // Read uncertain "1.2" or "1.2 +/-0.05".
        //cout << v << endl;  // Echo input URealUncorr & 
        Meas vm(v, "", valueCount); // Assign order of measurements.
        // No ID so far? time is default not_a_date_time.
        valueCount++;
        vs.push_back(v);  // push onto the back of the UReal vector vs.
        vms.push_back(vm); // push vm onto the back of the Meas vector vms.
              
        if (is.eof() == true)
        {  // Tried to read more but no units given.
          cout << "is.eof() == true" << " after " << v << "(no unit)." << endl;
          break;
        } // if
        if (is.fail() == true)
        { // Always seems to be fail == true when eof
          cout << "is.fail == true!" << endl;
          cout << v << "(no unit)." << endl;
          break;
        }
        // int avail = is.rdbuf()->in_avail();
      } // do
      while (is.good() && (is.rdbuf()->in_avail() != 0));
    } // Not blank line.
  } // while
  // cout << "vs.front() == " << vs.front() << ", vs.back() == " << vs.back() << endl;
  // cout << "*vs.begin() == " << *vs.begin() << ", *vs.end() -1 == " << *(vs.end() -1) << endl;
  // vector<URealUncorr>::ostream_iterator vi = vs.begin();
  // ostream_iterator<URealUncorr> voi(cout, " ");
  // ostream_iterator<URealUncorr> vi1(cout);  // default delimiter.
  

  vCount = vs.size()-1; // -1 becos 1st is expected uncertainty
  double expectUncValue = vs[0].getValue();  // Expected +/- uncertainty.
  float expectUncUnc = vs[0].getStdDeviation();
  vs[0].setStdDeviation(float(vs[0].getValue() /10.));
  vs[0].setDegFree(6);
  URealUncorr expectUnc = vs[0];
  cout  << vCount << " values, with expected uncertainty "
    << plusminus
    //<< adddegfree
    << addreplicates  // Only show degrees of freedom > 1.
    << expectUnc
    << ", on " << lineCount << " lines." << endl;




  // Output to file using copy.

  setUncDefaults(fout);

  fout << nl << vCount << " uncertain Values with expected uncertainty "
    << plusminus
    << addreplicates // show degrees of freedom if > 1.
    << expectUnc
    << "\n""Input from file:"  << testInputFilename << endl;


  copy(vs.begin() + 1, vs.end(), std::ostream_iterator<URealUncorr>(fout, "\n"));
  // vs.begin() is vs[0] which is expected uncertainty.
  fout << endl;  // vs[1] = 1.10 1.20 1.00 ... 

  fout << vms.size() << "  measurements " << endl;
  copy(vms.begin() + 1, vms.end(), std::ostream_iterator<Meas>(fout, "\n"));
  // vms.begin() is vs[0] which is expected uncertainty.
  fout << endl;  // vs[1] = 1.10 1.20 1.00 ... 

  if(true)
  { 
    std::vector<URealUncorr>::iterator vuBegin = vs.begin() + 1; // 0th is expected unc.
    std::vector<URealUncorr>::iterator vuEnd = vs.end();
    URealUncorr total(0.);
    total = accumulate(vuBegin, vuEnd, total);
    setUncDefaults(cout);
    cout << total << endl;
    sort(vuBegin, vuEnd);
    sort(vuBegin, vuEnd, lessAbs<URealUncorr>() ); // Functor using binary_function in <functional>
    sort(vuBegin, vuEnd, URealUncorr::lessU ); // static member function
    sort(vuBegin, vuEnd, URealUncorr::moreU );
    //sort(vuBegin, vuEnd, URealUncorr::equalU );  // asserts invalid operator<
  }

    // Now calculate mean and check for outliers etc.
  { // Sum uncertain values using while loop.
    cout << vCount << " Uncertain Values:" << plusminus << addlimits << endl;
    std::vector<URealUncorr>::iterator vi = vs.begin() + 1;
    // +1 because 0th value is expected uncertainty.
    unsigned int i = 0;
    URealUncorr sum(0.);
    while(vi != vs.end())
    {
      sum += *vi;
      ++i; // # number from 1 to n
      cout << "Value #" << i << space << *vi 
      // << ", sum " << sum
      << nl; // Value 1 1.10, ...
      ++vi;
    }  // while, leaving vi == vs.end();
    if ( i != (vs.size() - 1) ) // Check i is correct.
      cout << i << space << vs.size() << endl;
    cout << "mean = " << sum / i << endl; 
    }
    
    std::ostream_iterator<Meas> osmit(cout, ", ");  // Set up iterator and comma separator.
    cout.iword(uncFlagsIndex) &= ~plusMinus;  // clear bit 3 = 0 to mean Do NOT add +/-.
    cout << showuncflags << endl;
    cout << nl << "plusMinus " << noplusminus << nolimits << showuncflags << endl;

  // Examples of sorting by various criteria.
    sort(vms.begin() +1, vms.end());  // begin and (past the) end.
    cout << "Sorted Meas vector using default operator<." << nl;
    copy(vms.begin() +1, vms.end(), osmit);  // using std::ostream_iterator osmit for 1-based.
    cout << endl;

    //sort(vms.begin() +1, vms.end(), less);  // begin at [1] and (past the) end.
    // cannot deduce template arguments.  Should be Meas::less


    // Using other member functors precedes and lessU
    //bool Meas::precedes(const Meas& l, const Meas& r); // Needs to be static member function.
    sort(vms.begin() +1, vms.end(), Meas::precedes);  // Sort by m_order.
    cout << "Sorted by order using static member functor Meas::precedes." << nl;
    copy(vms.begin() +1, vms.end(), osmit);  cout << endl;

    cout << nl << "plusMinus " << plusminus << showuncflags << endl;
    cout << vs.at(1) << endl;

    // Demo use of:
//		bool Meas::precedes(const Meas& l, const Meas& r); // Needs to be static member function.
//		bool Meas::lessU(const Meas& l, const Meas& r); // Needs to be static member function.
//		bool Meas::lessU2(const Meas& l, const Meas& r); // Needs to be static member function.

    sort(vms.begin() +1, vms.end(), Meas::precedes);  // Sort by m_order.

    sort(vms.begin() +1, vms.end());  // begin at [1] and (past the) end.
    cout << "Sorted by value using static member functor Meas::less." << nl;
    copy(vms.begin() +1, vms.end(), osmit);	cout << nl << endl;
    sort(vms.begin() +1, vms.end(), Meas::precedes);  // Sort by m_order.
    cout << "Sorted by order using static member functor Meas::precedes." << nl;
    copy(vms.begin() +1, vms.end(), osmit);	cout << nl << endl;

    sort(vms.begin() +1, vms.end(), Meas::lessU);  // Sort by uncertain value within 1 sd.
    cout << "Sorted by uncertain value using static member functor Meas::lessU." << nl;
    copy(vms.begin() +1, vms.end(), osmit); cout << nl << endl;

    sort(vms.begin() +1, vms.end(), Meas::lessU2);  // Sort by uncertain value within 2 sd.
    cout << "Sorted by uncertain value using static member functor Meas::lessU2." << nl;
    copy(vms.begin() +1, vms.end(), osmit); cout << nl<< endl;

    sort(vms.begin() +1, vms.end(), Meas::lessAbsM);  // begin at [1] and (past the) end.
    cout << "Sorted using functor lessAbsM." << nl;
    copy(vms.begin() +1, vms.end(), osmit); cout<< nl << endl;



  // Now consider use of Wilcoxon test.
    fin.close(); // Input file.
    fout.close();  // Output file.
    cout << '\n' << __FILE__ << " ended " <<  __TIMESTAMP__ << endl;
} // BOOST_AUTO_TEST_CASE(quan1_test_3)

    //return 0;


/*


*/
  /*
  { // Output to console using while loop.
    cout << vCount << " Uncertain Values input from file:" << plusminus << adddegfree << endl;
    std::vector<URealUncorr>::iterator vi = vs.begin() + 1; // 0th value is expected uncertainty.
    int i = 1;
    while(vi != vs.end())
    {
      // cout << "Value " << i << space << *vi << endl; // Value 1 1.10, ...
      ++i;
      ++vi;
    }  // while, leaving vi == vs.end();
  }
  // Copy to two C arrays containing separate values and uncertainties.
  // Use vector or valarray instead?
  double* vvs = new double[vCount+1]; // Values.
  float* vus = new float[vCount+1];	// Uncertainties.
  {	// Output using for loop.
    int count = 1;
    for (std::vector<URealUncorr>::iterator vi = vs.begin()+1; vi != vs.end(); ++vi, ++count)
    {
      cout << "Value #" << count << space << *vi << space; // "Value# 1 1.10 implicit"
      cout << (((*vi).getUncTypes() & UNC_IMPLICIT) ? "implicit" : "explicit") << endl;
      vvs[count] = (*vi).getValue();
      vus[count] = (*vi).getStdDeviation();
    }	// for
    cout << endl; // Value 2 1.20  Value 3 1.00
  }
  
  cout << setprecision(16) << endl;  // full precision, including noisy digit.
  
  {	// Check against expected uncertainty using a for loop.
    int count = 1;
    for (std::vector<URealUncorr>::iterator vi = vs.begin()+1; vi != vs.end(); ++vi, ++count)
    {
      if ((vus[count] - expectUncValue) > expectUncUnc ) // absolute uncertainty.
      {
        bool im = ((*vi).getUncTypes() & UNC_IMPLICIT);
        cout << "Value " << count << space << *vi << ", "
          << (((*vi).getUncTypes() & UNC_IMPLICIT)  ? "implicit" : "explicit")
          << " uncertainty "
          << vus[count]
          << " is greater than expected "<< vs[0] << '!' << endl;
      }
    }	// for
  }
  
  delete[] vvs;  // Need explicit destruction.
  delete[] vus;
    */	// Change in global locale, and thus that imbued to input and output,
  // used to check that uncertainRead and uncertainPrint
  // do fail (as expected) for input 1.23.

  //locale krautLocale(std::locale::classic(), locale("German_Germany"), locale::numeric | locale::monetary); // category
  //locale::global(krautLocale);
  //// making decimal point a comma not a period.
  ////locale krautLocale("German_Germany");
  //locale aussieLocale ("English_Australia");
  //locale frogLocale("French");
  //BOOST_CHECK(use_facet<ctype<char> >(aussieLocale).is(std::ctype_base::alpha, 'a') == true);

  //const numpunct <char>& npunctK = use_facet <numpunct <char> >(krautLocale);
  //cout << krautLocale.name( ) << " truename "<< npunctK.truename() << endl;
  //cout << aussieLocale.name( ) << " falsename "<< npunctK.falsename() << endl;
  //BOOST_CHECK(npunctK.decimal_point() == ',');

  //BOOST_CHECK(use_facet <numpunct <char> >(frogLocale).truename() == "true");
  //BOOST_CHECK(use_facet <numpunct <char> >(aussieLocale).decimal_point() == '.');

  //cout.imbue(locale("German_Germany")); //  X Open == De_DE
  //URealUncorr v(1.234, 0.002F, 20);
  //cout << " v(1.234, 0.002, 20) == "<< v << endl; // v(1.234, 0.002, 20) == 1,2340 Note , !
                
/*								

other code to handle file input?

{
  std::vector<URealUncorr> vs(4);
  cout << "Space for " << vs.size() << " values." << endl;
  int vNumber = 0;
  for (std::vector<URealUncorr>::iterator it = vs.begin();
  it != vs.end(); ++it, ++vNumber)
  {  // do all tests.
    vs[vNumber] = *it;
    cout << "vs " << vNumber << ": " << *it << ' ';
    istringstream myIstream(line); // "  1.2 +/- 0.1 ";
    // Read value and deduce if int or real, and implied uncertainty.
    // 1, 10, 100, 1000 imply exact integer.
    // 100. implies 100.0 +/- 0.5
    // 100.0 +/- 5.0 , 100 +/- 50 can only be input with explicit +/-
    // 100.0 implies 100.00 +/- 0.05
    // 1.0 & 1.1  implies +/- 0.05
    // 1.10 & 1.23 implies +/- 0.005
    // Can't fractional part as int because leading zero is significant.
    // Can read first part before . as an int cos leading zeros carry no info.
    
    int avail = myIstream.rdbuf()->in_avail();
    // cout << " (" << avail << ")   ";
    if (avail == 0)
    {
      cerr << "No input!" << endl;
    }
  }
}

//	bool operator<(const MyInt& x) const { return _value < x._value; }
//	bool operator==(const MyInt& x) const { return _value == x._value; }


------ Build started: Project: Quan1, Configuration: Debug Win32 ------
  Running 4 test cases...
  Platform: Win32
  Compiler: Microsoft Visual C++ version 10.0
  STL     : Dinkumware standard library version 520
  Boost   : 1.49.0
  Quan1 test: TestQuan1.cpp at "Wed Jan 18 12:44:01 2012", MSVC version 160040219.
  Quan1 Test output to Quan1Out.txt Wed Jan 18 12:44:01 2012
  Quan1 input from Quan1In.txt Wed Jan 18 12:44:01 2012
  top of lower 1.1 = 1.2
  bottom of higher 1.1 = 1
  sqrt_sumsquares sds = 0.141421
  Difference of values in max sds between 1.10 +/-0.2 and 1.10 +/-0.2 == 0
  Difference in sds between 1.10 +/-0.2 and 1.10 +/-0.2 == 0
  Difference in sds between 1.40 +/-0.2 and 1.10 +/-0.2 == 2.12132
  Difference in sds between 1.60 +/-0.2 and 1.10 +/-0.2 == 3.53553
  Means 1.10 +/-0.2	1.40 +/-0.2, Difference of means is 0.30 +/-0.3
  SDs 0.1	0.1
  hypot sds 0.141421
  Degrees of freedom 1	1
  Standard errors 0.01	0.01, mean se 0.01 sqrt() 0.141421
  Variances 0.01	0.01
  variance/df 0.01	0.01
  df top 0.0004
  df bot 0.0002
  df 2
  sqrt(varlln + varhhn) 0.141421
  Student's t 2.12132, df 2
   l == h 
  (less<double>(1.2, 3.4 )) 
  Test member predicate function 1.10 +/-0.2 < 1.10 +/-0.2
  (lessUnc(l, h))  l ~= h 
   l 1.10 +/-0.2 ~=  h 1.10 +/-0.2
   ll 1.10 +/-0.2 !=  hh 1.40 +/-0.2
   l 1.10 +/-0.2 ~=  h 1.10 +/-0.2
   ll 1.10 +/-0.2 ~=  hh 1.40 +/-0.2
  Testing static memberfunction predicate lessU<isCorrelated>
  1.10 +/-0.2 is < 1.40 +/-0.2
   lc < hc 
  1.10 +/-0.2 is < 1.40 +/-0.2
  lessU 1.1 1.2  >=  1 1.14142 1.1 0.141421 0.2
  lessU 1.1 1.2  <  1.3 1.44142 1.4 0.141421 -0.1
  lessU 1 1  <  2 2 2 0 -1
  lessU 1.1 1.2  <  1.3 1.44142 1.4 0.141421 -0.1
  lessU 1.1 1.3  <  1.2 1.48284 1.4 0.282843 0.1
  lessU 1.1 1.3  <  1.4 1.68284 1.6 0.282843 -0.1
  lessU 0 0  <  1.3 1.4 1.4 0.1 -1.3
  lessU 1.1 1.2  <  99 99.1 99 0.1 -97.8
  1.10 +/-0.2 is << 1.40 +/-0.2
  1.10 +/-0.2 is << 1.60 +/-0.2
   0. < 1.40 +/-0.2
  1.10 +/-0.2 is < 99. +/-0
  1.10 +/-0.2 is <= 99. +/-0
  1.10 +/-0.2 is ~= 1.10 +/-0.2
  Space reserved for 20 values.
  is.eof() == true after 1.1000 +/-0.001(no unit).
  is.eof() == true after 0.9700 +/-0.005(no unit).
  6 values, with expected uncertainty 0.0500 +/-0.01, on 3 lines.
  6.620
  6 Uncertain Values:
  Value #1 1.2500 +/-0.003
  Value #2 1.20 +/-0.03
  Value #3 1.10 +/-0.03
  Value #4 1.1000 +/-0.001
  Value #5 1.00 +/-0.03
  Value #6 0.9700 +/-0.005
  mean = 1.103 +/-0.007
  
  
  plusMinus 
  Sorted Meas vector using default operator<.
  0.97 # 6, 1 # 3, 1.1 # 1, 1.1 # 4, 1.2 # 2, 1.25 # 5, 
  Sorted by order using static member functor Meas::precedes.
  1.1 # 1, 1.2 # 2, 1 # 3, 1.1 # 4, 1.25 # 5, 0.97 # 6, 
  
  plusMinus 
  1.2500 +/-0.003
  Sorted by value using static member functor Meas::less.
  0.97 # 6, 1 # 3, 1.1 # 1, 1.1 # 4, 1.2 # 2, 1.25 # 5, 
  
  Sorted by order using static member functor Meas::precedes.
  1.1 # 1, 1.2 # 2, 1 # 3, 1.1 # 4, 1.25 # 5, 0.97 # 6, 
  
  Sorted by uncertain value using static member functor Meas::lessU.
  0.97 # 6, 1 # 3, 1.1 # 1, 1.1 # 4, 1.2 # 2, 1.25 # 5, 
  
  Sorted by uncertain value using static member functor Meas::lessU2.
  0.97 # 6, 1 # 3, 1.1 # 1, 1.1 # 4, 1.2 # 2, 1.25 # 5, 
  
  Sorted using functor lessAbsM.
  0.97 # 6, 1 # 3, 1.1 # 1, 1.1 # 4, 1.2 # 2, 1.25 # 5, 
  
  
  TestQuan1.cpp ended Wed Jan 18 12:44:01 2012
  lessU 1 1.0125  >=  0.9675 0.980248 0.97 0.0127475 0.045
  lessU 1 1.0125  >=  0.9675 0.980248 0.97 0.0127475 0.045
  lessU 1.1 1.1125  >=  0.9675 0.980248 0.97 0.0127475 0.145
  lessU 1.1 1.1125  >=  0.9875 1.00518 1 0.0176777 0.125
  lessU 1.1 1.1005  >=  0.9675 0.97005 0.97 0.00254951 0.133
  lessU 1.1 1.1005  >=  1.0875 1.10001 1.1 0.01251 0.013
  lessU 1.2 1.2125  >=  0.9675 0.980248 0.97 0.0127475 0.245
  lessU 1.2 1.2125  >=  1.0995 1.11201 1.1 0.01251 0.113
  lessU 1.25 1.25125  >=  0.9675 0.970295 0.97 0.00279509 0.28375
  lessU 1.25 1.25125  >=  1.1875 1.20006 1.2 0.0125623 0.06375
  MU #2 1.2 1.2  >=  1.1 1.1 #1 1.1
  MU #2 1.2 1.2  >=  1.1 1.1 #1 1.1
  MU #3 1 1  <  1.1 1.1 #1 1.1
  MU #1 1.1 1.1  >=  1 1 #3 1
  MU #4 1.1 1.1  >=  1 1 #3 1
  MU #4 1.1 1.1  <  1.2 1.2 #2 1.2
  MU #2 1.2 1.2  >=  1.1 1.1 #4 1.1
  MU #4 1.1 1.1  >=  1.1 1.1 #1 1.1
  MU #5 1.25 1.25  >=  1 1 #3 1
  MU #5 1.25 1.25  >=  1.2 1.2 #2 1.2
  MU #6 0.97 0.97  <  1 1 #3 1
  MU #3 1 1  >=  0.97 0.97 #6 0.97
  U2 #3 1 1  >= 0.97 0.97 #6 0.97
  U2 #3 1 1  >= 0.97 0.97 #6 0.97
  U2 #1 1.1 1.1  >= 0.97 0.97 #6 0.97
  U2 #1 1.1 1.1  >= 1 1 #3 1
  U2 #4 1.1 1.1  >= 0.97 0.97 #6 0.97
  U2 #4 1.1 1.1  >= 1.1 1.1 #1 1.1
  U2 #2 1.2 1.2  >= 0.97 0.97 #6 0.97
  U2 #2 1.2 1.2  >= 1.1 1.1 #4 1.1
  U2 #5 1.25 1.25  >= 0.97 0.97 #6 0.97
  U2 #5 1.25 1.25  >= 1.2 1.2 #2 1.2
  
  *** No errors detected
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========



*/