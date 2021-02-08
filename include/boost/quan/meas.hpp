/*! \file
   \brief Class for measurement using uncertain class.
   \details class meas with uncertain type UReal and measurement order & time-stamp.
*/

// meas.hpp
// Copyright Paul A. Bristow 2012, 2021

#ifndef MEAS_HPP
#define MEAS_HPP

#include <boost/date_time/gregorian/gregorian.hpp> // include all types plus i/o.
#include <boost/date_time/posix_time/posix_time.hpp> // include all types plus i/o.
// using ptime and 'not_a_date_time'.

#include <boost/quan/unc.hpp> // uncertain types, mainly
typedef unc<false> uncun; // Uncertain Uncorrelated (the normal case).

// Forward Declaration.
//class Meas;  // Measured uncertain value AND its id and order and/or time-date stamp.

#include <iostream>
  //using std::ostream;
  //using std::istream;
#include <string>
 // using std::string;  
 

/*!
 \brief Measured uncertain value AND its id and order and/or time-date stamp.
*/

class Meas : public uncun
{
  friend std::ostream& operator<< (std::ostream&, const Meas&);
  friend std::istream& operator>> (std::istream&, Meas&);

public:
 // Meas(); // Constructor - all defaults, compiler provided.
  Meas(double const d = 0.);   // Constructor from (assumed exact) @ double (no extra info).
  //Meas::Meas(uncun u);  // Constructor from uncertain uncun with default id, time and order#.
  //  Meas(int const);   // Constructor from int - use automatic conversion int to double.
  // Meas(uncun u, string id = "", boost::posix_time::ptime ti = boost::posix_time::not_a_date_time);
  Meas(uncun u, // Uncertain value including uncertainty (standard deviation and degress of freedom etc).
    std::string id = "",  // identification (default null string).
    boost::posix_time::ptime ti = (boost::posix_time::not_a_date_time), // Time of measurement (default @cboost::posix_time::not_a_date_time).
    int o = -1); // Order of this measurement in a sequence.
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

  // Get and set member functions.
  //! \returns a identication @c std::string describing the measurement. 
  std::string& id()
  {
    return id_;
  }

  //! Set a identication string describing the measurement. 
  //! \param ident as a @ std::string
  void id(std::string& ident)
  {
    id_ = ident;
  }
  //! \returns a time_point as ptime when the measurement was made. 
  boost::posix_time::ptime& time()
  {
    return time_;
  }

  //! Set a time_point as ptime when the measurement was made. 
  void time(boost::posix_time::ptime& tim)
  {
    time_ = tim;
  }
  //! \return Order of this measurement in a sequence of measurements, often at fixed intervals.
  int order()
  {
    return order_;
  }
  //! Set the order of this measurement in a sequence of measurements, often at fixed intervals.
  void order(int order_no)
  {
    order_ = order_no;
  }

  // Meas Member comparison functions.
  // Note static as only one instance of functions for all Meas objects.
  // Usage: Meas::lessU(a, b);
  static bool less(const Meas& l, const Meas& r); // l < r value (ignoring uncertainty).
  static bool lessU(const Meas& l, const Meas& r); // < uncertain value.
  static bool less2U(const Meas& l, const Meas& r); // < 2 * uncertainty.
  static bool precedes(const Meas& l, const Meas& r); // < order.
  static bool earlier(const Meas& l, const Meas& r);  // < time.
  static bool greaterU(const Meas& l, const Meas& r); // > uncertain value.
  static bool greater2U(const Meas& l, const Meas& r); // > 2 * uncertain value.
  static bool equal_toUnc(const Meas& l, const Meas& r); //
  static bool lessAbsM(const Meas& l, const Meas& r); // abs value < abs value.

 // Meas Member variables.
public:
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
std::ostream& operator<< (std::ostream& os, const Meas& m);
std::istream& operator>> (std::istream& is, Meas& m);

std::ostream& operator<< (std::ostream& os, const std::pair<Meas, Meas>& mp)
{ /*! Output a pair (X and Y) of uncertain measurement values with (if defined) uncertainty and degrees of freedom etc
     \details For example: "1.23 +/- 0.01 (13), 3.45 +/- 0.06 (78)".
   */
  os << mp.first << ", " << mp.second;
  return os;
} // std::ostream& operator<< (std::ostream& os, const std::pair< Meas<correlated>, Meas<correlated> >& mp)

// Might add other mixed types here too?

// Definitions in meas.ipp.
#include <boost/quan/impl/meas.ipp>

#endif // MEAS_HPP
