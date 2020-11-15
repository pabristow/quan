/*! \file
   \brief Class for measurement using uncertain class.
   \details class meas with uncertain type UReal and measurement order & time-stamp.
*/

// meas.hpp
// Copyright Paul A. Bristow 2012

#ifndef MEAS_HPP
#define MEAS_HPP

#include <boost/date_time/gregorian/gregorian.hpp> // include all types plus i/o.
#include <boost/date_time/posix_time/posix_time.hpp> // include all types plus i/o.

#include <boost/quan/unc.hpp>

// Declaration.
//class Meas;  // Measured uncertain value AND its id and order and/or time-date stamp.

#include <iostream>
  using std::ostream;
  using std::istream;
#include <string>
  using std::string;  // not_a_date_time

typedef unc<false> uncun; // Uncertain Uncorrelated (the normal case).

/*!
 \brief Measured uncertain value AND its id and order and/or time-date stamp.
*/

class Meas : public uncun
{
  friend ostream& operator<< (ostream&, const Meas&);
  friend istream& operator>> (istream&, Meas&);

public:
  //Meas(); // Constructor - all defaults.
  Meas(double const d = 0.);   // Constructor from double (no extra info).
  //Meas::Meas(uncun u);  // Constructor from uncertain uncun.
  //	Meas(int const);   // Constructor from int - use automatic conversion int to double.
  // Meas(uncun u, string id = "", boost::posix_time::ptime ti = boost::posix_time::not_a_date_time);
  Meas(uncun u, string id = "", boost::posix_time::ptime ti = (boost::posix_time::not_a_date_time), int o = -1);
  //Meas(uncun u, string id = "", int o = -1);
  Meas(const Meas&);   // Copy constructor.
  ~Meas();  // Default destructor.

  // Operators.
  Meas& operator= (const Meas& rhs); //!< Assignment operator.
  Meas& abs (const Meas& rhs); // abs assignment operator.
  Meas& abs (Meas& rhs); // abs operator.

  bool operator== (const Meas& p) const; // Equality operator.
  bool operator!= (const Meas& p) const; // Inequality.
  bool operator< (const Meas& rhs) const; // Used by less.
  bool operator> (const Meas& rhs) const; // Used by greater.
  Meas operator+ (const Meas&) const // Unary +
  { // All members remain unchanged.
    return *this;
  }
  Meas operator- (void) const // Unary -
  {  // uncun measValue;
    Meas m(*this); // copy.
    m.value(-m.value()); // Negate only the value.
    return m;
  }
  Meas operator- (void)  // Unary -
  {  // Avoid copy but cannot be const.
    Meas m(*this); // copy.
    m.value(-m.value()); // Negate only the value.
    return *this;
  }

  // Meas Member functions.
  // Note static as only one instance of functions for all Meas objects.
  // Usage: Meas::lessU(a, b);
  static bool less(const Meas& l, const Meas& r); // l < r value (ignoring uncertainty).
  static bool lessU(const Meas& l, const Meas& r); // < uncertain value.
  static bool less2U(const Meas& l, const Meas& r); // < 2 * uncertainty .
  static bool precedes(const Meas& l, const Meas& r); // < order.
  static bool earlier(const Meas& l, const Meas& r);  // < time.
  static bool greaterU(const Meas& l, const Meas& r); // > uncertain value.
  static bool greater2U(const Meas& l, const Meas& r); // > 2 * uncertain value.
  static bool equal_toUnc(const Meas& l, const Meas& r); //
  static bool lessAbsM(const Meas& l, const Meas& r); // abs value < abs value.

 // Meas Member variables.
public: // Inconvenient to make private?
  std::string id_; // Identification info, if any, else "".

  // Time and order values could be:
  // 1 no known order.
  // 2 known order, but no times.
  // 3 times, from which order can be calculated,
  // or both order and times given, so may need to be checked for consistency.

  boost::posix_time::ptime time_; // Posix time from Boost.Date_time.
  int order_;  // Index from 0 (or 1 perhaps?)  -1 == unknown?

}; // class Meas

// Output and input operator<< & operator>>
ostream& operator<< (ostream& os, const Meas& m);
istream& operator>> (istream& is, Meas& m);

// Definitions in meas.ipp.
#include <boost/quan/impl/meas.ipp>

#endif // MEAS_HPP
