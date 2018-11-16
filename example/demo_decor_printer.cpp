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
// but this abstract_printer print has order of arguments reversed,
// Steven Watanabe putting ostream first, so always have to specify it.
// This does not permit having std::cout as a default?

// Copy of Boost type_erasure_printers.hpp but with added decor_printer etc.
// I:\modular-boost\libs\quan\include\boost\quan\type_erasure_printers.hpp
// https://www.boost.org/doc/libs/release/doc/html/boost_typeerasure.html
// A polymorphic range formatter
// https://www.boost.org/doc/libs/release/doc/html/boost_typeerasure/examples.html#boost_typeerasure.examples.print_sequence
// print_sequence.cpp

#include <boost/quan/type_erasure_printers.hpp> // for abstract_printer.

int main()
{
  // Some sample data to print:
  double da[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.};
  double db[] = {11., 12., 10., 9., 8., 7., 6., 5., 4., 3., 2., 1};
 
  // decor_printer is an expanded version that allows colum and row counts, prefix, separator, suffix and terminator.

  // Output to std::ostream std::cout as a single line with no prefix, separator, or terminator.
  decor_printer line_printer; // All parameters get default arugments.
  line_printer.print(da, std::cout); // Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" to std::cout
  line_printer.print(da);  // also Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" using std::cout as a default.

  // Output as a block or table of items, separated by commas, 3 per line.
  decor_printer plain_printer(3, 0, "\n", ", ", "\n", "\n");
  plain_printer.print(da, std::cout);

  decor_printer my_comma_printer(0,0,"", ","); // See nearly all the parameters in one constructor call (leaving others to take the defaults).
  my_comma_printer.print(da);  // 1,2,3,4,5,6,7,8,9,10,11,12
  std::cout << std::endl;
  // Output to some other std::ostream, for example, std::cerr or a file.
  my_comma_printer.print(da, std::cerr);  // 1,2,3,4,5,6,7,8,9,10,11,12

  decor_printer default_printer;
  default_printer.width(0); // Set width = 0 (default == no fixed width).
  default_printer.columns(0); // Set columns = 0 (default == do not split into columns).
  default_printer.columns(3).width(4); // Set two parameters using chaining.
  default_printer.columns(8).print(da); // Set one parameter and then print range using chaining.
  default_printer.width(5).print(da); // Set another parameter and then print range using chaining.
  default_printer.layout("{\n|", ", ", "|\n|", "|\n"); // Set all the layout parameters in one function.
  default_printer.print(da); // Printer using previous settings.
  default_printer.layout("{\n|", ", ", "|\n|", " |\n}\n").print(da); // // Set all the layout parameters in one function and then print.
  default_printer.prefix("{\n|").separator(", ").suffix("|\n|").terminator(" |\n}\n").print(db); // Set all layout parameters separately, and then print.
  default_printer.columns(6).width(2).layout("{\n", ", ", "\n", "}\n").print(da); // 

  /*

  */

} // int main()

/*
  Output:


*/


