/*! \file
  \brief Extra iostream manipulators.
  \details Definitions of declarations in xiostream.hpp.

  \author Paul A. Bristow

*/
// file /impl/xiostream.ipp

// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/quan/xiostream.hpp> // Declarations of items defined below.
// Now included from this file.

#ifndef XIOSTREAM_IPP
#define XIOSTREAM_IPP

#include <boost/math/special_functions/fpclassify.hpp>

std::ios_base& lowercase(std::ios_base& _I)
{ // lowercase is the inverse of std::ios_base::uppercase.
  _I.unsetf(std::ios_base::uppercase); // Default is lowercase.
  return _I;
} // lowercase

// Function to set base hex & showbase & uppercase too.
// Usage: out << hexbase << ... for 1234ABCD
// equivalent to out << hex << showbase << uppercase ...
std::ios_base& hexbase(std::ios_base& _I)
{
  _I.setf(std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase, // set bits,
    std::ios_base::basefield | std::ios_base::showbase | std::ios_base::uppercase); // mask.
  // Care: std::ios_base::basefield); doesn't set showbase & uppercase!
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

spaces::spaces(int n) : num(n)
{ // Constructor.
}

std::ostream& operator<< (std::ostream& os, const spaces& s)
{
  for (int i = s.num; i > 0; i--) os << ' ';
  return os;
}

tabs::tabs(int n) : num(n)
{ // Constructor.
}

std::ostream& operator<< (std::ostream& os, const tabs& s)
{
  for (int i = s.num;  i > 0; i--) os << '\t';
  return os;
}

stars::stars(int n) : num(n)
{ // Constructor.
}

std::ostream& operator<< (std::ostream& os, const stars& s)
{
  for (int i = s.num; i > 0; i--) os << '*';
  return os;
}

// Two parameter manipulator chars (not using template, as spaces)
// Usage: << chars(5,'_') ...  for 5 underlines.
chars::chars(int n, char c) : num(n) , character(c)
{ // Constructor.
}

std::ostream& operator<< (std::ostream& os, const chars& s)
{
  for (int i = s.num;  i > 0; i--) os << s.character;
  return os;
}

/*
// Manipulator to set specified base and to show base letter O or X uppercase.
// Usage: out << setupperbase(16) ...
setupperbase::setupperbase(int b) : base(b)
{ // Constructor.
}

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
*/
void outIOstates(std::ios_base::iostate rdState, std::ostream& os, const char* term)
{ // Usages:
  // Default logs cout iostate to cerr, for example "IOstate: good", or "IOstate: fail"
  // outIOstates(); // Same as:
  // outIOstates(cout.rdState(), cerr, ".\n");
  // outIOstates(cin.rdState());
  // outIOstates(cerr.rdState(), cout, ", ");
  // outIOstates(cout.rdState(), cerr, " iostate.\n ");
  std::ios_base::fmtflags const savedflags = os.flags();  // Save to restore.
  // Clear any unused and invalid bits in rdState.
  rdState &= std::ios_base::goodbit | std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::goodbit;
  // enum _Iostate {goodbit = 0x0, eofbit = 0x1,  failbit = 0x2, badbit = 0x4, _Statmask = 0x7};
  // but use std::ios_base::goodbit; for portability.
  // MSVC /Dinkumware defines _Statmask 0x17, _Hardfail also added recently.
  os << "rdState ("<< std::showbase << std::hex << rdState << ") " << std::dec ;
  if (rdState == std::ios_base::goodbit)
  {
    os << "good"; // 1st so no leading space.
  }
  else
  { // At least one bit set, so all need a space first.
    if ((rdState & std::ios_base::eofbit) != 0)
    {
      os << " eof";
    }
    if ((rdState & std::ios_base::failbit) != 0)
    {
      os << " fail";
    }
    if ((rdState & std::ios_base::badbit) != 0)
    {
      os << " bad";
    }
  }
  os << term; // eg "\n" or ", " or " ".
  os.flags(savedflags); // Restore.
}  // outIOstates

const char* fmtFlagWords[16] =
{ // Descriptions of each bit.
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

void outFmtFlags(std::ios_base::fmtflags fmtFlags, std::ostream& os, const char* term)
{// Usage:  outFmtFlags(flags, cerr);
  // For example, logs to cerr "FormatFlags: skipws showbase right dec"
  // Defaults in xiostream.hpp:
  // void outFmtFlags(fmtflags fmtFlags = cout.flags(), std::ostream& os = cerr, const char* term = ".\n");
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

std::ostream& showiostate(std::ostream& os)
{ // Show IO stream state in words for this stream.
  // Usage: cout << showiostate ...
  outIOstates(static_cast<std::ios_base::iostate>(os.rdstate()), os, ". ");
  // May be problem with output if state is bad!
  return os;
}

std::ostream& showformat(std::ostream& os)
{ // Show IO stream format flags in words for this stream.
  // Usage: cout << showformat ...
  outFmtFlags(static_cast<std::ios_base::fmtflags>(os.flags()), os, ". ");
  return os;
}

void setiosDefaults(std::ostream& os)
{ // Return state and flags to same as when initialised.
  // Would like to use std::ios_base::init, but is protected.
  // See also setUncDefaults.
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

void outFpClass(double value, std::ostream& os);

std::ostream& FPclass(std::ostream& os, double value)
{ // Show Floating point type or value.
  // Usage: cout << FPclass(NaN) ...
  outFpClass(value, os);
  return os;
} // std::ostream& FPclass(std::ostream& os, double value)

void outFpClass(double value, std::ostream& os = std::cerr)
// Usage: outFpClass(x, std::cerr);
{  // Custom outputs for NaN, inf ... (rather than default 1#IND ...)

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

#endif // #define XIOSTREAM_IPP
