// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012, 2021

/*!
  \file
  \brief Initialisation of @c std::stream iword to store uncertainty information.
   \details This sets the index values for data stored using the @c std::ios_base::xalloc mechanism.
   This allows a memory of stream state from previous calls of a @c std::iostream.
   This file @c unc_init.hpp is included from @c unc.hpp for many functions.
   Function @c setUncdefaults() is used to set default values for these xalloc items
   for a specified @c std::iostream (default @c std::cout).
*/

#ifndef BOOST_QUAN_UNC_INIT
#define BOOST_QUAN_UNC_INIT

#include <ios> // for std::ios_base.
// unc_init.hpp is called from unc.hpp semi-automatically on first use of output of a @c class @c unc for a @c std::ostream.

namespace boost {
namespace quan {

  //! This block of definitions MUST be positioned before function main() is called.
  //! 14 indexes of long iwords allocated by calls of std::ios_base::xalloc();
  //! 1st call of xalloc() returns 0 so std::ios_base.iword(0) used for magic ID,
  //! 2nd call of xalloc() returns 1 so std::ios_base.iword(1) used for uncFlags,
  //! 3rd calls returns 2, so iosword(2) used for sigDigits ...
  //! Order of assignment must ensure these match enum uncindex (if used).
  //! zero_index and top_index are used to hold a 'magic' unique number
  //! to show that the @c std::ios_base::xalloc values have been initialised, and not corrupted.
  const long zeroIndex = std::ios_base::xalloc(); // 1st iword[0] to hold a 'magic' ID.

  const long uncFlagsIndex = std::ios_base::xalloc(); // long& uncFlags = iword(1)
  const long oldUncFlagsIndex = std::ios_base::xalloc(); // long& olduncFlags = iword(2)
  const long sigDigitsIndex = std::ios_base::xalloc(); // sigDigits = iword(3)
  const long oldSigDigitsIndex = std::ios_base::xalloc(); // oldsigDigits = iword(4)
  const long setSigDigitsIndex = std::ios_base::xalloc(); // setsigDigits = iword(5)
  const long uncSigDigitsIndex = std::ios_base::xalloc(); // setUncSigDigitsIndex = iword(6)
  const long setUncSigDigitsIndex = std::ios_base::xalloc(); // setUncSigDigitsIndex = iword(7)
  const long oldUncSigDigitsIndex = std::ios_base::xalloc(); // oldUncSigDigitsIndex = iword(8)
  const long scaleIndex = std::ios_base::xalloc(); // scale = iword(9)
  const long oldScaleIndex = std::ios_base::xalloc(); // oldScale = iword(10)
  const long setScaleIndex = std::ios_base::xalloc(); // setScale = iword(11)
  const long uncWidthIndex = std::ios_base::xalloc(); // uncWidth = iword(12)
  const long oldUncWidthIndex = std::ios_base::xalloc(); // oldUncwidth = iword(13)
  const long oldUncSetWidthIndex = std::ios_base::xalloc(); // oldUncSetWidth = iword(14)
  const long usedIndex = std::ios_base::xalloc(); // used = iword(15)
  const long oldUncUsedIndex = std::ios_base::xalloc(); // oldUsed = iword(16)
  const long widthIndex = std::ios_base::xalloc(); // width = iword(17)
  const long oldWidthIndex = std::ios_base::xalloc(); // oldWidth = iword(18)
  const long roundingLossIndex = std::ios_base::xalloc(); // roundingLoss = iword(19)
  const long confidenceIndex = std::ios_base::xalloc(); // roundingLoss = iword(20)

  const long topIndex = std::ios_base::xalloc(); // long& topIndex = iword(21] == iword(0] check!
  //const long indexID;  //!< 'Magic' value defined in unc_init.hpp.
  bool isIndexed = true; //!< Above indexes have been initialised.
  // This is tested to on each use of @c std::ostream to check that it has been initialised.

} // namespace boost
} //  namespace quan

#endif // BOOST_QUAN_UNC_INIT

