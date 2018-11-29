// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012, 2018.

#include <boost/quan/impl/versions.hpp> // macros BOOST_COMPILER, BOOST_VERSION, BOOST_STDLIB etc

#include <boost/quan/unc.hpp>

#include <iostream>

int main()
{
  std::cout << "quan_simple.cpp" << std::endl;
  std::cout << version_info() << std::endl;

 // using boost::quan::spaces;
  using namespace boost::quan;
  std::cout << "|" << spaces(10) << "|" << std::endl; // |          |
  std::cout << "|" << stars(10) << "|" << std::endl; // |**********|
  std::cout << "|" << chars(10, '!') << "|" << std::endl; // |!!!!!!!!!!|
  std::cout << "|" << tabs(2) << "t" << tabs(2) << "t" << tabs(2) << "|" << std::endl; // |		t		t		|
  

  using boost::quan::setUncDefaults;
  using boost::quan::uncun;
  using boost::quan::uncertainflags;
  using boost::quan::plusminus;

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