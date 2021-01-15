// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012, 2021.

#include <boost/version.hpp>
#include <boost/quan/unc.hpp>
#include <boost/quan/unc_init.hpp>

#include <iostream>
// using std::cout;
// using std::endl;

std::string versions()
{
  std::stringstream message;
  message << "Program: " __FILE__  << "\n";
#ifdef __TIMESTAMP__
  message << __TIMESTAMP__;
#endif
  message << "\nBuildInfo:\n" "  Platform " << BOOST_PLATFORM;
  message << "\n  Compiler " BOOST_COMPILER;
#ifdef _MSC_FULL_VER
  message << "\n  MSVC version "<< BOOST_STRINGIZE(_MSC_FULL_VER) << ".";
#endif
  message << "\n  STL " BOOST_STDLIB;
  message << "\n  Boost version " << BOOST_VERSION/100000 << "." << BOOST_VERSION/100 % 1000 << "." << BOOST_VERSION % 100 << std::endl;
  return message.str();
} // std::string versions()

int main()
{
   std::cout << std::endl;

   std::cout << versions() << std::endl;
  setUncDefaults(std::cout);
  uncun u(1.23, 0.45F);
  std::cout << "URealCorr u(1.23, 0.45); = " << plusminus << addlimits << adddegfree << u << std::endl;


} // int main()

/*
 Output:

  quan_simple.cpp
  quan_simple.vcxproj -> I:\Cpp\quan\quan_skeleton\Debug\quan_simple.exe
  Autorun "I:\Cpp\quan\quan_skeleton\Debug\quan_simple.exe"

  Program: I:\boost\libs\quan\example\quan_simple.cpp
  Thu Jan 14 17:39:36 2021
  BuildInfo:
    Platform Win32
    Compiler Microsoft Visual C++ version 14.2
    MSVC version 192829515.
    STL Dinkumware standard library version 650
    Boost version 1.75.0

  URealCorr u(1.23, 0.45); = 1.2 +/-0.45 <0.3500, 2.1100> (1)
*/
