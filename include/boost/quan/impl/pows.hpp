
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012, 2018.

//  pows.hpp

#ifndef BOOST_QUAN_POWS
#define BOOST_QUAN_POWS

//! Squared function, notationally convenient for a^2, but overflow possible?
template <typename Type>
inline Type sqr(const Type& a)
{
  return a * a;
}

//! Cubed function, notationally convenient for x^3, but overflow possible?
template <typename Type>
inline Type cube(const Type& a)
{
  return a * a * a;
}

//! Quaded function, notationally convenient for x^4, but overflow possible?
//! Used by Welch-Satterthwaite formula.
template <typename Type>
inline Type pow4(const Type& a)
{
  return a * a * a * a;
}

// TODO better to use std::hypot here, templated for C++11
/*! Hypot or Square root of the sum of the squares of two numbers,
which is equal to the length of the hypotenuse of a right
triangle if the two arguments are the lengths of the legs.
Used for uncertainty propagation.
\tparam T arithmetic_type  
*/
template <typename T>
inline T sqrtSumSqrs (const T& a, const T b)
{
  return (Type)sqrt(a * a + b * b);
}

#endif // BOOST_QUAN_POWS


