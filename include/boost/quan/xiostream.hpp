/*! \file xiostream.hpp
  \brief Extra iostream manipulators and output of stream state descriptions.
  \details 

  \author Paul A. Bristow
  \date Sep 2009, 2021
*/

// Copyright Paul A. Bristow 2009, 2021
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_QUAN_XIOSTREAM_HPP
#define BOOST_QUAN_XIOSTREAM_HPP

#include <iostream>

namespace boost {
namespace quan {

// Single int parameter manipulators.
// Example: out << chars(12, '_') << stars(8) ) ...
// stars done with simple way, NOT using template,
// See S Teale p 181-3, said to be longer but faster.

//! \brief Manipulator class to help to output a number of stars.
 //! Example:  \code out << stars(10) ... \endcode
class stars
{  
  friend std::ostream& operator<< (std::ostream&, const stars&);
public:
  stars(int n) : num_(n)
  { // Constructor.
  }
private:
  int num_; // How many stars to output.
};

//! \brief Manipulator to output a number of stars.
//! Example:  \code out << stars(10) ... \endcode
std::ostream& operator<< (std::ostream& os, const stars& s)
{
  for (int i = s.num_; i > 0; i--) os << '*';
  return os;
}

//! Manipulator class to help output int repeat count & character chars.
//! Example: \code  out << chars(10, '_') .... for 10 underlines. \endcode
class chars 
{ 
  friend std::ostream& operator<< (std::ostream&, const chars&);
public:
  chars(int n, char c) : num_(n), character_(c)
  { // Constructor.
  }
private:
  int num_;
  char character_;
};

//! Manipulator to help output int repeat count & character chars.
//! Example: \code  out << chars(10, '_') .... for 10 underlines. \endcode
std::ostream& operator<< (std::ostream& os, const chars& s)
{
  for (int i = s.num_; i > 0; i--) os << s.character_;
  return os;
}

//! @brief class setupperbase to output uppercase or capital letter B, X and O when using base other than 10
class setupperbase
{
  friend std::ostream& operator<< (std::ostream&, const setupperbase&);
  friend std::istream& operator>> (std::istream&, const setupperbase&);
public:
  setupperbase(int base) : base_(base) // Constructor.
  {}
private:
  int base_;
}; // class setupperbase

void outIosFmtFlags(long flags, std::ostream& os); // Show all std iostream flags.

void outIOstates(std::ios_base::iostate rdstate = std::cout.rdstate(), std::ostream& os = std::cerr, const char* term = ". ");
// Outputs rdstate as words "good", "eof", "fail" and/or "bad".
void outFmtFlags(std::ios_base::fmtflags fmtFlags = std::cout.flags(), std::ostream& os = std::cerr, const char* term = ". ");
// Outputs ios_base fmtflags as words like "skipws", "scientific", "left"...
// Note that the default values must be in the header file, NOT in the .cpp or .ipp file.

// Ostream manipulators to show state of stream.
std::ostream& showiostate(std::ostream& os);  // Show IO stream state in words for this stream.
// Example: cout << showiostate ...

std::ostream& showformat(std::ostream& os); // Show IO stream format flags in words for this stream.
// Example: cout << showformat ...

std::ios_base& lowercase(std::ios_base& _I)
{ // lowercase is the inverse of std::ios_base::uppercase.
  _I.unsetf(std::ios_base::uppercase); // Default is lowercase.
  return _I;
} // lowercase

//! Function to set base to std::hex & std::showbase & std::uppercase too.
//! Example: \code out << hexbase << ... for 1234ABCD \endcode
//! equivalent to \code std::cout << std::hex << std::showbase << std::uppercase ...  \endcode
std::ios_base& hexbase(std::ios_base& _I)
{
  _I.setf(std::ios_base::hex | std::ios_base::showbase | std::ios_base::uppercase, // 3 bits to set == 1,
    std::ios_base::basefield | std::ios_base::showbase | std::ios_base::uppercase); // mask.
  // Note: std::ios_base::basefield); doesn't set showbase & uppercase!
  return _I;
}

// Manipulator template.
// Copy of template for manipulator (from std iomanip)
// Example:  omanip<int>setw(int);
template<typename T> class omanip  // Manipulator for ostream.
{
  // friend std::ostream& operator<< (std::ostream&, const omanip<T>&);
public:
  omanip(std::ostream& (*f)(std::ostream&, T), T v) : func(f), val(v)
  {
  }
private:
  std::ostream& (*func)(std::ostream&, T);  // Function like setw.
  T val;  // Parameter like width.
}; // class omanip
// Could also provide an istream version for operator >>

// Applicator is a class that stores a pointer to a function that takes
// a reference to an std::ios_base (or derived translator) argument, and
// an argument of the type for which the applicator is parameterized.
// Applicator classes have the function call operator overloaded
// so as to simulate a function call with the argument of the parameter type.
// Use, for example: oapp<int> stars(_stars);  // allows << stars(5) ...
// where function is std::ostream& _stars(std::ostream&, int);

// Global ostream applicator using template oapp instantiated for type int,
// & initialised with the address of function with one int parameter.
// Possible to use oapp<int> stars(_stars);  which allows << stars(5) ...
// but instead stars & chars done a simpler way, see S Teale p 181-3.
template<typename T> class oapp  // Applicator for ostream.
{
public:
  oapp(std::ostream& (*f)(std::ostream&, T)) : func(f)
  {
  };
  omanip<T> operator()(T v)
  {
    return omanip<T>(func, v);
  }
private:
  std::ostream& (*func)(std::ostream&, T);
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
//  Example: cout << "regular" << bold << "now bold" << endl;
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

/*! \brief Output all std::ios iostates.
    \details Usages:
 Default logs @c std::cout iostate to @c std::cerr, for example "IOstate: good", or "IOstate: fail"
  \code
    outIOstates(); // Same as:
    outIOstates(cout.rdState(), cerr, ".\n");
    outIOstates(cin.rdState());
    outIOstates(cerr.rdState(), cout, ", ");
    outIOstates(cout.rdState(), cerr, " iostate.\n ");
  \endcode
  */

void outIOstates(std::ios_base::iostate rdState, std::ostream& os, const char* terminator)
{ 
  std::ios_base::fmtflags const savedflags = os.flags();  // Save to restore.
  // Clear any unused and invalid bits in rdState.
  rdState &= std::ios_base::goodbit | std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::goodbit;
  // enum _Iostate {goodbit = 0x0, eofbit = 0x1,  failbit = 0x2, badbit = 0x4, _Statmask = 0x7};
  // but use std::ios_base::goodbit; for portability.
  // MSVC/Dinkumware defines _Statmask 0x17, _Hardfail also added recently.
  os << "rdState ("<< std::showbase << std::hex << rdState << ") " << std::dec;
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
  os << terminator; // eg "\n" or ", " or " ".
  os.flags(savedflags); // Restore.
}  // outIOstates

const char* fmtFlagWords[16] =
{ // Descriptions of meaning of each bit in @c std::ios::fmtflag.
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

/*! Output string descriptions of @c std::ios::fmtflags.
* \details
  For example, logs to std::cerr \verbatim "FormatFlags: skipws showbase right dec" \endverbatim
  \code void outFmtFlags(fmtflags fmtFlags = cout.flags(), std::ostream& os = cerr, const char* term = ".\n"); \endcode
  */
void outFmtFlags(std::ios_base::fmtflags fmtFlags, std::ostream& os, const char* terminator)
{ 
  const int count = 16;  // because using unsigned short int for flags.
  std::ios_base::fmtflags flags = os.flags(); // Save to restore.
  fmtFlags &= static_cast<std::ios_base::fmtflags>(0x7FFF);  // _Fmtmask // clear un-used bits.
  os << "iosFormatFlags (" << std::showbase << std::hex << fmtFlags << std::dec << ")"; // hex value.
  if (fmtFlags != 0)
  {
    for (int i = 0, j = 1; i < count; ++i)
    {
      if ((fmtFlags & j) != 0)
      {
        os << ' ' << fmtFlagWords[i];
      }
      j <<= 1;
    }
  }
  os << terminator; // eg "\n" or ". "
  os.flags(flags);  // Restore.
}  // outFmtFlags

 //! Show std::IO stream state in descriptive words for this stream.
 //! Example: \code std::cout << showiostate ... \endcode
std::ostream& showiostate(std::ostream& os)
{
  outIOstates(static_cast<std::ios_base::iostate>(os.rdstate()), os, ". ");
  // May be problem with output if state is bad!
  return os;
}

//! Show IO stream format flags descriptions in words for this stream.
//! Example: \code std::cout << showformat  ... \endcode
std::ostream& showformat(std::ostream& os)
{
  outFmtFlags(static_cast<std::ios_base::fmtflags>(os.flags()), os, ". ");
  return os;
}

void setiosDefaults(std::ostream& os)
{ // Return state and flags to same default state as when initialised.
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

template<typename T>
std::ostream& FPclass(std::ostream& os, T value)
{ // Show description of non-finite Floating-point type or value.
  // Example: \code std::cout << FPclass(NaN) ... \endcode
  outFpClass(value, os);
  return os;
} // std::ostream& FPclass(std::ostream& os, T value)

//! Custom outputs for non-finite values NaN, inf ... (rather than Microsoft default \"1\#IND ...\")
//! Example: \code outFpClass(x, std::cerr); \endcode
//! \param value Floating-point value to be output to @c std::ostream.
//! \param os @c std::ostream for output.
//! \tparam T Floating-point not-finite type (usually @c double or @c float) to be described.
template<typename T>
void outFpClass(T value, std::ostream& os = std::cerr)
{ 
  if (std::isfinite(value))
  {
    os << value;
  }
  else if (std::isnan(value))
  {
    os << "NaN";

  }
  else if (std::isinf(value))
  {
    os << "inf";
  }
  else
  {
    os << "?" << value << "?";  // Normal value is unexpected if this function is called.
  }
}  // OutFpclass

}  // namespace quan
} //namespace boost

#endif // BOOST_QUAN_XIOSTREAM_HPP
