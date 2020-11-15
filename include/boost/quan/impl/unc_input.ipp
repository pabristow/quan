/*!
  \file
  \brief Testing uncertain classes using Boost Test Tool.
  \details Class for simple Propagation of Uncertainties
     according to a pure Gaussian model.
*/

// Copyright Paul A. Bristow 1998, 2012.

// unc_input.cpp

#include <iosfwd>
//#include <boost/quan/unc.hpp>

void unc_input(
                   double& value,  // mean, central or most probable value.
                   double& stdDev, // or float& perhaps?
                   unsigned short int& degreesOfFreedom,  // 1 observation.
                   unsigned short int& types, // TODO settings bits.
                   std::istream& is = std::cin)
{	// Inputs uncertainty as value, (implicitly exact, std deviation = 0).
  // & optionally [[+]|[-] <standard deviation * 2. >],
  // (1.0 implies 1. +|- 0.5 and sd of 0.5,
  // 1.00 implies 1. +|- 0.05 and sd of 0.05)

  // & optionally degrees of freedom [(<short int>)] like (99)
  // Used by istream& operator>> (istream&, unc<is_correlated>&)
  // Original simple version:
  //char plus, slash, minus;
  //s >> value >> plus >> slash >> minus >> stdDev;
  //if ((plus != '+') || (slash != '/') || (minus != '-'))
  //{
  //cerr << "Unexpected characters encountered in reading "
  //"value +/- stdDev !" << endl;
  //is.setf(ios_base::failbit);
  //}

  using std::istream;
  using std::ostream;
  using std::ios_base;
  using std::char_traits;
  using std::cout;
  using std::cerr;
  using std::cin;
  using std::endl;
  using std::flush;
  using std::ws;
  using std::streamsize;
  using std::boolalpha;
  using std::dec;
  using std::hex;
  using std::showbase;
  using std::fixed;
  using std::scientific;
  using std::right;
  using std::showpos;
  using std::noshowpos;
  using std::noshowbase;
  using std::noshowpoint;
  using std::showpoint;

  std::streamsize avail = is.rdbuf()->in_avail();
  if (avail == 0)
  {
    std::cerr << "No input available!" << std::endl;
    value = std::numeric_limits<double>::quiet_NaN();
    stdDev = std::numeric_limits<float>::quiet_NaN();
    degreesOfFreedom = 0;
    types = 0U;
    return;  // todo
  }

  // 'Default' values for unc_input arguments.
  value = 0.;	// double mean, central or most probable value.
  stdDev = 0.f;  // Exact float std deviation, unless find otherwise.
  degreesOfFreedom = 1U;  // Default == 1 observation.
  types = 0U;
    //static_cast<unsigned short>(~(VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL| VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY | UNC_KNOWN | UNC_NOPLUS
    //	| UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_EXPLICIT | UNC_UNIFORM | UNC_TRIANGULAR | DEG_FREE_EXACT | DEG_FREE_KNOWN));

  // All these need to be 'global' to uncertainRead, so declared here.
  // char_traits<char>::int_type c;
  // no_unused_variable_warning(c);
  int iv = 0;  // Integer part of floating point number.
  double fv = 0.;  // Fractional part of floating point number.
  int exponent = 0; // Exponent power of 10, default 10 ^ 0 = 1.
	bool isPositive = true; // Value input is >= 0.
  bool isNegativeValue = false; // Value input < 0.
  bool isPlusMinus = false;  // Normal case is +/- or +|-.
  bool isPlus = false; // +0
  bool isMinus = false; // -0
  bool isExactValue = true; // Assume no digits follow.
  bool isZeroValue = true; // Until find either integer or fractional part != 0
  bool isIntegerValue = false;
  bool isExponent = false;
  //	bool isValueZero = false;
  // Rationale for doing own read rather than using standard is to be able to distinguish
  // integer "0" or "123" implicitly exact from real "0." implicitly +/- 0.5.
  // Read integer part as one integer, option decimal as another, and fractional part as another,
  // then optional exponential part,
  // before any +/- and degrees of freedom as (999).

  is >> ws;  // Skip any leading whitespace.
  is >> iv;  // & read integer part of value (perhaps ii part of floating point number ii.ffe+ddd).
  // if (is.bad()) // Not useful to distinguish overflow etc.
  // if (is.eof()) // Sets fail for overflow, so not useful.
  if (!is)
  { // No integer part of value.
    cerr << "No numeric value input! " << showiostate << endl;
    value = std::numeric_limits<double>::quiet_NaN();
    stdDev = std::numeric_limits<float>::quiet_NaN();
    types = static_cast<unsigned short>(~(VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL| VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY | UNC_KNOWN | UNC_NOPLUS
      | UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_EXPLICIT | UNC_UNIFORM | UNC_TRIANGULAR | DEG_FREE_EXACT | DEG_FREE_KNOWN));
    // == 0U; // None of the above are true.
    // is.fail() == true;  is.good() == false; // already
    return;
  } // is.failed
  isIntegerValue = true; // but may be negated if find a decimal point later.
  types |= UNC_KNOWN;
  types &= ~UNC_EXPLICIT; // Clear bit because implicit from
  //  number of significant decimal digits - unless + or - provided.

  isNegativeValue = (iv < 0) ? true : false;
  // Overflow possible here if > numeric_limits<long>::max() == 2147483647
  // for example, 99999999999999.
  if (is.fail())
  {  // Check for overflow & other problems.
    std::cerr << "\t" "Uncertain value input failed! "<< showiostate << std::endl;
    value = std::numeric_limits<double>::quiet_NaN();
    stdDev = std::numeric_limits<float>::quiet_NaN();
    types = static_cast<unsigned short>(~(VALUE_ZERO | VALUE_INTEGER | VALUE_RATIONAL| VALUE_NEGATIVE_ONLY | VALUE_POSITIVE_ONLY | UNC_KNOWN | UNC_NOPLUS
      | UNC_NOMINUS | UNC_QUAN_DECIMAL | UNC_QUAN_BINARY | UNC_EXPLICIT | UNC_UNIFORM | UNC_TRIANGULAR | DEG_FREE_EXACT | DEG_FREE_KNOWN));
    // == 0U; // None of the above true.
    return;
  } // is.fail

  if (is.peek() == '.')   // locale.decimal_point[0] better.
  {  // A decimal fractional part may follow.
    is.get();  // Get (and discard) decimal_point.
    isIntegerValue = false; // Because . follows digit, so "9" is exact integer but "9." is not,
    // Even if no digits follow.
    isExactValue = false;
    stdDev = 0.5; // Implicit +/- if "9.", implies sd = 0.5.
    types |= UNC_QUAN_DECIMAL;  // Quantized by least significant decimal digit.
    types &= ~UNC_EXPLICIT;  // Clear - explicit because implied by significant digits.
    int count = 0; // Number of digits.  TODO ?? is this info used?
    if (isdigit(is.peek()))
    {  // There is a decimal fraction part.
      double fracTen = 1.;
      while (isdigit(is.peek()))
      {
        ++count;
        stdDev *= 0.1f; // Implicitly 1. means +/- 0.5 == stdDev = 0.05, 0.005 ...
        fracTen *= 0.1;
        int digit = (is.get() -'0');
        fv += digit * fracTen;
        // Potential for underflow to zero here?
      }
      if (isNegativeValue) fv = -fv;
      if (fv != 0)
      {
        isIntegerValue = false;
      }
    }  // Is a fraction part.
  }  // Is a decimal_point.
  else if (is.peek() == '/')
  { // Is a fraction, for example 2/3, so flag as a rational.
    is.get();  // Get (and discard) /.
    int denom;
    is >> denom;
    if (denom != 0)
    {
      value = static_cast<double>(iv)/denom;
      iv = 0;
      fv = 0;
      types |= VALUE_RATIONAL;
      types &= ~VALUE_ZERO;
    }
  }
  value += iv + fv; // Combine integer and fractional parts.

  if ((is.peek() & 0x5F ) == 'E') // 'e' or 'E'
  { // Exponential part.
    is.get();  // and discard 'e' or 'E'.
    is >> exponent;
    isExponent = true;
  }  // Exponent

  if (value == 0)
  {
    isZeroValue = false;
    types |= VALUE_ZERO;
  }

  if (isIntegerValue)
  {
    value = iv;
    types |= (UNC_KNOWN | VALUE_INTEGER | VALUE_RATIONAL );
//   types |= (UNC_KNOWN | VALUE_INTEGER | VALUE_RATIONAL | UNC_NOPLUS | UNC_NOMINUS);
#ifdef UNC_TRACE
    { // Implicitly exact +/- 0
      std::cerr << "    URead: Exact integer = " << value << std::endl;
    }
#endif
  }
  else
  { // Real NOT integer.
    types |= UNC_KNOWN; // Known.
    types &= ~(VALUE_INTEGER | VALUE_RATIONAL | UNC_NOPLUS | UNC_NOMINUS );
#ifdef UNC_TRACE
    {
      std::cerr << "    URead: " << value << " implicit +/- " << stdDev << ". ";
    }
#endif
  } // integer or not.

  is >> ws; // Skip optional whitespace between "123.45" and "+/-".

  //	Check if explicit uncertainty +/- (or +|-, or +-) or + or - follows,
  //	& read uncertainty if it does.
  double expUnc = 0.; // Explicit uncertainty.
  // 0x0F1 is files code for the single char '+-' symbol,
  if (!is.eof())
  { // Uncertainty (standard deviation) and degrees of freedom may follow.
    if ((is.peek() == '+') || (is.peek() == '-'))
    { // Some uncertainty follows, for example: +0.123, -0.456 or "+/-2.34" +|-3.45 
      std::char_traits<char>::int_type cusign = is.get(); // '+' or '-' read and discard.
      if(cusign == '+')
      {
       isPlus = true;
      }
      if(cusign == '-')
      {
       isMinus = true;
      }
      std::char_traits<char>::int_type p = is.peek();
      // 0xDD is File code for | vertical bar.
      // 0x0F1 is single char in files for '+-' symbol.
      if (p == '/' || p == '|' || p == '\\' || p == 0xDD || p == 0x0F1)
      { // Skip over any separator.
        std::char_traits<char>::int_type cusep = is.get(); // '/' or '|' or '\' chars.
        if (cusep ==  0x0F1)
        { // '+-' symbol, so must be both + and -.
          isPlus = true;
          isMinus = true;
        }
        // is >> ws; // Ignore any layout. But a bad idea to allow "+/ -"?
        p = is.peek(); // Expect + or - after separator.
        if (p == '+' || p == '-')
        { // +/- and +|- both allowed (or -/+ or -|+).
          // Assume +/+ uncertainty means just + and -/-, and uncertainty just -.
          std::char_traits<char>::int_type cusign2 = is.get(); // expect 2nd '+' or '-' char.
          if (cusign2 == '+')
          {
            isPlus = true;
            // else "*/+"
            is >> expUnc;  // Input Explicit uncertainty.
            if (!is.fail())
            { // Read OK, set bits.
              types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN );
            }
            else
            { // Reading uncertainty (standard deviation) value was not OK, so clear.
              types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN );
            }
          }  // == '+'
          else if (cusign2 == '-')
          {// else "*/-"
            isMinus = true;
            is >> expUnc; // Input Explicit negative uncertainty (have already consumed the - sign).
            if (!is.fail())
            { // Read OK, set bits.
              types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN );
            }
            else
            { // NotOK, so clear to unknown status.
              types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN );
            }
          } // == '-'
        }
        else
        { // Unexpected char (not + or -) after separator!
          //  TODO Need to signal fail!
        }
      }
      else if (cusign == '+')
      { // Positive uncertainty only, for example: "+0.123"
        isPlus = true;
        is >> expUnc; // Will consume the + sign.
        if (!is.fail())
        { // Read OK, set bits.
          types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN | UNC_NOMINUS);
        }
        else
        { // NotOK, so clear.
          types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN | UNC_NOMINUS);
        }
      }
      else if (cusign == '-')
      { // // Negative uncertainty only, for example: "-0.123"
        isMinus = true;
        is >> expUnc;
        // expUnc = -expUnc; // Have already consumed the '-' sign! But stddev is always positive.
        if (!is.fail())
        { // OK, so set uncertain type flags.
          types |= (UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN );
        }
        else
        { // NotOK so clear  flags.
          types &= ~(UNC_KNOWN | UNC_EXPLICIT | UNC_QUAN_DECIMAL | UNC_KNOWN );
        }
      }  // == '-'
    }

    if (isPlus || isMinus)
    { // Some plus and/or minus value has been input.
      types |= (UNC_KNOWN | UNC_EXPLICIT);
      if ((isPlus) && (isMinus))
      {
        isPlusMinus = true;  // If both plus and minus uncertainty.
        types &= ~(UNC_NOMINUS | UNC_NOPLUS); // Clear both exclusive,
        stdDev = static_cast<float>(expUnc);
      }
      else
 //   if (!isPlusMinus)  // Only + or -, but not both.
      {
        if (isPlus)
        {
            types |= UNC_NOMINUS;
        }
        else if (isMinus)
        {
            types |= UNC_NOPLUS;
        }
        stdDev = static_cast<float>(expUnc);
      } // not both.
    } //  isPlus or isMinus
    else
    { // Implicit.
      stdDev = static_cast<float>(stdDev);  //
    }

#ifdef UNC_TRACE
      {
        std::cerr << "    Explicit uncertainty ";
        if (isPlusMinus)
        {
          std::cerr << " +/- ";
        }
        else
        {
          std::cerr << ((isPlus) ? "+ " : "");
          std::cerr << ((isMinus) ? "- " : "");
        }
        std::cerr  << expUnc << ", stdDev " << stdDev << endl;

      } // trace
#endif
    // Degrees of freedom?
    is >> ws; // Eat any whitespace before "(99)".
    if (!is.eof())
    { // Degrees of freedom as "(99)" may follow.
      // (Default degreesOfFreedom = 0; == 1 observation.
      if (is.peek() == '(')
      { // 99) follows
        std::char_traits<char>::int_type  c =	is.get();  // Discard '('
        long df; // degreesOfFreedom is only unsigned short 16 bit integer, so might overflow,
        is >> df;  // but read into a temporary long int to do some checks.
        if (is.fail())
        { // long int not read OK.
          degreesOfFreedom = (std::numeric_limits<unsigned short int>::max)(); // 0xFFFF; = Not known.
          types &= ~(DEG_FREE_KNOWN | DEG_FREE_EXACT); // Clear these flags.
        }
        else
        {  // long degrees of freedom read OK, so check >=0, and not too big.
          if (df < 0)
          { // Can't have negative degrees of freedom!
            degreesOfFreedom = (std::numeric_limits<unsigned short int>::max)(); // 0xFFFF; = Not known.
            types &= ~(DEG_FREE_KNOWN | DEG_FREE_EXACT); // NOT defined.
          }
          else
          { // df >= 0
            degreesOfFreedom =
              (df >= (std::numeric_limits<unsigned short int>::max)()) ?
              (std::numeric_limits<unsigned short int>::max)() -1
              // max df allowed.
              : static_cast<unsigned short int>(df); // 1 observation == 1 degfree.
          }
          c =	is.peek();  // Expect ')'
          if (c == ')')
          {
            is.get();  // Discard ')'
            types |= (DEG_FREE_KNOWN | DEG_FREE_EXACT);  // deg_free IS known and integer so exact.
          }
          if (c != ')' || (is.fail()) )
          { // (99 OK but missing bracket.
            degreesOfFreedom = (std::numeric_limits<unsigned short int>::max)(); // 0xFFFF; = Not known.
            types &= ~(DEG_FREE_KNOWN | DEG_FREE_EXACT); // NOT defined.
          }
        }
      } // (99)
      else
      { // is EOF so NO (df)
        types |= DEG_FREE_KNOWN ;  // deg_free IS implicitly known == 0.
        // degreesOfFreedom = 0U;  // Default == 1 observation.
      }
    } // eof
  } // eof
#ifdef UNC_TRACE
  {
    std::cerr << "\n### unc_input:""\n""\t" "value "
      << value
      << ", stdDev " << stdDev
      << ", df " << dec << degreesOfFreedom << ", "
      << showUncTypes(types) << ", "
      << showUncFlags(static_cast<unsigned short>(is.iword(uncFlagsIndex)))
      << std::endl;
  } // trace
#endif
  // Restore is stream flags - if altered.
} // void unc_input(double&, double&, unsigned short&, unsigned short&, std::istream&);
