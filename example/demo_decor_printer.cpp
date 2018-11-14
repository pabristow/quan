/*! \file
\brief  Demonstration of a 'pretty-printer' using type erasure.
*/
// demo_decor_printer.cpp
// Copyright Paul A. Bristow 2012, 2018

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// For main() examples see
//#include <libs/type_erasure/example/print_sequence.cpp>
// abstract_printer print has order of arguments reversed,
// Steven Watanabe putting ostream first, so always have to specify it.
// This does not permit having std__cout as a default?

// Copy of Boost type_erasure_printers.hpp but with added decor_printer etc.
// I:\modular-boost\libs\quan\include\boost\quan\type_erasure_printers.hpp

// This modified version reverses the order of arguments 
// so that ostream is last so can use a default std::cout.
#include <boost/quan/type_erasure_printers.hpp>

//#include         <boost/quan/type_erasure_printer.hpp>
// "I:\modular-boost\boost\quan\type_erasure_printer.hpp" copy 13 .sav 16:47 working PAB version OK 17:37 std::cout first and gcc810 too

int main()
{
  // Some sample data to print:
  double da[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12. };

  // Simple separator printer, based on Steven Watanabe's Boost.Type_erasurer example.
  //separator_printer<> my_default_printer; 
  //my_default_printer.print(da); // 1 2 3 4 5 6 7 8 9 10 11 12
  //std::cout << std::endl;
  //separator_printer<> my_comma_printer(",");
  //my_comma_printer.print(da);  // 1,2,3,4,5,6,7,8,9,10,11,12
  //std::cout << std::endl;
  //my_comma_printer.print(da, std::cerr);  // 1,2,3,4,5,6,7,8,9,10,11,12

  // decor_printer is an expanded version that allows prefix, separator, suffic and terminator.

  // Output to std::ostream std::cout as a single line with no prefix, separator, or terminator.
  decor_printer line_printer;
  // Now need <> to use the defaults
  //line_printer.print(std::cout, da); // Outputs: "1 2 3 4 5 6 7 8 9 10 11 12"
  line_printer.print(da, std::cout); // Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" to std::cout
  line_printer.print(da);  // also Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" using std::cout as a default.

  // Output as a block or table of items, separated by commas, 3 per line.
//  decor_printer plain_printer(3, 0, "\n", ", ", "\n", "\n");
//  plain_printer.print(std::cout, da);
  /*
    1, 2, 3,
    4, 5, 6,
    7, 8, 9,
    10, 11, 12
  */

} // int main()

/*
  Output:

  Autorun J:\Cpp\quan\MSVC\x64\Release\decor_printer.exe
  1 2 3 4 5 6 7 8 9 10 11 12
  1,2,3,4,5,6,7,8,9,10,11,12
  
  1 2 3 4 5 6 7 8 9 10 11 12
  
  
  1 2 3 4 5 6 7 8 9 10 11 12
  
  1,2,3,4,5,6,7,8,9,10,11,12

*/


