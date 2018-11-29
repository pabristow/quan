/*!
  \file
  \brief Uncertain class definitions.
  \details Class for simple Propagation of Uncertainties
     according to a pure Gaussian model.
     Uncertain subroutines, manipulators & applicators.
*/

// Copyright Paul A. Bristow 1998, 2012, 2018.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// File unc_io.hpp

#ifndef UNC_IO_HPP
#define UNC_IO_HPP

// included from 
// #include <boost/quan/unc.hpp> // Declarations of uncertain items.

#include <iostream>
//using std::ostream;

namespace boost {   
namespace quan
{

//! 'Unique' value used to check xalloc initialised iwords are OK, and are not corrupted.
const long indexID = 0x48dbaf8;  //! Random id value.

/*!
  \brief Set 16 default values for uncertain output flags.
  Might be best to make setUncDefaults also set ios_base defaults
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
  os.iword(confidenceIndex) = 50000; // 50000 / 1000000 == 0.05
  // 'Magic' number indexID
  os.iword(topIndex) = indexID;  // last .iword(16) == iword(0)
  // marking that all have been set to defaults.
  if (os.iword(zeroIndex) != indexID)
  { // Initialization of iwords failed!
    std::cerr << "os.iword(0) = "  << std::hex << os.iword(0)
      << " not expected ID " << indexID << '!'
      << "\nMissing call of setUncDefaults(ostream)?" << std::endl;
    // throw; some exception TODO
  }
} // void setUncDefaults(std::ios_base& stream)

/*! Output ALL the os.word() values to the ostream log.
  \param os Current stream uncertain value to be displayed in log.
  \param log stream for log.
*/
void outUncValues(std::ostream& os, std::ostream& log)
{ // Output ALL the os.word() values to the ostream log.
  log //<< hex << "zero " << os.iword(zeroIndex)  << dec // = indexID; // Mark if has been set to defaults.

    << " " "UncValues: "
    <<  "uncFlags " << std::hex << os.iword(uncFlagsIndex) << std::dec //= 0;  // iword(1)
    << ", setSigDigits " << os.iword(setSigDigitsIndex) // = 3;  // Default set 3 sig digits.
    << ", uncWidth " << os.iword(uncWidthIndex) // = 10;  // Default width 10 to allow +1.234E12
    << ", uncSetWidth " << os.iword(oldUncSetWidthIndex) // = 0;  // Not used
    << ", uncScale " << os.iword(scaleIndex) // = 0;  // Zero scaling means was not scaled.
    << ", uncSetScale " << os.iword(setScaleIndex) // = 0;  // Zero scaling means not scaled.
    << ", uncUsed " << os.iword(usedIndex) // = 0; //  None used.
    // Values saved by previous calls of uncprint.
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

    // << ", top " << hex << os.iword(topIndex) << dec
    << std::endl;// = indexID;  // last .iword(16) as check.
} // void outUncValues()

//! Description as a word of each bit in unc type, using enum unc_types.
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
}; // const char* uncTypeWords[16] =


/*! Output word description for each `unc_type` bit.\n
  Usage: \code  outUncTypes(unc.getUncTypes(), std::cerr); // logs to cerr. \endcode\n
  \param uncTypes uncertain types as a short int.
  \param os std::ostream for output, default = std::cerr
*/
void outUncTypes(unsigned short int uncTypes, std::ostream& os = std::cerr)
{
  const int count = 16;  // because using 16-bit unsigned short int.
  os << "uncTypes ("  << std::showbase << std::hex << uncTypes << std::dec << ")" ;
  for(int i = 0, j = 1; i < count; ++i)
  {
    if ((uncTypes & j) != 0)
    {
      os << ' ' << uncTypeWords[i] ;
    }
    j <<= 1;
  } // for
  os << ".";
}  // outUncTypes

/*!
  @c operator<< to send uncertainty types as words like "integer", "exact", "zero" to @c std::ostream.\n
  Usage: @code out << showUncTypes(uncType) \endcode
  \param ut Uncertain type flags.
  \param os @c std::ostream for output of types as words.
*/
std::ostream& operator<< (std::ostream& os, const showUncTypes& ut) 
{
  const int count = 16;  // because using 16-bit unsigned short int.
  unsigned short int uncTypes = ut.types_;
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

// Ostream manipulators to show state of stream.

//! Show IO stream state in words for this stream.
//! Usage: cout << showiostate ...
std::ostream& showiostate(std::ostream& os)
{ // Show IO stream state in words for this stream.
  // Usage: cout << showiostate ...
  outIOstates(static_cast<std::ios_base::iostate>(os.rdstate()), os, ". ");
  // May be problem with output if state is bad!
  return os;
} // std::ostream& showiostate(std::ostream& os)


/*! Show the set @b uncertain @b class io stream flags settings as words.
  \param uncFlags Output flags to be displayed as words.
  \param os Ostream for output.
  Usage:   \code outUncFlags(cout.flags(), cerr); // logs cout's flag to cerr. long& uncFlags = os.iword(uncFlagsIndex); \endcode
*/
void outUncFlags(long uncFlags, std::ostream& os = std::cerr)
{
  os << "uncFlags (" << std::showbase << std::hex << uncFlags << std::dec << ")";
  os << ((uncFlags & firm) ? " firm" : "" );
  if (uncFlags & setScaled) os << " set_scaled";
  if (uncFlags & autoScaled) os << " auto_scaled";
  if (uncFlags & plusMinus) os << " add_+/- ";
  //if (uncFlags & addSISymbol) os << " add_SI_symbol";
  //if (uncFlags & addSIPrefix) os << " add_SI_prefix";
  if (uncFlags & limits) os << " addlimits";
  if (uncFlags & noisyDigit) os << " addnoisy";
  if (uncFlags & useSetSigDigits) os << " set_sigDigits";
  if (uncFlags & useSetUncSigDigits) os << " set_uncsigDigits";
  if (uncFlags & degfree) os << " adddegfree";

  os << '.';
} // void outUncFlags(long uncFlags, std::ostream& os = std::cerr)

// Parameterless manipulators to switch format to switch uncFlag bits,
// flex - width just enough to display, suitable for non-tables,
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

// May need istream operator>> versions too?

////std::ios_base& showuncflags(std::ios_base&);
//// Why is this lowercase  TODO????
//std::ios_base& showuncflags(std::ios_base& iostr)
//{
//  showUncFlags(static_cast<unsigned short>(iostr.iword(uncFlagsIndex)));
//  return iostr;
//}

//std::ios_base& showuncflags(std::ios_base&);
//const std::ios_base& showUncFlags(std::ios_base& iostr)
//{
//  //showUncFlags(static_cast<unsigned short>(iostr.iword(uncFlagsIndex))); must be a lvalue.
//  unsigned short l = iostr.iword(uncFlagsIndex);
//  showUncFlags(l);  / needs an lvalue
//  return iostr;
//}

// Two overloaded uFlags functions, like ios_base setf.
// Two overloaded uFlags functions, like ios_base setf.
//! \param stream @c std::stream having uncertainty flags in `ios::iword`.
//! \returns current uFlags uncertainty flags. 
long uFlags(std::ios_base& stream)
{
  return stream.iword(uncFlagsIndex);  // Return streams current unc flags.
  // Unlike ios_base member function fmtflags ios_base::flags(),
  // these need to know their ios_base.
}

// Two overloaded uFlags functions, like ios_base setf.
//! \param stream @c std::stream having uncertainty flgas in iword.
//! \param flags New uncertainty flags to set.
//! \returns current uFlags uncertainty flags. 

long uFlags(std::ios_base& stream, long flags) // Assign new & return old.
{
  // lock();  may be required for multitasking (MT defined).
  long oldflags = stream.iword(uncFlagsIndex);  // Save to return
  stream.iword(uncFlagsIndex) = flags;  // Assign all bits.
  // unlock();
  return oldflags;  // Previous flags.
} // uFlags

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
// defined in unc.hpp header, but defined in unc_io.ipp.

// Possible confusion about if this is a long or an unsigned short 16-bit?
// Iword is a long, and so showUncFlags is also a long but could be only 16-bit,
// as only using much less than 16 bits so far.
// static_cast to unsigned short is now std::ostream& operator<< below.

//showUncFlags::showUncFlags(unsigned short int f) : flags(f)
//{ //! Constructor.
//  //! Usage: out << showUncFlags(static_cast<long>(is.iword(uncFlagsIndex))) ...
//}


//!Output to @c std::ostream the output flags set, for example: "add_+/-", "add_noisy", "use_set_uncsigfig" ...
std::ostream& operator<< (std::ostream& os, const showUncFlags& uf)
{
  unsigned short uncFlags = static_cast<unsigned short>(uf.flags_); // Set by showUncFlags constructor.
  os << "uncFlags ("<< std::hex << uncFlags << std::dec << ")";
  os << ((uncFlags & firm) ? " firm" : "" );
  if (uncFlags & setScaled) os << " setScaled";
  if (uncFlags & autoScaled) os << " autoScaled";
  if (uncFlags & plusMinus) os << " add_+/-";
  if (uncFlags & addSISymbol) os << " add_SI_symbol";
  if (uncFlags & addSIPrefix) os << " add_SI_prefix";
  if (uncFlags & noisyDigit) os << " add_noisy";
  if (uncFlags & useSetSigDigits) os << " use_set_sigfig";
  if (uncFlags & useSetUncSigDigits) os << " use_set_uncsigfig";
  os << ".";
  return os; // Make chainable.
} // ostream& operator<< (ostream& os, const showUncFlags&)

  //long& uncFlags = os.iword(uncFlagsIndex);
std::ostream& operator<< (std::ostream& os, const setAllUncFlags& sf)
{
  os.iword(oldUncFlagsIndex) = os.iword(uncFlagsIndex);  // Save all old flags.
  os.iword(uncFlagsIndex) = sf.flags_;  // Set all flags.
  return os;
}

std::istream& operator>> (std::istream& is, const setAllUncFlags& sf)
{
  is.iword(oldUncFlagsIndex) = is.iword(uncFlagsIndex);  // Save all old flags.
  is.iword(uncFlagsIndex) = sf.flags_;  // Set all flags.
  return is;
}

std::ostream& operator<< (std::ostream& os, const setUncFlags& sf)
{
  os.iword(oldUncFlagsIndex) = os.iword(uncFlagsIndex);  // Save all oldflags.
  os.iword(uncFlagsIndex) |= sf.flags_;  // Set (OR in) selected flags.
  return os;
}

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


std::ostream& operator<< (std::ostream& os, const setUncSigDigits& usf)
{
  os.iword(oldUncSigDigitsIndex) = os.iword(setUncSigDigitsIndex); // Save previous.
  os.iword(setUncSigDigitsIndex) = usf.uncSigDigits_;  // Set new uncSigDigits.
  return os;
}

std::ostream& operator<< (std::ostream& os, const setRoundingLoss& sl)
{ //! \note Can't store `double` in a `long`, so scale up * 1000 to an integer.
  os.iword(roundingLossIndex) = static_cast<long>(sl.roundingloss_ * 1000);
  return os;
}


std::ostream& operator<< (std::ostream& os, const setConfidence& sl)
{ //! \note Can't store `double` in a `long`, so scale up by 1000000 to an integer.
  os.iword(confidenceIndex) = static_cast<long>(sl.confidence_ * 1000000);
  return os;
}

std::istream& operator>> (std::istream& is, const setUncSigDigits& usf)
{
  is.iword(oldUncSigDigitsIndex) = is.iword(setUncSigDigitsIndex); // Save previous.
  is.iword(setUncSigDigitsIndex) = usf.uncSigDigits_;  // Set new uncSigDigits.
  return is;
}


template <class T>
double unc_of(T v)
{ //! \return zero if no uncertainty information is available (for built-in double, float, or long double).
  //! because will match the explicit specializations below.
  // or will is just fail to compile because it has no uncertainty element and member function std_dev().
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

//template <>
double unc_of(uncun u)
{ //! \return zero if no uncertainty information is available (for built-in double, float, or long double).
  return u.std_dev();
}

// Pairs of values.
//
//template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
//std::pair<double, double> values_of(std::pair<double, double> vp)
//{ //!< \return values of a pair of double values.
//  return std::make_pair(value_of(vp.first), value_of(vp.second));
//}
//
//// Const version
//template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
//std::pair<double, double> values_of(std::pair<double const, double> vp)
//{ //!< \return values of a pair of double values.
//  return std::make_pair(value_of(vp.first), value_of(vp.second));
//}

//template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<uncun, uncun> vp)
{ //!< \return values of a pair of double values.
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

//template <> //! \tparam T Built-in floating-point type, float, double, long double or unc or Meas.
std::pair<double, double> values_of(std::pair<uncun const, uncun> vp)
{ //!< \return values of a pair of double values.
  return std::make_pair(value_of(vp.first), value_of(vp.second));
}

//! Get uncertainties (standard deviation) of a pair of values.
//template <> //! \tparam T Built-in floating-point type or unc.
std::pair<double, double> uncs_of(std::pair<double, double> vp)
{
  return std::make_pair<double, double>(unc_of(vp.first), unc_of(vp.second));
}

//! Get uncertainties (standard deviation) as a pair of double values.
//template <> //! \tparam T Built-in floating-point type or unc.
std::pair<double, double> uncs_of(std::pair<uncun, uncun> vp)
{
  return std::make_pair<double, double>(unc_of(vp.first), unc_of(vp.second));
  //! \return uncs_of uncertainties (standard deviation) as a pair of double values.
}

//! Get uncertainties (standard deviation) as a pair of const float values.
template <class T> //! \tparam T Builtin-floating point type or unc.
const std::pair<float, float> uncs_of(std::pair<T, T> vp)
{ //! \return uncertainty parts (if any) as a pair of floats.
  // so can write
  // std::pair<const float, float> minmax = values_of(*result.first); // min unc & max unc for example/
  // whether T is built-in or unc.
  return std::make_pair(vp.first.unc(), vp.second.unc());
  //! \return uncs_of uncertainties (standard deviation) as a pair of @b float values.
}

//// Diagnostic printout version of constructors here just for testing.
//
//// Note both correlated and uncorrelated versions needed unless use a template.
//// C2906: 'unc<is_correlated>::~unc(void)' : explicit specialization requires 'template <>'
//template<> unc<true>::~unc()  // Definition.
//{
//#ifdef UNC_CD_TRACE
//  {
//    cerr << "\n___ Destruct unc<true> members: "
//      << value_ << ", Unc " << float(uncertainty_)
//      << ", df " << dec << degFree_
//      << showUncTypes(unctypes_)
//      << endl;
//  }
//#endif
//}  // Destructor
//
//template<> unc<false>::~unc()  // Definition.
//{
//#ifdef UNC_CD_TRACE
//  {
//    cerr << "\n___ Destruct unc<false> members: "
//      << value_ << ", Unc " << float(uncertainty_)
//      << ", df " << dec << degFree_
//      << showUncTypes(unctypes_)
//      << endl;
//  }
//#endif
//}  // Destructor

}   // namespace quan
}  // namespace boost 
#endif // UNC_IO_HPP
