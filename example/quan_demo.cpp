// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012.

#include <boost/version.hpp>
#include <boost/quan/unc.hpp>

#include <iostream>

int main()
{
  std::cout 
    << "BuildInfo:" << '\n'
    << "  platform "  << BOOST_PLATFORM << '\n'
    << "  compiler "  << BOOST_COMPILER << '\n'
    << "  STL "       << BOOST_STDLIB << '\n'
    << "  Boost version " << BOOST_VERSION/100000     << "."
                          << BOOST_VERSION/100 % 1000 << "."
                          << BOOST_VERSION % 100 
    << std::endl;

#ifdef __TIMESTAMP__
  std::cout << __TIMESTAMP__ << std::endl;
#endif
   std::cout << std::endl;
  
  setUncDefaults(std::cout);

  uncun uiz(0);
  std::cout << "uncun u(0) = " << plusminus << uiz << std::endl;

  uncun udz(0.);
  std::cout << "uncun udz(0.); = " << plusminus << udz << std::endl;

  uncun udzu(0., 0.01F);
  std::cout << "udzu(0., 0.01F); = " << plusminus << udzu << std::endl;

  uncun ui1(1); // int
  std::cout << "uncun ui(1) = " << plusminus << ui1 << std::endl; //  1 +/- 0

  uncun ud1(1.); // double
  std::cout << "uncun ud1(1.); = " << plusminus << ud1 << std::endl; //  1 +/- 0

  uncun ud1u(1., 0.01F); // double and uncertainty.
  std::cout << "ud1i(1., 0.01F); = " << plusminus << ud1u << std::endl; //  0 +/- 0.01
  // Might expect decimal point?

  uncun u(1.23, 0.45F, 12);
  std::cout << "u(1.23, 0.45); = " << plusminus << u << std::endl; // 1.2 +/- 0.45
  std::cout << "u(1.23, 0.45); = " << noplusminus << addnoisyDigit << u << std::endl; // 1.23
  std::cout << "u(1.23, 0.45); = " << nonoisyDigit << adddegfree << u << std::endl; // 1.2 (13)

  uncun uw5(127.835, 15.287F);  // Wimmer example 5(i b) e = 0.04
  std::cout << "uw5(127.835, 15.287);= " << plusminus << uw5 << std::endl; // 130. +/- 15 (1)
  std::cout << "uw5(127.835, 15.287);= " << addnoisyDigit << plusminus << uw5 << nonoisyDigit << std::endl; //  128. +/- 15 (1)
  std::cout << "uw5(127.835, 15.287);= " << plusminus << addlimits << uw5 << std::endl; //  130. +/- 15 <98, 158> (1)

  // Wimmer p 1662 example 7 - repeated measurements.
  // number of measurements is 10 and known variance sigma^2 is 0.5
  int n = 10;
  float stddev = static_cast<float>(sqrt(0.5));

  // std::cout << "stddev = " << stddev << std::endl; // Expect 0.223606797
  uncun uw7(87.93911210729208, stddev, 10-1);
  std::cout << ""  << plusminus << addlimits << uw7 << std::endl; //
   //  87.9 +/- 0.22 <87.79, 88.09> (10)
  
} // int main()

/*
   BuildInfo:
    platform Win32
    compiler Microsoft Visual C++ version 10.0
    STL Dinkumware standard library version 520
    Boost version 1.50.0
  Wed Jul 18 16:31:03 2012
  
  uncun u(0) = 0
  uncun udz(0.); = 0
  udzu(0., 0.01F); = 0.000 +/-0.010
  uncun ui(1) = 1
  uncun ud1(1.); = 1. +/-0
  ud1i(1., 0.01F); = 1.000 +/-0.010
  u(1.23, 0.45); = 1.2 +/-0.45
  u(1.23, 0.45); = 1.23
  u(1.23, 0.45); = 1.2 (12)
  uw5(127.835, 15.287);= 130. +/-15. (1)
  uw5(127.835, 15.287);= 128. +/-15. (1)
  uw5(127.835, 15.287);= 130. +/-15. <98.00, 158.0> (1)
  87.9 +/-0.71 <87.48, 88.40> (9)

  quan_demo.vcxproj -> J:\Cpp\quan\MSVC\x64\Debug\quan_demo.exe
  Autorun J:\Cpp\quan\MSVC\x64\Debug\quan_demo.exe
  BuildInfo:
    platform Win32
    compiler Microsoft Visual C++ version 14.1
    STL Dinkumware standard library version 650
    Boost version 1.69.0
  Tue Nov  6 18:10:42 2018
  
  uncun u(0) = 0
  uncun udz(0.); = 0
  udzu(0., 0.01F); = 0.000 +/-0.010
  uncun ui(1) = 1
  uncun ud1(1.); = 1. +/-0
  ud1i(1., 0.01F); = 1.000 +/-0.010
  u(1.23, 0.45); = 1.2 +/-0.45
  u(1.23, 0.45); = 1.23
  u(1.23, 0.45); = 1.2 (12)
  uw5(127.835, 15.287);= 130. +/-15. (1)
  uw5(127.835, 15.287);= 128. +/-15. (1)
  uw5(127.835, 15.287);= 130. +/-15. <98.0000, 158.0000> (1)
  87.9 +/-0.71 <87.4800, 88.4000> (9)

  Has an extra 2 decimal digits!




*/