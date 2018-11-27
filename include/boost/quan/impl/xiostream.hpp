/*! \file xiostream.hpp
  \brief Extra iostream manipulators
  \details Definitions in xiostream.cpp

  \author Paul A. Bristow
  \date Sep 2009, 2018
*/

// Copyright Paul A. Bristow 2009, 2016, 2018
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/*!
   xiostream provides some additional iostream utilities.

   \details Some functions provide repeating characters (spaces, tabs, stars, bell) etc.
   Others provide output of current iostream status.

   See : MSVC++ "Writing your own manipulators without arguments"
   https://docs.microsoft.com/en-us/cpp/standard-library/writing-your-own-manipulators-without-arguments?view=vs-2017
*/

#ifndef XIOSTREAM_HPP
#define XIOSTREAM_HPP

#include <boost/math/special_functions/fpclassify.hpp>

#include <iostream>
//using std::ios_base;
//using std::ostream;
//using std::istream;
//using std::cout;
//using std::cerr;
//using std::endl;
//using std::hex;
//using std::dec;
//using std::showbase;
//using std::ios_base::fmtflags;
//using std::ios_base::iostate;
#include <iomanip>

#include <cfloat>  // for <float.h> for _isnan, _finite, _fpclass & values.

namespace boost {
  namespace quan
{

// Parameterless ostream manipulators space, newline, bell, etc.
// Usage: out << bell << newline << tab << space << twobackslash ...
// deleted as are now done more simply using ' ', '\n', '\t', '\b' etc:

// Single int parameter manipulators, for example, to output repeated single characters.
// Usage: out << spaces(12) << stars(8) << tabs(2) ...
// spaces & stars done with simple way, NOT using template,
// See S Teale p 181-3, said to be longer but faster.

//! Class for manipulator to output repeated space characters.
/*! Usage:\n  Ensure that function is visible with either
    \code
      using boost::quan::spaces; // or
      using namespace boost::quan;
      std::cout << "|" << spaces(10) << "|" << std::endl;
      std::cout << "|" << stars(10) << "|" << std::endl;
    \endcode
    Outputs: |**********|
*/

class spaces 
{ // Usage: out << spaces(12) ...
  friend std::ostream& operator<< (std::ostream&, const spaces&);
public:
  spaces(int n) : num(n)
  { // Constructor initializes.
  }
private:
  int num;
};

//! Manipulator to output repeated space characters.
std::ostream& operator<< (std::ostream& os, const spaces& s)
{
  for (int i = s.num; i > 0; i--) os << ' ';
  return os;
}

//! Class for manipulator to output repeated tabs.
class tabs  // Definition for this file.
{ // Usage: out << tabs(2) ...
  friend std::ostream& operator<< (std::ostream&, const tabs&);
public:
  tabs(int n) : num(n)
  { // Constructor.
  }
private:
  int num;
};

//! Manipulator to output repeated tabs.
std::ostream& operator<< (std::ostream& os, const tabs& s)
{
  for (int i = s.num;  i > 0; i--) os << '\t';
  return os;
}

//! Class for manipulator to output repeated stars.
class stars 
{   // Usage:  out << stars(10) ...
  friend std::ostream& operator<< (std::ostream&, const stars&);
public:
  stars(int n) : num(n)
  { // Constructor.
  }
  int num;
};

//! Manipulator to output repeated stars.
std::ostream& operator<< (std::ostream& os, const stars& s)
{
  for (int i = s.num; i > 0; i--) os << '*';
  return os;
}

//! Manipulator class with int repeat count & character chars.
//! Usage: << chars(5,'_') ...  for 5 underlines.
class chars 
{ 
  friend std::ostream& operator<< (std::ostream&, const chars&);
public:
  chars(int n, char c) : num(n) , character(c)
  { // Constructor.
  }
private:
  int num;
  char character; 
};

//! Manipulator for repeated count & specified character chars.
//! Usage: << chars(5,'_') ...  for 5 underlines.
//! \param os std::ostream for output.
//! \param chars to output.
std::ostream& operator<< (std::ostream& os, const chars& s)
{
  for (int i = s.num;  i > 0; i--) os << s.character;
  return os;
}

//! Class for Manipulator to set specified base and to show base letter O or X uppercase.
//! Usage: out << setupperbase(16) ...
class setupperbase
{
  friend std::ostream& operator<< (std::ostream&, const setupperbase&);
 // friend std::istream& operator>> (std::istream&, const setupperbase&);
public:
  setupperbase(int b) : base(b)
  { // Constructor.
  }
private:
  int base;
}; // class setupperbase

//! Manipulator to set specified base and to show base letter O or X uppercase.
//! Usage: out << setupperbase(16) ...
//! \param os std::ostream for output.
//! \param setupperbase base required: 8, 10 or 16
std::ostream& operator<< (std::ostream& os, const setupperbase& s)
{
  os.setf(static_cast<std::ios_base::fmtflags>
    (
      std::ios_base::showbase | std::ios_base::uppercase |
      ( 16 == s.base ? 1 :  // std::ios_base::hex :std::ios_base::oct;
        8 == s.base ? std::ios_base::oct : std::ios_base::dec),
      // default dec if not 8 or 16
      std::ios_base::basefield | std::ios_base::showbase | std::ios_base::uppercase )  // mask
  );
  return os;
} // std::ostream& operator<< (std::ostream& os, const setupperbase& s)

  } // namespace quan
}// namespace boost

namespace std
{
  // Naughty, but convenient, to put these extra manipulators into std namespace.
  // Usage:  out << nofixed << noscientific << autofloat << noadjust ...
  using std::ios_base;

  //ios_base& defaultfloat(ios_base& _I) // Now in std already.
  //{ // Neither fixed nor scientific. This is the initialized default.
  //	_I.unsetf(ios_base::floatfield);
  //	// Equivalent to out << nofixed << noscientific
  //	return _I;
  //}

  ios_base& lowercase(std::ios_base& _I)
  { // lowercase is the inverse of std::ios_base::uppercase.
    _I.unsetf(std::ios_base::uppercase); // Default is lowercase.
    return _I;
  } // lowercase

    // Function to set base hex & showbase & uppercase too.
    // Usage: out << hexbase << ... for 1234ABCD
    // equivalent to out << hex << showbase << uppercase ...
  ios_base& hexbase(std::ios_base& _I)
  {
    _I.setf(std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase, // set bits,
      std::ios_base::basefield | std::ios_base::showbase | std::ios_base::uppercase); // mask.
                                                                                      // Care: std::ios_base::basefield); doesn't set showbase & uppercase!
    return _I;
  }

  std::ios_base& noadjust(ios_base& _I)
  { // Neither left, right nor internal. This is the initialized default.
    _I.unsetf(std::ios_base::adjustfield);
    return _I;
  }

  std::ios_base& nofixed(ios_base& _I)
  { // no fixed. This is the initialized default for this flag.
    _I.unsetf(std::ios_base::floatfield);
    return _I;
  }

  std::ios_base& noscientific(ios_base& _I)
  { // No scientific. This is the initialized default for this flag.
    _I.unsetf(std::ios_base::floatfield);
    return _I;
  }

  // Manipulator template.
  // Copy of template for manipulator (from std iomanip)
  // Usage:  omanip<int>setw(int);
  template<typename T> class omanip  // Manipulator for ostream.
  {
    // friend std::ostream& operator<< (std::ostream&, const omanip<T>&);
  public:
    omanip(std::ostream&(*f)(std::ostream&, T), T v) : func(f), val(v)
    {
    }
  private:
    std::ostream&(*func)(std::ostream&, T);  // Function like setw.
    T val;  // Parameter like width.
  }; // class omanip
     // Could also provide an istream version for operator >>

     // Applicator is a class that stores a pointer to a function that takes
     // a reference to an std::ios_base (or derived translator) argument, and
     // an argument of the type for which the applicator is parameterized.
     // Applicator classes have the function call operator overloaded
     // so as to simulate a function call with the argument of the parameter type.
     // Use, for example: oapp<int> spaces(_spaces);  // allows << spaces(5) ...
     // where function is std::ostream& _spaces(std::ostream&, int);

     // Global ostream applicator using template oapp instantiated for type int,
     // & initialised with the address of function with one int parameter.
     // Possible to use oapp<int> spaces(_spaces);  which allows << spaces(5) ...
     // but instead spaces, stars & chars done a simpler way, see S Teale p 181-3.
  template<typename T> class oapp  // Applicator for ostream.
  {
  public:
    oapp(std::ostream&(*f)(std::ostream&, T)) : func(f)
    {
    };
    omanip<T> operator()(T v)
    {
      return omanip<T>(func, v);
    }
  private:
    std::ostream&(*func)(std::ostream&, T);
  }; // class oapp

     // Template Manipulator Inserter <<
  template<typename T> std::ostream& operator<< (std::ostream& os, const omanip<T>& m)
  {
    (*m.func)(os, m.val);
    return os;
  };

  // C++ Std Parameterless manipulators not used.
  // std::ostream& operator<<(std::ostream& (*)(std::ostream&) );
  // See \include\ostream for manipulator template eg endl
  // & instantiations of endl, ends, flush near end of file.
  // only narrow char instantiated here, may need wide version too.

  //  Manipulator with no arguments which inserts into ostream.
  //  See MSVC++ 7 "Writing your own manipulators without arguments".
  //  No class derivation nor macros.
  //  Relies on std::ostream defining overloading of operator<< to accept
  //  function type, for example, std::ostream& bold(std::ostream&); declared in unc.h
  //  inline std::ostream& ostream::operator<<(std::ostream& (__cdecl * _f)(std::ostream&)) { (*_f)(*this); return *this; }
  //  inline std::ostream& ostream::operator<<(std::ios_base& (__cdecl * _f)(std::ios_base& )) { (*_f)(*this); return *this; }
  //  Usage: cout << "regular" << bold << "now bold" << endl;
  // std::ostream& bold(std::ostream& os)  // perhaps inline
  // {
  //  return os << '\033' << '[';  // Use if have ANSI terminal emulation.
  // }

  // Manipulators like dec, oct, dec ... see \include\std::ios_base
  // Parameterless manipulators like dec, hex ...
  // std::ios_base& (*)(std::ios_base&)
  // Pointer to function taking std::ios_base reference argument
  // & returning std::ios_base reference.
  // These apply to both istream and ostream.
  //
  // std::ostream& (*)(std::ostream&)  and istream& (*)(istream&)
  // Pointer to function taking ostream (or istream) reference argument
  // & returning ostream (or istream) reference, but NOT BOTH.
  // Inserters and extractors:
  // std::ostream& operator<<( std::ios_base&(*)(std::ios_base&) )
  // istream& operator<<( std::ios_base&(*)(std::ios_base&) )
  // Eg std::ios_base& dec(std::ios_base& s){s.setf(std::ios_base::dec, std::ios_base::basefield);}

} // namespace std;

namespace boost
{
  namespace quan
  {

// Output functions, mainly used for diagnostics.

 // Show std iostream rdstate as words.
void outIOstates(std::ios_base::iostate, std::ostream& , const char*);
//! Outputs rdstate as words "good", "eof", "fail" and/or "bad".
//! Default logs cout iostate to cerr, for example "IOstate: good", or "IOstate: fail"
//! Usages:\n
//! \code
//!   outIOstates(); // Same as:
//!   outIOstates(cout.rdstate(), cerr, ".\n");
//!   outIOstates(cin.rdstate());
//!   outIOstates(cerr.rdstate(), cout, ", ");
//!   outIOstates(cout.rdstate(), cerr, " iostate.\n ");
//! \endcode

void outIOstates(std::ios_base::iostate rdstate = std::cout.rdstate(), std::ostream& os = std::cerr, const char* term = ". ")
{ 
  std::ios_base::fmtflags savedflags = os.flags();  // Save flags to restore.
  // Clear any unused and invalid bits in rdstate.
  rdstate &= std::ios_base::goodbit | std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::goodbit;
  // enum _Iostate {goodbit = 0x0, eofbit = 0x1,  failbit = 0x2, badbit = 0x4, _Statmask = 0x7};
  // but use std::ios_base::goodbit; for portability.
  // MSVC /Dinkumware defines _Statmask 0x17, _Hardfail also added recently.
  os << "rdState ("<< std::showbase << std::hex << rdstate << ") " << std::dec ;
  if (rdstate == std::ios_base::goodbit)
  {
    os << "good"; // 1st so no leading space.
  }
  else
  { // At least one bit set, so all need a space first.
    if ((rdstate & std::ios_base::eofbit) != 0)
    {
      os << " eof";
    }
    if ((rdstate & std::ios_base::failbit) != 0)
    {
      os << " fail";
    }
    if ((rdstate & std::ios_base::badbit) != 0)
    {
      os << " bad";
    }
  }
  os << term; // eg "\n" or ", " or " ".
  os.flags(savedflags); // Restore.
}  // outIOstates

const char* fmtFlagWords[16] =
{ // Descriptions of meaning of each bit in st::ios_base::fmtflags.
  "skipws", "unitbuf", "uppercase","showbase","showpoint","showpos","left","right",
  "internal","dec","oct","hex","scientific","fixed","boolalpha", "?"
  /*
  enum _Fmtflags
  { // Show action when bit set == 1
  skipws = 0x0001,  //   Skip white space on input.
  unitbuf = 0x0002, // Output each char - unbuffered.
  uppercase = 0x0004,// Display uppercase A through F for hexadecimal values and E for scientific values
  showbase = 0x0008, // Display numeric constants in a format that can be read by the C++ compiler.
  showpoint = 0x0010, // Show decimal point and trailing zeros for floating-point values.
  showpos = 0x0020, // Show plus signs (+) for positive values.
  left = 0x0040, // Left-align values; pad on the right with the fill character.
  right = 0x0080,// Right-align values; pad on the left with the fill character (default alignment).
  internal = 0x0100, // Add fill characters after any leading sign or base indication, but before the value.
  dec = 0x0200, // Format numeric values as base 10 (decimal) (default radix).
  oct = 0x0400, // Format numeric values as base 8 (octal).
  hex = 0x0800, // Format numeric values as base 16 (hexadecimal).
  scientific = 0x1000,// Display floating-point numbers in scientific format.
  fixed = 0x2000, // Display floating-point numbers in fixed format.
  boolalpha = 0x4000, // Show bool as word true or false.
  // 0x8000 un-used. // show as ?
  adjustfield = 0x01c0,
  basefield = 0x0e00,
  floatfield = 0x3000,
  _Fmtmask = 0x7fff,
  _Fmtzero = 0
  };
  */
}; // const char* fmtFlagWords

//! Outputs ios_base fmtflags as words like "skipws", "scientific", "left"...
//! Usage:  outFmtFlags(flags, std::cerr);
//! For example, logs to @c std::cerr "FormatFlags: skipws showbase right dec"
//! \ param fmtFlags @c std::ios flags field.
//! \ param os @c std::ostream for output (default @c std::cerr).
//! \ param term terminator character string (default ". ")
void outFmtFlags(std::ios_base::fmtflags, std::ostream&, const char*);

void outFmtFlags(std::ios_base::fmtflags fmtFlags = std::cout.flags(), std::ostream& os = std::cerr, const char* term = ". ")
{
  const int count = 16;  // because using unsigned short int.
  std::ios_base::fmtflags flags = os.flags(); // save to restore.
  fmtFlags &= static_cast<std::ios_base::fmtflags>(0x7FFF);  // _Fmtmask // clear un-used bits.
  os << "iosFormatFlags (" << std::showbase << std::hex << fmtFlags << std::dec << ")" ; // hex value.
  if (fmtFlags != 0)
  {
    for(int i = 0, j = 1; i < count; ++i)
    {
      if ((fmtFlags & j) != 0)
      {
        os << ' ' << fmtFlagWords[i];
      }
      j <<= 1;
    }
  }
  os << term; // eg "\n" or ". "
  os.flags(flags);  // Restore.
}  // outFmtFlags

//! Show IO stream format flags in words for this ostream.
//! \param os std::osream for output.
std::ostream& showformat(std::ostream& os)
{ // Show IO stream format flags in words for this stream.
  // Usage: cout << showformat ...
  outFmtFlags(static_cast<std::ios_base::fmtflags>(os.flags()), os, ". ");
  return os;
} // showformat

//! Return state and flags to same as when initialised.
//! Would like to use std::ios_base::init, but is protected.
//! \param os @c std::ostream for output.
//! See also setUncDefaults
void setiosDefaults(std::ostream& os)
{ 
  using std::ios_base;
  // basic_streambuf<char, char_traits<char> >* os_buf = os.rdbuf();
  // basic_ios<char, char_traits<char> >::init(os.rdbuf(), true);
  // Cannot access protected member declared in class.
  os.clear(std::ios_base::goodbit); // Clearing any 'bad' bits.
  os.fill(' '); // Default fill char.
  os.precision(6); // Default precision.
  os.width(0); // Would be zeroed by any previous use like << 99 ...
  os.flags(std::ios_base::dec | std::ios_base::skipws); // All others are zero (cleared bits).
} // setiosDefaults(std::ostream& os)

//! Usage: outFpClass(x, std::cerr);
//! Custom outputs for NaN, inf ... (rather than MSVC default 1#IND ...)
//! \param value floating-point type to output string for not normal classes.
//! \param os @c std::ostream to which Nan or inf string should be output.
//!  TODO this shoudl be templated on value and should also check that is a FP type?
//void outFpClass(double value, std::ostream& os);  // Show Floating-point type.

void outFpClass(double value, std::ostream& os = std::cerr)
{ 

  if (boost::math::isfinite(value))
  {
    os << value;
  }
  else if (boost::math::isnan(value))
  {
    os << "NaN";

  }
  else if ( boost::math::isinf(value))
  {
    os << "inf";
  }
  else
  {
    os << "?" << value << "?";
  }
}  // OutFpclass

//! Show Floating point type or value.
  //! Usage: cout << FPclass(NaN) ...
std::ostream& FPclass(std::ostream& os, double value)
{ 
  outFpClass(value, os);
  return os;
} // std::ostream& FPclass(std::ostream& os, double value)

} // namespace quan
}  // namespace boost

#endif // XIOSTREAM_HPP

