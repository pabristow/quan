/*! \file 
   \brief Class for measurement using @b two uncertain class items.
   \details class meas2 with @b two uncertain type UReal and measurement order & time-stamp.
*/

// meas2.hpp 
// Copyright Paul A. Bristow 2012

#ifndef MEAS2_HPP
#define MEAS2_HPP

#include <boost/quan/meas.hpp>

class meas2;

class meas2
{
  friend ostream& operator<< (ostream&, const Meas&);
  friend istream& operator>> (istream&, Meas&);

  public:
  //Meas(); // Constructor - all defaults.
  meas2(uncun ux, uncun uy, string id = "", boost::posix_time::ptime ti= (boost::posix_time::not_a_date_time), int o = -1);
  // Construct from two values and all other info

   // Meas Member variables.
public: 
  uncun m_uncx;
  uncun m_uncy;

  // Time and order values could be:
  // 1 no known order.
  // 2 known order, but no times.
  // 3 times, from which order can be calculated,
  // or both order and times given, so may need to be checked for consistency.

  std::string m_id; // Identification info, if any, else "".

  int m_order;  // Index from 0 (or 1 perhaps?)  -1 == unknown?
  // time_t m_time; // Time too as time_t, notaTime if not known.
  boost::posix_time::ptime m_time; // Posix time from Boost.Date_time.
}; // class meas2

// Output and input operator<< & operator>> 
ostream& operator<< (ostream& os, const meas2& m);
istream& operator>> (istream& is, meas2& m);

#endif // MEAS2_HPP