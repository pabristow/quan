// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 1998, 2012.

/*!
  \file
  \brief Initialisation of std::stream iword to store uncertainty information.
   \details This sets the index values for data stored using the xalloc mechanism.
   This is included from unc.hpp and accessed from unc.ipp for many functions.
   Function setUncdefaults is used to set default values for these items for an iostream.
*/

#ifndef UNC_INIT
#define UNC_INIT

// unc_init.hpp called by unc.hpp

#include <ios> // for ios_base.

// This block of definitions MUST be positioned before main.
// 14 indexes of long iwords allocated by calls of ios_base.xalloc();
// 1st call of xalloc() returns 0 so ios_base.iword(0) used for magic id,
// 2nd call of xalloc() returns 1 so ios_base.iword(1) used for uncFlags,
// 3rd calls returns 2, so iosword(2) used for sigDigits ...
// Order of assignment must ensure these match enum uncindex (if used).
const long zeroIndex = std::ios_base::xalloc(); // 1st iword[0] to hold a 'magic' id.

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

extern const long indexID;  // 'Magic' value defined in unc_init.hpp.
bool isIndexed = true; // Above indexes have been initialised.

#endif
