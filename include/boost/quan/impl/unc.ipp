/*!
  \file
  \brief Uncertain class definitions.
  \details Class for simple Propagation of Uncertainties
     according to a pure Gaussian model.
     Uncertain subroutines, manipulators & applicators.
     but not <iomanip> because defined here (as a copy of code in Std iomanip).  <<<<<<<<<< TODO
*/

// Copyright Paul A. Bristow 1998, 2012, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// unc.ipp

//#ifndef UNC_IPP
//#define UNC_IPP

#include <boost/quan/unc.hpp> // Declarations of uncertain items.

#include <iostream>
//using std::ostream;

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
  os.iword(confidenceIndex) = 50000; // == 0.05 * 1e6

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
  \param os Current stream to be displayed in log.
  \param logstream for log.
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
};

/*! Output word description for each `unc_type` bit.\n
  Usage:  outUncTypes(unc.getUncTypes(), cerr); // logs to cerr.
  \param uncTypes uncertain types as a short int.
  \param os ostream for output, default = cerr
*/
void outUncTypes(unsigned short int uncTypes, std::ostream& os = std::cerr)
{// Usage:  outUncTypes(unc.getUncTypes(), cerr); // logs to cerr.
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
   Usage: `out << showUncTypes(uncType)`
  \param t Uncertain type flags.
*/
showUncTypes::showUncTypes(unsigned short int t) : types(t)
{ // Constructor.
}

/*!
  \nUsage: `out << showUncTypes(uncType)`
  \param ut Uncertain type flags.
  \param std::ostream for output of types as words.
*/
std::ostream& operator<< (std::ostream& os, const showUncTypes& ut)  // Define.
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

/*! Show all the std io stream flags settings as words.\n
  Usage:   \code outIosFlags(cout.flags(), cerr); // logs cout's flag to cerr. \endcode
  \param flags Iostream flags.
  \param os Ostream for output.
  */
void outIosFlags(long flags, std::ostream& os = std::cerr)
{ // Show all the std io stream flags settings as words.
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
    os << " hexFloat";
  // TR1 adds both fixed and scientific as hexfloat.
  // This is 15 - fmtflag appears to be limited by Dinkumware/MS fmtflags = 0xffff and most significant bit 15 is also used internally :-(
  os << ".";
}  // out ios_base::flags(long flags, ostream&)


/*! Show the set uncertain class io stream flags settings as words.
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
} //

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

std::ostream& operator<< (std::ostream& os, const showUncFlags& uf)  // Define.
{ // Output uncFlags to this ostream.
  unsigned short uncFlags = uf.flags;
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

// These all fail to link when placed here???? So left in unc.hpp until understand why.

//! Squared function, notationally convenient for a^2, but overflow possible?
//template <typename Type>
//inline Type sqr(const Type& a)
//{
//  return a * a;
//}

//! Cubed function, notationally convenient for x^3, but overflow possible?
//template <typename Type>
//inline Type cube(const Type& a)
//{
//  return a * a * a;
//}

////! Quaded function, notationally convenient for x^4, but overflow possible?
////! Used by Welch-Satterthwaite formula.
//template <typename Type>
//inline Type pow4(const Type& a)
//{
//  return a * a * a * a;
//}
/*! Hypot or Square root of the sum of the squares of two numbers,
   which is equal to the length of the hypotenuse of a right
   triangle if the two arguments are the lengths of the legs.
   Used for uncertainty propagation.
*/
//template <typename Type>
//inline Type sqrtSumSqrs (const Type& a, const Type& b)
//{
//  return (Type)sqrt(a * a + b * b);
//}
//

// Definitions for explicit instantiation for double uncun, mMeas.
// Definitions must only be in .cpp if unc.hpp included more than once
// or will get link error "already defined".

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

// Const vrsion
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
//#endif // UNC_IPP
