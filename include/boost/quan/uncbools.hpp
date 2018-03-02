	long& uncWidth = os.iword(uncWidthIndex); //
	long& oldUncWidth = os.iword(oldUncWidthIndex); //
	long& uncFlags = os.iword(uncFlagsIndex);
	long& Width = os.iword(widthIndex);
	long& oldWidth = os.iword(oldWidthIndex);
	long& setSigDigits = os.iword(setSigDigitsIndex);
	long& setStdDevSigDigits = os.iword(stdDevSigDigitsIndex);

	// bools showing output requirements.
	bool isNoisyDigit; // Extra 'noisy' guard digit to reduce risk of information loss.
	bool isPlusMinus;// Uncertainty as +/- is required too (but ignore if exact or integer).
	bool isSetScale;		// Defined to mean scale to specified power of 10.
	bool isAutoScale; // is to scale automatically to SI multple of 1000.
	bool isUppercase; // Exponential format is, for example, 1E6 else 1e6.
	bool isExpFormat;	// Taken to mean that exponential format wanted (always possible).
	bool isShowPoint;  // Means decimal point is always shown, for example 900. even if not needed.
	bool isShowPos; // Show + sign always.
	bool isFixed;	// Taken to mean that d.dddd format wanted, if possible.
	bool isLeftJustify; // left justify, trailing pad after.
	bool isRightJustify; // right justify, leading pad before.
	bool isInternalJustify;  // Not defined yet, but use to center in field?
	// bools showing  state of arguments value, stdDev, df.
	bool isExactValue; // value is exact (integer, rational, zero stdDev or noplus & nominus).
	bool isIntegerValue; // value is an integer, or stdDev = 0.f.
	bool isUncKnown; // StdDev or uncertainty is valid.
	bool isValueNaN; // value is NaN
	bool isStdDevNaN; // StdDev is NaN
	bool isValueInfinite; // value is infinity.
	bool isNegativeValue = false; // Value is negative.
	bool isZeroValue; // Value is zero.
	bool isCenter; // Both left and right, so center (on decimal point if possible).
	bool isFirmFormat; // os << setw(9) has prescribed a width.

