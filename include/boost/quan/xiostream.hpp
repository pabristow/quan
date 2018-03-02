/*! \file xiostream.hpp
  \brief Extra iostream manipulators
  \details Definitions in xiostream.cpp

  \author Paul A. Bristow
  \date Sep 2009
*/

// Copyright Paul A. Bristow 2009
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef XIOSTREAM_HPP
#define XIOSTREAM_HPP

#include <iostream>
#include <cfloat>  // for <float.h> for _isnan, _finite, _fpclass & values.

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

// Parameterless ostream manipulators space, newline, bell, etc.
// Usage: out << bell << newline << tab << space << twobackslash ...
// Now done more simply using:
const char nl = '\n';
const char tab = '\t';
const char space = ' ';
const char sp = ' ';
const char bell = '\a';

// Single int parameter manipulators.
// Usage: out << spaces(12) << stars(8) << tabs(2) ...
// spaces & stars done with simple way, NOT using template,
// See S Teale p 181-3, said to be longer but faster.

// Declarations.
class spaces;
class tabs;
class stars;
class chars;

class spaces  // Definition for this file.
{ // Usage: out << spaces(12) ...
  friend std::ostream& operator<< (std::ostream&, const spaces&);
public:
  spaces(int); // Constructor defined in xiostream.cpp.
private:
  int num;
};

class tabs  // Definition for this file.
{ // Usage: out << tabs(2) ...
  friend std::ostream& operator<< (std::ostream&, const tabs&);
public:
  tabs(int); // Constructor defined in xiostream.cpp.
private:
  int num;
};

class stars  // Definition for this file.
{   // Usage:  out << stars(10) ...
  friend std::ostream& operator<< (std::ostream&, const stars&);
public:
  stars(int);  // Constructor defined in xiostream.cpp.
  // private:
  int num;
};

// Manipulator class with int repeat count & character chars.
class chars  // Definition for this file.
{ // Usage:  out << chars(10, '_') ....
  friend std::ostream& operator<< (std::ostream&, const chars&);
public:
  chars(int, char);  // Constructor defined in xiostream.cpp.
private:
  int num;
  char character;
};

class setupperbase
{
  friend std::ostream& operator<< (std::ostream&, const setupperbase&);
  friend std::istream& operator>> (std::istream&, const setupperbase&);
public:
  setupperbase(int);  // Constructor defined in xiostream.cpp.
private:
  int base;
}; // class setupperbase

void outIosFlags(long flags, std::ostream& os); // Show std iostream flags.
void outFpClass(double value, std::ostream& os);  // Show Floating-point type.

void outIOstates(std::ios_base::iostate rdstate = std::cout.rdstate(), std::ostream& os = std::cerr, const char* term = ". ");
// Outputs rdstate as words "good", "eof", "fail" and/or "bad".
void outFmtFlags(std::ios_base::fmtflags fmtFlags = std::cout.flags(), std::ostream& os = std::cerr, const char* term = ". ");
// Outputs ios_base fmtflags as words like "skipws", "scientific", "left"...
// Note that the default values must be in the header file, NOT in the .cpp or .ipp file.

// Ostream manipulators to show state of stream.
std::ostream& showiostate(std::ostream& os);  // Show IO stream state in words for this stream.
// Usage: cout << showiostate ...

std::ostream& showformat(std::ostream& os); // Show IO stream format flags in words for this stream.
// Usage: cout << showformat ...

#include <boost/quan/impl/xiostream.ipp> // Definitions.

#endif // XIOSTREAM_HPP
