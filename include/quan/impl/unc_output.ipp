/*! \file unc_print.cpp
  \brief Output of uncertain values.
  \details Definitions of unc_input declarations in unc.hpp.\n
    Outline:
    ~~~~~~~~
    1 Get ios info: width, fill & precision, save to restore..
    2 Get the requested info from uncflags.
    3 Get the value, stddev, degfree & uncTypes..
    4 Check value & stdDev for Nan, inf, zero, neg.
    5 Determine significant digits and stdDeviation significant digits.
    6 Round.
    7 Determine padding.

    Output.
    ~~~~~~
    Output leading padding.
    output rounded value & exponent.
    output +/- value, if requested.
    output trailing padding, if required.
    Output degrees of freedom, if required.

    Record width used etc.
*/

// Copyright Paul A. Bristow 2009, 2012.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/quan/unc.hpp>

#include <boost/math/special_functions/fpclassify.hpp>
  //using boost::math::isnan;
  //using boost::math::isinf;
  //using boost::math::isfinite;
  //using boost::math::isnormal; // isfinite and not denormalised.

#include <boost/math/special_functions/sign.hpp>
 // using boost::math::signbit;
 // using boost::math::sign;
 // using boost::math::copysign;

// Restore ios fill and precision, and set width to zero, as if used.
#include <boost/io/ios_state.hpp>

#include <cassert>
#include <ostream>
#include <sstream>
 // using std::stringstream;

/*! 
  \brief Output to stream given value as a decimal digit string,
  rounding properly using the uncertainty information in stdDev,
  and optionally including uncertainty as +/-, and degrees of freedom,
  with addition control from additional unc stream manipulators.

  \param value  Mean or most likely value.
  \param stdDev Uncertainty estimate as standard deviation.
  \param degFree Degrees of freedom -1. (Default zero for 1 observation).
  \param uncTypes 16 Uncertain type flags about the value.
  \param os Output stream, default is to `std::ostream`.
  \exception Throws `std::bad_alloc()` if 'os::iword' has not been initialised correctly,
  or has been corrupted.
*/
void unc_output(double value, // Mean or most likely value.
                float stdDev, // Standard deviation (uncertainty).
                unsigned short int degFree, // Degrees of freedom -1.
                unsigned short int uncTypes, // 16 Uncertain type flags.
                std::ostream& os = std::cout)  // Output stream.
{
  using boost::math::isnan;
  using boost::math::isfinite;

  using std::setprecision;
  using std::setw;
  using std::resetiosflags;
  
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
  using std::noadjust;  // Restore to default state.
  using std::nofixed;  // Restore to default state.
  using std::noscientific;  // Restore to default state.
  using std::defaultfloat; // Restore fixed & scientific to default state.
  using std::hexbase;
  using std::lowercase;

  // Check iwords are setup correctly.
  if ((os.iword(zeroIndex) != indexID) || (os.iword(topIndex) != indexID))
  { // iwords are corrupt!
    cerr << "\n!!! unc_output: " "iword corrupt!" << endl;
    outUncValues(os, cerr);  // Log all the iword values to cerr.
    os << "?????" << endl;
    throw std::bad_alloc(); // Or which other exception?
    // This means the function `setUncDefaults(os)` has not been called for the stream os!
    return; // int notOK might be better?
  }
  // References to os.iword()
  long& uncWidth = os.iword(uncWidthIndex); // Width required for uncertain value.
  long& oldUncWidth = os.iword(oldUncWidthIndex); // Width required for previous uncertain value.
  long& uncFlags = os.iword(uncFlagsIndex); //  
  long& Width = os.iword(widthIndex); // Width specified by setw(99).
  long& oldWidth = os.iword(oldWidthIndex); // width previously specified by setw(99)
  long& setSigDigits = os.iword(setSigDigitsIndex); // Specified number of digits to use for value.
  long& setStdDevSigDigits = os.iword(setUncSigDigitsIndex); // Specified number of digits to use for sd.
  long& setScale = os.iword(setScaleIndex); // Scale factor, stored by `<< setScale(6)`
  // Only actually scale multiple, if also request to set scale with `out << setscale ...`

  // bools showing output requirements specified using unc additional ostream manipulators.
  // Note that these bools are NOT initialised here,
  // assuming compiler will warn if used before being initialised.
  /*
    \var bool isNoisyDigit
    \brief Add an extra 'noisy' guard digit to reduce risk of information loss.
  */
  bool isNoisyDigit; 
  bool isPlusMinus;//!< Uncertainty as +/- is required too (but ignore if value is exact or integer).
  bool isUppercase; //!< Exponential format is, for example, 1E6 else 1e6.
  bool isScientificFormat;  //!< Taken to mean that exponential format wanted (always possible).
  bool isShowPoint;  //!< Means decimal point is always shown, for example 900. even if not needed.
  bool isShowPos; //!< Show + sign always because ios flag was set with `<< showpo`s.
  bool isFixed; //!< `os << fixed ...` ios decimal fixed d.dddd format (rather than scientific).
  bool isWidthSet; //!< `os << setw(9)` has prescribed a width (rather than default width == 0).
  bool isNoAdjust;  //!< std = default but unc usage not defined yet, center?
  bool isRightJustify; //!< right justify, prepend leading pad before. `<< right << ...`
  bool isLeftJustify; //!< left justify, append trailing pad after. `<< left ...`
  bool isInternalJustify;  //!< Not defined yet, but use to center in field?
  bool isCenter; //!< center if BOTH left and right specified.
  bool isAlign; //!< Align on decimal point?

  // bools showing state of arguments value, stdDev, df.
  // Note that these bools are NOT initialised here,
  // assuming the compiler will warn if use before initialized.
  // They all need to be 'global' to `unc_output`.
  bool isValueExact = false; //! value is exact (integer, rational, zero stdDev or noplus & nominus).
  bool isValueInteger = false; //! value is an integer, or stdDev = 0.f.
  bool isUncKnown = false; //!< StdDev or uncertainty is valid.
  bool isValueNaN = false; //!< value is NaN.
  bool isValueInfinite = false; //!< Value is infinite (postive or negative).
  bool isValueNegative  = false; //!< Value is negative.
  bool isValueZero = false; //!< Value is zero.
  bool isValueNearZero = false; //!< So near to zero that can't calculate relative stdDev.
  bool isValueTooBigforUnc = false;  //!< Magnitude > 1e38
  bool isValueTooBigforSI = false;  //!< magnitude > 1e24
  bool isValueZeroFractional = false; //!< Value to output has a non-zero fractional part.
  bool isStdDevNaN = false; //!< StdDev is NaN.
  bool isStdDevInf = false; //!< StdDev is inf.

  //int sigDigits = std::numeric_limits<double>::max_digits10; // Default significant digits for value.
  // Show all potentially significant?
  int sigDigits = 6; // Default significant digits for value.
  // For non-zero: 3 means ddd0. or ddd. or dd.d or d.dd or d.dd or 0.ddd or 0.0ddd ...
  // For zero, sigDigits = 1 means just 0, 2 means 0.0, 3 means 0.00 ...
  // If sigDigits < 0 means significant digits is undefined.
  int stdDevSigDigits = 1;  // Default significant digits for stdDev.

#ifdef UNC_TRACE
  { // Log the unc_output arguments:
    std::cerr << "\n  <<< unc_output: "
      << setprecision(2 + std::numeric_limits<double>::digits * 3010/10000) // aka max_digits10
      << value // scientific if necessary.
      << ", stdDev " << stdDev
      << ", df " << degFree
      << " ";
    // and log the info from the unc iwords:
    outUncFlags(uncFlags, std::cerr);
    std::cerr << "  ";
    outUncTypes(uncTypes, std::cerr);
    outUncValues(os, std::cerr);
  }
#endif
  //const int savedUncFlags = uncFlags; // Save to restore.
  //const int savedUncWidth = os.iword(uncWidthIndex); // Save to restore.
  // Why restore? - passed by value, so can alter if want to.

  // Width, precision, flags & fillChar data from stream os. ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  streamsize iosWidth = os.width(); //!< \warning Width must be read BEFORE any use of os
  // which would reset width back to zero!
  // & prevent any ios formatting during os << ...
  // because unc_output does all its own formatting.
  isWidthSet = (iosWidth > 0); // For example by os << setw(99)
  // Fixed width field suits tables.
  // For example: os << setw(10) means fit into a width of 10, with padding if necessary.
  // stream width <= 0 means no justification or padding.
  // Since default width = 0 after each item output so ostream,
  // defaults to flex format, suitable for flexible non-tabulated layout.
  const std::streamsize iosPrecision = os.precision();
  // Number of significant decimal digits prescribed by \c setprecision(int), Default is 6.
  const char iosFillChar = os.fill(); // Default is a space.
  // Save format flags from \c ostream, so can restore on exit, and set \c ios default values.
  const int iosFlags = os.flags();  // Save fmtflagsin case need to restore.
  os.width(0); // Would be zeroed by any previous use like << avalue ...
  os.flags(ios_base::dec | ios_base::skipws); // All other format flags are zero (cleared bits).
  // Leaving fill and precision as on entry.
#ifdef UNC_TRACE
  { // Log ios fillchar, precision & width.
    // fill char set by \c os.fill('~'); or \c << \c setfill('~')
    // precision by \c << setprecision(10)  or cout.precision(10),
    // width by \c setw(10) or \c cout.width(10)
    std::cerr <<  "  IOstream: "
      "fill char " << hex << showbase
      << int(iosFillChar) << space
      << '\'' << iosFillChar<< '\''<< dec
      << ", precision " << iosPrecision << ", width " << iosWidth << ", "
      << std::endl;
    std::cerr << "  "; outFmtFlags(os.flags(), std::cerr, ".\n");
    std::cerr << "  "; outIOstates(os.rdstate(), std::cerr, ".\n");
  } // trace
#endif

  // Get print format requirements from std::ios flags. ****************************
  isUppercase = static_cast<bool>(iosFlags & ios_base::uppercase); // E not e.
  // TODO No mechanism to change uppercase at present in round_* functions.
  isScientificFormat = static_cast<bool>(iosFlags & ios_base::scientific); // exp format wanted.
  isFixed = static_cast<bool>(iosFlags & ios_base::fixed); // `<< fixed` Fixed format wanted.
  // means d.dddd format wanted, if possible, rather than scientific or exp format.
  isNoAdjust = !static_cast<bool>(iosFlags & ios_base::adjustfield); // `<< noadjust`
  // no adjustfield flags are set (default, and == << right - a waste!).
  isLeftJustify = static_cast<bool>(iosFlags & ios_base::left);
  // left justify, append trailing padding with fillchar after.
  isRightJustify = static_cast<bool>(iosFlags & ios_base::right);
  // right justify, prepend leading padding with fillchar before.
  isInternalJustify = static_cast<bool>(iosFlags & ios_base::internal);
  // IO streams use to output fill char between sign and 1st digit.
  // And meaning of more than one bit is specially defined for unc_output:
  isCenter = isLeftJustify && isRightJustify;
  // center if BOTH left and right ios specified.
  // Done with ` << ` center rather than `<< left << right ... `.
  isAlign = isInternalJustify && isLeftJustify && isRightJustify;
  // Align decimal point with previous value?

  isShowPoint = static_cast<bool>(iosFlags & ios_base::showpoint);
  // Means decimal point is always shown, for example 900. even if not needed.
  isShowPos = static_cast<bool>(iosFlags & ios_base::showpos);
  // Show + sign always.

  // Get print format requirements from unc flags. 
  isNoisyDigit = static_cast<bool>(uncFlags & noisyDigit); // `<< addNoisyDigit`
  isPlusMinus = static_cast<bool>(uncFlags & plusMinus); // `<< plusminus`
  // Uncertainty as +/- is required too (but ignore if integer).

  int exponent = 0; // To hold calculated 10 ^ exponent.
  size_t used = 0; // Count of chars actually output to stream os.

#ifdef UNC_TRACE
  { // Log display options.
   std:: cerr << "  " "Display: "
      << (isPlusMinus ? "+/- " : "")
      << (isUppercase ? "upper " : "lower ")
      << (isScientificFormat ? "expFormat " : "")
      << (isWidthSet ? "set_width" : "")
      << (isFixed ? "fix " : " ")
      << (isNoAdjust ? "no_adjust " : "")
      << (isLeftJustify  ? "left " : "")
      << (isRightJustify ? "right " : "")
      << (isInternalJustify ? "internal " : "")
      << (isCenter ? "center " : "")
      << (isAlign ? "align " : "")
      << (isShowPos ? "show_+ " : "" )
      << (isShowPoint ? "show_. " : "")
      << std::endl;
  } // trace
#endif

  // Check value, stdDev & unc types for finite, zero, integer, exact. @@@@@@@@@@@@@@@@@@@@@@@@@@@@

  // Check value features first.
  const double savedValue = value;  // Save in case need original (signed & unscaled).
  isValueInteger = static_cast<bool>(uncTypes & VALUE_INTEGER); // Explicitly flagged as integer.
  isValueExact = static_cast<bool>
    (( ((uncTypes & (UNC_NOPLUS | UNC_NOMINUS)) == (UNC_NOPLUS | UNC_NOMINUS))// Both NOPLUS & NOMINUS bits set == 1,
    || (uncTypes & VALUE_EXACT) == VALUE_EXACT) // or flagged as exact VALUE_EXACT, like 2.54
    || ((uncTypes & (VALUE_INTEGER | VALUE_RATIONAL)) != 0) );  // Both integers (99) & rationals (22/7) are implicitly exact,
  // But zero values, for example 0.000 (implicitly +/- 0.0005) or 0. +/- 0.05f
  // are NOT exact (nor integer), unless stdDev = 0.f, or integer constructed from "0".

  if (isValueExact)
  { // By convention, uncTypes take precedence over any value of stdDev.
    // But should also cross-check if VALUE_EXACT and stdDev != 0.f
    stdDev = 0.f; // Ensuring it is finite in test below.
    degFree = (unsigned short)0;
    // By convention, exact values have 1 reading & so degfree == 0
    if (isValueInteger)
    { // No +/- because is integer, even if requested y << plusminus.
      isPlusMinus = false;
    } //
  } // isValueExact

  // Check if value is finite.
  if (!isfinite(static_cast<double>(value)))
  { // (value == NaN) || (value == SNaN) || (value == INFINITY)
    if (isnan(value))
    { // NaN
      isValueNaN = true;
      isValueNegative = false; // By convention, only positive zero and positive NaNs.
    }
    else
    { // If not NaN then assume  + or - infinity.
      isValueInfinite = true;
      isValueNaN = false;
      // Unlike zero and NaN, can have negative infinity.
    } // isNaN?
    isValueZero = false;
    isValueExact = false;
    isUncKnown = false;
  } // value not finite.
  else
  { // value IS finite, so can test if value is negative or zero, too big or too small.
    isValueInfinite = false;
    isValueNaN = false;
    isValueZero = (0. == value);
    if (isValueZero)
    { // Keep display of zero value simple (& avoid risk of divide by zero!).
      isValueNegative = false; // By convention, only +0 and +NaNs.
      isValueNearZero = true; // Use to avoid relative std deviation.
      isScientificFormat = false;
      exponent = 0;
    }  // is ZeroValue.

    if (value < 0.)
    { // is Negative (OK to use < 0. test here because have already eliminated NaN and inf).
      isValueNegative = true;
      value = fabs(value); // Can now ignore effect of negativeness, adding sign for output.
    }  // value < 0.

    isValueNearZero = (value < numeric_limits<double>::min() * 10.);
    // May cause divide-by-zero trouble if use relative std deviation.

  } // value zero, finite, NaN, outsize tests.

  // Check on features of stdDev.
  isUncKnown = uncTypes && UNC_KNOWN; // Uncertainty is flagged as KNOWN from uncType,
  // (but still need to check that stdDev is finite).
  if (!isfinite(stdDev))
  { // stdDev is NaN or infinity.
    isValueInteger = false; // ???
    isValueExact = false; // Uncertainty must be zero for exact, sp can't be exact if uncertainty is undefined, .
    uncTypes &= ~(UNC_KNOWN | UNC_NOPLUS | UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_UNIFORM | UNC_TRIANGULAR);
    // and clear many bits for unc_types which cannot be correct.
    isUncKnown = false; // Despite uncType flag UNC_KNOWN.
    if (isnan(stdDev))
    {
      isStdDevNaN = true; // (So uncertainty undefined)
      // but value is OK, so leave the value as is.
    }
    else // If stdDev not NaN then assume stdDev is infinite (+ or -).
    { // If stdDev == infinity, then, by convention, value == NaN.
      value = numeric_limits<double>::quiet_NaN(); // So the value becomes undefined too.
      isValueNaN = true;
      isStdDevInf = true;
    } // stdDev is NaN or infinity.
  } // stdDev NOT finite.
  else
  { // stdDev is finite (normal case).
    isStdDevInf = false;
    isStdDevNaN = false;
    isUncKnown = true;
    if ((0.f == stdDev) && isUncKnown)
    { // stdDev is finite zero (& uncertainty is known) so
      isValueExact = true; // value explicitly exact.
#ifdef UNC_TRACE
        {
          std::cerr << "  " "StdDev exact value "<< value;
        } // trace
#endif
    } // stddev finite
    else
    { // stdDev != 0, so value cannot be exact.
      isValueExact = false;
    }
  } // End checks on stdDev.

#ifdef UNC_TRACE
  { // Show output settings.
    std::cerr
      << "  " "Output Settings: " << boolalpha << dec
      << (isWidthSet ? "width is set, " : "no width set, ")
      << (isPlusMinus ? " +/- " : "")
      << (isUppercase ? "upper " : "lower ")
      << (isScientificFormat ? "expFormat " : "")
      << (isFixed ? "fix " : " ")
      << (isLeftJustify  ? "left " : "")
      << (isNoAdjust ? "no adjust " : "")
      << (isRightJustify ? "right " : "")
      << (isInternalJustify ? "int " : "")
      << (isShowPos ? "show_+ " : "" )
      << (isShowPoint ? "show . " : "")
      << (isValueZero ? "isValueZero " : "" )
      << (isValueNearZero ? "isValueNearZero " : "" )
      << (isValueExact ? "isValueExact " : "" )
      << (isValueInteger ? "isValueInteger " : "" )
      << (isValueNegative ? "isValueNegative " : "" )
      << (isValueInfinite? "isValueInfinite " : "" )
      << (isValueNaN ? "isValueNaN " : "" )
      << (isStdDevNaN ? "isStdDevNaN " : "" )
      << (isValueTooBigforSI ? "Too_big for_SI " : "")
      << (isValueTooBigforUnc ? "Too_big_for_unc " : "")
      << (isUncKnown ? "isUncKnown " : "uncUnknown" )
      << std::endl;

      std::cerr << "  " "Value " << setprecision(18) << value << (isValueNearZero ? "isValueNearZero " : "");
  } // trace
#endif
  // Scale the value, if required by unc flags setScaled or autoScaled. __________________________
  // 0 == scale10 means (not)scaled by unity. 3 means divided by 1000 ...
  if (!isValueInteger && !isValueZero && !isValueInfinite && !isValueNaN)
  { // autoscaled or set scaled.
    // but NOT isValueInteger, so means output 10000 m not 10.000 km,
    // nor zero because pointless.
  } // if (isScaled && !isValueInteger)

  // ScaledValue is in range OK, exponent == 0
  // so find power of ten of ms digit, 9 = 10 ^ 0, 90 = 10 ^ 2
  // int stdDevExponent = 0;  // If value is d.d, then exponent = 0.

  // Calculate how many significant digits for value, and then for stdDev. 
  // Or, if requested, use the set values.

  // int sigDigits; // Significant digits for value,
  // for example: 3 means ddd0. or ddd. or dd.d or d.dd or d.dd or 0.ddd or 0.0ddd ...
  // For zero, sigDigits = 1 means just 0, 2 means 0.0, 3 means 0.00 ...
  // int stdDevSigDigits;  // Significant digits for stdDev.
  if (isValueInteger)
  { // Integer 9876
    stdDevSigDigits = 0; // No uncertainty known!
    // But even if << plusminus, do NOT output "9 +/- 0" for an integer.
    // Do output decimal point if requested by showpoint.
    // Do we want to scale integers if requested by << setscale?  TODO
    sigDigits = exponent + 1; // So for integers 1 to 9: exponent = 0 & sigDigits = 1,
    //                              for integers 10 to 19, exponent = 1, sigDigits = 2 ...
    isPlusMinus = false; // Never show "2 +/-0" for integer value 2.
    // (even if requested with out << plusminus << integer ... !).
  } // isValueInteger.
  else if (isValueExact)
  { // Show full precision, but NO trailing zeros, so get "2.54" not "2.5400000000000"
    sigDigits = 1;
    // sigDigits = numeric_limits<double>::digits10; // Typically 15 digits are guaranteed.
    // (Although max_digits10 are perhaps significant).
    // Show "+/-0" if requested for real exact for example: "2.54 +/-0"
    // (unlike NOT for integer).
    stdDevSigDigits = 0; // "+/-0" not "+/-0.", "+/-0.0", "+/-0.1" ...
    exponent = 0;
  } // isValueExact.

  if (os.iword(uncFlagsIndex) && useSetSigDigits)
  { // Use set sig digits instead of calculated from uncertainty.
    sigDigits = setSigDigits;
    os << setprecision(setSigDigits) << value;
  }

  // Inexact and known uncertainty (including 0.0000 +/- 0.00019 case).
#ifdef UNC_TRACE
  { // scaling.
    std::cerr << std::setprecision(sigDigits+2) << dec
      << "  "
      << ", sigDigits " << sigDigits
      << ", stdDevSigDigits " << stdDevSigDigits;
      if (os.iword(uncFlagsIndex) && useSetSigDigits);
      { // Only if flag set.
        std::cerr << ", setSigDigits " << setSigDigits;
      }
      if (os.iword(uncFlagsIndex) && setStdDevSigDigits))
      { // 
        std::cerr << ", useSetUncSigDigits " << setStdDevSigDigits; 
        << std::endl;
      }
  } // trace
#endif

  const char signChar = (isValueNegative) ? '-': // Always show '-' sign.
  (isShowPos ? '+' :  // show '+' only if isShowPos.
  //(isWidthSet ? ' ' : 0 ) ); // space where sign char would be only for isWidthSet.
  (isWidthSet ? 0 : 0 ) ); // zero where sign char would be only for isWidthSet.

  // Save previous before actually output anything, so can restore on exit.
  os.iword(oldSigDigitsIndex) = os.iword(sigDigitsIndex);  // Save previous sigDigitsIndex.
  os.iword(sigDigitsIndex) = sigDigits;  // Save or 'return' sigDigits used next.

  // ###################  Actually Output value (append +/- if isPlusMinus). ####################################

#ifdef UNC_TRACE
  {
    std::cerr << "  " " ios Width " << iosWidth
      << ", ios fill char " << showbase << hex << int(iosFillChar) << dec
      << (isprint(iosFillChar) ? iosFillChar : '  ')
      << ", signChar (" << showbase << hex << int(signChar) << dec << noshowbase << ") " << ((signChar != 0) ? signChar : ' ')
      << std::endl;
  } // trace
#endif

  std::ostringstream oss; // Value etc string to really output, with pre & /or after padding, if necessary.

  bool beenPointed = false; // Decimal point has not been output yet.
  // Might use this later for alignment of table the decimal points.

  if (isValueInfinite)
  {
    oss << ((isValueNegative) ? "-infinity" : "+infinity");
  }
  else if(isValueNaN)
  {
    oss << "NaN"; // "NaN"
  }
  boost::io::ios_precision_saver precision_saver(os); // Save and restore on exit.
  boost::io::ios_flags_saver flags_saver(os);
    
    // round here.
    //os << val.value() << "(+/-" << val.uncertainty() << ")";

  //os << oss.str(); // "1.23 +/- 0.1 (9) mV"

  used = oss.str().size();

#ifdef UNC_TRACE
  {
    std::cerr << "  " << dec << used << " chars used in total."  << std::endl;
  } // trace
#endif

  os.iword(oldUncUsedIndex) = os.iword(usedIndex); // Save previous used.
  os.iword(usedIndex) = static_cast<long>(used);  // Record chars actually output to os.
  // This may be needed to calculate layout of a table,
  // Std behaviour is to return to default width.
  // Field width reverts to its default behavior, width = 0
  // (the necessary width) after one field has been printed,
  // so must be last item before a ';' to be read.  Not ideal!!
  // os.width = savedWidth; // Restoring width is pointless.
  os.flags(iosFlags);  // Restore ios flags.
} // void unc_output(double value, float stdDev,
// unsigned short int degFree, unsigned short int uncTypes, ostream& os);
