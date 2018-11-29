// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2018.

//! Forward declarations for uncertain class files.

// file unc_fwd.hpp

#ifndef BOOST_UNC_FWD_HPP
#define BOOST_UNC_FWD_HPP

#include <iostream>

namespace boost {
  namespace quan {

    template <bool is_correlated = false> // Default is uncertainties that are NOT correlated.
    class unc;

    // Two actual uncertain floating-point types for the user:

    typedef unc<false> uncun;  //! Uncertainties are NOT correlated.
                               //! Uncorrelated is the normal case when uncertainties add.

    typedef unc<true> unccorr;   //! Uncertainties ARE correlated.
    //! This is the unusual case where the sum must be exact,
    //! so the uncertainties are subtracted.
    //! Example: two blocks which fit into a box perfectly.
    //! So if both have an uncertainties, they must cancel when the uncertainties are added.
    //! Also applies to items like concentrations which must add up to 100 percent or unity.

    void unc_input(double&,  // mean (central or most probable) value.
      double&, // standard deviation.
      unsigned short int& ,  // degreesOfFreedom degrees of freedom, zero if just 1 observation.
      unsigned short int&,  // uncTypes uncertain types, zero, exact ...
      std::istream&); // is input stream

 // Output uncertain values.
    void outUncValues(std::ostream&, std::ostream&);
 // Set Unc class defaults for std::stream os.  
    void setUncDefaults(std::ios_base&);

                                         //! macros for 16 type bits used by unc uncTypes. Bit set == 1 == true means a positive attribute.
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
      | UNC_UNIFORM | UNC_TRIANGULAR);  //
    const unsigned short int DEG_FREE_DEF = (DEG_FREE_EXACT | DEG_FREE_KNOWN);

    extern const char* uncTypeWords[];

    
    //! uncertain flags that control of output of uncertain values, similar to @c std::ios flags.
    enum uncertainflags
    {
      defaults = 0, //!< Default.
      firm = 1 << 0,  //!< bit 0: == 0 == false means flex, or firm == 1 == true.
      setScaled = 1 << 1, //!< bit 1 Set scaled == 1 or not scaled == 0.
                          //! by fixed factor SetScale and << scale ...
                          //! power10 in range -max to +max.
                          autoScaled = 1 << 2,  //!< bit 2: auto = 1  with << autoscale ...
                          plusMinus = 1 << 3, /*!< bit 3 = 1 means add +/- uncertainty
                                              //! Usage:  \code std::cout << plusminus << u; \endcode
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
                                                                        // 0xC, D, E and F spare - and iword is actually a long so could use 32 flags.
    }; // enum uncertainflags

    void outFpClass(double, std::ostream&);  // Special output for inf, NaN...

                                             // Declarations of parameterless Manipulators,
                                             // similar to ios_base flags but for uncertain printing.
                                             // Use upperand lower case convention for these,
                                             // but all lowercase for manipulators like hex, oct ...
                                             // Usage:  out << scale << noscale << autoscale << noautoscale
                                             // firmform << flexform << plusminus << noplusminus ....

                                             //! All @c std::ios_base functions declared below are defined in unc.ipp
                                             //! \note that all of these are entirely lower case names, like @c std::ios manipulators.

    std::ios_base& scale(std::ios_base&); //!< To use value set with setScale(int)
    std::ios_base& noscale(std::ios_base&);  //!< or not scale.
    std::ios_base& autoscale(std::ios_base&); //!<  Automatically Scale to suitable prefix symbol like M, k, m...
    std::ios_base& noautoscale(std::ios_base&);  // or not to scale to prefix symbol.
    std::ios_base& firmform(std::ios_base&);  //!< Firm fixed layout using setUncWidth.
    std::ios_base& flexform(std::ios_base&); //!< Flexible free format.
    std::ios_base& plusminus(std::ios_base&);  //!< Add +/- uncertainty. 
    std::ios_base& noplusminus(std::ios_base&);  //!< Do not add +/- uncertainty
    std::ios_base& addsiprefix(std::ios_base&); //!< Add SI prefix like kilo, micro ..
    std::ios_base& nosiprefix(std::ios_base&);  //!< (Takes precedence over SI symbol if both set).
    std::ios_base& addsisymbol(std::ios_base&); //!< Add SI symbol like M, k, m.
    std::ios_base& nosisymbol(std::ios_base&);  //!< Do not add SI symbol like M, k, m.
    std::ios_base& addnoisyDigit(std::ios_base&);  //!< Add an extra 'noisy' digit for less risk of loss.
    std::ios_base& nonoisyDigit(std::ios_base&);  //!< No noisy to suit human reading.
    std::ios_base& autosigdigits(std::ios_base&);  //!< Calculate sigdigits from uncertainty (default).
    std::ios_base& setsigdigits(std::ios_base&);    //!< Use sig digits stored with `<< setSigDigits(6)` for value.
    std::ios_base& autouncsigdigits(std::ios_base&);  //!< Calculate stdDev sig digits from uncertainty.
    std::ios_base& uncsigdigits(std::ios_base&); //!< Use stdDev sigDigits stored with << useSetUncSigDigits(2) ...
    std::ios_base& adddegfree(std::ios_base&);  //!< Add degrees of freedom as (99).
    std::ios_base& nodegfree(std::ios_base&);  //!< Do not add degrees of freedom to output of value.
                                               //
    std::ios_base& addlimits(std::ios_base&);  //!< Add lower and upper limits a 0.95 > 1.00 > 1.05.
    std::ios_base& nolimits(std::ios_base&);  //!< Do not add lower and upper limits.

                                              // Obselete
                                              //std::ios_base& confidence(std::ios_base&);
                                              /*!< Use stdDev sigDigits stored with `<< setconfidence( 0.01) ...`
                                              or alpha to control estimation of confidence interval.  0.01 means 99% confidence.
                                              */
                                              //std::ios_base& roundingloss(std::ios_base&);  //!< Set the acceptable loss from rounding.
                                              //std::ios_base& addreplicates(std::ios_base&);  //!< Add degrees of replicates > 1 as [99].
                                              //std::ios_base& noreplicates(std::ios_base&);  //!< Do not add degrees of freedom or replicates >1.

                                              /*! Functions to change uncertain flags on specified ios_base.
                                              Usage:\n
                                              \code
                                                f = uFlags(out); f = uFlags(out, 0xFF);
                                                f = setuFlags(out, 0xFF); f = resetuFlags(out, 0xFF);
                                              \endcode
                                              */
    long uFlags(std::ios_base&);  //!< Returns current uncertain flags.
    long uFlags(std::ios_base&, long); //!< Assigns all uncertain flags & returns previous.
    long setuFlags(std::ios_base&, long);  //!< Set specific flags = 1
    long resetuFlags(std::ios_base&, long);  //!< Reset/clear specific flags = 0

                                             // Forward declarations.
    class showUncFlags;  //!< Output uncertain flags to @c ostream, for example: \code ostream& << showUncFlags  \endcode.
    class showUncTypes;  //!< Output uncertain types to @c ostream, for example: \code ostream& << showUncTypes  \endcode.

    class setAllUncFlags;  //!< Assign value to set (and/or clear) all unc flags.
    class setUncFlags;  //!< Set specific unc flags bits.
    class setMaskedUncFlags;  //!< Clear mask & then set unc flag bits.
    class resetUncFlags;  //!< Reset all unc flags bits.
    class resetMaskedUncFlags; //!< Reset specific unc flags bits.
    class setUncWidth; //!< Set width of uncertainty output.
    class setScale; //!< Set scaling factor like 1e3, 1e6 ... `<< setScale(6)`.
    class setSigDigits;  //!< sigdigits to use for value if `<< setsigdigits(4)`.
    class setUncSigDigits;  //!< sigdigits to use for uncertainty if `<< setuncsigdigits(1)`.
    class setRoundingLoss;  //!< Set acceptable loss due to rounding.
    class setConfidence;  //!< Set acceptable loss due to rounding.

                          // Operator declarations, classes defined below, with constructors in unc_io.hpp.
    std::ostream&operator<< (std::ostream&, const showUncFlags&);
    std::ostream&operator<< (std::ostream&, const setAllUncFlags&);
    std::istream&operator>> (std::istream&, const setAllUncFlags&);
    std::ostream&operator<< (std::ostream&, const setUncFlags&);
   // std::istream&operator>> (std::istream&, const setUncFlags&);
    std::ostream&operator<< (std::ostream&, const setMaskedUncFlags&);
    std::istream&operator>> (std::istream&, const setMaskedUncFlags&);
    std::ostream&operator<< (std::ostream&, const resetUncFlags&);
    std::istream&operator>> (std::istream&, const resetUncFlags&);
    std::ostream&operator<< (std::ostream&, const resetMaskedUncFlags&);
    std::istream&operator>> (std::istream&, const resetMaskedUncFlags&);
    std::ostream&operator<< (std::ostream&, const setUncWidth&);
    std::istream&operator>> (std::istream&, const setUncWidth&);
    std::ostream&operator<< (std::ostream&, const setScale&);
    std::istream&operator>> (std::istream&, const setScale&);
    std::ostream& operator<< (std::ostream&, const setUncSigDigits&);
    std::istream&operator>> (std::istream&, const setUncSigDigits&);
    std::ostream&operator<< (std::ostream&, const setRoundingLoss&);
    std::ostream&operator<< (std::ostream&, const setConfidence&);
    std::ostream&operator<< (std::ostream&, const setSigDigits&);
    std::istream&operator>> (std::istream&, const setSigDigits&);

    // Output std::stream iostate uncertain flags and types, functions.
    void outIOstates(std::ios_base::iostate, std::ostream& , const char*); 
    void outUncFlags(long, std::ostream&); // long of flags bit set like "add_+/-", "add_noisy",  
    void outUncTypes(unsigned short int, std::ostream&); // Output name of 16 bits for uncertain types like "exact" if EXACT.

  } //   namespace quan
} // namespace boost
#endif // BOOST_UNC_FWD_HPP
