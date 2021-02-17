/*! \file
   \brief Class for simple Propagation of Uncertainties according to a pure Gaussian model.

    \details Based on code by Evan Manning (manning@alumni.caltech.edu)
    Evan Marshal Manning, C/C++ Users Journal, March 1996 page 29 to 38.
    original downloaded from ftp://beowulf.jpl.nasa.gov/pub/manning.
    This is a simple model of uncertainties, designed to
    accompany an article published in C/C++ Users Journal March 1996
    at http://www.ddj.com/cpp/184403147 .
    A fuller collection of even fancier classes also given in Evan Marshal Manning's unc.h.

    Standard deviation & its uncertainty added Paul A Bristow 31 Mar 98 ... .

     Uncertainty as Applied to Measurements and Calculations John Denker
      http://www.av8n.com/physics/uncertainty.htm#sec-crank3
      Here are some simple rules that apply whenever you are writing down a number:

    1. Use many enough digits to avoid unintended loss of information.
    2. Use few enough digits to be reasonably convenient.
    3. For other calculations, keep a potentially significant digits (std::numeric_limits<>::max_digits10), 17 for double.

  \mainpage

  \n
  \b Boost.Quan

This is the standalone Doxygen main page of the proposed Boost.Quan library.

This library provides a collection of classes and functions for handling uncertain reals.

Uncertainty as standard deviation and degrees of freedom are
propagated through calculations using the uncertain type,
and are input and output including the uncertainty estimates.

See Boost.Quan HTML Manual at

  https://svn.boost.org/svn/boost/sandbox/quan/libs/quan/doc/html/index.html

and/or equivalent PDF Manual at:

  https://svn.boost.org/svn/boost/sandbox/quan/libs/quan/doc/quan.pdf

Examples are in folder:

 /boost/libs/quan/example/

and C++ include files are in folder:

  /boost/boost/quan/
*/

// unc.hpp

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012, 2021
// 
//#define BOOST_QUAN_DIAGNOSTICS if diagnositics required.

#ifndef BOOST_QUAN_UNC_HPP
#define BOOST_QUAN_UNC_HPP

#include <boost/math/special_functions/fpclassify.hpp>
  //using boost::math::isnan;
  //using boost::math::isinf;
  //using boost::math::isfinite;
  //using boost::math::isnormal; // isfinite and not denormalised.

#include <cstdlib>  //
#include <cmath>   // for log, exp etc
#include <iostream> //
  //using std::istream;
  //using std::ostream;
  //using std::ios_base;
  //using std::char_traits;
  //using std::cout;
  //using std::cerr;
  //using std::cin;
  //using std::endl;
  //using std::flush;
  //using std::ws;
  //using std::streamsize;
  //using std::boolalpha;
  //using std::dec;
  //using std::hex;
  //using std::showbase;
  //using std::fixed;
  //using std::scientific;
  //using std::right;
  //using std::showpos;
  //using std::noshowpos;
  //using std::noshowbase;
  //using std::noshowpoint;
  //using std::showpoint;

#include <iomanip>  //
  //using std::setprecision;
  //using std::setw;
  //using std::resetiosflags;
#include <algorithm>
  //using std::max;
  //using std::min;

#include <sstream> //
 // using std::ostringstream;
#include <limits>  // for numeric_limits<unsigned short>::max() etc
//  using std::numeric_limits;
//#include <locale> // for . or , for decimal point
#include <exception> //
  //using std::exception;
  //using std::bad_alloc;

// Files for Quan library:
#include <boost/quan/xiostream.hpp> // for extra manipulators, spaces.
#include <boost/quan/unc_init.hpp> // Defines indexes to xalloc iword(index) variables.
#include <boost/quan/rounding.hpp> // Rounding functions.

// Boost libraries.
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
BOOST_STATIC_ASSERT (std::numeric_limits<double>::is_iec559); // Assume IEEE 754 ONLY.
#include <boost/io/ios_state.hpp>  // IO state saver.
#include <boost/units/io.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/type_traits.hpp>

namespace boost {
namespace quan {

// Forward declarations.
template <bool is_correlated> // Default is uncertainties that are NOT correlated (the normal case).
class unc;

// Two actual uncertain floating-point types for the user:
typedef unc<false> uncun;  //! Uncertainties are NOT correlated.
//! Uncorrelated is the normal case when uncertainties add.

typedef unc<true> unccorr;   //! Uncertainties ARE correlated.
//! This is the unusual case where the sum must be exact,
//! so the uncertainties are subtracted.
//! Example: two blocks which fit into a box perfectly.
//! So if both have an uncertainties, they must cancel when the uncertainties are added.
//! Also applies to items like concentrations which must add up to 100% or unity.

void unc_input(double& mean,  // mean (central or most probable) value.
                   double& stdDev,
                   unsigned short int& degreesOfFreedom,  // 1 observation.
                   unsigned short int& uncTypes,
                   std::istream& is);

void outIosFmtFlags(long, std::ostream&); // Output std::ios flags.
void outUncTypes(unsigned short int, std::ostream&); // Output uncertain types.

// Implemented below.

//! Quaded function, notationally convenient for x^4, but overflow possible?
//! Used by Welch-Satterthwaite formula.
template <typename Type>
inline Type pow4(const Type& a)
{
  return (Type)(a * a * a * a);
}

// SI units from CRC Handbook of Chemistry & Physics 76th edition 1995,
// ISBN 08493 0476-8 page 1-22.

// SI Usage, ISO 30-0 to 30-13:1992 is 'International Consensus Document'.

// ANSI/IEEE Std 268-1992 Americal National Standard for Metric Practice,
// (adopted for US Department of Defence)

// ASTM E 380-93 now consolidated with ANSI 268.

// NIST Special Publication 811,  Barry N Taylor
// Guide for the Use of the International System of Units 1995
// which was derived from IEEE Std 268, IEEE Standard Metric Practice (1982)

// Most useful commentary on the these standards, and annotated bibliography.
// (available from www.physics.NIST.gov)

// ISO 2955 = BS 6430 :1983 (1988)
// Method for representing SI and other units in information processing
// system with limited character sets.

// Gives two sets of representation, for SI units and other ISO 1000 units,
// for use in data interchange by systems with limited graphic character sets.
// Mainly important for allowing u instead of greek mu.

// Gunther Schadow & Clement J. McDonald,
// The Unified Code for Units of Measure,
// A critique and proposals of ISO and ANSI, and ENV 12435
// http://aurora.rg.iupui.edu/~schadow/units/UCUM/ucum.html

void outUncValues(std::ostream& os, std::ostream& log); // Output uncertain values.
void setUncDefaults(std::ios_base& os); // Set Unc class defaults for std::ostream& os.

 //! 16 type bits used by unc uncTypes. Bit set = 1 means a positive attribute.
 //! Unctypes are
enum unc_types
{
  VALUE_ZERO = 1 << 0, //!< bit zero = 1 means value is zero.
  //! (Too near zero to allow relsd = sd /mean.)
  //! A few times std::numeric_limits<float>::min() may be a suitable limit?
  VALUE_INTEGER = 1 << 1, //!< Exact integer 2,3,4 ... may not be possible to store `int` into `double`?
  VALUE_RATIONAL = 1 << 2, /*!< Integers & Fractions 1/2, 1/3, 2/3, 5/4 ...
   so don't need irrational - must be if not rational.
   Irrational like pi, e ... represented as accurately as possible by the double floating-point type,
   usually for 64-bit double, 15 to 17 decimal digits.
  */
  VALUE_NEGATIVE_ONLY = 1 << 3, //!< Value can ONLY be < 0.. ? or <=0?
  VALUE_POSITIVE_ONLY = 1 << 4, //!< Value can ONLY be >=0..
  UNC_KNOWN = 1 << 5, //!< Uncertainty known (but if not, value may still be known OK).
  UNC_NOPLUS = 1 << 6, //!< Uncertainty can only be < value, + = zero.
  UNC_NOMINUS = 1 << 7, /*!< Uncertainty can only be > value - = zero.
   Integer, Rational or Exact by definition like 25.4 mm per inch,
   if unc = 0.0f or both UNC_NOPLUS & UNC_NOMINUS.
  */
  UNC_QUAN_DECIMAL = 1 << 8, //!< Quantised by least significant decimal digit.
  UNC_QUAN_BINARY = 1 << 9, //!< Quantised by least significant binary digit.
  UNC_EXPLICIT = 1 << 10, /*!< If bit set = 1, uncertainty implicit from read significant digits,
   for example: unc_input of 10.0 will set this bit and set uncertainty = 0.05,
   or explicit if +/- or calculated (normal default case, bit = 0).
  */
  UNC_UNIFORM = 1 << 11, //!< Uncertainty has rectangular or uniform probability distribution.
  UNC_TRIANGULAR = 1 << 12, //!< Uncertainty has triangular probability distribution.
  //! If not rectangular or triangular bits set, then normal or Gaussian distribution, the default.
  //! Both bits 11 and 12 == 1 then has distribution un-defined as yet.
  //! Distribution type is used to estimate confidence intervals and rounding.
  DEG_FREE_EXACT = 1 << 13, //!< Known number of observations -1,
  //! so default zero means 1 observation.
  //! else estimated, for example, using Welch-Satterthwaite.
  DEG_FREE_KNOWN = 1 << 14, //!< Degrees of freedom is defined.
  SPARE = 1 << 15 //!< This last unc_type is spare.
}; // enum unc_types

const unsigned short int VALUE_EXACT = (UNC_NOMINUS | UNC_NOPLUS);  //!< Both set.
const unsigned short int UNC_DEF = (UNC_KNOWN | UNC_NOMINUS | UNC_NOPLUS | //!< Any defined means that uncertainty is defined.
                                    UNC_QUAN_DECIMAL | UNC_QUAN_BINARY |
                                    UNC_EXPLICIT //!< Not implicit from number of significant digits.
                                    | UNC_UNIFORM | UNC_TRIANGULAR);  // default Guassian distribution.
const unsigned short int DEG_FREE_DEF = (DEG_FREE_EXACT | DEG_FREE_KNOWN);

enum uncIOflags
{  //! \enum uncIOflags Control of printing uncertain values, similar to std::ios flags.
  //! @brief  Can be output, for example:
  //! \code outUncIOFlags(std::cout.flags(), std::cerr); // uncFlags (0x201) firm adddegfree.  \endcode
  defaults = 0, //!< Default.
  firm = 1  << 0,  //!< bit 0: == 0 == false means flexible layout, or firm == 1 means true.
  setScaled = 1 << 1, //!< bit 1 Set scaled == 1 or not scaled == 0.
  //! by fixed factor SetScale and << scale ...
  //! power10 in range -max to +max.
  autoScaled = 1 << 2,  //!< bit 2: auto = 1  with << autoscale ...
  plusMinus = 1 << 3, /*!< bit 3 = 1 means add +/- uncertainty
  Usage: \code std::cout << plusminus << u; \endcode
  */
  addSISymbol = 1 << 4,  //!< bit 4 = 1, add suffix SI symbol G, M, k, m, u, n, p ...
  //! If one is applicable, else = 0 do nothing.
  addSIPrefix = 1 << 5,  //!< bit 5 = 1, add suffix SI prefix Giga, Mega, kilo ...
  noisyDigit = 1 << 6, /*!< Add extra 'noisy' decimal digit to value & sd.
   Usage: std::cout `<< addnoisyDigit << u;`
   This means is suitable for input to other calculations because the
   random extra 2 to 3 bits will approximate a continuous function.
   Quantization to fewer digits would distort statistical calculations.
   Default is to use minimum decimal digits for clearer display.
   Extra digit also added when degrees of freedom > 10.
  */
  useSetSigDigits = 1 << 7, //!< if bit 7 = 1 use set sig digits else calculate from sd(isUseSetSigDigits).
  useSetUncSigDigits = 1 << 8, //!< if bit 8 = 1 use set Unc sigfig (isStdDevSigDigitsSet).
  //! else calculate from degrees of freedom.
  degfree = 1 << 9, //!< 0x200, If bit 9 == 1, add output of degrees of freedom as (99).
  // Usage: std::cout `<< adddegfree << u;`
  replicates = 1 << 0xA, //!< 0x400 If == 1, add output of replicates as [99] if > 1.
  limits = 1 << 0xB, //!< 0X800 add limits.
  // 0xC, D, E and F spare - and std::ios::iword is actually a long integer so could use 32 flags.
}; // enum uncIOflags

void outFpClass(double, std::ostream&);  // Special output for inf, NaN...

class showUncFlags
{  // Constructor & operator<< defined below
  friend std::ostream& operator<< (std::ostream&, const showUncFlags&);
public:
  showUncFlags(unsigned short int);  // Definition below.
  unsigned short int flags;
};


//!  Description as a word of each bit in @c unc_type, using enum unc_types.
//! These bits record the type of value stored, for example: VALUE_ZERO, UNC_KNOWN, DEG_FREE_KNOWN, UNC_UNIFORM.
//! Used by function outUncTypes.
//! Example: \code   \endcode
const char* uncTypeWords[16] =
{
    "zero", //!< 0 VALUE_ZERO value is zero.
    "integer", //!< 1 VALUE_INTEGER value is an integer.
    "rational", //!< 2 VALUE_RATIONAL value is rational.
    "-only", //!< 3 VALUE_NEGATIVE_ONLY Value can ONLY be < 0.. ? or <=0?
    "+only",  //!< 4 VALUE_POSITIVE_ONLY Value can ONLY be >=0.
    "uncKnown", //!< 5 UNC_KNOWN Uncertainty known
    "noPlus", //!< 6 UNC_NOPLUS Uncertainty can only be < value, + = zero.
    "noMinus", //!< 7 UNC_NOMINUS certainty can only be > value - = zero.
    "quantize10", //!< 8 UNC_QUAN_DECIMAL Quantised by least significant decimal digit.
    "quantize2", //!< 9 UNC_QUAN_BINARY Quantised by least significant binary digit.
    "explicit",  //!< 10 UNC_EXPLICIT not inferred from sig digits.
    "uniform", //!< 11 UNC_UNIFORM uniform  (or rectangular) probability distribution.
    "triangular", //!< 12 UNC_TRIANGULAR Triangular probability distribution.
    //! If neither bits set, then distribution type is normal (or gaussian).
    //! If BOTH bits are, the distribution is undefined (as yet).
    "df_exact", //!< 13 DEG_FREE_EXACT Exactly known number of observations.
    "df_known", //!< 14 DEG_FREE_KNOWN Degrees of freedom defined.
    "spare" //!<  15 SPARE
};


/*!
* \brief Helper function for @c operator<<
*/
class showUncTypes
{
  friend std::ostream& operator<< (std::ostream&, const showUncTypes&);
public:
  showUncTypes(unsigned short int t) : types(t)
  { // Constructor.
  }
  unsigned short int types;
};

/*! \brief @c Extract operator<< to show all the  types of an uncertain item.
  Usage: \code std::cout << showUncTypes(uncType) \endcode
  \param ut Uncertain type flags (bits) for the @c std::ostream.
  \param os @c std::ostream for output of uncertain types as words, for example: integer, zero, df_exact.
*/
std::ostream& operator<< (std::ostream& os, const showUncTypes& ut)  // Definition.
{
  const int count = 16;  // because using 16-bit unsigned short int.
  unsigned short int uncTypes = ut.types;
  os << "uncTypes (" << std::showbase << std::hex << uncTypes  << std::dec << ")";
  for (int i = 0, j = 1; i < count; ++i)
  {
    if ((uncTypes & j) != 0)
    {
      os  << ' ' << uncTypeWords[i];
    }
    j <<= 1;
  } // for
  os << ".";
  return os;
} // ostream& operator<< (ostream& os, const showUncTypes& ut)


// setAllUncflags(int flags, int mask);
// Usage: out << setAllUncFlags(0x5a) ...
class setAllUncFlags  // Set ALL uncertain flags (not just OR selected bits).
{
  friend std::ostream& operator<< (std::ostream&, const setAllUncFlags&);
  friend std::istream& operator>> (std::istream&, const setAllUncFlags&);
public:
  setAllUncFlags(int); // w) : flags(w) {}  // Constructor initialisation flags = w.
  int flags; // setAllUncFlags.flags used by operators << and >>
};

class setUncFlags  // Set selected uncertain flags.
{// Usage: out << setUncFlags(0xFFFF, 0x7, 0x3) ...
  // or setUncFlags(out, 0x7);

  friend std::ostream& operator<< (std::ostream&, const setUncFlags&);
  friend std::istream& operator>> (std::istream&, const setUncFlags&);
public:
  setUncFlags(int); // Constructor initialisation flags.
  int flags; // setUncFlags.flags used by operators << and >>
};

// setMaskedUncflags(int flags, int mask);
// Usage: out << setMaskedUncFlags(0xFFFF, 0x7, 0x3) ...
// or setUncFlags(out, 0x7);
class setMaskedUncFlags
{
  friend std::ostream& operator<< (std::ostream&, const setMaskedUncFlags&);
  friend std::istream& operator>> (std::istream&, const setMaskedUncFlags&);
public:
  setMaskedUncFlags(int, int); // : flags(w), mask(m) {}  // Constructor initialisation flags = w.
  int flags; // setMaskedUncFlags.flags used by operators << and >>
  int mask;  // Selected bits to deal with by zeroing before setting flags.
};

class resetUncFlags  // Reset = 0 selected uncertain flags.
{// Usage: out << resetUncFlags(0xFFFF, 0x7, 0x3) ...
  // or setUncFlags(out, 0x7);

  friend std::ostream& operator<< (std::ostream&, const resetUncFlags&); // Declarations
  friend std::istream& operator>> (std::istream&, const resetUncFlags&);
public:
  resetUncFlags(int);// w) : flags(w) {}  // Constructor initialisation flags = w.
  int flags; // setUncFlags.flags used by operators << and >>
};

// setMaskedUncflags(int flags, int mask);
// Usage: out << resetMaskedUncFlags(0xFFFF, 0x7, 0x3) ...
// or setUncFlags(out, 0x7);
class resetMaskedUncFlags
{
  friend std::ostream& operator<< (std::ostream&, const resetMaskedUncFlags&);
  friend std::istream& operator>> (std::istream&, const resetMaskedUncFlags&);
public:
  resetMaskedUncFlags(int, int); // : flags(w), mask(m) {}  // Constructor initialisation flags = w.
  int flags; // setMaskedUncFlags.flags used by operators << and >>
  int mask;  // Selected bits to deal with by zeroing before setting flags.
};

class setUncWidth // Set uncertain width.
{
  friend std::ostream& operator<< (std::ostream&, const setUncWidth&); // Declarations
  friend std::istream& operator>> (std::istream&, const setUncWidth&); // Defined below.
  // Allows ostream& operator>> and << to access private member width.
public:
  setUncWidth(int);
  int uncWidth; // setUncWidth used by operators << and >>
};

// setScale(int scale);
// Usage: out << setScale(6)  // == 10**6) ...
class setScale  // Set uncertain scale.
{
  friend std::ostream& operator<< (std::ostream&, const setScale&); // Declarations
  friend std::istream& operator>> (std::istream&, const setScale&); // Defined below.
  // Allows @c std::ostream& operator>> and << to access private member scale.
public:
  setScale(int); // n) : scale(n) {}  // Constructor - initialisation scale = n.
  int scale; // setScale.scale used by operators << and >>
};

// Usage via class: setSigDigits(int sigDigits);
// Usage via operator << : out << setSigDigits(6) ...
class setSigDigits
{
  friend std::ostream& operator<< (std::ostream&, const setSigDigits&); // Declarations
  friend std::istream& operator>> (std::istream&, const setSigDigits&);
public:
  setSigDigits(int);// w) : sigDigits(w) {}  // Constructor in unc.ipp
  // Initialisation means sigDigits = w.
  int sigDigits_; // setSigDigits.flags used by operators << and >>
};

/*! setUncSigDigits(int uncSigDigits);
  Permits choice of number of uncertain or stddev value:\n
  2 is the ISO recommendation.\n
    Uncertainty of measurement -- Part 3: Guide to the expression of uncertainty in measurement (GUM:1995)\n
    ISO Guide 98 (1995) and updated version 2008.\n
  1 is more appropriate for very small degrees of freedom,
   (but it gives a big step when the value starts with 1 or 2,
   when the difference between 1.4 (rounded to 1) and 1.6 (rounded to 2.) is a doubling.
  3 is appropriate only for large degrees of freedoms, >= 1000.
  \warning Values < 1 or > 3 are silently ignored.
  -1 passes through to allow dynamic choice of uncertainty significant digits or precision based on degrees of freedom.
  Usage: out << setUncSigDigits(3) ...
*/
class setUncSigDigits
{
  friend std::ostream& operator<< (std::ostream&, const setUncSigDigits&); // Declarations
  friend std::istream& operator>> (std::istream&, const setUncSigDigits&);
public:
  setUncSigDigits(int);// w) : uncSigDigits(w) {}  // Constructor
  // Initialisation means uncSigDigits = w.
  //protected:
  int uncSigDigits_; // Set by constructor.
}; // class setUncSigDigits

//! setRoundingLoss(int setRoundingLoss); to control the acceptable rounding loss.
//! Usage: `out << setRoundingLoss(0.01)` ...

class setRoundingLoss
{
  friend std::ostream& operator<< (std::ostream&, const setRoundingLoss&); // Declarations
  friend std::istream& operator>> (std::istream&, const setRoundingLoss&);
public:
  setRoundingLoss(double);// eps) : setRoundingLoss(eps) {}  // Constructor
  // Initialisation means setRoundingLoss = round_loss
  //protected:
  double roundingloss_; // Set by constructor.
}; // class setRoundingLoss

//! setConfidence(int setConfidence); to control the confidence interval.
//! Usage: `out << setConfidence(0.01)` ...

class setConfidence
{
  friend std::ostream& operator<< (std::ostream&, const setConfidence&); // Declarations
  friend std::istream& operator>> (std::istream&, const setConfidence&);
public:
  setConfidence(double);// alpha) : setConfidence(alpha) {}  // Constructor
  // Initialisation means setConfidence = alpha
  //protected:
  double confidence_; // Set by constructor.
}; // class setConfidence

//  template std::ostream& operator<< (std::ostream&, const std::pair< unc<false>, unc<false> >&);
//  template std::ostream& operator<< (std::ostream&, const std::pair< unc<true>, unc<true> >&);

/*! Uncertain number template class unc,
   using mean and measure of uncertainty (perhaps standard deviation if pure Gaussian distribution),
   but also includes information about uncertainty as degrees of freedom & distribution (default Gaussian).

  \tparam is_correlated if true, standard deviation is correlated, else not correlated (the common case).
*/template <bool is_correlated = false>
class unc : public std::char_traits<char>
{
public:
  typedef double value_type;

  friend std::ostream& operator<< (std::ostream& os, const unc<is_correlated == false>&);
  friend std::istream& operator>> (std::istream& is, const unc<is_correlated == false>&);

  friend void unc_input(double& mean,  // Mean (central or most probable) value.
                   double& stdDev, // Uncertainty estimate as Standard deviation.
                   unsigned short int& degreesOfFreedom,  // Degrees of freedom -1. (Default zero for 1 observation).
                   unsigned short int& types, // 16 Uncertain type flags showing type of value.
                   std::istream& is);

  // Previously used
  friend void unc_output(double value, // Mean(central or most probable) value.
                    float stdDev, // Uncertainty estimate as Standard deviation.
                    unsigned short int degFree, // Degrees of freedom.
                    unsigned short int uncTypes, //  16 Uncertain type flags showing type of uncertain value.
                    std::ostream& os);  // Output stream, default std::cout.

   using std::char_traits<char>::int_type; // Derivation from public \c std::char_traits<char> needed for \c int_type.

public:
  /*! \note It is convenient to use 64-bit floating-point format for mean of central value
     (so even really accurate values like weights can be precise enough),
     32-bit floating-point is ample accuracy for standard deviation fractional variation,
     leaving two 16-bit for degrees of freedom and other flags,
     so that total is same as two doubles & can be efficiently aligned.
  */
  double value_; //!< aka mean, estimate, central or most likely value.

  //! Measure of uncertainty, typically, standard deviation, if known.
  //! \note Reduced precision (float guarantees 6 decimal digits not 15) and range e38 not E304
  //! should not be a problem unless value is (near) less than 1e38.
  //! Can be zero, meaning exact, and can be negative or anti-correlated,
  //! for example when values must add up to a total like 100%.
  //! Relative (Fraction) Coefficent of variation =
  //! Standard deviation / value (aka % +|- /100)
  //! Relative is a problem if value near zero.
  //! +|- is std deviation, so
  //! for input of "1.0" implicit standard deviation and uncertainty limits +|- 0.05.
  float uncertainty_;

  /*! Degrees of freedom, usually = number of observations -1, so zero if just one observation or measurement.
   Range from 0 (usually 1 observation) to 65534 = std::numeric_limits<unsigned short int>::max() - 1
   so for 2 observations assign 1 to degFree_ degree of freedom,for 3 observations assing 2 ....
   Higher numbers of observations are indistinguishable from infinite observations.
   Max unsigned value 0xFFFF == 65535 is used to indicate degFree_ is NOT meaningful.
   BUT many programs seem to use NON-integer degrees of freedom,
   so a float might seem better, but fundamental @c float is 32 bits not 16, so use 16 floating-point type for compact struct?
 */
  short unsigned int degFree_;

  //!  Information about the value and uncertainty, encoded as a bitmap 16 bits, 0 to 15. \sa enum unc_types. \n
  //! \note These sizes mean that total size of a unc is 64 = 32 + 32 = 128 bits for IEEE-754 systems with 64-bit double.
  short unsigned int unctypes_;

public:
  // Member functions to get mean value, stdDev, degfree & uncTypes.
  //! Get central estimate of value of uncertain type.
  //! \return Central 'best' estimate of value of uncertain type.
  double value()
  {
    return value_;
  }
  //! Get estimate of uncertainty of value of uncertain type.
  //! \return Estimate of uncertainty as standard deviation of value of uncertain type.
  float std_dev()
  {
    return uncertainty_;
  }
  //! \return Estimate of number of degrees of freedom, usually = number of observations -1.
  unsigned short int deg_free()
  {
    return degFree_;
  }
  //! \return Types of uncertain real, encoded as a bitmap.
  unsigned short int types()
  {
    return unctypes_;
  }

  // Set member functions.
  //! Set central estimate of value of uncertain type.
  //! \param value Central estimate of value of uncertain type.
  void value (double value)
  {
    value_ = value;
  } // void setValue (double value)

  //! Set estimate of uncertainty of value of uncertain type.
  //! \param unc Estimate of uncertainty as standard deviation of value of uncertain type.
  void std_dev (float unc)
  {
    uncertainty_ = unc;
    if (boost::math::isfinite(static_cast<double>(unc)))
    {
      unctypes_ |= UNC_KNOWN; // Set flag to show uncertainty is known.
      if (unc > 0.f)
      { // NOT exact.
        unctypes_ &= ~(VALUE_EXACT | VALUE_RATIONAL | VALUE_INTEGER | UNC_NOPLUS | UNC_NOMINUS); // Clear.
      }
      else
      { // Exact.
        unctypes_ |= (VALUE_EXACT | UNC_NOPLUS | UNC_NOMINUS); // Set
      }
    }
    else
    { // unc not finite!
      unctypes_ &= ~UNC_KNOWN; // Clear as unc not known.

    }
  } // void std_dev (float unc)

  //! \param df Number of degrees of degrees of freedom, usually = number of observations -1, so = means just one observation.
  void deg_free (short unsigned int df)
  {
    degFree_ = df;
    unctypes_ |= DEG_FREE_KNOWN; // set.
  } // void deg_free (short unsigned int df)

  // TODO rename these.
  //! \param type bits indicating type of uncertain real.
  void setUncTypes (short unsigned int type)
  {
    unctypes_ = type;
  }  // Set all type flag(s).

  void types (short unsigned int type)
  {
    unctypes_ &= ~type;
  }  // Clear all type flag(s).

  // unc class Constructors.

 /*! Default constructor from double value & float uncertainty.
     Constructor from just a double value assumed exact,
     so behaves like a normal double.
     Normal conversion from double to unc.
     (See also constructor from integer which alone sets integer flag.)
    */
  unc(
    // Default values: exact zero? with no uncertainty, 1 observation.
    const double val,  //  = 0., // also const int = 0.
    // Ignore warning C4520: 'unc<1>' : multiple default constructors specified
    // (because they will be the same default values).
    const float unc = 0.0f,  // Default value exact.
    const short unsigned int df = 0,  // df = 0 means number of observations = 1.
    const short unsigned int uncTypeFlags = UNC_KNOWN | UNC_EXPLICIT| DEG_FREE_EXACT | DEG_FREE_KNOWN) // unc type flags.
    : value_(val), uncertainty_(unc), degFree_(df), unctypes_(uncTypeFlags)
  {
    // Check on value, uncertainty and unctypes.
    if (
      (boost::math::isfinite(value_)) // Value is finite, so neither infinity nor NaN!
      && (0.0f == uncertainty_) // Uncertainty parameter is zero (default).
      && (unctypes_ & UNC_KNOWN) // In case UNC_KNOWN flag is cleared by value of uncTypeFlags.
      // default unc = 0.f to suit construction from a double uncun(2.)
      // but NOT exact if more than one value or unc unknown signaled.
      )
    {  // Value is Exact double.
      //unctypes_ |= VALUE_EXACT;
      if (value_ == 0.)
      {  // Speed simplest & common zero integer case.
        unctypes_ |= (VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL);
      }
      else
      { // Not exact.
        // Only if construct from int, then set flag as VALUE_INTEGER.
        // Else leave unctypes_ as set by constructor, perhaps setting VALUE_INTEGER and/or VALUE_RATIONAL.
      }
    }
    else
    { // Value is Nan or Infinity.
      // Use a flag here?  VALUE_NONFINITE ??
    }
    // Check if unc (std dev) is NaN or infinity.
    if (!boost::math::isfinite(unc))
    { // unc (std dev) is NaN or infinity.
      unctypes_ &= ~(UNC_KNOWN | UNC_EXPLICIT| DEG_FREE_EXACT | DEG_FREE_KNOWN) ; // Clear all UNC_KNOWN flag.
    }
    else
    {  // unc (stdDev) is finite.
      if (unc == 0.0f)
      {
        unctypes_ |= VALUE_EXACT; // Set.
      }
      else if (unc < 0.0f)
      { // Negative uncertainty.
        if (!is_correlated) // NOT correlated case.
        { // Error! Throw? Or set uncertainty to zero?
          unctypes_ &= ~UNC_KNOWN;
          std::cerr << "Uncertain warning: Negative uncertainty " << unc << ", Value " << val << "!" << std::endl;
          // uncertainty_ = 0.0f; or NaN?
        }
        else // is correlated.
        { // But CAN have negative uncertainty for correlated case.
          std::cout << "Uncertain warning: Negative uncertainty for correlated case is not yet implemented." << std::endl;
        }
        if (unctypes_ && (UNC_NOPLUS | UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_UNIFORM | UNC_TRIANGULAR | UNC_EXPLICIT) != 0)
        { // Any uncertainty specifier bit means uncertainty is known.
          unctypes_ |= UNC_KNOWN;  // But do NOT specify as explicit.
        }
      }
      if ((uncTypeFlags & VALUE_EXACT) == VALUE_EXACT)
      { // Check for a valid uncertainty, but type parameter is exact.
        if (uncertainty_ != 0.f)
        {
          std::cout << "Uncertain Warning: value " << value_ << " is flagged as uncTypeFlags == VALUE_EXACT, but uncertainty " <<  uncertainty_ << " is not zero!" << std::endl;
          uncertainty_ = 0.f;  // Override any unc provided.
          // This indicates a programmer logic error!
        }
        if (degFree_ != 0)
        {
          std::cout << "Uncertain Warning: value " << value_ << " flagged as uncTypeFlags == VALUE_EXACT, but degfree " << degFree_ << " is not zero!" << std::endl;
          degFree_ = 0;  // Override any degfree provided.
          // This indicates a programmer logic error!
        }
      } //
    } // unc finite check
  };  // unc constructor from double.

  //!< unc Constructor from integer value.
  explicit unc(
    const int ivalue = 0, // Default value integer zero.
    const float unc = 0.0f,  // Exact.
    const short unsigned int df = 0,  // means n observations - 1 = 0.
    const short unsigned int uncTypeFlags =  // unc type flags.
      DEG_FREE_DEF | // See also constructor from double.
      VALUE_INTEGER | VALUE_RATIONAL |  // If integer, must be rational too.
      UNC_NOPLUS | UNC_NOMINUS | UNC_KNOWN) // Both so Exact.
    : value_(ivalue),
      uncertainty_(unc), // Not quite sure if sensible to allow uncertainty?
      degFree_(df) // OK to allow degrees of freedom (many integers).
    , unctypes_(uncTypeFlags) // Don't allow this as a parameter to avoid conflicting settings?
  {
      if (value_ == 0.)
      { // Zero integer case.
        unctypes_ |= VALUE_ZERO;
      }
  } // Constructor from integer value.

  unc(const unc& ud) //!< Constructor copy from another unc.
    : value_(ud.value_), uncertainty_(ud.uncertainty_),
    degFree_(ud.degFree_), unctypes_(ud.unctypes_)
  {  // Just copy all 4 member data.
  } // Constructor from unc.

   // Unary operators + and -.
  // No change to degrees of freedom or unc_types.
  // (unc<is_correlated> operator +(void) const; produces:
  // C2535 member function already defined or declared).

  unc<is_correlated> operator+ (void) const  // Unary +
  {  // 'slope' d f(x)/dx = +1
    return *this;  // All members remain unchanged.
  }

  unc<is_correlated> operator- (void) const // Unary -
  {  // 'slope' d f(x)/dx = -1
    if (is_correlated) // Negate BOTH value & uncertainty.
      return unc<is_correlated>(-value_, -uncertainty_);
    else // Don't negate sign of uncertainty if not correlated, only value.
      return unc<is_correlated>(-value_, uncertainty_);
  }

  // Binary add & subtract operators.
  // (Implemented using += and -= operators).
  friend unc<is_correlated> operator+ (unc<is_correlated> a,
    const unc<is_correlated>& b)
  {
    return a += b;
  }

  friend unc<is_correlated> operator+ (unc<is_correlated> a,
    const double& b)
  {
    return a += b;
  }

  friend unc<is_correlated> operator+ (const double& b,
    unc<is_correlated> a)
  {
    return a += b;
  }

  friend unc<is_correlated> operator- (unc<is_correlated> a,
    const unc<is_correlated>& b)
  {
    return a -= b;
  }

  friend unc<is_correlated> operator- (unc<is_correlated> a,
    const double& b)
  {
    return a -= b;
  }

  friend unc<is_correlated> operator- (const double& b,
    unc<is_correlated> a)
  {
    a -= b; return -a;
  }

  // Pre-Increment & pre-decrement operators.
  // (Implemented using += and -= operators).
  // Since using integer unity, uncertainty & rationality is unchanged.
  unc<is_correlated> operator++ (void)
  { return (*this += 1.); }

  unc<is_correlated> operator-- (void)
  { return (*this -= 1.); }

  unc<is_correlated> operator++ (int)
  {
    unc<is_correlated> retval(*this);
    *this += 1.;
    return retval;
  }

  unc<is_correlated> operator-- (int)
  {
    unc<is_correlated> retval(*this);
    *this -= 1.;
    return retval;
  }

  // Multiply & divide operators (using *= etc assignment operators).
  friend unc<is_correlated> operator* (unc<is_correlated> a,
    const unc<is_correlated>& b)
  {
    return a *= b;
  }

  // Uncertainty unchanged by all operations using a double.
  friend unc<is_correlated> operator* (unc<is_correlated> a,
    const double& b)
  {
    return a *= b;
  }

  friend unc<is_correlated> operator* (const double& b,
    unc<is_correlated> a)
  {
    return a *= b;
  }

  friend unc<is_correlated> operator/ (unc<is_correlated> a,
    const unc<is_correlated>& b)
  {
    return a /= b;
  }

  friend unc<is_correlated> operator/ (unc<is_correlated> a,
    const double& b)
  {
    return a /= b;
  }

  friend unc<is_correlated> operator/ (const double& b,
    unc<is_correlated> a)
  {
    unc<is_correlated> retval;
    retval.uncertainty_ = -b * a.uncertainty_ / (a.value_ * a.value_);
    retval.value_ = b / a.value_;
    retval.unctypes_ &= ~(VALUE_RATIONAL | VALUE_INTEGER);
    // Must assume double b is irrational and NOT integer.
    // (Use a unc instead to divide by a rational).
    return retval;
  }

  // Assignment operators +=, -=, *=, /=
  // (Used by plain +, -, * and / operators).
  // If VALUE_NEGATIVE_ONLY might check that value is still < 0?
  // If VALUE_POSITIVE_ONLY might check that value is still > 0?

  unc<is_correlated>& operator+= (const unc<is_correlated>& ud)
  { // unc += unc
    if (is_correlated)
    {
      uncertainty_ += ud.uncertainty_;  // If correlated then just add.
      degFree_ = // Choose degrees of freedom of the max uncertainty.
        (uncertainty_ > ud.uncertainty_) ? degFree_ : ud.degFree_;
      // Exact method of estimating not known, but is a reasonable approximation
      // provided really is highly correlated because less well predicted
      // variance can be predicted well with the better predicted.
      // See email from Steve LR Ellison, Lab of Government Chem, 24 Apr 98.
    }
    else
    { // Uncorrelated so Propagation of Uncertainty rules apply.
      // Barry N Taylor & Chris E Kuyatt, NIST Technical note 1297 (1994),
      // Guidelines for Evaluating & Expressing the Uncertainty of Measurements.
      // http:// physics.nist.gov/pubs/guidelines.appa.html
      // 'Combined uncertainty' Appendix A formula A-3 = "sqrt(sum of squares)"
      double comb_uncertainty = sqrtSumSqrs(uncertainty_, ud.uncertainty_);
      degFree_ =  // Welch-Satterthwaite - see NIST Appendix A3 & Appendix B-1.
        // Effective degrees of freedom <= - see Eq B-2, so use floor function.
        static_cast<unsigned short>( // Degrees of freedom must be integer by definition.
        floor(
        pow4<double>(comb_uncertainty)/
        (pow4<double>(uncertainty_)/degFree_ + pow4<double>(ud.uncertainty_) / ud.degFree_))
        );
      uncertainty_ = float(comb_uncertainty);
    }
    unctypes_ &= ~DEG_FREE_EXACT; // This warns of an approximation - no longer exact.
    unctypes_ &= ud.unctypes_; // Imposes rules for all additions of uncs.
    // zero + zero = zero, restrictions on pos & neg only only retained if both,
    // int + int is int, rational + rational is rational, int + rational is rational.
    // either or both plusminus same if BOTH.
    // Quantisation retains only if both.
    value_ += ud.value_;
    return *this;
  }

  operator value_type() const
  {
    return value_;
  }

  unc<is_correlated>& operator+= (const double& a)
  { // unc += double
    value_ += a;
    // Uncertainty & Degrees of freedom both unchanged.
    unctypes_ &= ~(VALUE_ZERO | VALUE_RATIONAL | VALUE_INTEGER | VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY);
    // All restrictions on value removed.
    return *this;
  }

  unc<is_correlated>& operator-= (const unc<is_correlated>& ud)
  { // unc -= double
    if (is_correlated)
    {  // Correlated.
      uncertainty_ -= ud.uncertainty_;
      degFree_ =  // Biggest - see +=
        (uncertainty_ > ud.uncertainty_) ? degFree_ : ud.degFree_;
    }
    else
    {  // Uncorrelated.
      float comb_uncertainty = sqrtSumSqrs(uncertainty_, ud.uncertainty_);
      degFree_ =  // Welch-Satterthwaite - see NIST Appendix A3 Appendix B-1.
        (unsigned short)  // Must be integer by definition.
        floor(   // Round or floor?
        pow4(comb_uncertainty)/
        (pow4(uncertainty_)/degFree_+pow4(ud.uncertainty_)/ud.degFree_)
        );
      uncertainty_ = float(comb_uncertainty);

    }
    unctypes_ &= ~DEG_FREE_EXACT;  // No longer exact.
    unctypes_ &= ud.unctypes_; // See += operator.
    value_ -= ud.value_;
    return *this;
  }

  unc<is_correlated>& operator-= (const double& a)
  {
    value_ -= a;
    // Uncertainty & Degrees of freedom both unchanged.
    unctypes_ &= ~(VALUE_ZERO | VALUE_RATIONAL | VALUE_INTEGER | VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY);
    // All indicators & restrictions on value removed.
    return *this;
  }

  unc<is_correlated>& operator*= (const unc<is_correlated>& ud)
  {
    if (is_correlated)
    {
      uncertainty_ = float(uncertainty_ * ud.value_ + ud.uncertainty_ * value_);
      degFree_ = // Choose degrees of freedom of the max uncertainty.
        (uncertainty_ > ud.uncertainty_) ? degFree_ : ud.degFree_;
    }
    else  // Uncorrelated.
    {
      double comb_uncertainty =
        sqrtSumSqrs(uncertainty_ * ud.value_, ud.uncertainty_ * value_);
      degFree_ =  // Welch-Satterthwaite - see NIST Appendix A3 Appendix B-1.
        static_cast<unsigned short> ( // must be integer by definition.  Round or floor?
        pow4(comb_uncertainty)/
        (pow4(uncertainty_)/degFree_+pow4(ud.uncertainty_)/ud.degFree_) );
      uncertainty_ = float(comb_uncertainty);
    }
    unctypes_ &= ~DEG_FREE_EXACT;  // No longer exact.
    unctypes_ &= ud.unctypes_; //
    value_ *= ud.value_;
    return *this;
  }

  unc<is_correlated>& operator*= (const double& a)
  {
    value_ *= a;
    uncertainty_ *= (float)a;  // Relative uncertainty unchanged.
    // Multiplying by constant doesn't change degrees of freedom.
    unctypes_ &= ~(VALUE_ZERO | VALUE_RATIONAL | VALUE_INTEGER | VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY);
    // All indicators & restrictions on value removed.
    return *this;
  }

  unc<is_correlated>& operator/= (const unc<is_correlated>& ud)
  { // unc /= unc
    if (is_correlated)
    {  // Correlated.
      uncertainty_ = uncertainty_ / ud.value_
        - (ud.uncertainty_ * value_) / (ud.value_ * ud.value_);
      degFree_ = // Choose degrees of freedom of the max uncertainty.
        (uncertainty_ > ud.uncertainty_) ? degFree_ : ud.degFree_;
    }
    else
    { // Uncorrelated.
      float comb_uncertainty = sqrtSumSqrs(uncertainty_ / ud.value_,
        (ud.uncertainty_ * value_) / (ud.value_ * ud.value_));
      degFree_ =  // Welch-Satterthwaite - see NIST Appendix A3 Appendix B-1.
        (int)  // must be integer by definition.  Round or floor?
        pow4(comb_uncertainty)/
        (pow4(uncertainty_)/degFree_+pow4(ud.uncertainty_)/ud.degFree_);
      uncertainty_ = float(comb_uncertainty);
    }
    unctypes_ &= ~DEG_FREE_EXACT;  // No longer exact.
    if ((unctypes_ & VALUE_INTEGER) && (ud.unctypes_ & VALUE_INTEGER) )
    { // int / int
      unctypes_ |= VALUE_RATIONAL; // becomes rational,
      unctypes_ &= ~VALUE_INTEGER; // but no longer int.
    }
    else
    {// rational / rational is still rational.
      unctypes_ &= ud.unctypes_;
    }
    if ((ud.unctypes_ & VALUE_ZERO) != 0)
    { // Avoid divide by zero, unless both zero!
      value_ = (value_ != 0.) ? INFINITY : 0.;
    }
    else
    { // Probably safe to divide. Check value == 0 as well??
      value_ /= ud.value_;
    }
    {  // Check if result is really an integer.
      double i; // For integer part from modf.
      double frac = modf(value_, &i);  // modf only works for doubles,
      // so can't test frac = 0.0f and this is better anyway.
      if (frac <= std::numeric_limits<double>::epsilon())  // nearly == 0
        // std::numeric_limits<double>::round_loss() = 2.22045e-016 aka DBL_EPSILON
      {
        unctypes_ |= (VALUE_INTEGER | VALUE_RATIONAL);
      }
      // TODO Any perhaps check if value is (near) zero?
    }
    return *this;
  }

  /*! operator /=
  \note Dividing by constant doesn't change degrees of freedom or uncertainty.
     All indicators & restrictions on value removed.
     Must assume double is irrational and not integer,
     NOT int/int becomes rational as operator /= .
  */
  unc<is_correlated>& operator/= (const double& a)
  {
    value_ /= a;
    uncertainty_ /= float(a);
    unctypes_ &= ~(VALUE_ZERO | VALUE_RATIONAL | VALUE_INTEGER | VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY);
    return *this;
  }

  inline bool operator== (const unc<is_correlated>& x) const
  { // Predicate compare equal.
    return value_ == x.value_; // Compare values EXACTLY.
    // Might also close_to compare within uncertainty - see equalU
  } // operator==

  inline bool operator!= (const unc<is_correlated>& x) const
  { // Predicate compare not equal.
    return value_ != x.value_; // Compare values EXACTLY.
    // Might also close_to compare within uncertainty - see equalU
  } // operator!=

  inline bool operator< (const unc<is_correlated>& x) const
  { // Predicate compare operator< for use by std::sort etc.
    // (Note const needed to use with less!)
    return value_ < x.value_;
    // Use normal signed values (no allowance for uncertainties).
    // Might also compare within uncertainty - see lessU
  } // operator<

  //! Insert operator<< for unc.
  //! (Should cover both correlated and uncorrelated cases?)
  friend std::ostream& operator<< (std::ostream& os, const unc<is_correlated == false>& val)
  {
    boost::io::ios_precision_saver precision_saver(os);
    boost::io::ios_flags_saver flags_saver(os);
    // both ios_flags and precision are restored by destructor.
    std::ostringstream oss; // Build up string to output.

    double mean = val.mean();
    float uncertainty = val.deviation();
    unsigned short int degFree = val.degFree();
    unsigned short int unc_flags = val.uncFlags();

    // bools showing output requirements specified using unc additional ostream manipulators.
    // Note that these bools are NOT initialised here,
    // assuming compiler will warn if used before being initialised.
    /*!
      \var bool isNoisyDigit
      \brief Add an extra 'noisy' guard digit to reduce risk of information loss.
    */
    bool isNoisyDigit;  //!< Add an extra 'noisy' digit.
    bool isDegFree;  //!<  Append degrees of freedom.
    bool isPlusMinus; //!< Uncertainty as +/- is required too (but ignore if value is exact or integer).
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
    bool isConfidenceInterval; //!<  Append confidence interval, for example, "<1.23, 1.56>"
    bool isSetSigDigits;  //!<  Use set sigdigits instead of calculate from uncertainty.
    bool isSetUncSigDigits;  //!<  Use set sigdigits instead of calculate from uncertainty.
    // Get print format requirements from std::ios flags. ****************************
    const int iosFlags = os.flags();  // Save fmtflags in case need to restore.

     // Width, precision, flags & fillChar data from stream os. ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    std::streamsize iosWidth = os.width(); //!< \warning Width must be read BEFORE any use of os
    // which would reset width back to zero!
    // & prevent any ios formatting during os << ...
    // because unc_output does all its own formatting.
    isWidthSet = (iosWidth > 0); // For example by os << setw(99)

    isUppercase = static_cast<bool>(iosFlags & std::ios_base::uppercase); // E not e.
    // TODO No mechanism to change uppercase at present in round_* functions.
    isScientificFormat = static_cast<bool>(iosFlags & std::ios_base::scientific); // exp format wanted.
    isFixed = static_cast<bool>(iosFlags & std::ios_base::fixed); // `<< fixed` Fixed format wanted.
    // means d.dddd format wanted, if possible, rather than scientific or exp format.
    isNoAdjust = !static_cast<bool>(iosFlags & std::ios_base::adjustfield); // `<< noadjust`
    // no adjustfield flags are set (default, and == << right - a waste!).
    isLeftJustify = static_cast<bool>(iosFlags & std::ios_base::left);
    // left justify, append trailing padding with fillchar after.
    isRightJustify = static_cast<bool>(iosFlags & std::ios_base::right);
    // right justify, prepend leading padding with fillchar before.
    isInternalJustify = static_cast<bool>(iosFlags & std::ios_base::internal);
    // IO streams use to output fill char between sign and 1st digit.
    // And meaning of more than one bit is specially defined for unc_output:
    isCenter = isLeftJustify && isRightJustify;
    // center if BOTH left and right ios specified.
    // Done with ` << ` center rather than `<< left << right ... `.
    isAlign = isInternalJustify && isLeftJustify && isRightJustify;
    // Align decimal point with previous value?

    isShowPoint = static_cast<bool>(iosFlags & std::ios_base::showpoint);
    // Means decimal point is always shown, for example 900. even if not needed.
    isShowPos = static_cast<bool>(iosFlags & std::ios_base::showpos);
    // Show + sign always.

    // Get print format requirements from unc flags.
    long& uncFlags = os.iword(uncFlagsIndex); //
    isPlusMinus = static_cast<bool>(uncFlags & plusMinus); // `<< plusminus`
    isNoisyDigit = static_cast<bool>(uncFlags & noisyDigit); // `<< addNoisyDigit`
    isConfidenceInterval = static_cast<bool>(uncFlags & limits); // `<< addlimits`
    isDegFree = static_cast<bool>(uncFlags & degfree); // `<< addDegrees`
    isSetSigDigits = static_cast<bool>(uncFlags & useSetSigDigits); // `<< setsigdigits`
    isSetUncSigDigits = static_cast<bool>(uncFlags & useSetUncSigDigits); // `<< setsigdigits`

    // Pick up distribution type from unc_flags.
    // UNC_UNIFORM = 1 << 11, //!< Uncertainty has rectangular or uniform probability distribution.
    // UNC_TRIANGULAR = 1 << 12, //!< Uncertainty has triangular probability distribution.
    distribution_type distrib;
    if (unc_flags & UNC_UNIFORM)
    {
      distrib = uniform;
    }
    else if (unc_flags & UNC_TRIANGULAR)
    {
      distrib = triangular;
    }
    else
    {
      distrib = gaussian;
    } // Distribution type set.

    long& roundloss = os.iword(roundingLossIndex);
    double round_loss;
    if (roundloss <= 0)
    { // Not been set (or silly),
      round_loss = 0.05; // so use a default.
    }
    else
    { // Has been set by a call like `out << confidence(0.01);`.
      // rounding loss is stored as a long, so scaled by 1000,
      // so that 0.05 or 1% is stored as 50.
      round_loss = roundloss / 1000.;  // `<< roundingloss(0.05)`
    }
    //! Confidence or alpha to compute confidence interval is similarly scaled.
    //! Usage: `out << confidence(0.01) << ...` means 1 - confidence = 99% confidence.
    //!   double confidence = os.iword(conf) / 1000.;  // `<< confidence(0.05)` aka 95%

    //int round_m(double round_loss = 0.01, double unc = 0., unsigned int uncsigdigits = 2, distribution_type distrib = gaussian);
    //void out_confidence_interval(std::pair<double, double> ci, int m, std::ostream& os = std::cout);
    //void out_value_limits(double mean, double unc, std::pair<double, double> ci, int m, std::ostream& os = std::cout);

    long& conf = os.iword(confidenceIndex);
    double confidence;
    if (conf <= 0)
    { // Has not been set, so use default.
      confidence = 0.05;  // 95% confidence.
    }
    else
    {
      confidence = conf / 1.e6;  // Unscale the scaled value stored as an int - back into a double.
    }
    using boost::math::isfinite;
    using boost::math::isnan;
    using boost::math::isinf;

    constexpr int max_digits10 = std::numeric_limits<double>::max_digits10; // std::numeric_limits<double>::digits10 * 3010/10000;

    double intpart;
    if (boost::math::isfinite(mean))
    {
      if(isSetSigDigits)
      { // Use explicit number of digits of precision.
        long& sigDigits = os.iword(setSigDigitsIndex);
        if ((sigDigits <= 0) || (sigDigits > max_digits10) )
        { // Show all potentially significant digits.
          sigDigits = max_digits10;
        }
        oss << std::showpoint << std::setprecision(sigDigits) << mean;
      }
      else
      { // Use uncertainty of value to control digits of precision output.
        if (mean == 0)
        { // isValueZero = true;
          if (uncertainty == 0.F )
          { // Is exact integer zero, so not rounded, and no decimal point.
            oss << "0";
          }
          else if (!isfinite(uncertainty))
          { // Show all possibly significant digits for NaN and inf uncertainty.
            oss << std::showpoint << std::setprecision(max_digits10) << mean;
          }
          else
          { // Inexact zero.
            int m = round_m(round_loss, uncertainty, 2, distrib);
            if (isNoisyDigit)
            { // Move rounding digit to one less significant position.
              m--;
            }
            oss << round_ms(0., m);
          }
        } // mean == 0
        else if(unc_flags & VALUE_INTEGER)
        { // Value is flagged as an integer (used integer constructor or set flag).
          double fracpart = std::modf(mean, &intpart);
          oss << std::noshowpoint << static_cast<long>(intpart); // Integer, so not rounded, and no decimal point,
          // even if decimal point was specified by showpoint!
        }
        else if (unc_flags & VALUE_EXACT)
        { // Value flagged as exact like 2.54 because constructor std_dev == 0 (but NOT integer)
          // (used double constructor or set flag) so show decimal point.
          oss.unsetf(std::ios_base::showpoint); // Ignore showpoint so NO trailing zeros (because exact).
          if (isShowPos)
          { // User specified showing + sign;
            oss << std::showpos;
          }
          // Kinda want a decimal point here, but causes trouble with setw :-(
          oss << std::setprecision(std::numeric_limits<double>::digits10) << mean;
          double fracpart = std::modf(mean, &intpart);
          if (fracpart == 0.)
          { // Avoid two decimal points!
            oss << '.';
          }
        }
        else
        { // Non-zero uncertainty, sd != 0.
          if (isfinite(uncertainty))
          {
            int m = std::numeric_limits<double>::digits10; // Effectively no rounding?
            if (uncertainty > 0.F)
            { // Rounding is appropriate.
              m = round_m(round_loss, uncertainty, 2, distrib); // m is rounding digit index.
            }
            if (isNoisyDigit)
            { // Move rounding digit to one less significant position.
              m--;
            }
            if (isShowPos == true)
            {
              oss << '+';
            }
            if (mean < 1e+15)
            { // Will fit into 1000000000000 (digits10 = 15).
              if (uncertainty > 0.F)
              {
                oss << round_ms(mean, m);
              }
              else
              {
                oss  << mean;
              }
            }
            else
            { // Won't fit into 1000000000000 (digits10 = 15), so switch to exponent format.
              oss << mean;
              //round_e(mean, m); // This value of m is wrong TODO !!!
              // Need to round but and not display exp as "e+009"  TODO.
            }
          }
          else
          { // Uncertainty NAN or infinite, so show all possibly significant digits.
            oss << std::showpoint << std::setprecision(max_digits10) << mean;
          }
        }
      }
    }
    else
    { // Mean is NaN or infinity.
      if (isnan(mean))
      {
        oss << "NaN" ;
      }
      else if (isinf(mean))
      {
         oss << ((mean < 0) ? "-inf" : (isShowPos) ? "+inf" : "inf");
      }
      // Or could leave as native output, but MSVC format is ugly.
    } // Mean

    if (isPlusMinus && !(unc_flags & VALUE_INTEGER))
    { // Want estimate of uncertainty.
      if (isfinite(uncertainty))
      {
        if (uncertainty == 0.F)
        { // Special case short version.
          oss << " +/-0";
        }
        else
        { // Non-zero uncertainty.
          int uncSigDigits = os.iword(setUncSigDigitsIndex);
          // Default is round to 2 sig digit - ISO rule.
          if (uncSigDigits <= 0)
          { // Automatically choose uncSigDigits based on degrees of freedom.
            // Passed negative values through to allow
            // an auto mode for w < 0 that chooses from degrees of freedom,
            // From table H page 457 in Oliver & Goldsmith, confidence interval
            // of standard deviation is about +/- 20% at 10 degrees of freedom,
            // and only < +/- 10% above 100 observations (needing 2 stdDev sig Digits).
            uncSigDigits = abs(uncSigDigits);
            if (degFree > 100)
            {
              uncSigDigits = 3;
            }
            else if (degFree > 10)
            {
              uncSigDigits = 2;
            }
            else
            { // degFree in common range 1 to 10.
              // Choose between 1 and 2 digits based on 1st digit of uncertainty.
              // Would be too big a step if most significant digit was 1 or 2.
              // std::ostringstream oss; ??
              oss << std::scientific << std::setprecision (1) << uncertainty; // Assume sd positive.
              if(oss.str()[0] == '1') // Check 1st digit before decimal point.
              { // Would be too big a step if most significant digit was 1 or 2.
                uncSigDigits = 2;
              }
              else if (oss.str()[0] == '2')
              {
                uncSigDigits = 2;
              }
              else
              { // 1st digit > 2 so can use just 1 digit.
                 uncSigDigits = 1;
              }
            }
          }
         if (unc_flags & UNC_NOPLUS)
          {
            oss << " +0/-";
          }
          else if (unc_flags & UNC_NOMINUS)
          {
            oss << " -0/+";
          }
          else
          { // normal plus and minus.
            oss << " +/-";
          }
          //oss.unsetf(std::ios_base::scientific);
          oss << std::noshowpos  // Prefixed by " +/-" so never add +.
            << std::fixed // << std::noscientific
            << std::showpoint
            << std::setprecision(uncSigDigits); // switches to e format :-(
          //double unc_rounded = round_sig(uncertainty, uncSigDigits);
          //oss << unc_rounded;
           //std::string round_f<FPT>(FPT v, int sigdigits);
         std::string s = round_f<float>(uncertainty, uncSigDigits);
          oss << s;
        }
      }
      else
      { // Not finite uncertainty - NaN or inf.
        if (isnan(uncertainty))
        {
          oss << " +/-?";
        }
        else if (isinf(uncertainty))
        {
          oss << " +/-inf";
        }
      }
    } // uncertainty.

    if(isConfidenceInterval)
    { // Want to append confidence interval as <1.23, 2.34>.
      if (boost::math::isfinite(mean) && boost::math::isfinite(uncertainty) && degFree >= 0)
      { // degfree 1 means 2 observations, so possible to compute confidence limits or interval in < > angle brackets.
       // std::streamsize osprec = os.precision(); // Save precision. TODO but don't seem to restore?
       // oss.precision(3); //
        //std::pair<double, double> conf_interval(double mean, double unc, double df = 1., double alpha = 0.05, distribution_type distrib = gaussian);
        double alpha = os.iword(confidenceIndex) / 1.e6; // Pick up and unscale alpha.
       // double round_loss = os.iword(roundingLossIndex) / 1.e3; // Pick up and rounding loss and unscale.
        // TODO check has already been set above at line 1503
        int uncSigDigits = os.iword(setUncSigDigitsIndex);  // Pick up significant digits for uncertainty.
        if(isNoisyDigit)
        {
          uncSigDigits++;
        }
        std::pair<double, double> ci  = conf_interval(mean, uncertainty, degFree, alpha, distrib);
        int m = round_m(round_loss, uncertainty, uncSigDigits, distrib);  // Compute roundER digit number.
        using boost::lexical_cast;
        oss << " <"
            << lexical_cast<double>(round_ms(ci.first, m-1)) << ", "
            << lexical_cast<double>(round_ms(ci.second, m-1))
            << ">";
      }
      else
      { // Not possible to compute confidence limits or interval.
        oss << " < ?, ? >" ;
      }
    } // Appended confidence interval.

    if (isDegFree)
    { // Add any degrees of freedom information (in brackets, for example: "(99)" )
      if (degFree == (std::numeric_limits<unsigned short int>::max)())
      { // Is valid.
         oss << " (?)";
      }
      else if (degFree == 0u)
      { // Might show that this is implied by default? with oss << (" (0?)" ?
         oss << " (0)";

      }
      else
      {
        oss << " (" << degFree << ")"; // " (99)"
      }

    }
    os << oss.str();
    return os;
  } // friend std::ostream& operator<< (std::ostream& os, const unc<is_correlated>& val)

  //! Extract operator<< for unc type.
  friend std::istream& operator>> (std::istream& is, unc<is_correlated>& ud)
  {
    double mean;
    double stdDev;
    unsigned short int degreesOfFreedom;
    unsigned short unctypes;
    unc_input(mean, stdDev, degreesOfFreedom, unctypes, is);
    // At present, uses a separate function for input (unlike output).
    ud = unc<is_correlated>(mean, float(stdDev), degreesOfFreedom, unctypes);
    // Inputs mean, SD & degrees of freedom & set unc types... as a string from file.
    return is;
  }

  // Math functions for uncertain class.
  friend unc<is_correlated> ceil(unc<is_correlated> arg)
  {
    arg.value_ = ceil(arg.value_);
    arg.uncertainty_ = 0.;  // Becomes exact & rational integer.
    arg.unctypes_ |= UNC_NOPLUS | UNC_NOMINUS | VALUE_INTEGER | VALUE_RATIONAL; // Exact.
    arg.degFree_ = (std::numeric_limits<unsigned short>::max)(); //= USHRT_MAX; = 0xFFFF
    return arg;
  }

  friend unc<is_correlated> floor(unc<is_correlated> arg)
  {
    arg.value_ = floor(arg.value_);
    arg.uncertainty_ = 0.; // Becomes exact & rational integer.
    arg.unctypes_ |= UNC_NOPLUS | UNC_NOMINUS| VALUE_INTEGER | VALUE_RATIONAL;  // Exact.
    arg.degFree_ = (std::numeric_limits<unsigned short>::max)(); // USHRT_MAX; = 0xFFFF
    return arg;
  }

  friend unc<is_correlated> fabs(unc<is_correlated> arg)
  {
    if (is_correlated && (arg.value_ < 0.))
      arg.uncertainty_ *= -1.;
    // Else uncorrelated so uncertainty unchanged?
    arg.value_ = fabs(arg.value_);
    unsigned short int plusMinus = arg.unctypes_ & (UNC_NOPLUS | UNC_NOMINUS);
    if (( plusMinus != 0) // Neither, & not both
      && (plusMinus != (UNC_NOPLUS | UNC_NOMINUS))
      )
    {  // Just one asymetric uncertainty.
      plusMinus = ~plusMinus;  // Switch bits over.
      arg.unctypes_ &= ~(UNC_NOPLUS | UNC_NOMINUS); // Clear both.
      arg.unctypes_ |= plusMinus;  // Set switched.
    }
    return arg;
  }

  friend unc<is_correlated> abs(unc<is_correlated> arg)
  { // Copy of fabs version above.
    if (is_correlated && (arg.value_ < 0.))
      arg.uncertainty_ *= -1.;
    // Else uncorrelated so uncertainty unchanged?
    arg.value_ = fabs(arg.value_);
    unsigned short int plusMinus = arg.unctypes_ & (UNC_NOPLUS | UNC_NOMINUS);
    if (( plusMinus != 0) // Neither, & not both
      && (plusMinus != (UNC_NOPLUS | UNC_NOMINUS))
      )
    {  // Just one asymetric uncertainty.
      plusMinus = ~plusMinus;  // Switch bits over.
      arg.unctypes_ &= ~(UNC_NOPLUS | UNC_NOMINUS); // Clear both.
      arg.unctypes_ |= plusMinus;  // Set switched.
    }
    return arg;
  }

  friend unc<is_correlated> ldexp(unc<is_correlated> arg,
    int intarg)  // Real from significand or mantissa arg & exponent intarg.
  {
    if (is_correlated)
      arg.uncertainty_ *= ldexp(1., intarg);
    else
      arg.uncertainty_ *= fabs(ldexp(1., intarg));
    arg.value_ = ldexp(arg.value_, intarg);
    arg.unctypes_ &= ~(UNC_KNOWN | UNC_NOPLUS | UNC_NOMINUS);  //
    return arg;  // arg * 2**intarg
  }

  friend unc<is_correlated> modf(unc<is_correlated> arg, double* intpart)
  { // Split unc into updated integer and return fraction parts.
    arg.value_ = modf(arg.value_, intpart);  // value = fractional part.
    // Must assume not rational fraction, so clear many bits.
    arg.unctypes_ &= ~(VALUE_INTEGER | VALUE_RATIONAL | UNC_KNOWN | UNC_NOPLUS | UNC_NOMINUS | UNC_EXPLICIT | DEG_FREE_EXACT | DEG_FREE_KNOWN);
    return arg; // Fractional part (mantissa or significand).
  }

  friend unc<is_correlated> frexp(unc<is_correlated> arg, int* intarg)
  {  // Raise arg to integer power.
    arg.uncertainty_ *= float(pow(2., double(-*intarg)));
    arg.value_ = frexp(arg.value_, intarg);
    // If zero, integer or rational remains so.
    return arg;
  }

  friend unc<is_correlated> fmod(const unc<is_correlated>& arg,
    const unc<is_correlated>& divisor)
  { // Floating-point remainder of x / divisor such that
    //   x = i * divisor + remainder,
    // where i is an integer, remainder has the same sign as x, and
    // absolute value of remainder is less than the absolute value of y.
    unc<is_correlated> remainder(0.);  // For remainder.
    if (divisor.value_ == 0.)
    { // ANSI 4.5.6.4 if divisor == zero return zero.
      remainder.value_ = 0.;
      remainder.uncertainty_ = std::numeric_limits<float>::infinity();
      remainder.unctypes_ = 0U;  //
    }
    else
    {
      double slope1 = 1. / divisor.value_; // If zero would cause divide by zero!
      double slope2 = ((arg.value_ / divisor.value_) > 0.) ?
        -floor(arg.value_ / divisor.value_) :
      floor(-arg.value_ / divisor.value_);
      if (is_correlated)
        remainder.uncertainty_ = float(slope1 * arg.uncertainty_
        + slope2 * divisor.uncertainty_);
      else
        remainder.uncertainty_ = float(sqrtSumSqrs(slope1 * arg.uncertainty_,
        slope2 * divisor.uncertainty_));
      remainder.value_ = fmod(arg.value_, divisor.value_);
      remainder.unctypes_ = arg.unctypes_;  // Better choice than divisor?
      remainder.unctypes_ &= ~(VALUE_INTEGER | VALUE_RATIONAL);
      // Remainder can't be integer & unlikely to be rational, so clear these.
    }
    return remainder;  // remainder.
  }

  friend unc<is_correlated> sqrt(unc<is_correlated> arg)
  { // sqrt(unc)
    arg.value_ = sqrt(arg.value_);
    if (is_correlated)
      arg.uncertainty_ /= 2. * arg.value_;
    else  // Uncorrelated.
      arg.uncertainty_ /= fabs(2. * arg.value_);
    arg.unctypes_ &= ~(VALUE_INTEGER); // Must assume not integer.
    // sqrt of rational reamins rational?
    return arg;
  }

  friend unc<is_correlated> sin(unc<is_correlated> arg)
  {
    if (is_correlated)
      arg.uncertainty_ *= cos(arg.value_);
    else
      arg.uncertainty_ *= fabs(cos(arg.value_));
    arg.value_ = sin(arg.value_);
    arg.unctypes_ &= ~(VALUE_INTEGER | VALUE_RATIONAL); // Must assume not.
    return arg;
  }

  friend unc<is_correlated> cos(unc<is_correlated> arg)
  {
    if (is_correlated)
      arg.uncertainty_ *= -sin(arg.value_);
    else
      arg.uncertainty_ *= fabs(sin(arg.value_));
    arg.value_ = cos(arg.value_);
    return arg;
  }

  friend unc<is_correlated> tan(unc<is_correlated> arg)
  {
    double costemp = cos(arg.value_);
    arg.uncertainty_ /= costemp * costemp;
    arg.value_ = tan(arg.value_);
    return arg;
  }

  friend unc<is_correlated> asin(unc<is_correlated> arg)
  {
    arg.uncertainty_ /= sqrt(1. - arg.value_ * arg.value_);
    arg.value_ = asin(arg.value_);
    return arg;
  }

  friend unc<is_correlated> acos(unc<is_correlated> arg)
  {
    if (is_correlated)
      arg.uncertainty_ /= -sqrt(1. - arg.value_ * arg.value_);
    else
      arg.uncertainty_ /= sqrt(1. - arg.value_ * arg.value_);
    arg.value_ = acos(arg.value_);
    return arg;
  }

  friend unc<is_correlated> atan(unc<is_correlated> arg)
  {
    arg.uncertainty_ /= 1. + arg.value_ * arg.value_;
    arg.value_ = atan(arg.value_);
    return arg;
  }

  friend unc<is_correlated> atan2(const unc<is_correlated>& arg1,
    const unc<is_correlated>& arg2)
  {
    unc<is_correlated> retval;
    double slope1 = 1., slope2 = 1.; // slope = d f(x)/dx or f'(x).
    double sum2 = arg2.value_ * arg2.value_ + arg1.value_ * arg1.value_;
    if (sum2 != 0.)
    {
      slope1 = arg2.value_ / sum2;
      slope2 = -arg1.value_ / sum2;
    }
    if (is_correlated)
    {
      retval.uncertainty_ = slope1 * arg1.uncertainty_
        + slope2 * arg2.uncertainty_;
    }
    else
    { // Uncorrelated.
      retval.uncertainty_ = sqrtSumSqrs(slope1 * arg1.uncertainty_,
        slope2 * arg2.uncertainty_);
    }
    retval.value_ = atan2(arg1.value_, arg2.value_);
    return retval;
  }

  friend unc<is_correlated> exp(unc<is_correlated> arg)
  {
    arg.value_ = exp(arg.value_);
    if (is_correlated)
      arg.uncertainty_ *= arg.value_;
    else
      arg.uncertainty_ *= fabs(arg.value_);
    return arg;
  }

  friend unc<is_correlated> log(unc<is_correlated> arg)
  {
    if (is_correlated)
      arg.uncertainty_ /= arg.value_;
    else
      arg.uncertainty_ /= fabs(arg.value_);
    arg.value_ = log(arg.value_);
    return arg;
  }

  friend unc<is_correlated> log10(unc<is_correlated> arg)
  {
    const double oneDivLog10 = 0.434294481903251827651128918917;
    // Use Boost.Math?
    if (is_correlated)
      arg.uncertainty_ *= oneDivLog10 / arg.value_;
    else
      arg.uncertainty_ *= oneDivLog10 / fabs(arg.value_);
    arg.value_ = log10(arg.value_);
    return arg;
  }

  friend unc<is_correlated> sinh(unc<is_correlated> arg)
  {
    arg.uncertainty_ *= cosh(arg.value_);
    arg.value_ = sinh(arg.value_);
    return arg;
  }

  friend unc<is_correlated> cosh(unc<is_correlated> arg)
  {
    if (is_correlated)
      arg.uncertainty_ *= sinh(arg.value_);
    else
      arg.uncertainty_ *= fabs(sinh(arg.value_));
    arg.value_ = cosh(arg.value_);
    return arg;
  }

  friend unc<is_correlated> tanh(unc<is_correlated> arg)
  {
    double coshtemp = cosh(arg.value_);
    arg.uncertainty_ /= coshtemp * coshtemp;
    arg.value_ = tanh(arg.value_);
    return arg;
  }

  // Power
  friend unc<is_correlated> pow(const unc<is_correlated>& arg1,
    const unc<is_correlated>& arg2)
  {
    unc<is_correlated> retval;
    retval.value_ = pow(arg1.value_, arg2.value_);
    double slope1, slope2;

    if (0 == arg1.value_)
    {
      slope2 = 0.;  slope1 = 0.;
      if (1. == arg2.value_)
        slope1 = 1.;
    }
    else if (arg1.value_ < 0.)
    { // pow(arg1, arg2) for arg1 < 0. is only defined for integer arg2.
      slope1 = arg2.value_ * retval.value_ / arg1.value_;
      slope2 = 0.;
    }
    else
    {
      slope1 = arg2.value_ * retval.value_ / arg1.value_;
      slope2 = log(arg1.value_) * retval.value_;
    }
    if (is_correlated)
      retval.uncertainty_ = slope1 * arg1.uncertainty_
      + slope2 * arg2.uncertainty_;
    else
      retval.uncertainty_ = sqrtSumSqrs(slope1 * arg1.uncertainty_,
      slope2 * arg2.uncertainty_);
    return retval;
  }

  // Getter functions for read-only access to data members:
  // uncertainty_, value_, degrees of freedom and unc type.

  // These are duplicates.

  double mean(void) const {return value_;}

  float deviation(void) const {return uncertainty_;}  //

  unsigned short int degFree(void) const {return degFree_;}

  unsigned short int uncFlags(void) const {return unctypes_;} // 16 flags.

  // For testing single argument functions like sin,cos, tan, log, exp.
  friend unc<is_correlated> PropagateUncertaintiesBySlope(
    double (* certain_func)(double),  // For example sin
    const unc<is_correlated>& arg)  // (angle)
  {
    unc<is_correlated> retval;
    double std_deviation_up_value, std_deviation_down_value;
    retval.value_ = certain_func(arg.value_);
    std_deviation_up_value = certain_func(arg.value_ + arg.uncertainty_);
    std_deviation_down_value = certain_func(arg.value_ - arg.uncertainty_);
    retval.uncertainty_ = (std_deviation_up_value - std_deviation_down_value) * 0.5;
    if (!is_correlated) retval.uncertainty_ = fabs(retval.uncertainty_);
    return retval;
  }

  // For testing double argument functions like atan2, pow.
  friend unc<is_correlated> PropagateUncertaintiesBySlope
    (
    double (*certain_func)(double, double),  // for example atan2 or pow
    const unc<is_correlated>& arg1,  // For example arg1 is number to be raised to
    const unc<is_correlated>& arg2)  // arg2 power.
  {
    unc<is_correlated> retval;

    retval.value_ = certain_func(arg1.value_, arg2.value_);
    if (is_correlated)
    {
      double up_val = certain_func(arg1.value_ + arg1.uncertainty_,
        arg2.value_ + arg2.uncertainty_);
      double down_val = certain_func(arg1.value_ - arg1.uncertainty_,
        arg2.value_ - arg2.uncertainty_);
      retval.uncertainty_ = 0.5 * (up_val - down_val);
    }
    else
    {
      double up_val1 = certain_func(arg1.value_ + arg1.uncertainty_,
        arg2.value_);
      double down_val1 = certain_func(arg1.value_ - arg1.uncertainty_,
        arg2.value_);
      double up_val2 = certain_func(arg1.value_,
        arg2.value_ + arg2.uncertainty_);
      double down_val2 = certain_func(arg1.value_,
        arg2.value_ - arg2.uncertainty_);
      retval.uncertainty_ = 0.5 * sqrtSumSqrs(up_val1 - down_val1,
        up_val2 - down_val2);
    }
    return retval;
  }

  // Predicate Comparison functions.
  // Should be global to allow leftmost argument to use implicit type conversion
  // according to Scott Meyers, Effective C++, Item 19 p 68 - 71.
  // But these are member functions and seem to work OK.
  // Note MUST be static bool.
  //! Compare two uncertain values.
  //! \param l Uncertain value.
  //! \param r Uncertain value.
  //! \returns true if l is effectively less than r.
  //!
static bool lessU(const unc<is_correlated>& l, const unc<is_correlated>& r)
  { // less using Comparison criterion including ONE standard deviation.
    // (Comparison is possibly different for correlated case but not implemented yet).

    using boost::math::hypot; // sqrt(sum of squares) to estimate an 'average' uncertainty.

    const double lhi = (l.value_ + l.uncertainty_);  // Upper confidence limit of left.
    const double rlo = (r.value_ - r.uncertainty_); // Lower confidence limit of right.
    const double diff = (l.value_ + l.uncertainty_) - (r.value_ - r.uncertainty_);
    // diff = lhi - rlo;
    bool isLess =  lhi < (rlo + hypot(l.uncertainty_, r.uncertainty_)); // == less
    double tol = hypot(l.uncertainty_, r.uncertainty_); // 'Average' of two uncertainties.
#ifdef BOOST_QUAN_DIAGNOSTICS
    { // Output diagnostic info.
      std::cerr << "lessU " << l.value_ << ' '
        << lhi // (l.value_ + l.uncertainty_)
        << ' ' << ((isLess) ? " < " : " >= ")
        // << (r.value_ - r.uncertainty_) == rlo
        << ' ' << rlo << ' '<< rlo + hypot(l.uncertainty_, r.uncertainty_) // tol
        << ' ' << r.value_
        << ' ' << tol
        << ' ' << diff
        << std::endl;
    }
#endif // BOOST_QUAN_DIAGNOSTICS
    return isLess;
  } // bool lessU(const unc<is_correlated>& l, const unc<is_correlated>& r)

  static bool lessU2(const unc<is_correlated>& l, const unc<is_correlated>& r)
  { // less using Comparison criterion including TWO standard deviations.
    // (Comparison is possibly different for correlated case but not implemented yet).

    using boost::math::hypot;

    double lhi = (l.value_ + l.uncertainty_ + l.uncertainty_ );  // Upper confidence limit of left.
    double rlo = (r.value_ - r.uncertainty_ - r.uncertainty_);  // Lower confidence limit of right.
    // double diff = (l.value_ + l.uncertainty_) - (r.value_ - r.uncertainty_);
    double diff = lhi - rlo;
    double tol = hypot(l.uncertainty_ + l.uncertainty_, r.uncertainty_ + r.uncertainty_); // 'Average' of two uncertainties.
    bool isLess =  lhi < (rlo + hypot(l.uncertainty_+ l.uncertainty_, r.uncertainty_ + r.uncertainty_)); // == less
#ifdef BOOST_QUAN_DIAGNOSTICS
    { // Output diagnostic info.
      std::cerr << "lessU " << l.value_ << ' '
        << lhi // (l.value_ + l.uncertainty_ * 2)
        << ' ' << ((isLess) ? " < " : " >= ")
        // << (r.value_ - r.uncertainty_ * 2) == rlo
        << ' ' << rlo << ' '<< rlo + tol
        << ' ' << r.value_
        << ' ' << tol
        << ' ' << diff
        << std::endl;
    }
#endif // BOOST_QUAN_DIAGNOSTICS
    return isLess;
  } // bool lessU2(const unc<is_correlated>& l, const unc<is_correlated>& r)

  static bool moreU(const unc<is_correlated>& l, const unc<is_correlated>& r)
  { // less using Comparison criterion within ONE standard deviation.
    // Comparison is possibly different for correlated case but not implemented yet.
    return l.value_ - l.uncertainty_ > r.value_ + r.uncertainty_; // Values with uncertainty.
  } // bool moreU

  static bool equalU(const unc<is_correlated>& l, const unc<is_correlated>& r)
  { // less using Comparison criterion outside ONE standard deviation.
    // Comparison is possibly different for correlated case but not implemented yet.
    return !((l.value_ - l.uncertainty_ > r.value_ + r.uncertainty_) // >
     || (l.value_ + l.uncertainty_ < r.value_ - r.uncertainty_)); // <
  } // bool equalU

  static bool equalU2(const unc<is_correlated>& l, const unc<is_correlated>& r)
  { // less using Comparison criterion outside TWO standard deviations.
    // Comparison is possibly different for correlated case but not implemented yet.
    return !((l.value_ - l.uncertainty_ - l.uncertainty_> r.value_ + r.uncertainty_ + r.uncertainty_) // >
     || (l.value_ + l.uncertainty_ + l.uncertainty_ < r.value_ - r.uncertainty_ - r.uncertainty_)); // <
  } // bool equalU2

}; // template <bool is_correlated> class unc

//!  https://en.cppreference.com/w/cpp/language/operators operator<< must be non-member functions, 
//! friend std::ostream& operator<<(std::ostream& os, const T& obj)
//! https://en.cppreference.com/w/cpp/language/friend and may need to be friend  to access private data of the class
//! 

template <bool correlated>
std::ostream& operator<< (std::ostream& os, const std::pair< unc<correlated>, unc<correlated> >& up)
{ /*! Output a pair (X and Y) of uncertain values with (if defined) uncertainty and degrees of freedom.
     \details For example: "1.23 +/- 0.01 (13), 3.45 +/- 0.06 (78)".
   */
  os << up.first << ", " << up.second;
  return os;
} // std::ostream& operator<< (ostream& os, const pair<unc<correlated>, unc<correlated> >& u)

//template std::ostream& operator<< (std::ostream& os, const std::pair< unc<false>, unc<false> >& u);
// unc<false> version is not normally needed.

//! Two helper functions to provide values and uncertainties as pairs.
//! \note Names: value_of (single value) and plural valueS_of (pair).

/*! Allow value part of variables of class unc to be assigned to, and compared with double.
\tparam T Built-in floating-point type, float, double or long double, or uncertain type unc.
*/
template <class T>
double value_of(T);

/*! Allow uncertainty (standard deviation) part of variables of class unc to be assigned to, and compared with float.
\tparam T Built-in floating-point type, float, double or long double, or uncertain type unc.
*/
template <class T>
double unc_of(T);

// Pairs
template <class T> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<T, T>);

template <class T1, class T2> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<T1, T2>);

template <class T> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> uncs_of(std::pair<T, T>);

template <class T1, class T2> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> uncs_of(std::pair<T1, T2>);

// Predicate compare operators for use by sort etc.
// Functors are _preferred_ to functions for STL algorithms.
// Scott Meyers, ESTL, item 46, page 201..

template<typename Type> // Predicate Functor modelled on STL less in functional.
// Used below by min_element, etc.
// was struct lessAbs : public std::binary_function<Type, Type, bool>
  struct lessAbs : public std::function<Type>
{  // Usage: if (lessAbs<Meas>()(lm, hm)) ...
  bool operator() (const Type& a, const Type& b) const
  { // Note const to prevent modification - must be pure function.
    // (Implies operator< for Type must also be const!)
    return ((a < static_cast<Type>(0)) ? -a : a) < ((b < static_cast<Type>(0)) ? -b : b);
    // Allows use with both POD like int and double,
    // and other types for which both operator< and operator- are defined.
    // Instead of:
    // return fabs(a) < fabs(b); // fabs allows use with int and double,
    // at price of conversion to double.
    // But will fail for other types,  so abs function written here using only operator< and operator-.
  }

}; // template <bool is_correlated> class unc

// Specialization of autoprefix_norm for UDT boost::units::uncun
// See /boost-trunk/libs/units/example/measurement.hpp
// For autoprefix_norm see /boost/units/io.hpp.
// This specialization is required to get autoprefix to work with this class.

using boost::units::autoprefix_norm_impl;

autoprefix_norm_impl<unc<false>, true>::type
autoprefix_norm(const unc<false> & arg)
{
  return autoprefix_norm_impl<double, true>::call(arg);
}

//! 'Unique' value used to check xalloc initialised iwords are OK, and are not corrupted.
static const long indexID = 0x48dbaf8;  //! Random id value.

/*!
  \brief Set 16 default flag bit values for uncertain output flags.
  Might be best to make setUncDefaults also set std::ios_base defaults
  with void setiosDefaults(ostream&);\n
  Usage: \code setUncDefaults(cout); \endcode
  \warning  @c setUncDefaults() MUST be called before using a stream.
  \details
     Default iword(*) values are all zero - these are not usable.
     Function @c setUncDefaults() sets some defaults
     Can set individual value, for example, for 3 sig Digits, os.iword(sigDigitsIndex) = 3;
     \note Index values must have been initialised by xalloc calls.
*/
void setUncDefaults(std::ios_base& os)
{
  os.iword(zeroIndex) = indexID; // Mark stream starting to set to defaults:

  os.iword(uncFlagsIndex) = 0;  // long& uncFlags == iword(1)
  // uncFlags defaults mean flex, not set_scaled, not auto_scaled, not +/-,
  // no symbol, no prefix, no noisy, not setSigDigits.
  os.iword(oldUncFlagsIndex) = 0;  // Old  - not set yet. long& oldUncFlagsIndex == iword(2)
  os.iword(sigDigitsIndex) = 3;  // Default 3 significant digits for value.
  os.iword(oldSigDigitsIndex) = 3;  // Old @b set 3 significant digits for value.
  os.iword(setSigDigitsIndex) = 3;  // Default @b set 3 significant digits for value.
  os.iword(uncSigDigitsIndex) = 2;  // Default @b set 3 significant digits for value.
  os.iword(setUncSigDigitsIndex) = 2;  // Default 2 significant digits, as ISO standard.
  os.iword(oldUncSigDigitsIndex) = -1;  // No previous value for unc stdDev.
  os.iword(scaleIndex) = 0;  // Zero scale means was not scaled.
  os.iword(oldScaleIndex) = -1;  // No previous scale.
  os.iword(setScaleIndex) = 0;  // Assigned by uncSetScale(9).
  os.iword(uncWidthIndex) = 10;  // Default width 10 to allow +1.23E+12 & 1 fill_char.
  os.iword(oldUncWidthIndex) = -1;
  os.iword(oldUncSetWidthIndex) = -1;  // Not used?
  os.iword(usedIndex) = 0; //  No chars output = used.
  os.iword(oldUncUsedIndex) = -1;  //  No previous value.
  os.iword(widthIndex) = -1; // ios setwidth
  os.iword(oldWidthIndex) = -1; // previous ios setwidth
  os.iword(roundingLossIndex) = 50; // 0.01 * 1000; // == 0.01
  os.iword(confidenceIndex) = 50000; // == 0.05 * 1e6

  os.iword(topIndex) = indexID;  // last .iword(16) == iword(0)
  // marking that all have been set to defaults.
  if (os.iword(zeroIndex) != indexID)
  { // Initialization of iwords failed!
    std::cerr << "os.iword(0) = "  << std::hex << os.iword(0)
      << " not expected ID " << indexID << '!'
      << "\nMissing call of setUncDefaults(ostream)?" 
      << std::endl;
  }
} // void setUncDefaults(std::ios_base& stream)

/*! Output ALL the @c os.word() values to the @c std::ostream& log file, for example: setSigdigits, uncFlags, uncSigDigits.
  \param os Current @c std::ostream& to be displayed in log.
  \param log @c std::ostream& for log.
*/
void outUncValues(std::ostream& os = std::cout, std::ostream& log = std::cerr)
{ // Output ALL the os.word() values to the @c std::ostream& log.
  if (os.iword(zeroIndex) != indexID)
  {
    std::cout << "Magic index word is corrupted, should be " << std::hex << indexID << std::dec << "!" << std::endl;
  }
  if (os.iword(topIndex)  != indexID)
  {
    std::cout << "Magic index top word is corrupted, should be "  << std::hex << indexID << std::dec << "!" << std::endl;
  }
  log 
   // << std::hex << "indexID " << os.iword(zeroIndex)  << std::dec // = indexID; // Mark if has been set to defaults.
    << " UncValues: "
    <<  "uncFlags " << std::hex << os.iword(uncFlagsIndex) << std::dec //= 0;  // iword(1) holding
    //  bits meaning: add_+/-  add_SI_symbol add_SI_prefix addnoisy set_sigDigits adddegfree replicates addlimits...
    << ", setSigDigits " << os.iword(setSigDigitsIndex) // = 3;  // Default set 3 sig digits.
    << ", uncWidth " << os.iword(uncWidthIndex) // = 10;  // Default width 10 to allow +1.234E12
    << ", uncSetWidth " << os.iword(oldUncSetWidthIndex) // = 0;  // Not used
    << ", uncScale " << os.iword(scaleIndex) // = 0;  // Zero scaling means was not scaled.
    << ", uncSetScale " << os.iword(setScaleIndex) // = 0;  // Zero scaling means not scaled.
    << ", uncUsed " << os.iword(usedIndex) // = 0; //  None used.
    // Values saved by previous calls of unc ouput.
    << ", uncOldFlags " << os.iword(oldUncFlagsIndex) // = 0;  // 0xFFFFFFFF to mean not valid?
    << ", uncOldUncWidth " << os.iword(oldUncWidthIndex) // = 0;
    << ", uncOldScale " << os.iword(oldScaleIndex) // = 0;
    << ", uncSigDigits " << os.iword(setUncSigDigitsIndex) // = 2;  // Default 2 sig digits.
    << ", uncoldSigDigits " << os.iword(oldUncSigDigitsIndex) // = 0;  //  No previous value.
    << ", oldUncUsed " << os.iword(oldUncUsedIndex) // = 0;  //  No previous value.
    << ", oldStdDevSigDigits " << os.iword(oldUncSigDigitsIndex) // = 0;  // No previous value for unc stdDev.
    << ", setUncSigDigits " << os.iword(setUncSigDigitsIndex)
    << ", roundingLossIndex " << os.iword(roundingLossIndex) / 1.e3
    << ", confidenceIndex " << os.iword(confidenceIndex) / 1.e6
     << ", top " << std::hex << os.iword(topIndex) << std::dec // = indexID;  // last .iword(16) as check.
    << std::endl;
  /* Before call of setUnc_defaults() all are zero (probably) and after call:
  * indexID 48dbaf8
    UncValues: 
      uncFlags 0, setSigDigits 3, uncWidth 10, uncSetWidth -1, uncScale 0, uncSetScale 0, uncUsed 0, uncOldFlags 0, uncOldUncWidth -1, uncOldScale -1, uncSigDigits 2, uncoldSigDigits -1, oldUncUsed -1, oldStdDevSigDigits -1, setUncSigDigits 2, roundingLossIndex 0.05, confidenceIndex 0.05, 
    top 48dbaf8
  */
} // void outUncValues()



  /*! Output word description for each `unc_type` bit.\n
  Usage:  outUncTypes(unc.getUncTypes(), cerr); // logs to cerr.
  \param uncTypes uncertain types as a short int.
  \param os @c std::ostream& for output, default = cerr
*/
void outUncTypes(unsigned short int uncTypes, std::ostream& os = std::cerr)
{// Usage:  \code outUncTypes(unc.getUncTypes(), cerr); \endcode // logs to cerr.
  const int count = 16;  // because using 16-bit unsigned short int.
  os << "uncTypes ("  << std::showbase << std::hex << uncTypes << std::dec << ")";
  for (int i = 0, j = 1; i < count; ++i)
  {
    if ((uncTypes & j) != 0)
    {
      os << ' ' << uncTypeWords[i];
    }
    j <<= 1;
  } // for
  os << ".";
}  // outUncTypes

//   /*!
//  Usage: \code out << showUncTypes(uncType) \endcode
//  \param ut Uncertain type flags for the @c std::ostream.
//  \param os @c std::ostream& for output of uncertain types as words, for example: integer, zero, df_exact.
//*/
//std::ostream& operator<< (std::ostream& os, const showUncTypes& ut)  // Definition.
//{
//  const int count = 16;  // because using 16-bit unsigned short int.
//  unsigned short int uncTypes = ut.types;
//  os << "uncTypes (" << std::showbase << std::hex << uncTypes  << std::dec << ")";
//  for (int i = 0, j = 1; i < count; ++i)
//  {
//    if ((uncTypes & j) != 0)
//    {
//      os  << ' ' << uncTypeWords[i];
//    }
//    j <<= 1;
//  } // for
//  os << ".";
//  return os;
//} // ostream& operator<< (ostream& os, const showUncTypes& ut)

/*! Show all the @c std::ios stream& flags settings as words, for example: true, dec, right.\n
  Usage:   \code outIosFmtFlags(std::cout.flags(), std::cerr); // logs cout's flag to cerr. \endcode
  \param flags Iostream& flags.
  \param os @c std::ostream& for output, default @c std::cerr
  */
void outIosFmtFlags(long flags, std::ostream& os = std::cerr)
{ // Show all the std::ios stream& flags settings as words.
  os << "iosflags (" << flags << ")" << std::dec;
  if (flags & std::ios_base::boolalpha) //  Show bool as word strings "true" or "false".
    os << " boolalpha";
  if (flags & std::ios_base::skipws) //   Skip white space on input.
    os << " skipwhite";
  if (flags & std::ios_base::left) //  Left-align values; pad on the right with the fill character.
    os << " left";
  if (flags & std::ios_base::right) //   Right-align values; pad on the left with the fill character (default alignment).
    os << " right";
  if (flags & std::ios_base::internal) // Add fill characters after any leading sign or base indication, but before the value.
    os << " internal";
  if (flags & std::ios_base::dec) // Format numeric values as base 10 (decimal) (default radix).
    os << " dec";
  if (flags & std::ios_base::oct) // Format numeric values as base 8 (octal).
    os << " oct";
  if (flags & std::ios_base::hex) // Format numeric values as base 16 (hexadecimal).
    os << " hex";
  if (flags & std::ios_base::showbase) // Display numeric constants in a format that can be read by the C++ compiler.
    os << " showbase";
  if (flags & std::ios_base::showpoint) // Show decimal point and trailing zeros for floating-point values.
    os << " showpoint";
  if (flags & std::ios_base::showpos) // Display plus sign in non-negative field.
    os << " showpos";
  if (flags & std::ios_base::uppercase) //  Display uppercase A through F for hexadecimal values and E for scientific values.
    os << " upper";
  if (flags & std::ios_base::showpos) // Show plus signs (+) for positive values.
    os << " show +";
  if (flags & std::ios_base::scientific) // Display floating-point numbers in scientific format, for example: 1.23457e+001.
    os << " scientific";
  if (flags & std::ios_base::fixed) // Display floating-point numbers in fixed format, for example: 12.3456.
    os << " fixed";
  if ((flags & std::ios_base::scientific) && (flags & std::ios_base::fixed)) // Display floating-point numbers in hex format.
    os << " hexfloat";
  os << ".";
}  // out ios_base::flags(long flags, ostream&)

/*! Show the set uncertain class io stream& flags settings as words, for examples: add_/-, set_scaled, addlimits .
  \param uncFlags Output flags value to be displayed as words.
  \param os std::ostream& for output.
  Usage:  
    \code
      outUncIOFlags(std::cout.iword(1), std::cerr); // uncFlags (0xa08) add_+/-  adddegfree replicates addlimits
   \endcode

   \sa Version that takes the std::ostream& as parameter
   \code void outUncIOFlags(std::ostream& os = std::cout, std::ostream& osout = std::cerr, std::string terminator = ".\n") \endcode
*/
void outUncIOFlags(long uncFlags, std::ostream& os = std::cerr, std::string terminator = ".\n")
{
  os << "uncFlags (" << std::showbase << std::hex << uncFlags << std::dec << ")";
  os << ((uncFlags & firm) ? " firm" : ""); // bit 1 = 1
  if (uncFlags & setScaled) os << " set_scaled";
  if (uncFlags & autoScaled) os << " auto_scaled";
  if (uncFlags & plusMinus) os << " add_+/- ";
  if (uncFlags & addSISymbol) os << " add_SI_symbol";
  if (uncFlags & addSIPrefix) os << " add_SI_prefix";
  if (uncFlags & noisyDigit) os << " addnoisy";
  if (uncFlags & useSetSigDigits) os << " set_sigDigits";
  if (uncFlags & useSetUncSigDigits) os << " set_uncsigDigits";
  if (uncFlags & degfree) os << " add_degfree";
  if (uncFlags & degfree) os << " add_replicates";
  if (uncFlags & limits) os << " add_limits"; // hex 800 = 2048
  os << terminator;
} //

/*! Show the set uncertain class io stream flags settings as words, for examples: add_/-, set_scaled, addlimits .
  \param os @c std::ostream whose unc flags are to be output.
  \param oslog @c std::ostream log to receive output.
  Usage:
    \code
      outUncIOFlags(std::cout.iword(1), std::cerr); // uncFlags (0xa08) add_+/-  adddegfree replicates addlimits
   \endcode

   \sa Version that takes the long uncFlags as parameter
   \code void outUncIOFlags(long uncFlags, std::ostream& osout = std::cerr, std::string terminator = ".\n") \endcode
*/
void outUncIOFlags(std::ostream& os = std::cout, std::ostream& osout = std::cerr, std::string terminator = ".\n")
{
  outUncIOFlags(os.iword(uncFlagsIndex));
}

// Parameterless manipulators to switch format to switch uncFlag bits,
// flex - width just enough to display, suitable for in-line display non-tables,
// or firm to fit into a set width, suitable for tables,
// & similarly for scale and autoscale flag bits.
// Note all lowercase to match convention of hex, oct ...

std::ios_base& flexform(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~firm;  // clear bit 0 = 0 to mean flex.
  return iostr;
}

std::ios_base& firmform(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) |= firm;  // set bit 0 = 1 to mean firm not flex.
  return iostr;
}

std::ios_base& scale(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) |= setScaled;  // set bit 1 = 1 to mean scaled.
  return iostr;
}

std::ios_base& noscale(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~setScaled;  // clear bit 1 = 0 to mean not scaled.
  return iostr;
}

std::ios_base& autoscale(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) |= autoScaled;  // set bit 2 = 1 to mean auto.
  return iostr;
}

std::ios_base& noautoscale(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~autoScaled;  // clear bit 2 = 0 to mean fixed.
  return iostr;
}

std::ios_base& plusminus(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) |= plusMinus;  // set bit 3 = 1 to mean plusminus.
  return iostr;
}

std::ios_base& noplusminus(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~plusMinus;  // clear bit 3 = 0 to mean not.
  return iostr;
}

std::ios_base& addsisymbol(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) |= addSISymbol;  // set bit 4 = 1 to add SI symbol.
  return iostr;
}

std::ios_base& nosisymbol(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~addSISymbol;  // clear bit 4 = 0 for no SI symbol.
  return iostr;
}

std::ios_base& addsiprefix(std::ios_base& iostr)
{ // Prefix (like kilo) used rather than Symbol (like k) if both SI prefix & symbol set.
  iostr.iword(uncFlagsIndex) |= addSIPrefix;  // set bit 5 = 1 to add SI prefix
  return iostr;
}

std::ios_base& nosiprefix(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~addSIPrefix;  // clear bit 5 = 0 for NO prefix.
  return iostr;
}

std::ios_base& addnoisyDigit(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) |= noisyDigit;  // set bit 6 = 1 to add noisy digit
  return iostr;
}

std::ios_base& nonoisyDigit(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~noisyDigit;  // clear bit 6 = 0 for NO noisy digit.
  return iostr;
}

std::ios_base& autosigdigits(std::ios_base& iostr)
{ // Use auto (calculated from uncertainty) not sig digits stored with `<< setSigDigits(6)` for value.
  iostr.iword(uncFlagsIndex) &= ~useSetSigDigits;  // clear bit 7 = 0 for auto sigdigits.
  return iostr;
}

std::ios_base& setsigdigits(std::ios_base& iostr)
{ // Use sig digits stored with `<< setSigDigits(6)` for value (not calculated from uncertainty).
  iostr.iword(uncFlagsIndex) |= useSetSigDigits;  // set bit 7 = 1 to use set sigdigits.
  return iostr;
}

std::ios_base& autouncsigdigits(std::ios_base& iostr)
{ // Calculate stdDev sig digits from uncertainty.
  iostr.iword(uncFlagsIndex) &= ~useSetUncSigDigits;  // clear bit 8 = 0 for auto unc sigdigits.
  return iostr;
}

std::ios_base& setuncsigdigits(std::ios_base& iostr)
{ //!< Use stdDev sigDigits stored with `<< useSetUncSigDigits(2) ...`
  iostr.iword(uncFlagsIndex) |= useSetUncSigDigits;  // set bit 8 = 1 to use set uncsigdigits.
  return iostr;
}

std::ios_base& adddegfree(std::ios_base& iostr)
{  // Add degrees of freedom as `(99)`.
  iostr.iword(uncFlagsIndex) |= degfree;  // set bit 9 = 1 to mean show degrees of freedom.
  return (iostr);
}

std::ios_base& nodegfree(std::ios_base& iostr)
{ // Do not add degrees of freedom
  iostr.iword(uncFlagsIndex) &= ~degfree;  // clear bit 9 = 0 to mean no degrees of freedom.
  return (iostr);
}

std::ios_base& addreplicates(std::ios_base& iostr)
{ // Show replicates (but only if if > 1).
  iostr.iword(uncFlagsIndex) |= replicates;  // set bit 0xA = 1 to mean replicates > 1.
  return (iostr);
}

std::ios_base& noreplicates(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~replicates;  // clear bit 0xA = 0 to mean no replicates.
  // BUT still show degrees of freedom if required.
  return (iostr);
}

std::ios_base& addlimits(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) |= limits;  // Set bit 0xB = 0 to mean show limits.
  return (iostr);
}

std::ios_base& nolimits(std::ios_base& iostr)
{
  iostr.iword(uncFlagsIndex) &= ~limits;  // clear bit 0xB = 0 to mean no limits shown.
  return (iostr);
}

// May need istream& operator>> versions too?

std::ios_base& showuncflags(std::ios_base& iostr)
{
  showUncFlags(static_cast<unsigned short>(iostr.iword(uncFlagsIndex)));
  return iostr;
}

// Two overloaded uFlags functions, like ios_base setf.
// (1st just returns current, other sets & returns old).
long uFlags(std::ios_base& str)
{
  return str.iword(uncFlagsIndex);  // Return streams current unc flags.
  //  Unlike ios_base member function fmtflags ios_base::flags(),
  //  these need to know their ios_base.
}

long uFlags(std::ios_base& str, long flags) // Assign new & return old.
{
  // lock();  may be required for multitasking (MT defined).
  long oldflags = str.iword(uncFlagsIndex);  // Save to return
  str.iword(uncFlagsIndex) = flags;  // Assign all bits.
  // unlock();
  return oldflags;  // Previous flags.
}

long setuFlags(std::ios_base& str, long flags)  // Set selected bits & return old.
{
  long oldflags = str.iword(uncFlagsIndex);  // Save to return.
  // lock();
  str.iword(uncFlagsIndex) |= flags;  // OR Set = 1 specific bits.
  // unlock();
  return oldflags;  // Previous;
}

long resetuFlags(std::ios_base& str, long flags)  // Reset bits & return old.
{
  long oldflags = str.iword(uncFlagsIndex);  // to return
  // lock();
  str.iword(uncFlagsIndex) &= (~flags);  // reset specified bits.
  // unlock();
  return oldflags;  // Previous.
}

// Manipulator creation using class whose name is that of the manipulator
// and << and >> operators.
// Steve Teale p 182 suggests that this is longer code than using template
// O, I or Smanip, but simpler to code and understand, and will run faster.

// Manipulator uncFlags. Usage: out << showUncFlags(flags)

// Class showUncFlags whose name is that of the manipulator & << >> operators,
// defined in unc.hpp header, but constructor defined in unc.cpp.

showUncFlags::showUncFlags(unsigned short int f) : flags(f)
{ //! Constructor.
  //! Usage: out << showUncFlags(static_cast<long>(is.iword(uncFlagsIndex))) ...
}

//! Output uncFlags as descriptive word strings to this @c std::ostream.
std::ostream& operator<< (std::ostream& os, const showUncFlags& uf)  // Definition.
{ 
  unsigned short uncFlags = uf.flags;
  os << "uncFlags ("<< std::hex << uncFlags << std::dec << ")";
  os << ((uncFlags & firm) ? " firm" : "");
  if (uncFlags & setScaled) os << " setScaled";
  if (uncFlags & autoScaled) os << " autoScaled";
  if (uncFlags & plusMinus) os << " add_+/-";
  if (uncFlags & addSISymbol) os << " add_SI_symbol";
  if (uncFlags & addSIPrefix) os << " add_SI_prefix";
  if (uncFlags & noisyDigit) os << " add_noisy";
  if (uncFlags & useSetSigDigits) os << " use_set_sigfig";
  if (uncFlags & useSetUncSigDigits) os << " use_set_uncsigfig";
  os << ".";
  return os;
} // ostream& operator<< (ostream& os, const showUncFlags&)
  //long& uncFlags = os.iword(uncFlagsIndex);

// setAllUncflags(int flags);
// Usage: out << setAllUncFlags(0x5a) ...
setAllUncFlags::setAllUncFlags(int w) : flags(w) {}
// Constructor initialisation assigns flags = w.

std::ostream& operator<< (std::ostream& os, const setAllUncFlags& sf)
{
  os.iword(oldUncFlagsIndex) = os.iword(uncFlagsIndex);  // Save all old flags.
  os.iword(uncFlagsIndex) = sf.flags;  // Set all flags.
  return os;
}

std::istream& operator>> (std::istream& is, const setAllUncFlags& sf)
{
  is.iword(oldUncFlagsIndex) = is.iword(uncFlagsIndex);  // Save all old flags.
  is.iword(uncFlagsIndex) = sf.flags;  // Set all flags.
  return is;
}

// setUncflags(int flags, int mask);
// Usage: out << setUncFlags(0x7) ...
setUncFlags::setUncFlags(int w) : flags(w)
{ // Constructor initialisation flags = w.

}

std::ostream& operator<< (std::ostream& os, const setUncFlags& sf)
{
  os.iword(oldUncFlagsIndex) = os.iword(uncFlagsIndex);  // Save all oldflags.
  os.iword(uncFlagsIndex) |= sf.flags;  // Set (OR in) selected flags.
  return os;
}

std::istream& operator>> (std::istream& is, const setUncFlags& sf)
{
  is.iword(oldUncFlagsIndex) = is.iword(uncFlagsIndex);  // Save all old flags.
  is.iword(uncFlagsIndex) |= sf.flags;   // Set (OR in) selected flags.
  return is;
}

// setMaskedUncflags(int flags, int mask);
// Usage: out << setMaskedUncFlags(0xFFFF, 0x7) ...
setMaskedUncFlags::setMaskedUncFlags(int w, int m) : flags(w), mask(m) {}
// flags = w; mask = m; in effect.

std::ostream& operator<< (std::ostream& os, const setMaskedUncFlags& sf)
{
  os.iword(oldUncFlagsIndex) = os.iword(uncFlagsIndex);  // Save all flags.
  os.iword(uncFlagsIndex) =
    (sf.flags & sf.mask) | (sf.flags & ~sf.mask);  // Set selected flags to mask.
  return os;
}

std::istream& operator>> (std::istream& is, const setMaskedUncFlags& sf)
{
  is.iword(oldUncFlagsIndex) = is.iword(uncFlagsIndex);  // Save all flags.
  is.iword(uncFlagsIndex) =
    (sf.flags & sf.mask) | (sf.flags & ~sf.mask);  // Set selected flags to mask.
  return is;
}

// resetUncflags(int flags);
// Usage: out << setMaskedUncFlags(0xF) ...
resetUncFlags::resetUncFlags(int w) : flags(w) {}  // Constructor initialisation flags = w.

std::ostream& operator<< (std::ostream& os, const resetUncFlags& sf)
{
  os.iword(oldUncFlagsIndex) = os.iword(uncFlagsIndex);  // Save previous.
  os.iword(uncFlagsIndex) &= ~sf.flags;  // Clear specific bits = 0.
  return os;
}

std::istream& operator>> (std::istream& is, const resetUncFlags& sf)
{
  is.iword(oldUncFlagsIndex) = is.iword(uncFlagsIndex);  // Save previous.
  is.iword(uncFlagsIndex) &= ~sf.flags;  //  Clear specific bits = 0.
  return is;
}

// setMaskedUncflags(int flags, int mask);
// Usage: out << setMaskedUncFlags(0xFFFF, 0x7) ...
resetMaskedUncFlags::resetMaskedUncFlags(int w, int m) : flags(w), mask(m) {}
// Constructor initialisation flags = w.

std::ostream& operator<< (std::ostream& os, const resetMaskedUncFlags& sf)
{
  os.iword(oldUncFlagsIndex) = os.iword(uncFlagsIndex);  // Save all flags.
  os.iword(uncFlagsIndex) =
    (sf.flags & sf.mask) & ~(sf.flags & ~sf.mask);  // Clear selected flags to mask.
  return os;
}

std::istream& operator>> (std::istream& is, const resetMaskedUncFlags& sf)
{
  is.iword(oldUncFlagsIndex) = is.iword(uncFlagsIndex);  // Save all flags.
  is.iword(uncFlagsIndex) =
    (sf.flags & sf.mask) & ~(sf.flags & ~sf.mask);  // Clear selected flags to mask.
  return is;
}

// Usage: out << setUncWidth(12) ...
setUncWidth::setUncWidth(int w) : uncWidth(w)
{ // Constructor.
}

std::ostream& operator<< (std::ostream& os, const setUncWidth& sw)
{
  os.iword(oldUncWidthIndex) = os.iword(uncWidthIndex);  // Save old.
  os.iword(uncWidthIndex) = sw.uncWidth;  // Set new uncertain width.
  return os;
}

std::istream& operator>> (std::istream& is, const setUncWidth& sw)
{
  is.iword(oldUncWidthIndex) = is.iword(uncWidthIndex);  // Save old.
  is.iword(uncWidthIndex) = sw.uncWidth;
  return is;
}

// setScale(int scale);
// Usage: out << setScale(6)  ... // == 10**6
// & << scale to use this set scale value, or << noscale to ignore.
setScale::setScale(int scale) : scale(scale)
{ // Constructor.
}

std::ostream& operator<< (std::ostream& os, const setScale& sc)
{ //
  os.iword(oldScaleIndex) = os.iword(setScaleIndex);  // Save old.
  os.iword(setScaleIndex) = sc.scale;
  return os;
}

std::istream& operator>> (std::istream& is, const setScale& sc)
{
  is.iword(oldScaleIndex) = is.iword(setScaleIndex);  // Save old.
  is.iword(setScaleIndex) = sc.scale;
  return is;
}

/*! setSigDigits(int sigDigits);
\brief Set the number of significant digits that can be used.
(If required by `<< sigfiged` then use set sigDigits value, or noSigfiged to @b NOT use sigDigits).
// Usage: `out << setSigDigits(5) << setsigdigits ...`
// & either `<< setsigdigits` to use set sigDigits value, or ` << nosetsigdigits` to not use the set sigDigits.
*/
setSigDigits::setSigDigits(int w) : sigDigits_(w)
{  // Constructor sigDigits = w;
}

std::ostream& operator<< (std::ostream& os, const setSigDigits& sf)
{
  os.iword(oldSigDigitsIndex) = os.iword(setSigDigitsIndex);  // Save previous.
  os.iword(setSigDigitsIndex) = sf.sigDigits_;  // Set new sigDigits for value.
  return os;
}

std::istream& operator>> (std::istream& is, const setSigDigits& sf)
{
  is.iword(oldSigDigitsIndex) = is.iword(setSigDigitsIndex);  // Save previous.
  is.iword(setSigDigitsIndex) = sf.sigDigits_;  // Set new sigDigits.
  return is;
}

//! setUncSigDigits(int sigDigits);
//! Usage: `out << setUncSigDigits(3) ...`
// has the effect `cout.iword[setUncSigDigitsIndex] = 3;`
setUncSigDigits::setUncSigDigits(int w) // : uncSigDigits_(w)
{
  if (w == 0)
  {
    w = 2;  // ISO default.
  }
  else if (w > 3)
  {
    w = 3;  // Limit to biggest that makes sense.
  }
  // Passes negative values through to allow
  // an auto mode for w < 0 that chooses from degrees of freedom,
  // (can't read degfree from here).
  // From table H page 457 in Oliver & Goldsmith, confidence interval
  // of standard deviation is about +/- 20% at 10 degrees of freedom,
  // and only < +/- 10% above 100 observations (needing 2 stdDev sig Digits).
  uncSigDigits_ = w;
} // Constructor sets uncSigDigits = w.

std::ostream& operator<< (std::ostream& os, const setUncSigDigits& usf)
{
  os.iword(oldUncSigDigitsIndex) = os.iword(setUncSigDigitsIndex); // Save previous.
  os.iword(setUncSigDigitsIndex) = usf.uncSigDigits_;  // Set new uncSigDigits.
  return os;
}

//! setroundingLoss(double eps);
//! Usage: out << setroundingLoss(0.01) ... = cout.iword[roundingLoss] = 0.01;
setRoundingLoss::setRoundingLoss(double eps)
{ // Constructor sets roundingLoss = parameter eps (scaled by operator<<).

  roundingloss_= eps;
}
std::ostream& operator<< (std::ostream& os, const setRoundingLoss& sl)
{ //! \note Can't store `double` in a `long`, so scale up to an integer.
  os.iword(roundingLossIndex) = static_cast<long>(sl.roundingloss_ * 1.e3);
  return os;
}

//! setConfidence(double alpha);
//! Usage: out << setConfidence(0.01) ... = cout.iword[confidence] = 0.01;
setConfidence::setConfidence(double alpha)
{  // Constructor sets confidence = parameter alpha (scaled by operator<<).
  confidence_= alpha;
}

std::ostream& operator<< (std::ostream& os, const setConfidence& sl)
{ //! \note Can't store `double` in a `long`, so scale up to an integer.
  os.iword(confidenceIndex) = static_cast<long>(sl.confidence_ * 1.e6);
  return os;
}

std::istream& operator>> (std::istream& is, const setUncSigDigits& usf)
{
  is.iword(oldUncSigDigitsIndex) = is.iword(setUncSigDigitsIndex); // Save previous.
  is.iword(setUncSigDigitsIndex) = usf.uncSigDigits_;  // Set new uncSigDigits.
  return is;
}

/*! Inputs uncertainty as value, (implicitly exact, std deviation = 0).
   & optionally an explicit measure of uncertainty [[+]|[-] <standard deviation * 2. >],
   (1.0 implies 1. +|- 0.5 and sd of 0.5, 1.00 implies 1. +|- 0.05 and sd of 0.05)

   & optionally degrees of freedom [(<short int>)] like (99)
   Used by istream& operator>> (istream&, unc<is_correlated>&)
   Original simple version:
  char plus, slash, minus;
  s >> value >> plus >> slash >> minus >> stdDev;
  if ((plus != '+') || (slash != '/') || (minus != '-'))
  {
  cerr << "Unexpected characters encountered in reading "
  "value +/- stdDev !" << endl;
  is.setf(ios_base::failbit);

  Used by std::istream& operator>> (std::istream& is, const unc
  }
*/
void unc_input(
  double& value,  // mean, central or most probable value.
  double& stdDev, // or float& perhaps?
  unsigned short int& degreesOfFreedom,  // = observations -1 .
  unsigned short int& types, // TODO settings bits.
  std::istream& is = std::cin)
{	
  std::streamsize avail = is.rdbuf()->in_avail();
  if (avail == 0)
  {
    std::cerr << "No input available!" << std::endl;
    value = std::numeric_limits<double>::quiet_NaN();
    stdDev = std::numeric_limits<float>::quiet_NaN();
    degreesOfFreedom = 0;
    types = 0U;
    return;  // todo errno?
  }

  // 'Default' values for unc_input arguments.
  value = 0.;	// double mean, central or most probable value.
  stdDev = 0.f;  // Exact float std deviation, unless find otherwise.
  degreesOfFreedom = 0;  // Default zero means 1 observation.
  types = 0U;
  //static_cast<unsigned short>(~(VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL| VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY | UNC_KNOWN | UNC_NOPLUS
  //	| UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_EXPLICIT | UNC_UNIFORM | UNC_TRIANGULAR | DEG_FREE_EXACT | DEG_FREE_KNOWN));

// All these need to be 'global' to uncertainRead, so declared here.
// char_traits<char>::int_type c;
// no_unused_variable_warning(c);
  int iv = 0;  // Integer part of floating point number.
  double fv = 0.;  // Fractional part of floating point number.
  int exponent = 0; // Exponent power of 10, default 10 ^ 0 = 1.
  bool isPositive = true; // Value input is >= 0.
  bool isNegativeValue = false; // Value input < 0.
  bool isPlusMinus = false;  // Normal case is +/- or +|-.
  bool isPlus = false; // +0
  bool isMinus = false; // -0
  bool isExactValue = true; // Assume no digits follow.
  bool isZeroValue = true; // Until find either integer or fractional part != 0
  bool isIntegerValue = false;
  bool isExponent = false;
  //	bool isValueZero = false;
  // Rationale for doing own read rather than using standard is to be able to distinguish
  // integer "0" or "123" implicitly exact from real "0." implicitly +/- 0.5.
  // Read integer part as one integer, option decimal as another, and fractional part as another,
  // then optional exponential part,
  // before any +/- and degrees of freedom as (999).

  is >> std::ws;  // Skip any leading whitespace.
  is >> iv;  // & read integer part of value (perhaps ii part of floating point number ii.ffe+ddd).
  // if (is.bad()) // Not useful to distinguish overflow etc.
  // if (is.eof()) // Sets fail for overflow, so not useful.
  if (!is)
  { // No integer part of value.
    std::cerr << "No numeric value input! " << boost::quan::showiostate << std::endl;
    value = std::numeric_limits<double>::quiet_NaN();
    stdDev = std::numeric_limits<float>::quiet_NaN();
    types = static_cast<unsigned short>(~(VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL| VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY | UNC_KNOWN | UNC_NOPLUS
      | UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_EXPLICIT | UNC_UNIFORM | UNC_TRIANGULAR | DEG_FREE_EXACT | DEG_FREE_KNOWN));
    // == 0U; // None of the above are true.
    // is.fail() == true;  is.good() == false; // already
    return;
  } // is.failed
  isIntegerValue = true; // but may be negated if find a decimal point later.
  types |= UNC_KNOWN;
  types &= ~UNC_EXPLICIT; // Clear bit because implicit from
  //  number of significant decimal digits - unless + or - provided.

  isNegativeValue = (iv < 0) ? true : false;
  // Overflow possible here if > numeric_limits<long>::max() == 2147483647
  // for example, 99999999999999.
  if (is.fail())
  {  // Check for overflow & other problems.
    std::cerr << "\t" "Uncertain value input failed! "<< boost::quan::showiostate << std::endl;
    value = std::numeric_limits<double>::quiet_NaN();
    stdDev = std::numeric_limits<float>::quiet_NaN();
    types = static_cast<unsigned short>(~(VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL| VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY | UNC_KNOWN | UNC_NOPLUS
      | UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_EXPLICIT | UNC_UNIFORM | UNC_TRIANGULAR | DEG_FREE_EXACT | DEG_FREE_KNOWN));
    // == 0U; // None of the above true.
    return;
  } // is.fail

  if (is.peek() == '.')   // locale.decimal_point[0] better.
  {  // A decimal fractional part may follow.
    is.get();  // Get (and discard) decimal_point.
    isIntegerValue = false; // Because . follows digit, so "9" is exact integer but "9." is not,
    // Even if no digits follow.
    isExactValue = false;
    stdDev = 0.5; // Implicit +/- if "9.", implies sd = 0.5.
    types |= UNC_QUAN_DECIMAL;  // Quantized by least significant decimal digit.
    types &= ~UNC_EXPLICIT;  // Clear - explicit because implied by significant digits.
    int count = 0; // Number of digits.  TODO ?? is this info used?
    if (isdigit(is.peek()))
    {  // There is a decimal fraction part.
      double fracTen = 1.;
      while (isdigit(is.peek()))
      {
        ++count;
        stdDev *= 0.1f; // Implicitly 1. means +/- 0.5 == stdDev = 0.05, 0.005 ...
        fracTen *= 0.1;
        int digit = (is.get() -'0');
        fv += digit * fracTen;
        // Potential for underflow to zero here?
      }
      if (isNegativeValue) fv = -fv;
      if (fv != 0)
      {
        isIntegerValue = false;
      }
    }  // Is a fraction part.
  }  // Is a decimal_point.
  else if (is.peek() == '/')
  { // Is a fraction, for example 2/3, so flag as a rational.
    is.get();  // Get (and discard) /.
    int denom;
    is >> denom;
    if (denom != 0)
    {
      value = static_cast<double>(iv)/denom;
      iv = 0;
      fv = 0;
      types |= VALUE_RATIONAL;
      types &= ~VALUE_ZERO;
    }
  }
  value += iv + fv; // Combine integer and fractional parts.

  if ((is.peek() & 0x5F) == 'E') // 'e' or 'E'
  { // Exponential part.
    is.get();  // and discard 'e' or 'E'.
    is >> exponent;
    isExponent = true;
  }  // Exponent

  if (value == 0)
  {
    isZeroValue = false;
    types |= VALUE_ZERO;
  }

  if (isIntegerValue)
  {
    value = iv;
    types |= (UNC_KNOWN | VALUE_INTEGER | VALUE_RATIONAL);
    //   types |= (UNC_KNOWN | VALUE_INTEGER | VALUE_RATIONAL | UNC_NOPLUS | UNC_NOMINUS);
#ifdef BOOST_QUAN_UNC_TRACE
    { // Implicitly exact +/- 0
      std::cerr << "    URead: Exact integer = " << value << std::endl;
    }
#endif // BOOST_QUAN_UNC_TRACE
  }
  else
  { // Real NOT integer.
    types |= UNC_KNOWN; // Known.
    types &= ~(VALUE_INTEGER | VALUE_RATIONAL | UNC_NOPLUS | UNC_NOMINUS);
#ifdef BOOST_QUAN_UNC_TRACE
    {
      std::cerr << "    URead: " << value << " implicit +/- " << stdDev << ". ";
    }
#endif // BOOST_QUAN_UNC_TRACE
  } // integer or not.

  is >> std::ws; // Skip optional whitespace between "123.45" and "+/-".

  //	Check if explicit uncertainty +/- (or +|-, or +-) or + or - follows,
  //	& read uncertainty if it does.
  double expUnc = 0.; // Explicit uncertainty.
  // 0x0F1 is files code for the single char '+-' symbol,
  if (!is.eof())
  { // Uncertainty (standard deviation) and degrees of freedom may follow.
    if ((is.peek() == '+') || (is.peek() == '-'))
    { // Some uncertainty follows, for example: +0.123, -0.456 or "+/-2.34" +|-3.45 
      std::char_traits<char>::int_type cusign = is.get(); // '+' or '-' read and discard.
      if (cusign == '+')
      {
        isPlus = true;
      }
      if (cusign == '-')
      {
        isMinus = true;
      }
      std::char_traits<char>::int_type p = is.peek();
      // 0xDD is File code for | vertical bar.
      // 0x0F1 is single char in files for '+-' symbol.
      if (p == '/' || p == '|' || p == '\\' || p == 0xDD || p == 0x0F1)
      { // Skip over any separator.
        std::char_traits<char>::int_type cusep = is.get(); // '/' or '|' or '\' chars.
        if (cusep ==  0x0F1)
        { // '+-' symbol, so must be both + and -.
          isPlus = true;
          isMinus = true;
        }
        // is >> ws; // Ignore any layout. But a bad idea to allow "+/ -"?
        p = is.peek(); // Expect + or - after separator.
        if (p == '+' || p == '-')
        { // +/- and +|- both allowed (or -/+ or -|+).
          // Assume +/+ uncertainty means just + and -/-, and uncertainty just -.
          std::char_traits<char>::int_type cusign2 = is.get(); // expect 2nd '+' or '-' char.
          if (cusign2 == '+')
          {
            isPlus = true;
            // else "*/+"
            is >> expUnc;  // Input Explicit uncertainty.
            if (!is.fail())
            { // Read OK, set bits.
              types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN);
            }
            else
            { // Reading uncertainty (standard deviation) value was not OK, so clear.
              types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN);
            }
          }  // == '+'
          else if (cusign2 == '-')
          {// else "*/-"
            isMinus = true;
            is >> expUnc; // Input Explicit negative uncertainty (have already consumed the - sign).
            if (!is.fail())
            { // Read OK, set bits.
              types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN);
            }
            else
            { // NotOK, so clear to unknown status.
              types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN);
            }
          } // == '-'
        }
        else
        { // Unexpected char (not + or -) after separator!
          //  TODO Need to signal fail!
        }
      }
      else if (cusign == '+')
      { // Positive uncertainty only, for example: "+0.123"
        isPlus = true;
        is >> expUnc; // Will consume the + sign.
        if (!is.fail())
        { // Read OK, set bits.
          types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN | UNC_NOMINUS);
        }
        else
        { // NotOK, so clear.
          types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN | UNC_NOMINUS);
        }
      }
      else if (cusign == '-')
      { // // Negative uncertainty only, for example: "-0.123"
        isMinus = true;
        is >> expUnc;
        // expUnc = -expUnc; // Have already consumed the '-' sign! But stddev is always positive.
        if (!is.fail())
        { // OK, so set uncertain type flags.
          types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN);
        }
        else
        { // NotOK so clear  flags.
          types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN);
        }
      }  // == '-'
    }

    if (isPlus || isMinus)
    { // Some plus and/or minus value has been input.
      types |= (UNC_KNOWN | UNC_EXPLICIT);
      if ((isPlus) && (isMinus))
      {
        isPlusMinus = true;  // If both plus and minus uncertainty.
        types &= ~(UNC_NOMINUS | UNC_NOPLUS); // Clear both exclusive,
        stdDev = static_cast<float>(expUnc);
      }
      else
        //   if (!isPlusMinus)  // Only + or -, but not both.
      {
        if (isPlus)
        {
          types |= UNC_NOMINUS;
        }
        else if (isMinus)
        {
          types |= UNC_NOPLUS;
        }
        stdDev = static_cast<float>(expUnc);
      } // not both.
    } //  isPlus or isMinus
    else
    { // Implicit.
      stdDev = static_cast<float>(stdDev);  //
    }

#ifdef BOOST_QUAN_UNC_TRACE
    {
      std::cerr << "    Explicit uncertainty ";
      if (isPlusMinus)
      {
        std::cerr << " +/- ";
      }
      else
      {
        std::cerr << ((isPlus) ? "+ " : "");
        std::cerr << ((isMinus) ? "- " : "");
      }
      std::cerr  << expUnc << ", stdDev " << stdDev << endl;

    } // trace
#endif // BOOST_QUAN_UNC_TRACE
    // Degrees of freedom?
    is >> std::ws; // Eat any whitespace before "(99)".
    if (!is.eof())
    { // Degrees of freedom as "(99)" may follow.
      // (Default degreesOfFreedom = 0; == 1 observation.
      if (is.peek() == '(')
      { // 99) follows
        std::char_traits<char>::int_type  c =	is.get();  // Discard '('
        long df; // degreesOfFreedom is only unsigned short 16 bit integer, so might overflow,
        is >> df;  // but read into a temporary long int to do some checks.
        if (is.fail())
        { // long int not read OK.
          degreesOfFreedom = (std::numeric_limits<unsigned short int>::max)(); // 0xFFFF; = Not known.
          types &= ~(DEG_FREE_KNOWN | DEG_FREE_EXACT); // Clear these flags.
        }
        else
        {  // long degrees of freedom read OK, so check >=0, and not too big.
          if (df < 0)
          { // Can't have negative degrees of freedom!
            degreesOfFreedom = (std::numeric_limits<unsigned short int>::max)(); // 0xFFFF; = Not known.
            types &= ~(DEG_FREE_KNOWN | DEG_FREE_EXACT); // NOT defined.
          }
          else
          { // df >= 0
            degreesOfFreedom =
              (df >= (std::numeric_limits<unsigned short int>::max)()) ?
              (std::numeric_limits<unsigned short int>::max)() -1
              // max df allowed.
              : static_cast<unsigned short int>(df); // 1 observation == 1 degfree.
          }
          c =	is.peek();  // Expect ')'
          if (c == ')')
          {
            is.get();  // Discard ')'
            types |= (DEG_FREE_KNOWN | DEG_FREE_EXACT);  // deg_free IS known and integer so exact.
          }
          if (c != ')' || (is.fail()))
          { // (99 OK but missing bracket.
            degreesOfFreedom = (std::numeric_limits<unsigned short int>::max)(); // 0xFFFF; = Not known.
            types &= ~(DEG_FREE_KNOWN | DEG_FREE_EXACT); // NOT defined.
          }
        }
      } // (99)
      else
      { // is EOF so NO (df)
        types |= DEG_FREE_KNOWN;  // deg_free IS implicitly known == 0.
        // degreesOfFreedom = 0U;  // Default == 1 observation.
      }
    } // eof
  } // eof
} // void unc_input

//
//!< \tparam T value type convertible to double.
template<>
double value_of(double v)
{ //! \return value as a double.
  return double(v);
}

template <class T> //!< \tparam T value type convertible to double.
double value_of(T v)
{ //! \return value as a double.
  double result = static_cast<double>(v.value());
  return result;
}

template<>
double value_of(unc<true> v)
{ //! \return unc.value() as a double.
  return v.value();
}

template<>
double value_of(unc<false> v)
{ //! \return unc.value() as a double.
  return v.value();
}

template <class T>
double unc_of(T v)
{ //! \return zero if no uncertainty information is available (for built-in double, float, or long double).
  return v.std_dev();
}

template <>
double unc_of(double)
{ //! \return zero if no uncertainty information is available (for built-in double, float, or long double).
  return 0.F;
}

template <>
double unc_of(float)
{ //! \return zero if no uncertainty information is available (for built-in double, float, or long double).
  return 0.F;
}

template <>
double unc_of(uncun u)
{ //! \return zero if no uncertainty information is available (for built-in double, float, or long double).
  return u.std_dev();
}

// Pairs of values.
template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<double, double> vp)
{ //!< \return values of a pair of double values.
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

// Const version
template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<double const, double> vp)
{ //!< \return values of a pair of double values.
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<uncun, uncun> vp)
{ //!< \return values of a pair of double values.
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<uncun const, uncun> vp)
{ //!< \return values of a pair of double values.
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

//! Get uncertainties (standard deviation) of a pair of values.
template <> //! \tparam T Built-in floating-point type or unc.
std::pair<double, double> uncs_of(std::pair<double, double> vp)
{
  return std::make_pair<double, double>(unc_of(vp.first), unc_of(vp.second));
}

//! Get uncertainties (standard deviation) as a pair of double values.
template <> //! \tparam T Built-in floating-point type or unc.
std::pair<double, double> uncs_of(std::pair<uncun, uncun> vp)
{
  return std::make_pair<double, double>(unc_of(vp.first), unc_of(vp.second));
  //! \return uncs_of uncertainties (standard deviation) as a pair of double values.
}

//! Get uncertainties (standard deviation) as a pair of const float values.
template <class T> //! \tparam T Builtin-floating point type or unc.
const std::pair<float, float> uncs_of(std::pair<const T, T> vp)
{ //! \return uncertainty parts (if any) as a pair of floats.
  // so can write
  // std::pair<const float, float> minmax = values_of(*result.first); // min unc & max unc for example/
  // whether T is built-in or unc.
  return std::make_pair(vp.first.unc(), vp.second.unc());
  //! \return uncs_of uncertainties (standard deviation) as a pair of @b float values.
}

} // namespace quan
} //namespace boost

#endif // BOOST_QUAN_UNC_HPP
