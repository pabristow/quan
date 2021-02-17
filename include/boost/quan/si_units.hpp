/*!
  \file
  \brief SI Unit, names and abbreviations & conversion factors.

  \details
    See NIST 811 Special publication, Barry N Taylor (1995)
    "Guide for the Use of the International System of Units (SI)
    & ISO 31, a closely aligned document.
    NIST version contains more on non-SI units and many useful notes.
 */

// Data & code to show units in si_units.cpp which includes this header.
// Changed to unit starting with capital so time doesn't clash with C time()

// SIunits.hpp

// Copyright Paul A. Bristow 2009, 2012, 2021.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIUNITS_HPP
#define SIUNITS_HPP

#ifdef _MSC_VER
#  pragma once
#endif

#include <iostream>  // Used by show units functions.
using std::ostream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::size_t;

const unsigned int notOK = 0xffffffff;

const int maxPowerTen = 38;
// == std::numeric_limits<float>::max_exponent10;
// == FLT_MAX_10_EXP in float.h, so can represent FLT_MAX = 3.402823466e+38f
// to FLT_MIN_10_EXP in float.h, so can represent FLT_MIN = 1.175494351e-38f
// This is chosen to limit the size of the power-of-ten lookup table below,
// and make code the same if store value in a 32-bit double.
// For real-life data, value of a physical measurement unit is unlikely to exceed 10^38,
// despite DBL_MAX_10_EXP == 308.

extern const double powersOfTen [maxPowerTen + 1 + maxPowerTen];
 // 1 is middle zeroth power.
const int maxRounds = 17;
extern const double rounds[maxRounds];

const int maxPowerTens = maxPowerTen + 1 + maxPowerTen;
const int SImaxPowerTen = 24; // Define array sizes.
// SImaxPowerTen < maxPowerTen, until new names invented for more units.
const int SIminPowerTen = -24; // yotta & min 10**-24
const char* SIprefixes[]; // [0] to [48], countof = 49 (defined in si_units.cpp).
const char* SIsymbols [];
const int SImaxPowers = SImaxPowerTen + 1 + SImaxPowerTen;
const int SIprefixesLength = SImaxPowerTen + 1 + SImaxPowerTen;
// = sizeof SIprefixes /sizeof (const char*);

struct SImultiple;

//! Fundamental units: Mass, Length, Time, Charge, Temperature, Luminous Intensity & angle.
struct unit
{
public:
  const char* unitOf;  // "mass", "length", "time" ...
    // unsigned char here causes need to int( )
    // better to use short int?
    // if const.
    // C2552: non-aggregates cannot be initialized with initializer list.
    // but can't see why - looks aggregate enough to me.
  unsigned char unitSINameCount;  // Countof SI unitName (meter = 5 + 1 = 6).
  // = length of SIunitsNames[0]  ????????? not used and wrong?
  unsigned char unitAbbrevCount; // Count of chars in abbreviation (Hz = 2).
  unsigned char unitNamesCount; // Count of units (no plurals or aliases or abbreviations)
  // : meter, feet, inch...
  // = length of array unitNames.
  unsigned char unitsNamesCount;  // Total number of all names for all units.
  // (including plurals, abbreviations & aliases).
  // None of these include terminating zero.
  // Can't be const or triggered C2552 - wrongly?
  // So instantiate as const unit ...
  const int* unitLengths; // Count of aliases & abbreviations for each unit.
  const char** SIunitNames;  // SI or preferred unit, for example: "meter"
  // SI unit MUST be always FIRST in list of unitNames = unitsNames[0],
  // & SI abbreviation second.
  const char* SIunitAbbrev;  // "m", "Hz" = SIunitNames[1];
  const char*** unitNames; // "meter", "feet", "mile" ...
  // By convention, unitNames[0] is the base or full unit name, like meter,
  // & unitNames[1] is the abbreviation, like m.
  const double* unitToSIfactors;  // factors to convert to SI unit.
}; // unit
 
// Definitions in SIunits.cpp for separate compilation.

// Dimensionless units.
extern const int noUnitCount;
extern const char* noUnits [];
extern const unit noUnit;
extern const int perCentUnitCount;
extern const char* perCentUnits [];
extern const int perMilleUnitCount;
extern const char* perMilleUnits [];
extern const int perMillionUnitCount;
extern const char* perMillionUnits[];
extern const int perBillionUnitCount;
extern const char* perBillionUnits [];
extern const int perTrillionUnitCount;
extern const char* perTrillionUnits [];
extern const char** dimlessNoUnits [];
extern const int dimlessUnitsCounts[];
extern const double NoUnitsFactors [];
extern const unit noUnit;

// Units of mass: gram, kg, ton, cwt, lb, oz...
extern const int massUnitCount;
extern const int gramMassUnitCount;
extern const char* gramMassUnits [];
extern const int tonMassUnitCount;
extern const char* tonMassUnits[];
extern const int cwtMassUnitCount;
extern const char* cwtMassUnits[];
extern const int lbMassUnitCount;
extern const char* lbMassUnits [];
extern const int ozMassUnitCount;
extern const char* ozMassUnits [];
extern const char** massUnits [];
extern const int massUnitsCounts [];
extern const double massToSIfactors[];
extern const unit mass;

// Units of Length.
extern const int lengthUnitCount; // meter, mile, yard, feet, inch, point
extern const int meterLengthUnitCount;
extern const char* meterLengthUnits[];
extern const int mileLengthUnitCount;
extern const char* mileLengthUnits[];
extern const int yardLengthUnitCount;
extern const char* yardLengthUnits [];
extern const int feetLengthUnitCount;
extern const char* feetLengthUnits [];
extern const int inchLengthUnitCount;
extern const char* inchLengthUnits [];
extern const int pointLengthUnitCount;
extern const char* pointLengthUnits [];
extern const double lengthToSIfactors[];
extern const char** lengthUnits [];
extern const int lengthUnitsCounts[];
extern const unit length;

// Units of time: second, minute, hour ...
extern const int timeUnitCount;
extern const int secondTimeUnitCount;
extern const char* secondTimeUnits [];
extern const int secondTimeUnitCount;
extern const char* secondTimeUnits [];
extern const int minuteTimeUnitCount;
extern const char* minuteTimeUnits [];
extern const int hourTimeUnitCount;
extern const char* hourTimeUnits [];
extern const int dayTimeUnitCount;
extern const char* dayTimeUnits [];
extern const int monthTimeUnitCount;
extern const char* monthTimeUnits [];
extern const int yearTimeUnitCount;
extern const char* yearTimeUnits [];
extern const int fortnightTimeUnitCount;
extern const char* fortnightTimeUnits [];
extern const double timeToSIfactors[];
extern const char** timeUnits [];
extern const int timeUnitsCounts [];
extern const unit Time; // Upper case to avoid clash with C global time.
// time() is in C time.h

// Units of current: amp
extern const int currentUnitCount;
extern const int ampCurrentUnitCount;
extern const char* ampCurrentUnits[];
extern const double currentToSIfactors [];
extern const char** currentUnits [];
extern const int currentUnitsCounts [];
extern const unit current;

// Units of Temperature: degress Kelvin & Celsius
extern const int temperatureUnitCount;
extern const int kTemperatureUnitCount;
extern const char* kelvinTemperatureUnits[];
extern const int cTemperatureUnitCount;
extern const char* celsiusTemperatureUnits[];
extern const int fTemperatureUnitCount;
extern const char* fahrenheitTemperatureUnits[];
extern const double temperatureToSIfactors[];
extern const char** temperatureUnits[];
extern const int temperatureUnitsCounts [];
extern const unit temperature;

// Units of substance: Mole.
extern const int substanceUnitCount;  // just moles
extern const int molSubstanceUnitCount;
extern const char* molSubstanceUnits[];
extern const double substanceToSIfactors[];
extern const char** substanceUnits [];
extern const int substanceUnitsCounts [];
extern const unit substance;

// Units of luminous intensity: candela ...
extern const int lumintensityUnitCount;;
extern const int candelaLumintensityUnitCount;
extern const char* candelaLumintensityUnits[];
extern const double lumintensityToSIfactors[];
extern const char** lumintensityUnits [];
extern const int lumintensityUnitsCounts [];
extern const unit lumintensity;

// Frequency: Hertz.
extern const int freqUnitCount;
extern const int hertzFreqUnitCount;
extern const char* hertzFreqUnits [];
extern const double freqToSIfactors [];
extern const char** freqUnits [];
extern const int freqUnitsCounts [];
extern const unit freq;

  // Charge: Coulomb.
extern const int chargeUnitCount;
extern const int coulombChargeUnitCount;
extern const char* coulombChargeUnits[];
extern const int ampHourChargeUnitCount;
extern const char* ampHourChargeUnits[];
extern const double chargeToSIfactors[];
extern const char** chargeUnits [];
extern const int chargeUnitsCounts [];
extern const unit charge;

// Unknown unit.
extern const int unknownUnitCount;
extern const int unknownCount;
extern const char* unknownunit [];
extern const int unknownUnitsCount;
extern const char** unknownUnits [];
extern const int unknownUnitsCounts [];
extern const double unknownUnitsFactors [];
extern const unit unknownUnit;

// Array of all units.
extern const int unitsCount;
extern const unit* unitps[];  // 7 SI base units: & then others.

// Declarations of functions to show units (see SIunits.txt)
extern void showAllUnits(const unit*, ostream&);  // mass, length, time ...
extern void showSIunit(const unit*, ostream&);  //
extern void showSIunits(const unit*, ostream&); // Show the SI unit & aliases.
extern void showNonSIunits(const unit*, ostream&);
  // Show just the base Non SI units, but not aliases.
extern void showAllNonSIunits(const unit*, ostream&);
  // Show all the Non SI units.

unsigned int findUnit(const string, // multiple unit
                      const unit*, // unit
                      unsigned int&, // index of name, alias or abbrev given unit.
                      unsigned int&); // SIindex.

const unit& findAnyUnit(const string, // multiple unit
                      unsigned int&, // index.
                      unsigned int&, // index of name, alias or abbrev given unit.
                      unsigned int&); // SIindex.

#endif // SIUNITS_HPP
