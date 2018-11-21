// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012, 2018.

#include <boost/version.hpp> // macros BOOST_COMPILER, BOOST_VERSION, BOOST_STDLIB etc
#include <boost/quan/unc.hpp>
#include <boost/quan/unc_init.hpp>

#include <iostream>

//! Create a std::string containing boost and platform versions.
std::string versions()
{
  std::stringstream mess;
  mess << "Program: " __FILE__  << "\n";
  #ifdef __TIMESTAMP__
  mess << __TIMESTAMP__;
  #endif
  mess << "\nBuildInfo:\n" "  Platform " << BOOST_PLATFORM;
  mess << "\n  Compiler " BOOST_COMPILER;
  #ifdef _MSC_FULL_VER
    mess << "\n  MSVC version "<< BOOST_STRINGIZE(_MSC_FULL_VER) << ".";
  #endif

  #ifdef __GNUC__
  // mess << __GNUC__ << '.' << __GNUC_MINOR__ <<'.' 
    mess << "GCC 
    #ifdef __cplusplus
      mess << " C++ ";
    #else
      mess << " C ";
    #endif
    version " << __VERSION__ ;
  #endif

  #ifdef __clang__
    mess << "clang " << __clang_major__ << '.' << __clang_minor__ << '.' __GNUC_PATCHLEVEL__;
  #endif

  mess << "\n  STL " BOOST_STDLIB;
  mess << "\n  Boost version " << BOOST_VERSION/100000 << "." << BOOST_VERSION/100 % 1000 << "." << BOOST_VERSION % 100 << std::endl;
  return mess.str();
} // std::string versions()

int main()
{
 std::cout << std::endl;

  std::cout << versions() << std::endl;
  setUncDefaults(std::cout);
  uncun u(1.23, 0.45F);
  std::cout << "URealCorr u(1.23, 0.45); = " << u << plusminus << std::endl;
} // int main()

/*
  Description: Autorun J:\Cpp\quan\MSVC\Debug\quan_simple.exe
  
  Program: ..\..\libs\quan\example\quan_simple\quan_simple.cpp
  Thu Jul 19 16:06:56 2012
  BuildInfo:
    Platform Win32
    Compiler Microsoft Visual C++ version 10.0
    MSVC version 160040219.
    STL Dinkumware standard library version 520
    Boost version 1.50.0
  
  URealCorr u(1.23, 0.45); = 1.2


*/