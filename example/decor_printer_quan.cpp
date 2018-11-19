/*! \file
\brief  Demonstration of a 'pretty-printer' using type erasure.
\details Uses decor_printer to output a variety of data from simple double arrays
to arrays of uncertain values and measurements including metadata like time etc.

*/
// decor_printer_quan.cpp

// Copyright Paul A. Bristow 2012, 2018

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This version reverses the order of arguments 
// so that ostream is last so can use a default std::cout.
#include <boost/quan/type_erasure_printers.hpp>

#include <boost/units/cmath.hpp>
#include <boost/units/io.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/codata/physico-chemical_constants.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/units/quantity.hpp>

#include <boost/quan/unc.hpp> // Two uncertain types.
#include <boost/quan/meas.hpp>

#include <libs/units/example/measurement.hpp>  // An uncertain type called measure. 

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <set>

int main()
{
  // Some sample data to print:
  double da[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12. };

  // Printer using all the defaults
  decor_printer my_default_printer; 
  my_default_printer.print(da); // 1 2 3 4 5 6 7 8 9 10 11 12
  std::cout << std::endl;
  decor_printer my_comma_printer(0, 0, "", ","); // Just separator.
  my_comma_printer.print(da);  // 1,2,3,4,5,6,7,8,9,10,11,12
  std::cout << std::endl;
  my_comma_printer.print(da, std::cerr);  // 1,2,3,4,5,6,7,8,9,10,11,12

  // Output to std::ostream std::cout as a single line with no prefix, separator, or terminator.
  decor_printer line_printer;
  line_printer.print(da);  // also Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" using std::cout as a default.
  line_printer.print(da, std::cerr); // Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" to std::cerr
  // Use range
  // line_printer.print(da[1], da[10]);  // also Outputs: "1 2 3 4 5 6 7 8 9 10 11 12" using std::cout as a default.

// Output as a block or table of items, separated by commas, 3 per line.
//  decor_printer plain_printer(3, 0, "\n", ", ", "\n", "\n");
//  plain_printer.print(std::cout, da);
/*
1, 2, 3,
4, 5, 6,
7, 8, 9,
10, 11, 12
*/
  decor_printer cpp_printer(0, 0, "{", ",  ", "};\n"); // Construct a sequence printer with prefix {, comma separator and } newline terminator.
  cpp_printer.print(da); //


  decor_printer underscore_printer(0 ,0, "\n", "___"); // Construct a printer with three underscores as separator.
  underscore_printer.print(da); // 1___2___3___4___5___6___7___8___9___10
  std::cout << std::endl;

  underscore_printer.print(da); // Type now double.
  std::cout << std::endl; // 1,2,3,4,5,6,7,8,9,10

  std::cout.precision(5); // Add some fancy layout.
  std::cout << std::showpos << std::showpoint << std::endl;

  cpp_printer.print(da); // Type still double.
  std::cout << std::endl; // +1.0000,+2.0000,+3.0000,+4.0000,+5.0000,+6.0000,+7.0000,+8.0000,+9.0000,+10.000
  std::cout << std::noshowpos << std::endl; // Switch off fancy +.



  //                                          // Try an array of Boost.Units quantity.
  //quantity<length> testq[] = {2345.6 * meters, 123.4 * meters, 0.0123 * meters};
  //p1.print(std::cout, testq); // Type now quantity of length.
  //std::cout << std::endl; // 2.3456 km,  123.40 m,  12.300 mm
  //                        // Note the cool change of unit prefix from km to m to mm!

  //                        // Try an array of Boost.Units quantity with an uncertain type.
  //quantity<length, measurement<double> > testqu[] = {measurement<double>(45210.0, 1234.0) * meters, measurement<double>(789, 2.5) * meters, measurement<double>(0.000567,2.34e-5) * meters};
  //p1.print(std::cout, testqu); // Type now quantity of length with uncertainty estimate.
  //std::cout << std::endl; // 45.210(+/-1.2340) km,  789.00(+/-2.5000) m,  567.00(+/-23.400) um
  //                        // Note the cool change of unit prefix *and* showing uncertainty!

  //const char* testCstring[] = {"one", "two", "three", "four","five", "six","seven","eight", "nine", "ten"};
  //p1.print(std::cout, testCstring); // Type is now a C string.
  //std::cout << std::endl; 

  //std::string teststring[] = {"ONE", "TWO", "THREE", "FOUR","FIVE", "SIX","SEVEN","EIGHT", "NINE", "TEN"};
  //p1.print(std::cout, teststring); // Type is now a std::string.
  //std::cout << std::endl; 

  //// Now some other sequence printers.

  //column_separator_printer p2(",", 4);  // Construct another 4 column separator printer.
  //p2.print(std::cout, test); // 1,2,3,4, and then  5,6,7,8, and then 9,10
  //std::cout << std::endl;

  //aligned_column_printer p3(16, 4); // Print down columns 16 spaces across.
  //p3.print(std::cout, test); // 1 4 7 10 then 2 5 8, then 3 6 9
  //std::cout << std::endl;

  //// Check it works with a STL container like vector (copying values from array testd).
  //std::vector<double> testv (testd, testd + sizeof(testd)/ sizeof(testd[0]));
  //p1.print(std::cout, testv); //   1.0000,  2.0000,  3.0000,  4.0000,  5.0000,  6.0000,  7.0000,  8.0000,  9.0000,  10.000
  //std::cout << std::endl; // 

  //std::list<double> testl (testd, testd + sizeof(testd)/ sizeof(testd[0]));
  //p2.print(std::cout, testl);  // 1.0000,2.0000,3.0000,4.0000,
  //                             // 5.0000,6.0000,7.0000,8.0000,
  //                             // 7.0000
  //std::cout << std::endl; // 

  //std::set<double> my_set;
  //my_set.insert(3.);
  //my_set.insert(4.);
  //my_set.insert(5.);
  //my_set.insert(6.);
  //my_set.insert(7.);

  //p2.print(std::cout, my_set); //    3.0000,4.0000,5.0000,6.0000,
  //std::cout << std::endl;     //    7.0000

  //std::pair<const int, double> my_pair = std::make_pair(0, -2.); // 0, -2.00000
  //std::cout << my_pair << std::endl; // 0, -2.00000

  //std::map<int, double> my_map;
  //my_map.insert(std::make_pair(1, 9.9));
  //my_map.insert(std::make_pair(2, 8.8));
  //my_map.insert(std::make_pair(3, 7.7));
  //my_map.insert(std::make_pair(4, 6.6));

  //std::map<int, double>::iterator it;
  //it = my_map.begin();
  //std::cout << it->first << ", " << it->second << std::endl; // 1, 9.9000

  //std::cout << *(my_map.begin()) << std::endl; // First: 1, 9.9000
  //std::cout << *(--my_map.end()) << std::endl;  // Last: 4, 6.6000

  //for (std::map<int, double>::iterator it = my_map.begin(); it != my_map.end(); ++it)
  //{
  //  std::cout << *it << std::endl;
  //}
  //std::cout << std::endl;
  //// 1, 9.9000
  //// 2, 8.8000
  //// 3, 7.7000
  //// 4, 6.6000

  //p2.print(std::cout, my_map); //   
  //std::cout << std::endl; // 

  //                        // Construct a couple of uncertain types.
  //uncun u1(1.23, 0.056f, 42);
  //std::cout << plusminus << adddegfree << u1 << std::endl;
  //uncun u2(4.56, 0.021f, 99);
  //// And push back to a vector.
  //std::vector<uncun> uv;
  //uv.push_back(u1);
  //uv.push_back(u2);
  //// And print out.
  //p2.print(std::cout, uv); //   1.23 +/-0.056 (42),4.56 +/-0.021 (99)
  //std::cout << std::endl;

  //using boost::posix_time::ptime;
  //using boost::gregorian::days;
  //using boost::posix_time::seconds;
  //using boost::posix_time::second_clock;

  //ptime now1 =   ptime(second_clock::local_time());
  //ptime now2 =   ptime(second_clock::local_time() - seconds(99) - days(2));

  //Meas m1(u1, "First", now1, 1);
  //Meas m2(u2, "Second", now2, 1);

  //// Meas(uncun u, string id = "", boost::posix_time::ptime ti = (boost::posix_time::not_a_date_time), int o = -1);

  //std::cout << m1 << "\n" << m2 << std::endl;

  //std::vector<Meas> mv;
  //mv.push_back(m1);
  //mv.push_back(m2);

  //p2.print(std::cout, mv); //   1.23 +/-0.056 (42) First #1, 2012-Aug-06 12:36:57,4.56 +/-0.021 (99) Second #1, 2012-Aug-04 12:35:18
  //std::cout << std::endl;




} // int main()

/*
  Output:

*/