#pragma once
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012, 2018.

#include <boost/config.hpp> // macros BOOST_COMPILER, BOOST_VERSION, BOOST_STDLIB etc
#include <boost/version.hpp> // macros BOOST_COMPILER, BOOST_VERSION, BOOST_STDLIB etc

#include <sstream> // std::string

//! Create a @c std::string containing compiler, boost and platform versions info.
std::string version_info()
{
  std::stringstream mess;
  mess << "Program: " __FILE__  << "\n";
  #ifdef __TIMESTAMP__
  mess << __TIMESTAMP__;
  #endif
  mess << "\nBuildInfo:\n" "  Platform " << BOOST_PLATFORM;
  mess << "\n  Compiler " << BOOST_COMPILER;
  #ifdef _MSC_FULL_VER
  mess << "\n  MSVC version "<< BOOST_STRINGIZE(_MSC_FULL_VER) << ".";
  #endif

  #ifdef __GNUC__
  mess << "GCC "<< __GNUC__ << '.' << __GNUC_MINOR__ <<'.' 
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

  mess << "\n  STL " << BOOST_STDLIB;
  mess << "\n  Boost version " << BOOST_VERSION/100000 << "." << BOOST_VERSION/100 % 1000 << "." << BOOST_VERSION % 100 << std::endl;
  return mess.str();
} // std::string versions()
