/*!
  \file
  \brief Common rounding and 'proper' rounding.
  \details
    http://www.diycalculator.com/popup-m-round.shtml#A3
    all rounding types, including round-to-half and asymmetric and symmetric versions.\n
    http://www.chem1.com/acad/webtext/pre/mm3.html
     "The purpose in rounding off is to avoid expressing a value to a greater degree of precision
     than is consistent with the uncertainty in the measurement."
     "Observed values should be rounded off to the number of digits
     that most accurately conveys the uncertainty in the measurement."
 */

//   \file rounding.hpp

// Copyright Paul A. Bristow 2009, 2012, 2020.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/*! mainpage <b> Proper rounding testbed documentation. </b>
 *
 * \section intro_sec Introduction to proper rounding.

   This is a testbed for the rounding_loss-Proper rounding for a few distributions
   assumed to be appropriate for the data observations.\n\n
   Based on:\n
    Gejza Wimmer, Viktor Witkovsky, Tomas Duby \n
    Measurement Science and Technolology, 11 (2000) pages 1659-1665.
    ISSN 0957-0233 S0957-233(00)13838-X.\n
    Proper rounding of the measurement results under normality assumptions.\n\n
    Gejza Wimmer, Viktor Witkovsky, Proper rounding of the measurement result
    under the assumption of uniform distribution, \n
    Measurement Science Review, Vol 2, section 1, (2002), pages 1 - 7.\n\n
    Gejza Wimmer, Viktor Witkovsky, Proper rounding of the measurement result
    under the assumption of triangular distribution, \n
    Measurement Science Review, Vol 2, section 1, (2002), pages 21 to 31.\n
 */

#include <cmath>
// using std::floor;
// using std::pow;
// using std::log10;

#include <boost/math/special_functions/fpclassify.hpp>
//using boost::math::isnan;
//using boost::math::isinf;
//using boost::math::isfinite;
//using boost::math::isnormal; // isfinite and not denormalised.

#include <boost/math/special_functions/sign.hpp>
// using boost::math::signbit;
// using boost::math::sign;

#include <boost/math/distributions/students_t.hpp>
// using boost::math::students_t;
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/uniform.hpp>
#include <boost/math/distributions/triangular.hpp>
// using boost::math::normal;
//#include "pair_io.hpp"
// operator<<

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp> // is_floating_point trait
#include <boost/concept_check.hpp>
#include "boost/quan/xiostream.hpp" // Extra std items, defaultfloat & fixed.
//#include "boost/quan/si_units.hpp"
//#include "libs/quan/src/si_units.cpp"

#include <iostream>
//using std::cout;
//using std::cerr;
//using std::cin;
//using std::endl;
//using std::ends;
//using std::dec;
//using std::hex;
//using std::fixed;
//using std::left;
//using std::right;
//using std::showpoint;
//using std::nofixed;
//using std::noscientific;
//using std::defaultfloat;
//using std::noadjust;
//using std::scientific;
//using std::boolalpha;
//using std::showpos;
//using std::noshowpos;

#include <iomanip>
//using std::setprecision;
//using std::setw;
#include <string>
//using std::string;

#include <fstream>  // for fstream
//using std::fstream;
//using std::ofstream;

#include <sstream> // stream
//using std::ostringstream;
//using std::string;
//using std::basic_string;
//using std::ios_base;

#include <limits>
//using std::numeric_limits;

#include <locale>
// default locale for isdigit

BOOST_STATIC_ASSERT(std::numeric_limits<double>::is_iec559); // Assume IEEE 754 ONLY.
// _STATIC_ASSERT (numeric_limits<double>::is_iec559); // and MS STATIC assert.

// Might also work by checking that just std::numeric_limits is specialized?
// This might allow some embedded systems to use this software.
BOOST_STATIC_ASSERT(std::numeric_limits<double>::is_specialized); //

#ifdef BOOST_NO_CXX11_NUMERIC_LIMITS
// No support for std::numeric_limits<double>::max_digits10, so compute max_digits10
const unsigned int maxdigits10 = 2 + std::numeric_limits<double>::digits * 3010 / 10000;
#else
// std::numeric_limits<double>::max_digits10; IS supported.
// Any noisy or guard digits needed to display a one-bit difference are included in max_digits10.
const unsigned int maxdigits10 = std::numeric_limits<double>::max_digits10;
#endif

// Explicit specialization for output of a pair of doubles.
std::ostream& operator<<(std::ostream& os, std::pair<double, double>& p)
{ /*! Output a pair of `double`s, using < > angle brackets and comma separator, using current stream's precision..
  \details Explicit specialization for `std::pair` for `double`s.\n
  For example: <97.8725, 157.798>
  \param p Pair of doubles.
  \param os std::ostream for string output.
  */
  return os << "<" << p.first << ", " << p.second << '>'; // Angle bracketed, separated by comma & space.
}

// Two rounding algorithms returning double values (not strings).
template<typename FPT> FPT round_sig(FPT v, int n); // v rounded to n significant decimal digits.
template<typename FPT> FPT round_to_n(FPT v, int p); // Round value v to p decimal digits *after the decimal point*.

// Two rounding algorithms returning strings.
template<typename FPT> std::string round_f(FPT v, int sigdigits); // Round fixed (not-exponential) to sigdigits decimal digits to string.
//template<typename FPT> std::string round_e(FPT v, int sigdigits); // Round fixed (exponential) to sigdigits decimal digits to string.
// Not yet implemented?
template<typename FPT> std::string round_ms(FPT v, int m); // Round not-exponential to order m.

template<typename FPT>
FPT round_sig(FPT v, int n) { /*! \brief Returns v rounded to n significant decimal digits.\n
   http://www.jason.mock.ws/wordpress/2007/02/22/round-a-float-to-of-significant-digits,
   David A. Pimentel
   \details These use log10 and pow and so are vulnerable to binary rounding errors,
   as the value may not be exactly representable for the type specified.
   Should give the same value as printf precision.
   (int)(x < 0 ? x - 0.5 : x + 0.5))
   So do we need to do something different for negative doubles?
   The difference between symmetric and asymmetric rounding?
  */
  // Will fail if FPT is not a floating-point type (because will not output in scientific format!).
  BOOST_STATIC_ASSERT(boost::is_floating_point<FPT>::value);

  using std::pow;
  using std::log10;
  using std::ceil;
  using std::floor;

  if (n < 0) { // Probably a program error, so might throw?
    return 0;
  } else if (n == 0) { // Might also be an error, but make some sense, so
    return 0.;
  } else if (n > std::numeric_limits<FPT>::digits10) { // We might use max_digits10, but only digits10 are guaranteed.
    n = std::numeric_limits<FPT>::digits10;
    return v; // or just return value as is?
  }
  if (v == 0) {
    return 0.;
  }
  FPT l10 = log10(v);
  int l = static_cast<int> (ceil(l10));
  int pow10 = n - l;
  FPT p10 = pow(10., pow10);
  FPT f = v * pow(10., pow10) + 0.5;
  f = floor(f);
  FPT r = f / p10;
  return r;
  // return floor(v * pow(10., pow10) + 0.5) / pow(10., pow10);
} // FPT round_sig(FPT v, int n)

template<typename FPT>
FPT round_to_n(FPT v, int p)
{ /*! Round value v to p decimal digits @b after the decimal point.
    http://www.jason.mock.ws/wordpress/2007/02/22/round-a-float-to-of-significant-digits
    \note These use log10 and pow and so are vulnerable to binary rounding errors,
    as the value v may not be exactly representable for the type specified.
    Should give the same value as printf precision.

    \param v Value to round.
    \param p Number of decimal digits @b after the decimal point.
  */
  // Will fail if FPT is not a floating-point type (because will not output in scientific format!).
  BOOST_STATIC_ASSERT(boost::is_floating_point<FPT>::value);

  using std::pow;
  using std::floor;

  return static_cast<FPT> (floor(v * pow(10., p) + 0.5) / pow(10., p));
} // template<typename FPT> FPT round_to_n(FPT v, int p)

template<typename FPT>
std::string round_e(FPT d, int sigdigits)
{ /*! \brief Show only `sigdigits` digits, in minimal scientific or 'e' format only,
     rounded asymmetric (round-half-up, arithmetic rounding).\n
     Round asymmetric avoids values like 0.15,
     which cannot be stored exactly in binary IEEE 754 floating-point format
     and are stored internally as 1.499999999999, being rounded unexpectedly to 0.1 rather than 0.2.
     For example: format -1.2345e12 or 1.2345e-123.

     \note the difference in meaning of precision from default, fixed and scientific, for example:
     `d = 19.99;` \n
     `std::cerr << setprecision(2) << d << std::endl; // 20 // rounded`\n
     `std::cerr << fixed << setprecision(1) << d << std::endl; // 20.0 - rounded up.`\n
     `std::cerr << fixed << setprecision(2) << d << std::endl; // 19.99 - 2 digits after decimal point.`\n
     `std::cerr << scientific << setprecision(2) << d << std::endl; // 2.00e+001 - 2 digits after decimal point`

    \details Also remove all redundant exponent characters and digits,
    See http://en.wikipedia.org/wiki/Rounding .

    \note The normal rounding provided by C and C++ is chosen to avoid loss of accuracy and bias
    in any further @b binary calculations.
    The rounded decimal digit @b strings returned are provided solely for @b human viewing
    and should NOT be used for input to other calculations when one or
    two extra noisy digits are needed to avoid loss of information by rounding.
    If it is necessary to provide a decimal digit string for input,
    it should use @c std::numeric_limits<FPT>::max_digits10 in C++0x,
    or @c 2 + std::numeric_limits<FPT>::digits * 3010/10000
    if the platform implementation of C++ Standard IOstream library does not yet provide this.
    For 64-bit double, @c max_digits10 is 17, so write @c cout.precision(17).
    This will avoid any bias or loss of accuracy in statistical calculations caused by rounding.

    Use the scientific (exponential) format with precision max_digits10 (17 for double).

    Logic is using C++ std printf-style rounding to start with a decimal digit string with rounded value
    of digits10, only the guaranteed correct digits
    to get the best estimate of the digit to be used for the final rounding.
    Hopefully, any imprecision caused by inexact presentation is (binary) rounded away.

    \sa http://www.exploringbinary.com/ for much more detail of defects in other libraries.

    \note Only lowercase letter e is used, and positive exponents are not preceeded by + sign,
    and leading zeros are omitted.\n
    This is done to make the display as compact as possible.\n
    If a fixed width is needed, the user program must provide any necessary padding,
    using the size (length) of the string.

    \param d value to be converted to decimal digit string.
    \param sigdigits number of significant digits to show in string.
    \return std::string containing value rounded and converted to `sigdigits` decimal digits in exponential format.
  */
  // Will fail if FPT is not a floating-point type (because will not output in scientific format!).
  BOOST_STATIC_ASSERT(boost::is_floating_point<FPT>::value);

  if (boost::math::isinf(d)) {
    return (boost::math::signbit(d) ? "-Inf" : "Inf");
  }
  if (boost::math::isnan(d)) {
    return (boost::math::signbit(d) ? "-NaN" : "NaN");
  }
  if (sigdigits <= 0) {
    std::cout << "Uncertain warning: Trying to display " << sigdigits << " significant decimal digits!" << std::endl;
    return ""; // Or throw?
  }

  //const unsigned int max_digits10 = 2 + std::numeric_limits<FPT>::digits * 3010/10000;
  if (sigdigits >= std::numeric_limits<FPT>::digits10) // C++11 provides std::numeric_limits<double>::max_digits10;
  {
    sigdigits = std::numeric_limits<FPT>::digits10; // digits10 (15 for double) decimal digits
    // *after* the decimal point, so a total of 16 digits (plus a decimal point).
  }

  BOOST_STATIC_ASSERT(boost::is_floating_point<FPT>::value);
  // Will fail if FPT is not a floating-point type (because will not output in scientific format!).

  std::ostringstream ss;
  ss << std::scientific // use 9.99e+123 format
          << std::showpoint // force decimal point and trailing zeros.
          //<< setprecision(sigdigits)
          // output precision decimal digits *after the leading digit and decimal point*
          //  in scientific mode, so this is one more than significant digits.
          // Or should this be digits10 so that any imprecision
          // caused by inexact presentation is rounded away?
          << std::setprecision(std::numeric_limits<FPT>::digits10) //
          // output precision is
          // decimal digits *after the leading digit and decimal point*, total digits10+1 digits.
          << d;
  std::string s = ss.str(); // "1.294567890123457e+006"
  // std::cout <<'|' << s <<'|' << ' ' << s.size() << std::endl;
  //assert(s.size() == std::numeric_limits<FPT>::digits10 + 1 + 1 + 5); // == 22 if positive

  std::string::iterator is = s.begin(); // 1st most significant digit before decimal point.
  if ((*is) == '-') { // Skip over minus sign.
    // Can ignore + case as won't ever be output because default std::ostringstream is std::noshowpos.
    is++;
  }
  char s1 = *is; // Copy 1st digit before decimal point.
  std::string::iterator is1 = is; // Copy 1st digit before decimal point.
  assert((*is1 >= '0') && (*is1 <= '9'));
  is++; // Move to decimal point.
  assert((*is) == '.');
  // Do not delete decimal point - it will be needed.
  std::string::iterator isp = is; // Decimal point. Implied position after 1st most significant digit is known.
  assert((*isp) == '.');

  // Read the exponent part first into an integer.
  std::string::iterator se = is1 + std::numeric_limits<FPT>::digits10 + 2;
  assert(*se == 'e');
  se++; // skip 'e'.
  // Read exponent into an integer.
  bool neg_exp = ((*se) == '-') ? true : false;
  se++; // Skip over sign of exponent.
  std::locale loc;
  int exp = 0;
  do
  {
    if (std::isdigit(*se, loc))
    {
      exp = exp * 10 + (*se - '0');
    }
    se++;
  }
  while (se != s.end());

  if (neg_exp)
  {
    exp = -exp; // Make exponent signed.
  }

  // std::cout << "Exponent " << ((neg_exp) ? '-' : '+') << exp << std::endl;

  std::string::iterator is2 = is1 + 2; // 2nd digit (after decimal point).
  assert((*is2 >= '0') && (*is2 <= '9'));
  std::string::iterator isr = is2 + sigdigits - 1; // 2nd digit (after decimal point) if sigdigits == 1.
  assert((*isr >= '0') && (*isr <= '9'));
  int rounder = (*isr) - '0';
  // std::cout << "rounder " << rounder << std::endl;
  is = isr; // Least significant digit.
  is--;
  if (is == isp)
  { // Skip back over decimal point.
    is--;
  }
  assert((*is >= '0') && (*is <= '9'));

  s.erase(isr, s.end()); // Erase all the insignificant trailing decimal digits, and exponent.

  // Round the non-exponent part, in case a carry means we must increment the exponent.
  // For example, 9.9e0 will round up to 10. and so become 1e1.
  bool carry = false;
  if (rounder >= 5) {
    carry = true; // Increment next most significant digit.
    do {
      if (*is < '9') // 0 to 8.
      {
        (*is)++; // round up by 1.
        carry = false;
      } else if (*is == '9') {
        *is = '0';
        carry = true;
      } else {
        std::cout << "wrong ! *is = " << *is << std::endl;
        break;
      }
      if (is == is1) { // No more significant digit(s).
        break;
      }
      is--; // Next more significant digit.
      if (is == isp) { // Skip over decimal point.
        is--;
      }
      if (is == is1) {
        break;
      }
    } while (carry == true);
  } else { // Rounder = 0 to 4, so nothing to do.
    carry = false;
  }
  if (carry == true) { // Add one to 1st most significant digit.
    if (s1 == '9') { // Carry to most significant 1st digit '9' means now preceeded by a '1' digit.
      *is1 = '1';
      exp++; // Add one to signed exponent.
      // What if it exceeds max exponent?
    } else { // 0 to 8, so just add one.
      (*is1)++;
    }
  }
  if (exp != 0) { // Add e, - sign if necessary and exponent
    s += ('e'); // Since the stream is not known, there is no way to know if to use uppercase 'E'.
    if (neg_exp) { // Make unsigned.
      s += ('-');
      exp = -exp;
    }
    if (exp >= 100) { // Need exponent hundreds digit.
      int ed = exp / 100;
      s += static_cast<char> (ed + '0');
    }
    exp %= 100;
    if (exp >= 10) { //  // Need exponent tens digit.
      int ed = exp / 10;
      s += static_cast<char> (ed + '0');
    }
    exp %= 10; //  // Always need exponent units digit.
    s += static_cast<char> (exp + '0');
  }
  // std::cout << "s = " << s << std::endl;
  return s; // Rounded decimal digits string.
} // string round_e(FPT d, unsigned int sigdigits)

template<typename FPT>
std::string round_ms(FPT v, signed int m) {
  /*!
  \brief Round floating-point v (not-exponential) to order m.  (m is the index of the roundER digit).
    This is variously called 'common rounding', 'round_5_up'.
    \details Gejza Wimmer, Viktor Witkovsky, Tomas Duby\n
    Measurement Science and Technology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X\n
    Proper rounding of the measurement results under normality assumptions.\n
    Uncertainty of measurement -- Part 3: Guide to the expression of uncertainty in measurement (GUM:1995)\n
    ISO Guide 98 (1995) and updated version 2008.\n\n

    \note `m` is the index of the rounder digit, that is the just insignificant digit
    used to decide if the m+1th digit is to be rounded up or left as is.
    So `m == 0` means that the rounder digit is the units digits, used to round the tens digit,
    `m == +1` the rounder is the tens digit, and the rounded digit is the hundreds digit
    `m == -1` rounder is the tenths digit (0.1), rounding the hundredths digit (0.01).

    (Other authors specify the nth digit to be significant, where n = m + 1).

    \param v Value to convert to decimal digit string after rounding.
    \param m Signed digit position to round.
    \return `std::string` decimal digit string of rounded value.
  */
  BOOST_STATIC_ASSERT(boost::is_floating_point<FPT>::value);
  // Will fail if FPT is not a floating-point type (because will not output in scientific format!).

  // Use Boost.Math portable functions for testing infinity, NaN and their signs.
  using boost::math::signbit;
  using boost::math::isnan;
  using boost::math::isinf;
  int is_neg = signbit(v);
  if (isnan(v)) { //! \note that the most significant sign bit of NaN is recognized by using function signbit,
    //! 'sign' of NaNs cannot reliably and portably be tested using "x < 0"
    //! because all comparisons using NaN are false - by definition.
    return (is_neg) ? "-NaN" : "NaN";
  } else if (isinf(v)) {
    return (is_neg) ? "-inf" : "inf";
  }
  // Treat denormal differently too?

  // m can be up to about exponent 308.
  //  static  const int maxDecimalDigits = 1 + 1 + numeric_limits<double>::max_exponent + (2 + numeric_limits<double>::digits * 3010/10000)+1+1;
  // Number of decimal digits to display FPT max in fixed format with lots of trailing zeros & decimal point.
  // ostringstream smax;
  //  smax << setprecision(17) << fixed << -numeric_limits<double>::max() << endl; // numeric_limits<double>::max() = 1.7976931348623157e+308 but using fixed.
  // std::cout << smax.str() << std::endl;
  //-179769313486231610000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.00000000000000000
  // std:: << "length of smax " << smax.str().size() << std::endl; // 329
  // sign + max_digits10 (17+1 = 18) + 293  significant zeros + decimal point + 17 trailing digits (zeros).
  // max_exponent = 308, so expect 308 +1 = 309 digits before decimal point.
  // of these 309, 17 may be non-zero, but always have one digit so only 16, so 293 zeros.
  // 329 is longest decimal digit string that possibly can be output with double exponent.
  // But this function never wants to output the 'noisy' extra digits, so limit to digits10 not max_digits10.
  // So want sign + digits10 (15)
  //   + (max_exponent10 - digits10) (308-15 = 293) significant zeros
  //   + decimal point,
  //   and upto digits10 trailing zeros.
  static const int maxDecimalDigits = 1 + (std::numeric_limits<FPT>::max_exponent10
          - std::numeric_limits<FPT>::digits10) + 1 + std::numeric_limits<FPT>::digits10;
  //std::cout << "max dec " << maxDecimalDigits << std::endl; // 310

  if (m > maxDecimalDigits) { // limit abs(m) to maxDecimalDigits.
    m = maxDecimalDigits;
  }
  if (m < -maxDecimalDigits) {
    m = -maxDecimalDigits;
  }

  std::ostringstream ss;
  ss << std::scientific // Use scientific E format.
          << std::showpoint; // Force decimal point and trailing zeros.
  // skipws showpoint dec scientific

  ss.precision(std::numeric_limits<FPT>::digits10); // Output digits10 precision decimal digits (and decimal point).
  // std::numeric_limits<double>::digits10 is 15 for 64-bit double.
  // output precision decimal digits *after the leading digit and decimal point*, total digits10+1 digits.
  ss << v;
  std::string s = ss.str();

  std::string::iterator is = s.begin();
  if ((*is) == '-') { // delete minus sign.
    // Can ignore + case as won't ever be output as default std::ostringstream is std::noshowpos.
    // is_neg = true; already noted above using signbit.
    s.erase(is); // is++;
  }
  // std::cout <<'|' << s <<'|' << ' ' << s.size() << std::endl; // For example: |1.797693134862316e+308| 22
  /*
  MSVC always was 3 exponent digits  (not longer true, now following the C++ standard).
  Recent  MSVC and GCC and Clang can all now have two or three exponent digits 2021
  0.00000000000000000e+00
  1.23456789012345669e+00
  1.23456789012345666e+03
  1.23456789012345671e-03
  1.23456789012345658e+200
  1.23456789012345671e-200
   */

//#ifdef _MSC_VER
//  if (s.size() != std::numeric_limits<FPT>::digits10 + 1 + 1 + 5) // exponent sign,
//  { // for double == 21 (would be 23 if value was negative, but we are only dealing with abs value).
//    // 1.23000000000000000e+00
//    outFmtFlags(ss.flags());
//    std::cout << "\n" << v << ", " << m << ", " << s << ", "
//            << s.size() << ", " << std::numeric_limits<FPT>::digits10 + 1 + 1 + 5 << std::endl;
//    // assert(s.size() == std::numeric_limits<FPT>::digits10 + 1 + 1 + 5);
//    // 1.23, -2, 1.230000000000000e+00, 21, 22
//    // This happens if type FPT passed is not floating-point (integer?).
//    return s;
//  }
//#endif
//#ifdef (__GNUC__ ||  _MSC_VER)
  // Format varies with size of exponent - either two or three digits.
  if ((s.size() != std::numeric_limits<FPT>::digits10 + 1 + 1 + 4) // e+00
          && (s.size() != std::numeric_limits<FPT>::digits10 + 1 + 1 + 5)) // e+308
  { // for double == 21 (would be 22 if negative).
    outFmtFlags(ss.flags());
    std::cout << "\nFunction round_ms " << v << ", " << m << ", " << s << ", "
            << s.size() << ", " << std::numeric_limits<FPT>::digits10 + 1 + 1 + 4 << std::endl;
    return s;
  }
//#endif
  std::string::iterator is1 = s.begin(); // 1st digit.
  assert(*is1 >= '0');
  assert(*is1 <= '9');
  is++;
  assert(*is == '.');
  s.erase(is); // Erase decimal point. Implied position after 1st most significant digit is known.
  std::string::iterator ie = s.begin() + std::numeric_limits<FPT>::digits10 + 1;
  assert(*ie == 'e');
  std::string::iterator se = s.end(); // After last exponent digit.
  se--; // Least significant exponent 'units' digit.
  assert(*se >= '0');
  assert(*se <= '9');
  int exp = (*se - '0'); // Exponent units digit's value.
  se--;
  assert(*se >= '0');
  assert(*se <= '9');
  std::locale loc;
  if (std::isdigit(*se, loc))
  { // Exponent tens.
    exp += (*se - '0') * 10; // Exponent tens.
  }
  se--; // Previous might be exponent hundreds digit (always for MSVC), or sign.
  if (std::isdigit(*se, loc))
  { // Exponent tens digit.
    exp += (*se - '0') * 100; // Exponent hundreds.
    se--;
  }
  if (*se == '-') {
    exp = -exp;
  } else if (*se == '+') {
    exp = +exp;
  } else {
    std::cout << "Function round_ms expected digit or sign!" << *se << std::endl;
  }
  se--; // Must be the letter 'e' next forward.
  // std::cout << "exponent " << exp << std::endl;
  assert((*se == 'e') || (*se == 'E')); // Check is 'e' or 'E'.
  s.erase(se, s.end()); // Delete all exponent field: "e+123" or "e-01".
  // leaving an digits10 + 1 = 16 decimal digit string ,
  // with implied decimal point after the 1st digit, for example 1234567890123456.
  // std::cout << "digits string = " << s << std::endl;
  // Now round the string at m.
  int p = exp - m; // Offset.
  if (p <= 0) {
    return "0.";
  }

  std::string::iterator sr = s.begin(); //
  int o = (exp - m);
  assert(o > 0);
  if (o > std::numeric_limits<FPT>::digits10) { // No rounding possible.
    o = std::numeric_limits<FPT>::digits10;
  }
  sr += (o);
  // sr* is the rounding digit d subscript m.

  int rounder = *sr - '0'; // Rounding digit.
  std::string::iterator sis = sr; // Note position of the insignificant trailing decimal digits.
  s.erase(sis, s.end()); // Erase all the insignificant trailing decimal digits.

  //sr--; // Least significant digit.
  bool carry = false;
  if (rounder >= 5) {
    carry = true; // Increment next most significant digit.
    do {
      sr--; // more significant digit.
      if (*sr < '9') // 5 to 8
      {
        (*sr)++; // round up by 1.
        carry = false;
      } else
      if (*sr == '9')
      { // 9 will cause a carry.
        *sr = '0';
        carry = true;
      } else {
        std::cout << "Function round_ms is wrong ! *sr = " << *sr << std::endl;
        break;
      }
    } while (carry == true && sr != s.begin());
  } else { // nothing to do.
    carry = false;
  }
  if (carry == true) { // Carry to most significant digit means now preceed by a '1' digit.
    s.insert(s.begin(), '1');
    exp++;
  }
  if (m >= 0) { // May need zeros before decimal point.
    int z = exp - static_cast<int>(s.size()) + 1; // Number of significant zeros before decimal point.
    if (z > 0) { // More efficient to check if any zeros needed before calling insert?
      s.insert(sis, z, '0'); // Insert any significant zeros.
    }
  } else {// Any trailing zeros are already in the string.
  }
  std::string::iterator sd = s.begin();
  if (exp > 0) { // Will try to insert off the end of the string if > maxdigits10+1!
    sd += exp + 1;
    s.insert(sd, '.'); // Insert decimal point.
  } else if (exp == 0) { //
    sd += exp + 1;
    s.insert(sd, '.'); // Insert decimal point.
  } else { // exp < 0
    // Zero before decimal point might be optional?
    s.insert(sd, '0'); // Insert zero before point.
    sd++;
    s.insert(sd, '.'); // Insert decimal point.
    sd++;
    int z = -exp - 1;
    if (z > 0) {
      s.insert(sd, z, '0'); // Insert any zeros before significant digits (0.001234).
    }
  }
  if (is_neg) {
    s.insert(s.begin(), '-');
  }
  //std::cout << "Final digits string " << s << std::endl;

  return s;
} // template <typename FPT> string round_ms(FPT v, signed int m)

bool scaled = true; // want to scale and use prefix to avoid >1000 or < 1.

template <typename FPT>
std::string round_f(FPT v, int sigdigits) { /*! \brief Round floating-point value `v` (fixed, not-exponential) to `sigdigits` significant digits.
   This is variously called 'common rounding', 'round_5_up'.

  \details Gejza Wimmer, Viktor Witkovsky, Tomas Duby\n
    Measurement Science and Technology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X\n
    Proper rounding of the measurement results under normality assumptions.\n

    Uncertainty of measurement -- Part 3: Guide to the expression of uncertainty in measurement (GUM:1995)\n
    ISO Guide 98 (1995) and updated version 2008.

    \param v Value to be converted to a decimal digit string, rounded to sigdigits significant digits.
    \param sigdigits Number of significant digits after rounding.
    \return `std::string` containing decimal digit string, properly rounded.

    \note All these functions are templated on floating-point type and are not intended to be called
    (and thus instantiated) for integral (or other) types.
    This is because the value will not be output in scientific format, assumed by the code.
    A check is_floating_point == true is provide that will fail in debug mode.
    (A compile time check would be better).
  */

  // Will fail if FPT is not a floating-point type (because will not output in scientific format!).
  BOOST_STATIC_ASSERT(boost::is_floating_point<FPT>::value);

  // Use Boost.Math TR1 portable functions for testing infinity, NaN and their signs.
  using boost::math::signbit;
  int is_neg = signbit(v);
  using boost::math::isnan;
  using boost::math::isinf;

  if (isnan(v)) { // Note that the most significant sign bit of NaN is recognized by using function signbit.
    return (is_neg) ? "-NaN" : "NaN";
    // 'sign' of NaNs cannot reliably and portably be tested using "x < 0"
    // because all comparisons using NaN are false - by definition.
  } else if (isinf(v)) {
    return (is_neg) ? "-infinity" : "infinity";
  }
  // Treat denormal differently too?

  // sign + max_digits10 (17+1 = 18) + 293  significant zeros + decimal point + 17 trailing digits (zeros).
  // max_exponent = 308, so expect 308 +1 = 309 digits before decimal point.
  // of these 309, 17 may be non-zero, but always have one digit so only 16, so 293 zeros.
  // 329 is longest decimal digit string that possibly can be output with double exponent.
  // But this function never wants to output the 'noisy' extra digits, so limit to digits10 not max_digits10.
  // So want sign + digits10 (15) + (max_exponent10 - digits10) (308-15 = 293) significant zeros + decimal point, and digits10 trailing zeros.
  static const int maxDecimalDigits = 1 + (std::numeric_limits<FPT>::max_exponent10
          - std::numeric_limits<FPT>::digits10) + 1 + std::numeric_limits<FPT>::digits10;
  //std::cout << "max dec " << maxDecimalDigits << std::endl; // 310

  /*! \note C++0X provides @c std::numeric_limits<double>::max_digits10;
    which is maximum number of possibly significant digits,
    but only @c digit10 are guaranteed to be significant,
    so limit @c sigdigits to @c digits10 (15 for IEEE 64-bit double).
   */
  if (sigdigits > std::numeric_limits<FPT>::digits10) {
    std::cout << "Uncertain warning: Maximum significant digits is " << std::numeric_limits<FPT>::digits10 << std::endl;
    sigdigits = std::numeric_limits<FPT>::digits10; // digits10 (15 for double) decimal digits after the decimal point.
  }

  if (sigdigits < 0) { // Must be a mistake.
    std::cout << "Uncertain warning: Trying to output " << sigdigits << " significant digits!" << std::endl;
    return "";  // Or throw?
  } else if (sigdigits == 0) { // Might handle zero case differently from sigdigits < 0?
    std::cout << "Uncertain warning: Trying to output zero significant digits!" << std::endl;
    return "";
  }
  //else sigdigits > 0, so usable.

  std::ostringstream ss;
  ss << std::scientific // Use scientific format to get all the value as potentially significant decimal digits.
          << std::showpoint // Force decimal point and trailing zeros.
          << std::setprecision(std::numeric_limits<FPT>::digits10) // Output `digits10` precision decimal digits (and decimal point).
          // `std::numeric_limits<double>::digits10` is 15 for 64-bit double.
          // output precision decimal digits *after the leading digit and decimal point*, total digits10+1 digits (16 for double).
          << v;
  std::string s = ss.str(); // digit string like -1.797693134862316e+308.
  std::string::iterator is = s.begin();
  if ((*is) == '-') { // delete minus sign.
    // Can ignore + case as won't ever be output as default std::ostringstream is std::noshowpos.
    //is_neg = true; already noted using signbit.
    s.erase(is); // is++;
  }
  // std::cout <<'|' << s <<'|' << ' ' << s.size() << std::endl; // |1.234567890123457e+000| 22
  assert(
    (s.size() == std::numeric_limits<FPT>::digits10 + 1 + 1 + 5) // == 22
    ||( s.size() == std::numeric_limits<FPT>::digits10 + 1 + 1 + 4) // == 21
    ); // == 21
  // Might be only two exponent digits on some platforms.

  std::string::iterator is1 = is; // 1st digit.
  assert(*is1 >= '0');
  assert(*is1 <= '9');
  is++;
  assert(*is == '.');
  s.erase(is); // erase decimal point.  Implied position after 1st most significant digit is known.
  //std::cout << "digit & exponent string " << s << std::endl;

  // Read the exponent part first into an integer.
  std::string::iterator se = is1 + std::numeric_limits<FPT>::digits10 + 1;
  std::string::iterator ie = is1 + std::numeric_limits<FPT>::digits10 + 1;

  assert(*se == 'e');
  se++; // skip 'e'.
  // Read exponent into an integer.
  bool neg_exp = ((*se) == '-') ? true : false;
  se++; // Skip over sign of exponent.
  std::locale loc;
  int exp = 0;
  do
  {

    if (std::isdigit(*se, loc))
    {
      exp = exp * 10 + (*se - '0');
    }
    se++;
  }
  while (se != s.end());

  if (neg_exp)
  {
    exp = -exp; // Make exponent signed.
  }

  //std::cout << "exponent " << exp << std::endl;
  //se = s.end();
  //se -= 5;
  s.erase(ie, s.end()); // delete everything from e to end, "e+123" or "e-01"
  // leaving an digits10+1 decimal digit string,
  // with implied decimal point after the 1st digit, for example 1234567890123457.
  // std::cout << "decimal digits string " << s << std::endl;
  // now round the string at nth decimal digit.

  std::string::iterator sr = s.begin(); //
  int o = sigdigits;
  assert(o > 0);
  if (o > std::numeric_limits<FPT>::digits10)
  { // No rounding possible.
    o = std::numeric_limits<FPT>::digits10;
  }
  sr += (o);
  // sr* is the rounding digit d subscript sigdigits+1.

  int rounder = *sr - '0'; // rounding digit.
  std::string::iterator sis = sr; // Note position of the insignificant trailing decimal digits.
  s.erase(sis, s.end()); // Erase all the insignificant trailing decimal digits.

  //sr--; // Least significant digit.
  bool carry = false;
  if (rounder >= 5) {
    carry = true; // Increment next most significant digit.
    do {
      sr--; // more significant digit.
      if (*sr < '9') // 5 to 8
      {
        (*sr)++; // round up by 1.
        carry = false;
      } else if (*sr == '9') {
        *sr = '0';
        carry = true;
      } else {
        std::cout << "Function rounder_f is wrong ! *sr = " << *sr << std::endl;
        break;
      }
    } while (carry == true && sr != s.begin());
  } else { // nothing to do.
    carry = false;
  }
  if (carry == true) { // Carry to most significant digit means now preceed by a '1' digit.
    s.insert(s.begin(), '1');
    exp++;
  }
  // Check if are to change exponent and use prefix,
  // or to change to exponential format?
  //int multiple3 = 0;
  //bool is_scaled = false;
  //  if (scaled == true)
  //  { // If necessary change to use a prefix so value is in range 1.000000.. to 999.99999..
  //    if (exp > SImaxPowerTen)
  //    { // No SI names above yocto, so need to use exponential format 1.23e+25.
  //    }
  //    else if (exp < SIminPowerTen)
  //    { // No SI names below yotta, so need to use exponential format 1.23e-25.
  //    }
  //    else if (abs(exp) >= 3)
  //    { // Value >= 1000.0, so can use nearest abbreviation, kilo, mega...
  //      int powerten = exp / 3;
  //      multiple3 = powerten * 3;
  //      exp %= 3; // nearest 10^3 can only be 1.20, 12.0 120.0
  ///*
  //      const char* symbol = SIsymbols[multiple3 + SImaxPowerTen];
  //      const char* prefix = SIprefixes[multiple3 + SImaxPowerTen];
  //      std::cout << powerten << ' ' << symbol << ' ' << prefix << std::endl;
  //*/    is_scaled = true;
  //    }
  //     else if (exp < 0)
  //     { // value < 1.0000000000000
  //     }
  //  }
  // Now have the rounded decimal digit string, but may need some zeros.
  if (sigdigits >= 0) { // May need zeros *before* decimal point.
    int z = exp - static_cast<int>(s.size()) + 1; // Number of significant zeros before decimal point.
    if (z > 0) { // More efficient to check if any zeros are needed before calling insert.
      s.insert(sis, z, '0'); // Insert any significant zeros.
    }
  } else {// Any trailing zeros are already in the string.
  }

  std::string::iterator sd = s.begin();
  if (exp > 0) {
    sd += exp + 1;
    s.insert(sd, '.'); // Insert decimal point.
  } else if (exp == 0) { //
    sd += exp + 1;
    s.insert(sd, '.'); // Insert decimal point.
  } else { // exp < 0
    // Zero before decimal point might be optional?
    s.insert(sd, '0'); // Insert zero before point.
    sd++;
    s.insert(sd, '.'); // Insert decimal point.
    sd++;
    int z = -exp - 1; // May need zeros after decimal point, before significant digit(s).
    if (z > 0) {
      s.insert(sd, z, '0'); // Insert any zeros before significant digit(s) (0.001234).
    }
  }
  if (is_neg) {
    s.insert(s.begin(), '-');
  }
  //if (is_scaled == true)
  //{ // Append symbol and prefix and trailing space.
  //  bool add_symbol = true;
  //   //  const char* symbol = SIsymbols[multiple3 + SImaxPowerTen]; // kilo, mega ...
  //   //  const char* prefix = SIprefixes[multiple3 + SImaxPowerTen]; // k, M m ...
  //     // std::cout << ' ' << symbol << ' ' << prefix << std::endl;
  //     if (multiple3 != 0)
  //     {
  //       s.append(" "); // separator between last digit and symbol.
  //       s.append((add_symbol ? symbol : prefix));
  //     }
  //}

  //std::cout << "Final digits string " << s << std::endl;

  return s;
} // string round_f(FPT v, signed int m)


// Round number v *at* d th decimal place.
// Note d is different (roundED digit) from the Wimmer definition of m (roundER digit).

inline double round_nth(double v, unsigned int d) { //! round value v to d digits after decimal point, using d+1 digit to round.
  return std::floor(v * std::pow(10., static_cast<int> (d)) + 0.5) / std::pow(10., static_cast<int> (d));
}

// Three explicit versions for 1, 2 and 3 digits after decimal point which may be slightly more efficient.
inline double round_1(double v) { //! round value v to 1 digit after decimal point, using 1st digit to round.
  return std::floor(v * 10. + 0.5) / 10.;
}

inline double round_2(double v) { //! round value v to 2 digit2 after decimal point, using 2nd digit to round.
  return std::floor(v * 100. + 0.5) / 100.;
}

inline double round_3(double v) { //! round value v to 3 digit2 after decimal point, using 3rd digit to round.
  return std::floor(v * 1000. + 0.5) / 1000.;
}

enum distribution_type 
{ /*! \brief Distribution type, encoded into two bits in `short int unc_types`.
   \note `gaussian` name is chosen rather than `normal` to avoid name clash
   from name of bit 11 and bit 12 in `enum unc_types`.
  */
  gaussian = 0, //!< `unc_types` bit 11 and bit 12 == 0, aka normal.
  uniform = 1, //!< `unc_types` bit 11 == 1, aka rectangular.
  triangular = 2, //!< `unc_types` bit 12 == 1.
  undefined = 3 //!< `unc_types` bit 11 and bit 12 == '11' == 3.
};

double delta(double loss_risk, double rounded_div_value, distribution_type distrib = gaussian) 
{
  /*! \brief Calculate Wimmer delta function using equation 24, p 1664.
    \details Gejza Wimmer, Viktor Witkovsky, Tomas Duby\n
    Measurement Science and Technology, 11 (2000) pages 1659-1665.
    ISSN 0957-0233 S0957-233(00)13838-X.\n
    Proper rounding of the measurement results under normality assumptions.\n
    Gejza Wimmer, Viktor Witkovsky, Proper rounding of the measurement result under the assumption of uniform distribution,\n
    Measurement Science Review, Vol 2, section 1, (2002), pages 1 - 7.\n
    Gejza Wimmer, Viktor Witkovsky, Proper rounding of the measurement result under the assumption of triangular distribution,\n
    Measurement Science Review, Vol 2, section 1, (2002), pages 21 to 31.\n

    \note rounding_loss is called epsilon by Wimmer et all.

    \param rounding_loss Proper-rounding increase of confidence-interval because of rounding, must be positive and 'small'.
    \note rounding_loss is usually 0.05 (equivalent to 95%), values in range 0.2 to 0.01 make good sense,
       0.2 (80%) risks a fair bit of loss from rounding, 0.01 (1%) causes almost no loss.
    \param rounded_div_value Ratio rounded / unrounded (rounded_div_value is assumed <= 1).
    \param distrib Distribution type, normal or gaussian(default), uniform, triangular, or laplace.

    \return Approximation of Wimmer delta function using equation 24, p 1664,
    for normal distribution, and similar for triangular and uniform.
    Returns -1 if rounding_loss is too 'tight', so must be increased for delta to be calculated.
    Throwing an exception might be better here?

  */
  // rounded_div_value > rounded rounded_div_value (so rounded_div_value < 1.?)
  // Approximation error is less than 0.0123 for 0.005 <= rounding_loss < 0.1
  // and rounded_div_value > rounded rounded_div_value for this rounding_loss - see table 1.
  // assert(rounded_div_value > rounded_gamma);
  //assert (rounded_div_value <= 1);
  using std::sqrt;

  constexpr double sqrt_3 = 1.7320508075688772935274463415058723669428052543712;
  double d;
  double x;
  double threshold;

  switch (distrib) {
    case gaussian: // equation 24, p 1664.
      // Gejza Wimmer, Viktor Witkovsky, Tomas Duby
      // Measurement Science and Technology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X.

      d = (1.348 + 0.9886 * loss_risk + 0.2288 * sqrt(loss_risk));
      x = rounded_div_value - 1.0001 + 2.058 * loss_risk - 1.93 * loss_risk * loss_risk;
      if (x < (std::numeric_limits<double>::min)() * 100.) // Small value allows for approximation uncertainty.
      { // Not possible to have rounding_loss this small!
        threshold = x; // ???
        std::cout << "Uncertain warning: rounding_loss " << loss_risk << " is too small for rounded_div_value rounded/unrounded ratio " << rounded_div_value << ", threshold is " << threshold << " for gaussian distribution." << std::endl;
        // For example:
        // "rounding_loss 0.01 is too small for rounded_div_value rounded/unrounded ratio 0.981226, threshold is 0.99."
        return -1.; // Not sure how to signal the problem here. Throw?
      }
      x = sqrt(x);
      d = d * x;
      return d; // sqrt(rounded_div_value - 1.0001 + 2.058 * rounding_loss - 1.93 * rounding_loss * rounding_loss);
      break;
    case uniform:
      // Gejza Wimmer, Viktor Witkovsky, Proper rounding of the measurement result under the assumption of uniform distribution,
      // Measurement Science Review, Vol 2, section 1, (2002), pages 1 - 7.
      threshold = 1 - loss_risk; // Wimmer equation 17, page 5
      if (rounded_div_value < threshold) {
        std::cout << "Uncertain warning: rounding_loss " << loss_risk << " is too small for rounded_div_value rounded/unrounded ratio " << rounded_div_value << ", threshold is " << threshold << " for uniform distribution." << std::endl;
        return -1; // Not sure how to signal the problem here.
      }
      d = sqrt_3 * (rounded_div_value + 2 * loss_risk - 1); // Wimmer equation 20, page 6.
      return d;
    case triangular:
      // Gejza Wimmer, Viktor Witkovsky, Proper rounding of the measurement result under the assumtpion of triangular distribution,
      // Measurement Science Review, Vol 2, section 1, (2002), pages 21 to 31.
      threshold = (1 - rounded_div_value) / (1 + rounded_div_value);
      if (loss_risk < threshold) {
        std::cout << "Uncertain warning: rounding_loss " << loss_risk << " is too small for rounded_div_value rounded/unrounded ratio " << rounded_div_value << ", threshold is " << threshold << " for triangular distribution." << std::endl;
        return -1; // Not sure how to signal the problem here.
      }
      // d = sqrt(6 * (rounding_loss - ((1 - rounded_div_value) /(1 + rounded_div_value)))); // Wimmer equation 18, page 27.
      d = sqrt(6 * (loss_risk - threshold));
      return d;
    default:
      // Might add one more (4th) distribution here - assuming distribution type is encoded using 2 bits in unc class.
      return -1;
  } // switch
} // double delta(double rounding_loss, double rounded_div_value, distribution_type distrib = gaussian)

double rounded_div_value(double rounded, double value)
{
  /*! \brief Calculate Wimmer rounded_div_value rounded/unrounded function p 1664.
  \details Measurement Science and Technolology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X.
    Gejza Wimmer, Viktor Witkovsky, Tomas Duby
    Proper rounding of the measurement results under normality assumptions.

  \param rounded Value (as `double`) after rounding.
  \param value Value (as `double`) before rounding. Assumes `rounded < unrounded value`.
  \return Ratio rounded / value, a measure of loss of information caused by rounding.
  */
  // Is this OK for negative values?  Need to check or swap so result is <= 1.
  // Use abs?
  if (value == 0.) { // Avoid divide by zero (or very close to zero too?).
    return 0.;
  }
  double g = rounded / value;
    //  assert(g <= 1.); // Assumes rounded < unrounded.

  if (g > 1.) { // This seems necessary for some cases???
    //std::cout << "value " << value << ", rounded " << rounded << std::endl;
    g = 1. / g;
  }
  return g; 
} // double rounded_div_value(double rounded, double value)

int round_m(double rounding_loss = 0.01, double sigma = 0., unsigned int sigma_sigdigits = 2U, distribution_type distrib = gaussian) 
{ /*! \brief Calculate the Wimmer rounding digit m using delta and rounded_div_value functions p 1661, equation 12.
  \details Measurement Science and Technology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X.
  Gejza Wimmer, Viktor Witkovsky, Tomas Duby\n
  Proper rounding of the measurement results under normality assumptions.

  \param rounding_loss Rounding loss (as fraction) accepted (default is 0.01 or 1%).
  \param sigma Uncertainty as standard deviation, for example: 0.01.
  \param sigma_sigdigits Number of significant decimal digits (default = 2)
  for uncertainty to be rounded (range 1 to 4, default = 2), depending on degrees of freedom.\n
  If degrees_of_freedom <= 2, then 1, else if degrees_of_freedom > 1000, then 3 else default = 2.
  \param distrib Type of distribution (default gaussian, or triangular or uniform).

   \return m Signed position of the digit to be used for rounding,
    `m == 0` means use the units digit for rounding the tens digit.
  */

  if (sigma_sigdigits <= 0)
  { // sigma_sigdigits is too small to be plausible from the confidence interval of uncertainty.
     std::cout << "round_m sigma = " << sigma << ", sigma_sigdigits = " << sigma_sigdigits << " must be >= 1! " << std::endl;
   sigma_sigdigits = 1; // or throw?
  }
  else if (sigma_sigdigits > 4)
  { // sigma_sigdigits is too big to be plausible from the confidence interval of uncertainty.
    std::cout << "round_m sigma = " << sigma << ", sigma_sigdigits = " << sigma_sigdigits << " must be <= 4! " << std::endl;
    sigma_sigdigits = 4U;
  }
  // https://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/dist_ref/dists/chi_squared_dist.html
  // https://www.boost.org/doc/libs/release/libs/math/example/chi_square_std_dev_test.cpp

  //Confidence level (two-sided) =  0.0500000
  //Standard Deviation           =  1.0000000
  //_____________________________________________
  //Observations        Lower          Upper
  //                    Limit          Limit
  //_____________________________________________
  //         2         0.4461        31.9102
  //         3         0.5207         6.2847
  //         4         0.5665         3.7285
  //         5         0.5991         2.8736
  //         6         0.6242         2.4526
  //         7         0.6444         2.2021
  //         8         0.6612         2.0353
  //         9         0.6755         1.9158
  //        10         0.6878         1.8256
  //        15         0.7321         1.5771
  //        20         0.7605         1.4606
  //        30         0.7964         1.3443
  //        40         0.8192         1.2840
  //        50         0.8353         1.2461
  //        60         0.8476         1.2197
  //       100         0.8780         1.1617
  //       120         0.8875         1.1454
  //      1000         0.9580         1.0459
  //     10000         0.9863         1.0141
  //     50000         0.9938         1.0062
  //    100000         0.9956         1.0044
  //   1000000         0.9986         1.0014

  // and in Owen L. Davies and Peter L. Goldsmith, Statistical Methods in Research and production, ISBN 0 05 002437, p 457 (1972).

  // So for 2 observations, the uncertainty might be 30 times, or half the central estimate,
  // meaning that a second digit is not likely to be significant.
  // But for the more common several degrees of freedom, a second digit might be significant.
  // So the ISO GUM pragmatically states that 2 significant decimal digits should be used for the standard uncertainty.
  // It is only when at least 1000 observations are available (and there are no problems with drift)
  // then a fractional relative standard deviation of uncertainty is about 0.05
  // and that 3 significant decimal digits are needed avoid loss from rounding.
  double sigma_rounded = round_sig(sigma, sigma_sigdigits);
  // ISO GUM always rounds uncertainty to n=2 significant digits,
  // but elsewhere Wimmer et al round to only *one* significant digit, for reasons unclear.
  // So sigdigits is parameterized to allow variation from (possibly very many) (possibly known) degrees of freedom?
  // So tests that compare to the paper do not fail when rounding_loss has values in example 5, p1662.

  double g = rounded_div_value(sigma_rounded, sigma);
  // Check against limits in table 1, page 1662.
  double gl;
  double e = rounding_loss;
  if (e >= 0.1) { // <= 10% loss from rounding accepted.
    gl = 0.81271;
  } else if (e >= 0.05) {
    gl = 0.90175;
  } else if (e >= 0.02) {
    gl = 0.95951;
  } else if (e >= 0.01) {
    gl = 0.97954;
  } else { // // <= 1% loss from rounding accepted.
    gl = 0.98972;
  }
  if (gl >= g)
  { // Example: Cannot return a rounding m because chosen rounding_loss 0.05 is too small!
    std::cout << "Uncertain warning: Cannot return a rounding m because rounding_loss " << e << " is too small!" << std::endl;
    std::cout << "Uncertain warning: " 
      "sigma_rounded = " << sigma_rounded  << ", sigma = " << sigma
      << ", rounded_div_value(sigma_rounded, sigma) = " << g << ", gl = " << gl << std::endl;
    // Example from Wimmer and test_rounding.
    // BOOST_CHECK_NE(round_m(0.05, 0.0232, 1U), -3); // Example 5 (ii a) rounding_loss <= 0.05 should fail,
    // Uncertain warning: Cannot return a rounding m because rounding_loss 0.05 is too small!
    // Uncertain warning : sigma_rounded = 0.02, sigma = 0.0232, rounded_div_value(sigma_rounded, sigma) = 0.862069, gl = 0.90175
    return -9999;  // Or ???
  }
  // Or throw?

  double d = delta(rounding_loss, g, distrib);
  if (d < 0.) { // Chosen rounding_loss is too small! (for example, e = 0.01 loss is too demanding, so try 0.02).
    std::cout << "Uncertain warning: Cannot compute a rounding digit index m because rounding_loss " << rounding_loss << " is too small!" << std::endl;
    return -9999; // or throw.
  }
  d = d * sigma_rounded / (5 * g); // Wimmer equation 12, page 1661.
  // (and same for triangular distribution equation 10, page 24, and uniform distribution equation 22, page 6).
  d = log10(d);
  return static_cast<int> (floor(d)); // \return m the position of the digit to be used for rounding.
  // m == 0 means use the units digit for rounding the tens digit, m = 1 means using the tens digit for rounding the hundreds digit ...
} // int round_m(double rounding_loss, double sigma, unsigned int sigdigits)

std::string round_ue(double v, double sigma, double loss_risk = 0.01, unsigned int sigdigits = 2U) { 
  /*! \brief Properly round value to a decimal-digit as `std::string`.
  \details Measurement Science and Technolology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X.\n
    Gejza Wimmer, Viktor Witkovsky, Tomas Duby,
    Proper rounding of the measurement results under normality assumptions.
    p 1661, equation 12.

    \param v Central value, often estimate of mean.
    \param sigma Uncertainty of value, usually standard deviation.
    \param rounding_loss Fraction of loss of accuracy from rounding permitted (default 1%).
    \param sigdigits Number of digits that are significant (default 2).

    \return Decimal digit @c std::string containing properly rounded value.
  */
  //int round_m(double rounding_loss, double unc, unsigned int sigdigits, distribution_type t);
  int m = round_m(loss_risk, sigma, sigdigits, gaussian);
  std::string r = round_ms(v, m);
  return r;
} // string round_ue(double v, double unc, double rounding_loss = 0.01, unsigned int sigdigits = 2U)

std::pair<double, double> conf_interval(double mean, double sigma, double df = 1., double alpha = 0.05, distribution_type distrib = gaussian) { /*!
    \brief Calculate confidence interval for chosen alpha confidence level and chosen distribution type.
    \details Uses confidence interval equations from:\n
       Gejza Wimmer, Viktor Witkovsky, Tomas Duby
       Measurement Science and Technolology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X.
       Proper rounding of the measurement result under the assumption of gaussian distribution,\n
       Confidence interval equation 6, p 1660.

       Gejza Wimmer, Viktor Witkovsky, Tomas Duby\n
       Measurement Science and Technolology, 11 (2000) 1659-1665. ISSN 0957-0233 S0957-233(00)13838-X.\n
       Proper rounding of the measurement result under the assumption of triangular distribution,\n
       Measurement Science Review, Vol 2, section 1, (2002), pages 21 to 31.\n
       Confidence interval Equation 12, page 25.\n

       Gejza Wimmer, Viktor Witkovsky,\n
       Proper rounding of the measurement result under the assumption of uniform distribution,\n
       Measurement Science Review, Vol 2, section 1, (2002), pages 1 - 7.\n
       Confidence interval, page 5, equation 13.\n

    \param mean Interval estimate for mean mu.
    \param sigma Standard uncertainty sigma from variance sigma.
    \param df Degrees of freedom (usually number of observations -1).
    \param alpha Confidence (0. < unc < 1.), typically 0.05 for 95% confidence.
      alpha/2 = 0.025 quantile = -1.96, and 1 - alpha/2 is 1.96 for normal distribution,
      so typically confidence interval is roughly twice unc either side (+ or - ) of mean.
    \param distrib Distribution type, Gaussian(default), uniform, triangular, laplace.
    \return Confidence interval upper and lower limits as a @c std::pair of @c double.
  */
  //  &#x03C3; &#x00B2;
  assert(alpha >= 0.);
  assert(alpha <= 1.);
  std::pair<double, double> ci;
  switch (distrib)
  {
    case gaussian:
    {
      // Wimmer p 1660, equation 6.
      using boost::math::normal;
      normal dist; // zero mean and unit sd. == normal dist(0., 1.);
      // Divide alpha by 2 because estimating upper and lower bounds.
      double t = quantile(complement(dist, alpha / 2));
      // std::cout << t << endl; // 1.95996 for alpha 0.05
      // one-sided confidence interval.
      double u = (df > 1) ? sigma / sqrt(df) : sigma;
      double w = t * u; //
      ci.first = mean - w;
      ci.second = mean + w;
      return ci;
    }
      break;
    case triangular:
    { // Gejza Wimmer, Viktor Witkovsky,
      // Proper rounding of the measurement result under the assumption of triangular distribution,
      // Measurement Science Review, Vol 2, section 1, (2002), pages 21 to 31.
      // Confidence interval Equation 12, page 25.
      using boost::math::triangular;
      // boost::math version of triangular includes the mode c, as well as the limits a and b
      // We really want the symmetric_triangular, but approximate with a = -1, c = 0 and b = +1.
      // This NOT the boost::math default.
      // Or could implement symmetric_triangular?
      double u = (df > 1) ? sigma / sqrt(df) : sigma;
      static const double sqrt_6 = 2.4494897427831780981972840747058913919659474804966; // sqrt(6)
      triangular dist(-sqrt_6, 0., +sqrt_6); // Wimmer page 23, below eq 6.
      double tlo = quantile(complement(dist, alpha / 2));
      double thi = quantile(dist, alpha / 2);
      // Wimmer page 24, quantile function (cdf inverse), equation 8.
      ci.first = mean - tlo * u;
      ci.second = mean - thi * u;
      return ci;
    }
    case uniform: // aka rectangular distribution.
    { // Gejza Wimmer, Viktor Witkovsky,
      // Proper rounding of the measurement result under the assumption of uniform distribution,
      // Measurement Science Review, Vol 2, section 1, (2002), pages 1 - 7.
      // Confidence interval, page 5, equation 13.
      static const double sqrt_3 = 1.7320508075688772935274463415058723669428052543712; // sqrt(3)
      double u = (df > 1) ? sigma / sqrt(df) : sigma;
      using boost::math::uniform;
      uniform udist(-sqrt_3, +sqrt_3);
      double tlo = quantile(complement(udist, alpha / 2)); // effectively quantile(1 - alpha/2)
      double thi = quantile(udist, alpha / 2);
      ci.first = mean - tlo * u;
      ci.second = mean - thi * u;
      return ci;
    }
    default: // As yet undefined distribution.
      ci.first = std::numeric_limits<double>::quiet_NaN();
      ci.second = std::numeric_limits<double>::quiet_NaN();
      break;
  } //   switch (distribution_type)
  return ci;
} // std::pair<double, double> conf_interval(double value, double unc, double alpha = 0.05)

static const double sqrt_2 = 1.4142135623730950488016887242096980785696718751217; // sqrt(2)
static const double sqrt_6 = 2.4494897427831780981972840747058913919659474804966; // sqrt(6)
static const double oneDivSqrtSix = 0.40824829046386301636621401245098189866099124674943; // 1/sqrt(6)
static const double oneDivTwoSqrtSix = 0.20412414523193150818310700622549094933049562337472; // 1/(2*sqrt(6))
static const double sqrt_3 = 1.7320508075688772935274463415058723669428052543712; // sqrt(3)

double cdf_uni(double z) { /*! Cumulative Distribution Function (CDF) of Uniform distribution Un(-sqrt_3, + sqrt_3),
   with an expectation of mean zero and variance unity.
    Gejza Wimmer & Victor Witkovsky, Measurement Science Review, volume 2 section 1 2002, page 3, equation 5.
   \return Cumulative distribution Function of uniform distribution.
  */
  if (z <= -sqrt_3) {
    return 0.;
  } else if (z >= sqrt_3) {
    return 1.;
  } else if (z == 0.) {
    return 0.5;
  }
  return (z + sqrt_3) / (2 * sqrt_3);
} // double cdf_uni(double z)

double quantile_uni(double alpha) { /*! Quantile of uniform distribution Uniform(-sqrt_3, + sqrt_3), expectation or mean zero and variance unity.
   Wimmer & Witkovsky, Measurement Science Review, volume 2 section 1 2002, page 3, eq 6.
   \return Quantile of uniform distribution.
  */
  assert(alpha >= 0.);
  assert(alpha <= 1.);
  return sqrt_3 * (2 * alpha - 1);
} // double quantile_uni(double alpha)

double cdf_tri(double z) { /*! Cumulative Distribution Function (CDF) of Triangular distribution.
    Gejza Wimmer, Viktor Witkovsky, Tomas Duby,
    Proper rounding of the measurement result under the assumption of triangular distribution,
    Measurement Science Review, Vol 2, section 1, (2002), page 24, equation 7.

    \param z Ramdom variate.
    \returns CDF of triangular distribution.
  */

  if (z < -sqrt_6) {
    return 0.;
  } else if (z > sqrt_6) {
    return 1.;
  } else if (z == 0.) { // Or might check if very near zero too?
    return 0.5;
  }
  double zz = oneDivTwoSqrtSix * z * z;
  if (z < 0.) {
    return 0.5 + oneDivSqrtSix * (z + zz);
  } else { //  if (z > 0.)
    return 0.5 + oneDivSqrtSix * (z - zz);
  }
} // double cdf_tri(double z)

double quantile_tri(double alpha) { /*! Quantile or Inverse of cumulative distribution function CDF of triangular distribution.
  Gejza Wimmer, Viktor Witkovsky, Tomas Duby,
  Proper rounding of the measurement result under the assumption of triangular distribution,
  Measurement Science Review, Vol 2, section 1, (2002), page 24, equation 8.

  \param alpha Confidence level.
  \return Quantile or Inverse of cumulative distribution function CDF.
  */
  assert(alpha >= 0.);
  assert(alpha <= 1.);

  if (alpha < 0.5) { // 0<= alpha < 0.5
    return sqrt_6 * (sqrt(2. * alpha) - 1.);
  } else { // 0.5 < alpha < 1.
    return sqrt_6 * (1. - sqrt(2. * (1. - alpha)));
  }
} // double quantile_tri(double z)

void out_confidence_interval(std::pair<double, double> ci, int m, std::ostream& os = std::cout)
{  /*! Output to os, confidence interval enclosed in brackets and separated by comma.
      The precision is controlled by the rounding order `m` used for the mean
      but with one extra decimal digit of precision, so rounding to m-1 th place.
      For example: <99.5, 156.5> rounding to -1 position
      (where the mean was rounded with m == 0,
      using the units digit to round the tens digit to "128.")

      \param ci Confidence interval or limits.
      \param m Signed position for rounding.
      \param os `std::ostream` for output.
  */
  std::streamsize osp = os.precision(); // Save to restore.
  os.precision(3); // Uncertainty always rounded to 2 decimal digits.
  using boost::lexical_cast;
  os << "<" << lexical_cast<double>(round_ms(ci.first, m - 1)) << ", "
          << lexical_cast<double>(round_ms(ci.second, m - 1)) << ">";
  os.precision(osp); // Restore saved.
} // void out_confidence_interval(std::pair ci)

void out_value_limits(double mean, double unc, std::pair<double, double> ci, int m, std::ostream& os = std::cout)
{ /*! Output mean, uncertainty and confidence interval.
  For example: \verbatim  128. +/- 15 <99.5, 156.5>  \endverbatim

    \param mean Mean or central estimate of value.
    \param unc Uncertainty estimate as standard deviation.
    \param ci Confidence interval as an interval.
    \param m Rounding position.
    \param os `std::ostream` for output.
  */
  std::streamsize osp = os.precision(); // Save to restore.
  os.precision(2); // Uncertainty always rounded to 2 decimal digits.
  double unc_rounded = round_sig(unc, 2); // Round to 2 significant digit - ISO rules.
  // TODO increase here if noisydigit wanted,
  // and/or if degrees of freedom > 100.
  os << round_ms(mean, m) << " +/- " << unc_rounded;
  os.precision(6); //
  using boost::lexical_cast;
  os << " <" << lexical_cast<double>(round_ms(ci.first, m - 1)) << ", "
          << lexical_cast<double>(round_ms(ci.second, m - 1)) << ">";
  os.precision(osp); // Restore saved.
} // void out_value_limits

void out_value_df_limits(double mean, double unc, int degfree = 1, std::ostream& os = std::cout)
{ /*! Output mean, uncertainty and confidence interval.
  For example: \verbatim  128. +/- 15 \<99.5, 156.5\>  \endverbatim
    \param mean Mean or central estimate of value.
    \param unc Uncertainty estimate as standard deviation.
    \param degfree Number of degrees of freedom.
    \param os `std::ostream` for output.
  */
  std::streamsize osp = os.precision(); // Save to restore.

  int round_m(double loss_risk, double unc, unsigned int sigdigits, distribution_type t);
  // int round_m(double rounding_loss = 0.01, double unc = 0., unsigned int uncsigdigits = 2U, distribution_type distrib = gaussian)

  double loss_risk = 0.01; 
  int m = round_m(loss_risk, unc, 2U, gaussian);

  std::pair<double, double> ci;
  ci = conf_interval(mean, unc, degfree);

  os.precision(2); // Uncertainty always rounded to 2 decimal digits.
  double unc_rounded = round_sig(unc, 2); // Round to 2 significant digit - ISO rules.
  // TODO increase here if noisydigit wanted,
  // and/or if degrees of freedom > 100.
  os << round_ms(mean, m) << " +/- " << unc_rounded;
  os.precision(6); //
  using boost::lexical_cast;
  os << " <" << lexical_cast<double>(round_ms(ci.first, m - 1)) << ", "
          << lexical_cast<double>(round_ms(ci.second, m - 1)) << ">";
  os.precision(osp); // Restore saved.
} // void out_value_limits

// Above is missing degrees of freedom included in unc type.
