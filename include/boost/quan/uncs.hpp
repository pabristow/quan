// unc reference info from unc.hpp

enum unc_types
{   // 16 type bits used by UReal uncTypes. Bit set = 1 means a positive attribute.
	VALUE_ZERO = 1 << 0, // Too near zero to allow relsd = sd /mean
	// A few times std::numeric_limits<float>::min() is a suitable limit?
	VALUE_INTEGER = 1 << 1, // Exact integer 2,3,4 ... may not be possible to store.
	VALUE_RATIONAL = 1 << 2, // Integers & Fractions 1/2, 1/3, 2/3, 5/4 ...
	// so don't need irrational - must be if not rational.
	// Irrational like pi, e ... representated as accurately as possible.
	VALUE_NEGATIVE_ONLY = 1 << 3, // Value can ONLY be < 0.. ? or <=0?
	VALUE_POSITIVE_ONLY = 1 << 4, // Value can ONLY be >=0..
	UNC_KNOWN = 0x20,    // Uncertainty known (but if not value may known OK).
	UNC_NOPLUS = 0x40, // Uncertainty can only be < value, + = zero.
	UNC_NOMINUS = 0x80, // Uncertainty can only be > value - = zero.
	// Integer, Rational or Exact by definition like 25.4 mm per inch,
	// if unc = 0.0f or both UNC_NOPLUS & UNC_NOMINUS.
	UNC_QUAN_DECIMAL = 0X100, // Quantised by least significant decimal digit.
	UNC_QUAN_BINARY = 0X200, // Quantised by least significant binary digit.
	UNC_EXPLICIT = 0x400, //If bit set = 1, uncertainty implicit from sig digits.
	// or explicit if +/- or calculated (normal default case, bit = 0).
	UNC_RECTANGULAR = 0x800, // Uncertainty has rectangular probability distribution.
	UNC_TRIANGULAR = 0x1000, // Uncertainty has triangular probability distribution.
	//  If not rectangular or triangular, then normal or Gaussian.
	// But is triangular & gaussian different in real life?
	DEG_FREE_EXACT = 0x2000, // Known number of observations -1,
	// so default zero means 1 observation.
	// else estimated, for example, using Welch-Satterthwaite.
	DEG_FREE_KNOWN = 0x4000, // Degrees of freedom defined.
	SPARE = 0x8000 // This unc_type is spare.
};

const unsigned short int VALUE_EXACT = (UNC_NOMINUS | UNC_NOPLUS);  // Both set.
const unsigned short int UNC_DEF = (UNC_KNOWN | UNC_NOMINUS | UNC_NOPLUS |
																		UNC_QUAN_DECIMAL | UNC_QUAN_BINARY |
																		UNC_EXPLICIT | UNC_RECTANGULAR | UNC_TRIANGULAR);  //
const unsigned short int DEG_FREE_DEF = (DEG_FREE_EXACT | DEG_FREE_KNOWN);

enum uncertainflags
{  // For control of printing.
	none, // Default.
	firm = 1,  // bit 0: == 0 == false means flex, or firm == 1 == true.
	setScaled = 2, // bit 1 Set scaled == 1 or not scaled == 0.
	// by fixed factor SetScale and << scale ...
	// power10 in range -max to +max.
	autoScaled = 4,  // bit 2: auto = 1  with << autoscale ...
	plusMinus = 8, // bit 3 = 1 means add +/- uncertainty << plusminus ...
	addSISymbol = 0x10,  // bit 4 = 1, add suffix SI symbol G, M, k, m, u, n, p ...
	// If one is applicable, else = 0 do nothing.
	addSIPrefix = 0x20,  // bit 5 = 1, add suffix SI prefix Giga, Mega, kilo ...
	noisyDigit = 0x40, // Add extra 'noisy' decimal digit to value & sd
	// This means is suitable for input to other calculations because the
	// random extra 2 to 3 bits will approximate a continuous function.
	// Quantisation to fewer digits would distort statistical calculations.
	// Default is to use minimum decimal digits for clearer display.
	// Extra digit also added when degrees of freedom > 10.
	setSigFig = 0x80, // if bit 7 = 1 use set sigfig else calculate from sd.
	setUncSigFig = 0x100 // if bit 8 = 1 use set Unc sigfig
	// else calculate from degrees of freedom.
};

void uncertainPrint(  // Declaration, defined below.
										double, // Mean or most likely value.
										float, // Standard deviation.
										unsigned short int, // Degrees of freedom.
										unsigned short int, // 16 Uncertain type flags.
										ostream&);  // Output stream.


void uncertainRead(double& mean,  // mean (central or most probable) value.
		               double& stdDeviation,
									 unsigned short int& degreesOfFreedom,  // 1 observation.
									 unsigned short int& types,
									 istream& is);

// Parameterless Manipulators declarations.
// Similar to ios_base flags but for uncertain printing.
// Use uppercase convention for these,
// but all lowercase for manipulators like hex, oct ...
// Usage:  out << scale << noscale << autoscale << noautoscale
// firmform << flexform << plusminus << noplusminus ....

// All ios_base functions declared below are defined in unc.cpp
// Note that all of these are entirely lower case, like ios manipulators.
ios_base& scale(ios_base&); // To use value set with setUncScale(int)
ios_base& noscale(ios_base&);  // or not scaled.
ios_base& autoscale(ios_base&); //  Scale to suitable symbol like M, k, m...
ios_base& noautoscale(ios_base&);  // or not.
ios_base& firmform(ios_base&);  // Firm fixed layout using setUncWidth.
ios_base& flexform(ios_base&); // Flexible free format.
ios_base& plusminus(ios_base&);  // Add +/- uncertainty.
ios_base& noplusminus(ios_base&);  // No +/- uncertainty
ios_base& addSIprefix(ios_base&); // Add SI prefix like kilo, micro ..
ios_base& noSIprefix(ios_base&);	// (Takes precedence over SI symbol if both set).
ios_base& addSIsymbol(ios_base&); // Add SI symbol like M, k, m.
ios_base& noSIsymbol(ios_base&);  // No SI symbol like M, k, m
ios_base& addNoisyDigit(ios_base&);  // Add an extra 'noisy' digit for computer.
ios_base& noNoisyDigit(ios_base&);  // No noisy for human reading.
ios_base& setsigdigits(ios_base&);    // Use sigDigits set with << setSigDigits(4) ...
ios_base& autouncsigfigs(ios_base&);  // Calculate sigfigs from uncertainty.
ios_base& setuncsigfigs(ios_base&); // Use sigDigits set with << setUncSigDigits(2) ...
ios_base& autosigfigs(ios_base&);  // Calculate sigfigs from uncertainty.

// Functions to change uncertain flags on specified ios_base.
// Usage: f = uFlags(out); f = uFlags(out, 0xFF);
// f = setuFlags(out, 0xFF); f = resetuFlags(out, 0xFF);
long uFlags(ios_base&);  // Returns current uncertain flags.
long uFlags(ios_base&, long); // Assigns all uncertain flags & returns previous.
long setuFlags(ios_base&, long);  // Set specific flags = 1
long resetuFlags(ios_base&, long);  // Reset/clear specific flags = 0

// Forward declarations, defined in unc.cpp.
class showUncFlags;  // Output uncertain flags to ostream << showUncFlags ...
class setAllUncFlags;  // Assign value to set (aand/or clear) all unc flags.
class setUncFlags;  // Set specific unc flags bits.
class setMaskedUncFlags;  // Clear mask & then set unc flag bits.
class resetUncFlags;  //
class resetMaskedUncFlags;
class setUncWidth;
class setUncScale; //
class setSigDigits;  // sigfigs to use for value if << setsigdigits
class setUncSigDigits;  // sigfigs to use for uncertainty if << setuncsigfigs