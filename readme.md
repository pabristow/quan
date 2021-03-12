Warning - site under active development!  Master branch contains obselete versions and is not recommended.

C++ Classes to handle values that have information about their uncertainty.
This library provides a collection of classes and functions for handling uncertain reals.

Uncertainty as standard deviation and degrees of freedom are propagated through calculations using the uncertain type, and are input and output including the uncertainty estimates.

The number of decimal digits output reflect the uncertainty of the value, so insignificant trailing digits do not clutter the display.  This is used, for example, by the SVG_plot program to label data points neatly.

The uncertainty can be explicitly provided or can be implicit from the number of decimal digits from the input.

Many options for input and to display information about uncertainty using std::iostreams are provided. 

Author: Paul A. Bristow

[Boost license](http://www.boost.org/LICENSE_1_0.txt)
