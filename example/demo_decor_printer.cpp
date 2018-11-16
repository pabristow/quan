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
// This does not permit having std::cout as a default?

// Copy of Boost type_erasure_printers.hpp but with added decor_printer etc.
// I:\modular-boost\libs\quan\include\boost\quan\type_erasure_printers.hpp
// https://www.boost.org/doc/libs/release/doc/html/boost_typeerasure.html
// A polymorphic range formatter
// https://www.boost.org/doc/libs/release/doc/html/boost_typeerasure/examples.html#boost_typeerasure.examples.print_sequence
// print_sequence.cpp

//
#include <boost/quan/type_erasure_printers.hpp>

//#include         <boost/quan/type_erasure_printer.hpp>
// "I:\modular-boost\boost\quan\type_erasure_printer.hpp" copy 13 .sav 16:47 working PAB version OK 17:37 std::cout first and gcc810 too

int main()
{
  // Some sample data to print:
  double da[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12. };

  // Simple separator printer, based on Steven Watanabe's Boost.Type_erasur example.
  //separator_printer<> my_default_printer;
  //my_default_printer.print(da); // 1 2 3 4 5 6 7 8 9 10 11 12
  //std::cout << std::endl;
  //separator_printer<> my_comma_printer(",");
  //my_comma_printer.print(da);  // 1,2,3,4,5,6,7,8,9,10,11,12
  //std::cout << std::endl;
  //my_comma_printer.print(da, std::cerr);  // 1,2,3,4,5,6,7,8,9,10,11,12

  // decor_printer is an expanded version that allows colum and row counts, prefix, separator, suffic and terminator.

  // Output to std::ostream std::cout as a single line with no prefix, separator, or terminator.
  decor_printer line_printer;
  line_printer.print(da, std::cout); // Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" to std::cout
  line_printer.print(da);  // also Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" using std::cout as a default.

  // Output as a block or table of items, separated by commas, 3 per line.
  decor_printer plain_printer(3, 0, "\n", ", ", "\n", "\n");
  plain_printer.print(da, std::cout);

  decor_printer my_comma_printer(0,0,"", ",");
  my_comma_printer.print(da);  // 1,2,3,4,5,6,7,8,9,10,11,12
  std::cout << std::endl;
  // Output to some other std::ostream, for example, std::cerr or a file.
  my_comma_printer.print(da, std::cerr);  // 1,2,3,4,5,6,7,8,9,10,11,12

  decor_printer default_printer;
  default_printer.width(0);
  default_printer.columns(0); // OK
  default_printer.columns(0).width(2);
  default_printer.columns(1).print(da); // OK
  default_printer.width(5).print(da); // OK
  default_printer.layout("{\n|", ", ", "|\n|", "|\n"); //
  default_printer.print(da); //
  default_printer.width(2);
  default_printer.layout("{\n|", ", ", "|\n|", "|\n").print(da); //

   separator_printer space_printer; // using default separator paramenter.
   space_printer.print(da); // 1 2 3 4 5 6 7 8 9 10 11 12
   std::cout << std::endl;
   separator_printer comma_printer(","); // Chosen separator parameter is comma.
   comma_printer.print(da); // 1,2,3,4,5,6,7,8,9,10,11,12
   separator_printer counting_printer(",", 1); // Chosen separator parameter is comma. 1 repeat
   counting_printer.print(da);
   std::cout << std::endl;
   counting_printer.count(2).print(da); // 1,,2,,3,,4,,5,,6,,7,,8,,9,,10,,11,,12
   counting_printer.width(99).print(da); // OK
   counting_printer.count(3).width(99).print(da); //



  /*

  1 2 3 4 5 6 7 8 9 10 11 12


  1 2 3 4 5 6 7 8 9 10 11 12


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


