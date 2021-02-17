 /*! \file pair_io.hpp
  \brief Provide operator<< to output pair, surrounded by < > and separated by comma,
  for example: "<1.23, 4.56>".
  Typically used to show confidence intervals around a mean.
  \details Polite to keep this a private detail namespace to avoid clash with other implementations!
 \author Paul A. Bristow
 \date Oct 2009, 2012, 2021
*/

// boost\libs\quan\include\boost\quan\pair_io.hpp
#include <utility>
 // using std::pair;

//! Output both items of a pair of items (not necessarily the same type), enclosed by <> and separated by comma & space.
template<typename charT, typename traits, typename T1, typename T2>
inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<T1, T2>& p)
{
  return os << "<" << p.first << ", " << p.second << '>'; // Enclosed by <> and separated by comma & space.
}

//! Output pair, separated by comma & space, enclosed by <> and separated by comma & space.
// Explicit specialization for std::pair<double, double>.  (not sure why this is needed considering the above).
template<typename charT, typename traits>
inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<double, double>& p)
{
  return os << "<" << p.first << ", " << p.second << '>'; // Enclosed by <> and separated by comma & space.
}



