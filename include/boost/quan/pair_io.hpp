 /*! \file pair_io.hpp
  \brief Provide operator<< to output pair, surrounded by < > and separated by comma, 
  for example: "<1.23, 4.56>".
  Typically used to show confidence intervals around a mean.
  \details Polite to keep this a private detail namespace to avoid clash with other implementations!
 \author Paul A. Bristow
 \date Oct 2009, 2012
*/

#include <utility>
 // using std::pair;

// 
template<typename charT, typename traits, typename T1, typename T2>
inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<T1, T2>& p)
{
  return os << "<" << p.first << ", " << p.second << '>'; // Separated by comma & space.
}

// Explicit specialization for pair of doubles.
template<typename charT, typename traits>
inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<double, double>& p)
{
  return os << "<" << p.first << ", " << p.second << '>'; // Separated by comma & space.
}



