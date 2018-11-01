/*! \file
   \brief Class for measurement using uncertain class - definitions.
   \details class meas with UReal and measurement ID, order & time.
*/

// meas.ipp
// Copyright Paul A. Bristow 2003, 2012

#ifndef MEAS_IPP
#define MEAS_IPP

#ifdef _MSC_VER
#  pragma warning (push)
#pragma warning (disable : 4189) // local variable is initialized but not referenced TODO!
#  pragma warning (disable : 4146) // unary minus operator applied to unsigned type, result still unsigned
#  pragma warning (disable : 4305) //C4305: 'initializing' : truncation from 'unsigned int' to 'short'.
#  pragma warning (disable : 4309) // C4309: 'initializing' : truncation of constant value.
#endif

#include <boost/date_time/gregorian/gregorian.hpp> //include all types plus i/o
#include <boost/date_time/posix_time/posix_time.hpp> //include all types plus i/o

//using namespace boost::gregorian;
//using namespace boost::posix_time;

#include <iostream>
#include <cmath> // for fabs
#include <functional> // for binary_function min

#include <boost/quan/unc.hpp> // Declarations of class unc.
#include <boost/quan/meas.hpp>  // Declarations of class Meas, which calls this file.

class Meas;  // Measured uncertain value AND its id and order and/or time-date stamp.

//using std::cout;
//using std::cerr;
//using std::endl;
//using std::istream;
//using std::ostream;
//using std::min;
//using std::binary_function; // <functional>

// Global control of format output by class Meas.

//const char timeFormatString[] =	"%H:%M:%S"; // 12:20:55 time HH:MM:SS == "%X"
  // "%H:%M"; // time as 12:26 HH:MM only, no seconds.
  // "%X"; // 12:20:55 time HH:MM:SS
  // "%d %b %y %H:%M:%S"; // 20 Apr 01 12:16:17
  // "%d %b %y %H:%M"; // 20 Apr 01 12:16 // no seconds.
  // "%a %b %d %m %Y %H:%M:%S %p"; // Fri Apr 20 04 2001 12:18:48 PM
  // "%x"; // 04/20/01  US style MM/DD/YY.
  // "%d %b %y"; //  date as 20 Apr 01
  // "%H:%M"; // time as 12:26 HH:MM only, no seconds.

typedef double real_type;  // or
// typedef float real_type;

// class Meas definitions
// Measured value, id and its order and/or time-date.

std::ostream& operator<< (std::ostream& os, const unc<false>& ud);
// Need to instantiate for UReal<false> (might need for UReal<true> too).

/*
Meas::Meas()
{ // Default constructor Usage: Meas z; (not Meas z()!) is exact zero.
  value_ = 0.;
  uncertainty_ = 0.F;
  degFree_ = 0;
  unctypes_ =  UNC_KNOWN | UNC_EXPLICIT| DEG_FREE_EXACT | DEG_FREE_KNOWN;
  time_ = ptime(not_a_date_time);  // Time not known, and cannot be calculated from order.
  order_ = -1;  // Order not known, and cannot be calculated from time.
  id_ = "";
} // Meas::Meas()
*/

Meas::Meas(double d)
{ // Constructor from double - needed by Meas(0.) lessAbs.
  value_ = d;
  uncertainty_ = 0.F;
  degFree_ = 0;
  unctypes_ =  UNC_KNOWN | UNC_EXPLICIT| DEG_FREE_EXACT | DEG_FREE_KNOWN;
  time_ = boost::gregorian::not_a_date_time;  // Time not known, and cannot be calculated from order.
  order_ = -1;  // Order not known, and cannot be calculated from time.
  id_ = "";
#ifdef CD_TRACE
    cerr << "Construct Meas from just double value " << value_ << "." << endl;
#endif
}
/*
Meas::Meas(uncun u)
{ // Constructor from uncertain.
  value_ = u.value_;
  uncertainty_ = u.uncertainty_;
  degFree_ = u.degFree_;
  unctypes_ =  u.unctypes_;
  time_ = not_a_date_time;  // Time not known, and cannot be calculated from order.
  order_ = -1;  // Order not known, and cannot be calculated from time.
  id_ = "";
#ifdef CD_TRACE
  {
    cerr << "Construct Meas from uncertain uncun " << value_ << "." << endl;
  }
#endif
}

Meas::Meas(uncun u, string id, ptime ti)
:   id_(id), time_(ti)
{ // Constructor value, id & time (but NOT order) with default values.
  order_ = -1; // order not known.
  // (but may be calculated from time if in a sequence).
  value_ = u.value_;
  degFree_ =u.degFree_;
  unctypes_ =u.unctypes_;
  uncertainty_ = u.uncertainty_;
#ifdef CD_TRACE
  {
   cerr << "Construct Meas using known time " << time_ << "." << endl;
  }
#endif
}
*/

Meas::Meas(uncun u, string id, boost::posix_time::ptime ti /* = (boost::gregorian_time::not_a_date_time)*/, int order)
:   id_(id), time_(ti), order_(order)
{ // Constructor value, id & order (but NOT time) with default values.
  time_ = ti; // time not known, & cannot be calculated from order.
  value_ = u.value_;
  degFree_ =u.degFree_;
  unctypes_ =u.unctypes_;
  uncertainty_ = u.uncertainty_;
#ifdef CD_TRACE
  cerr << "Construct Meas using time AND known order " << time_ << ", " << order_ << "." << endl;
#endif
}

/*
Meas::Meas(uncun u, string id, int o)
:   id_(id), order_(o)
{ // Constructor value, id & order (but NOT time) with default values.
  time_ = boost::posix_time::not_a_date_time; // time not known, & cannot be calculated from order.
  value_ = u.value_;
  degFree_ =u.degFree_;
  unctypes_ =u.unctypes_;
  uncertainty_ = u.uncertainty_;
  if(cdtrace)
  {
    cerr << "Construct Meas using known order " << time_ << "." << endl;
  }
}
*/

Meas::Meas(const Meas& rhs)
{ // Copy constructor.
  value_ = rhs.value_;
  uncertainty_ = rhs.uncertainty_;
  degFree_ = rhs.degFree_;
  unctypes_ = rhs.unctypes_;
  id_ = rhs.id_;
  order_ = rhs.order_;
  time_ = rhs.time_;
  id_ = rhs.id_;
}

Meas::~Meas()
{  // Default destructor used.
}

// Operators.

// Meas& operator= (Meas&);  // Assignment operator.
Meas& Meas::operator= (const Meas& rhs)  // Assignment operator.
{ // Deep copy.
  if (this == &rhs) return *this;  // Assign to self, so do nothing.
  value_ = rhs.value_;
  uncertainty_ = rhs.uncertainty_;
  degFree_ = rhs.degFree_;
  unctypes_ = rhs.unctypes_;
  order_ = rhs.order_;
  time_ = rhs.time_;
  id_ = rhs.id_;
  return *this;
}

// Meas& abs (Meas&);  // abs Assignment operator.
Meas& Meas::abs(const Meas& rhs)  // abs Assignment operator.
{
  value_ = fabs(rhs.value_);
  uncertainty_ = rhs.uncertainty_;
  degFree_ = rhs.degFree_;
  unctypes_ = rhs.unctypes_;
  order_ = rhs.order_;
  time_ = rhs.time_;
  id_ = rhs.id_;
  return *this;
}

// Meas& abs (Meas&);  // abs operator.
Meas& Meas::abs(Meas& rhs)  // abs Assignment operator.
{
//  double d = rhs.value_;
  //rhs.value_ = abs(d); // Not OK for reasons unclear  - trying to use std::abs?
  rhs.value_ = fabs(rhs.value_); // OK
 return rhs;
}

bool Meas::operator== (const Meas& p) const
{ // Exact Meas value Equality Comparison.
  return (p.value_ == value_);
  // What about checking if time the same too?
} // Meas::operator==

bool Meas::operator!= (const Meas& p) const
{ // Comparison.
  return (p.value_ != value_);  // meas value differ (order  && (p.order_ != order_).
}  // Meas::operator!=

bool Meas::operator< (const Meas& rhs) const
{// less uses operator< the most probable central value.
  // member function so  *this.value_ < *this.rhs.value_;
  return (value_ < rhs.value_);  // Comparison criterion.
    // Only uses the measured value, not order.
} //  Meas::operator<

bool Meas::operator> (const Meas& rhs) const
{// greater uses operator> using the most probable value.
  // member function so  *this.value_ < rhs.value_;
  return (value_ > rhs.value_);
    // Comparison only uses the measured value (not order, nor time).
} // Meas::operator>

/*
// What is meaning of adding measurements?
// Has no meaning - can only add the values and uncertainty to get a new uncertain.
// And time and order number can't be added, so
// doubtful if operator+ and operator- are wise for Meas.
bool Meas::operator- (const Meas& rhs) const
{// Binary minus value using the most probable values.
  // member function so  *this.value_ < rhs.value_;
  return value_ - rhs.value_; <<<  wrong here <<<<<<<<<<<<
  // Only uses the measured value (not order, nor time).
} // Meas::operator>


*/

// Member predicate functions.

// less uses template binary_function and operator<
    /*
    // TEMPLATE STRUCT less in functional
template<class _Ty>
  struct less
    : public binary_function<_Ty, _Ty, bool>
  {	// functor for operator<
  bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {	// apply operator< to operands
    return (_Left < _Right);
    }
  };
  // So this function is the same as less<Meas> and so not needed.
  bool Meas::lessM(const Meas& l, const Meas& r) // == less
  {	// Only uses the central measured value (not order, nor time).
    return l.value_ < r.value_; // Comparison criterion.
  }
  // similarly nor is greater needed.
  */

bool Meas::lessAbsM(const Meas& l, const Meas& r)
{	// Only uses the absolute central measured value, not order.
  return ((l.value_ < 0.) ? -l.value_ : l.value_ ) < ((r.value_ < 0.) ? -r.value_ : r.value_);
  //	return std::abs(l.value_) < std::abs(r.value_);  // Comparison criterion.
  // but avoid abs because less efficient.
} // bool Meas::lessAbsM(const Meas& l, const Meas& r)

bool Meas::precedes(const Meas& l, const Meas& r)
{ // Comparison criterion is order of measurement.
  return l.order_ < r.order_;
}

/*  Do we need this?
bool Meas::succeeds(const Meas& l, const Meas& r)
{ // Comparison criterion is order of measurement.
  return l.order_ > r.order_;
}
*/

// Call this "before"?
bool Meas::earlier(const Meas& l, const Meas& r)
{ // Comparison criterion is time of measurement.
  // (Opposite, like greater, might be called 'later' - if needed).
  if (l.time_ == boost::posix_time::not_a_date_time) return true; // not_a_date_time < any other time.
  if (r.time_ == boost::posix_time::not_a_date_time) return false; // l.time_ IS a time, so r.time_ NOT <
  return l.time_ < r.time_;
} // bool Meas::earlier(const Meas& l, const Meas& r);

bool Meas::less(const Meas& l, const Meas& r)
{ // less using only value comparison.
  // Note if lhi == rlo same then difference is 1.49012e-9 = numeric_limits<float>::epsilon
  // (See also Meas::lessU and Meas::lessU2 which handle one and two standard deviations).

  return l.value_ < r.value_;
} // bool Meas::lessU(const Meas& l, const Meas& r)

bool Meas::lessU(const Meas& l, const Meas& r)
{ // less using comparison criterion including ONE standard deviation.
  // Note if lhi == rlo then difference is 1.49012e-9 = numeric_limits<float>::epsilon
  // (See also Meas::lessU2 which compares using TWO standard deviations).
  double lhi = (l.value_ + l.uncertainty_);  // Upper confidence limit of left.
  double rlo = (r.value_ - r.uncertainty_);	// Lower confidence limit of right.
  //	double diff = (l.value_ + l.uncertainty_) - (r.value_ - r.uncertainty_);
  // diff = lhi - rlo;
  //	double tol = hypot(l.uncertainty_, r.uncertainty_); // Middle of two uncertainties.
  bool isLess =  lhi < (rlo + hypot(l.uncertainty_, r.uncertainty_)); // == less
  if (true)
  { // Output diagnostic info.
    std::cerr << "<U #" << l.order_ << ' ' << l.value_ << "="
    << lhi // (l.value_ + l.uncertainty_)
    << ' ' << ((isLess) ? " < " : " !< ")
    // << (r.value_ - r.uncertainty_) == rlo
    << ' ' << rlo << ' ' << rlo + hypot(l.uncertainty_, r.uncertainty_) // tol
    << " #" << r.order_ << "=" << r.value_
  //	<< ' ' << tol
  //	<< ' ' << diff
  // For example: <U #1 1.1 1.2  !<  1.05 1.19142 #2 1.15
    << std::endl;
  }
  return isLess;
} // bool Meas::lessU(const Meas& l, const Meas& r)

bool Meas::less2U(const Meas& l, const Meas& r)
{ // less Comparison criterion including TWO standard deviations.
  double lhi = (l.value_ + l.uncertainty_);  // Upper confidence limit of left.
  double rlo = (r.value_ - r.uncertainty_);	// Upper confidence limit of right.
//  double diff = (l.value_ + l.uncertainty_) - (r.value_ - r.uncertainty_);
  double tol = hypot(l.uncertainty_, r.uncertainty_);
  // Might chose smallest, or largest uncertainty, or this compromise.
  // Reverses the order when small uncertainty compared to large uncertainty and large chosen for tol.
  bool isLess =  lhi < (rlo + 0.0 * hypot(l.uncertainty_, r.uncertainty_)); // == less
  if (true)
  {
    std::cerr << "<2U #" << l.order_ << "=" << l.value_ << ' '
    << (l.value_ + l.uncertainty_)
    << ' ' << ((isLess) ? " < " : " !< ")
    << (r.value_ - r.uncertainty_)
    << ' ' << rlo + 2.0 * tol
    << " #" << r.order_ << "=" << r.value_
//		<< ' ' << diff
    << std::endl;
  }
  return isLess;
} // bool Meas::lessU2(const Meas& l, const Meas& r)

bool Meas::greaterU(const Meas& l, const Meas& r)
{ // l > r
  // Uses Uncertain measured value, not order.
  double lhi = (l.value_ + l.uncertainty_);  // Upper confidence limit of left.
  double rlo = (r.value_ - r.uncertainty_);	// Lower confidence limit of right.
 // double diff = (l.value_ + l.uncertainty_) - (r.value_ - r.uncertainty_);
 // double tol = hypot(l.uncertainty_, r.uncertainty_);
  // Might chose smallest, or largest uncertainty, or this compromise.
  // Reverses the order when small uncertainty compared to large uncertainty and large chosen for tol.
  bool isLess =  lhi < (rlo + 0. * hypot(l.uncertainty_, r.uncertainty_)); // == less
  return isLess;
} // bool Meas::greater(const Meas& l, const Meas& r)

bool Meas::equal_toUnc(const Meas& l, const Meas& r)
{ // l ~== r equal within uncertainty.
  // Uses Uncertain measured value, not order.
  double lhi = (l.value_ + l.uncertainty_);  // Upper confidence limit of left.
  double rlo = (r.value_ - r.uncertainty_);	// Lower confidence limit of right.
//  double diff = (l.value_ + l.uncertainty_) - (r.value_ - r.uncertainty_);
 // double tol = hypot(l.uncertainty_, r.uncertainty_);
  // Might chose smallest, or largest uncertainty, or this compromise.
  // Reverses the order when small uncertainty compared to large uncertainty and large chosen for tol.
  bool isLess =  lhi < (rlo + 0. * hypot(l.uncertainty_, r.uncertainty_)); // == less
  return isLess;
} // bool Meas::greater(const Meas& l, const Meas& r)

// Global Output Meas::operator<<
std::ostream& operator<< (std::ostream& os, const Meas& p)  // Definition.
{
  // Uses:
//  std::ostream& operator<< (std::ostream& os, boost::posix_time::ptime);
//  std::ostream& operator<< (std::ostream& os, const unc<false>&);

   // std::cout << p; is transformed to operator<< (std::cout, c); which does this:

    //os << p.value_ << " "
    //<< "+/-" << p.uncertainty_
    //<< " (" << p.degFree_ << ")"
    ////<< showuncflags // to display.
    //<< " {" << std::hex << p.unctypes_ << std::dec << "}";
    ////<< " {"
    ////<< showUncTypes(p.unctypes_)
    ////<< "}";

    uncun u(p.value_, p.uncertainty_, p.degFree_, p.unctypes_);
    os << u;

  if(p.id_.size() != 0)
  {// string identifier (if any).
      os << " " << p.id_;
  }
  if (p.order_ >= 0)
  { // means order is defined.
    os << " #" << p.order_; // For example # 17
  }
  else
  { // order undefined.
    // So no output?
  }
  if (p.time_ != boost::posix_time::not_a_date_time)
  {
   os << ", " << p.time_;  // Output t
  }
  // else don't output anything.
  return os;  //  Allow chaining concatenation of << ... << .
} // operator<<

istream& operator>> (istream& is, Meas& p)
{
  uncun u;
  is >> u;
  p.value_ = u.value_;
  p.uncertainty_ = u.uncertainty_;
  p.degFree_ = u.degFree_;
  p.unctypes_ = u.unctypes_;
  is >> p.order_;
  is >> p.id_;
  is >> p.time_;

  // 2 3 (could make smarter and/or require [2,3]).
    return is;  //  Allow chaining concatention of >> ... >>.
} // operator<<

// Defined in unc.hpp, for example:

//template <class T>
//const double value_of(T&);

template<>
double value_of(Meas v) //! \return Meas.value() as a double.
{ //! \return Meas.value() as a double.
  return v.value();
}

template<>
double unc_of(Meas u) //! \return Meas.std_dev() as a double.
{
  return u.std_dev();
}

//! Get values of a pair of values.
template <> //! \tparam T1 and T2 Built-in floating-point types Meas and unc.
std::pair<double, double> values_of(std::pair<Meas, uncun> vp)
{
  return std::make_pair<double, double>(value_of(vp.first), value_of(vp.second));
}

template <> //! \tparam T1 and T2 Built-in floating-point types Meas and unc.
std::pair<double, double> values_of(std::pair<Meas const, uncun> vp)
{
  return std::make_pair<double, double>(value_of(vp.first), value_of(vp.second));
}

//! Get uncertainties (standard deviation) of a pair of values.
template <> //! \tparam T Built-in floating-point type or unc.
std::pair<double, double> uncs_of(std::pair<Meas, uncun> vp)
{
  return std::make_pair<double, double>(unc_of(vp.first), unc_of(vp.second));
}

template <> //! \tparam T Built-in floating-point type or unc.
std::pair<double, double> uncs_of(std::pair<Meas const, uncun> vp)
{
  return std::make_pair<double, double>(unc_of(vp.first), unc_of(vp.second));
}


#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif // MEAS_IPP
