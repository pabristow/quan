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


/*
Rebuild started...
1>------ Rebuild All started: Project: quan_simple, Configuration: Debug x64 ------
1>quan_simple.cpp
1>quan_simple.vcxproj -> I:\Cpp\quan\quan_skeleton\x64\Debug\quan_simple.exe
1>Autorun "I:\Cpp\quan\quan_skeleton\x64\Debug\quan_simple.exe"
1>
1>Program: I:\boost\libs\quan\example\quan_simple.cpp
1>Wed Feb 10 12:01:46 2021
1>BuildInfo:
1>  Platform Win32
1>  Compiler Microsoft Visual C++ version 14.2
1>  MSVC version 192829812.
1>  STL Dinkumware standard library version 650
1>  Boost version 1.75.0
1>
1>URealCorr u(1.23, 0.45); = 1.2 +/-0.45 <0.35, 2.11> (0)
========== Rebuild All: 1 succeeded, 0 failed, 0 skipped ==========

Projects build report:
  Status    | Project [Config|platform]
 -----------|---------------------------------------------------------------------------------------------------
  Succeeded | ..\quan_simple\quan_simple.vcxproj [Debug|x64]

Build time 00:00:02.809
Build ended at 10Feb2021 12:40:04
*/