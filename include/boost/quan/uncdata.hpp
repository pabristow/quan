/*!
\file
\brief Summary of uncFlagsIndex, unc_types and uncertainflags.

*/

extern const long uncFlagsIndex; // uncertain flags for unc_output
// by << flex << plusminus ...
extern const long oldUncFlagsIndex; // Saved previous uncertain flags.
extern const long sigDigitsIndex; // sigDigits auto calc in unc_output.
extern const long setSigDigitsIndex; // sigfigs to use if setSigDigits flag set.
extern const long oldSigDigitsIndex; // Previous value of sigDigitsIndex
// by << setsigdigits ... Value assigned by << setSigDigits(4) ...
extern const long stdDevSigDigitsIndex;  // Sigfigs to use for +|- output,
// if setuncsigfigs flag bit set by << setuncsigfigs...// Value set by << setUncSigDigits(3) ...
extern const long oldStdDevSigDigitsIndex;  // Previous value of stdDevSigDigitsIndex.
extern const long ScaleIndex; // Auto scale factor calculated by unc_output.
extern const long setScaleIndex; // Use if set scaled flag set by << setscale...
// Scale value set by << setScale(6) ...
extern const long oldScaleIndex; // Previous value of ScaleIndex.
extern const long uncWidthIndex; // width to use by unc_output if
// firm format uncFlags set by << flexform ...// Value assigned by << setUncWidth(9) ...
extern const long oldUncWidthIndex; // Previous value of uncWidthIndex.
extern const long oldUncSetWidthIndex; // Not used yet.
extern const long usedIndex; // output chars actually used.
extern const long oldUncUsedIndex;  //  Previous value of usedIndex.
extern const long widthIndex;  // ios setwidth.
extern const long oldWidthIndex;   // Previous value of ios setwidth.

enum unc_types
{   // 16 type bits used by unc uncTypes. Bit set = 1 means a positive attribute.
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
  UNC_UNIFORM = 0x800, // Uncertainty has rectangular probability distribution.
  UNC_TRIANGULAR = 0x1000, // Uncertainty has triangular probability distribution.
  //  If not rectangular or triangular, then normal or Gaussian.
  // But is triangular & gaussian different in real life?
  DEG_FREE_EXACT = 0x2000, // Known number of observations -1,
  // so default zero means 1 observation.
  // else estimated, for example, using Welch-Satterthwaite.
  DEG_FREE_KNOWN = 0x4000, // Degrees of freedom defined.
  SPARE = 0x8000 // This unc_type is spare.
};

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
  setUncSigFig = 0x100 // if bit 8 = 1 use set Unc sigfig else calculate from degrees of freedom.