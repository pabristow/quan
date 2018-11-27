// Boost.TypeErasure A decor printer.
//
// Copyright 2011 Steven Watanabe
// Copyright 2012 Paul A. Bristow
//
// Distributed under the Boost Software License Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Based on (but with major modifications):
// Copy of <libs/type_erasure/example/print_sequence.cpp>  Boost Release 1.69 Nov 2018
// https://www.boost.org/doc/libs/release/doc/html/boost_typeerasure.html
// A polymorphic range formatter
// https://www.boost.org/doc/libs/release/doc/html/boost_typeerasure/examples.html#boost_typeerasure.examples.print_sequence
// print_sequence.cpp
// https://www.boost.org/doc/libs/release/libs/type_erasure/example/print_sequence.cpp

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/tuple.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>

#include <iostream>
#include <iomanip>
#include <vector>

#ifndef BOOST_TYPE_ERASURE_DECOR_PRINTERS
#define BOOST_TYPE_ERASURE_DECOR_PRINTERS

using namespace boost::type_erasure;

struct _t : placeholder {};
struct _iter : placeholder {};
struct _os : placeholder {};

template<class T, class U = _self>
struct base_and_derived
{
  static T& apply(U& arg) { return arg; }
};

namespace boost {
  namespace type_erasure {

    template<class T, class U, class Base>
    struct concept_interface<base_and_derived<T, U>, Base, U> : Base
    {
      operator typename rebind_any<Base, const T&>::type() const
      {
        return call(base_and_derived<T, U>(), const_cast<concept_interface&>(*this));
      }
      operator typename rebind_any<Base, T&>::type()
      {
        return call(base_and_derived<T, U>(), *this);
      }
    }; // concept_interface
  }  // namespace type_erasure
} // namespace boost

/*!
  abstract_printer - An abstract base class for formatting sequences.
  A 'template' that is inherited to implement the examples of actual printers defined below.

  //! \tparam Range must be a Forward Range whose elements can be printed to `ostream os`.
  //! \param r Container to be printed, for example, a C array, std::vector, std::list ...
  //! \param os  `std::ostream` for printing, for example @c std::cout.

  This version reverses the order of arguments to the @c print function,
  putting the range to be printed first,
  and the ostream last so that a default @c std::cout can be used.
  \code
    line_printer.print(da, std::cout);
    line_printer.print(da);
  \endcode
  The (added) template arguments for abstract_printer do not seem to be used,
  and cause errors using GCC
    "declaration of template parameter 'CharT' shadows template parameter"
  if the same names CharT and Traits are used in Print function.

  No need for a concrete printer inheriting from abstract_print to append <>
  so can write
  \code
    decor_printer line_printer;
  \endcode

  Outputs items in a specified number of columns across rows with a separator (often comma and/or space(s)),
  and a suffix (usually newline) every `num_column`s items.\n
  Usage: `decor_printer simple_printer(3, 0, "\n", ", ", "\n", "\n");`\n
  Provide a container to be printed: `double da[] = {1., 2., 3., 4., 5., 6.};`\n
  Print to std::cout using: `simple_printer.print(std::cout, da);` \n
  Output:\n
  1, 2, 3,\n
  4, 5, 6,\n
  7, 8, 9,\n
  10, 11, 12\n
  The order of parameters is chosen to try to allow use of the defaults as much as possible,
  including all defaults placing all items on one line or row separated by spaces.

  \param num_columns number of columns (default 0, so all items are on the same line or row).
  \param width ostream width to use to each items (default 0 so that columns may be jagged).
  \param pre string to be output before the first column,
  (default newline which ensures that the first item is at the left margin).
  \param sep string to separate (or delimit) items on each row (default space).
  \param suf suffix at the end of each row (default newline).
  \param term string to terminate the last row.
*/

class abstract_printer
{
public:
    template<class Range, class CharT = char, class Traits = std::char_traits<char> >
    const abstract_printer& print(const Range& r, std::basic_ostream<CharT, Traits>& os = std::cout) const
    {
        // Capture the print arguments, Range of items to print, and ostream where to output.
        // Range iterators.
        typename boost::range_iterator<const Range>::type
            first(boost::begin(r)),
            last(boost::end(r));
        // Assemble requirements into tuple: range pair of iterators (first argument),
        // and ostream (last argument so that it can have a default of std::cout).
        tuple<requirements, _iter, _iter, _os&> args(first, last, os);
        // and forward to the real implementation.
        do_print(get<0>(args), get<1>(args), get<2>(args));
        // (get<0>() is first, get<1>() is last,  get<2>() is ostream in SW version.)
        // get<0>() is ostream,  get<1>() is range.first,   get<2>() is range.last,
        return *this; // function must return const abstract_printer& because *this is const.
    }
    virtual ~abstract_printer() {}
protected:
  // Define the requirements in an mpl::vector for containers to be printed.
    typedef boost::mpl::vector<
        base_and_derived<std::ios_base, _os>, // _os must derive from std::ios_base.
        ostreamable<_os, _t>, // type _t must be ostreamable.
        ostreamable<_os, const char*>, // C string must be ostreamable.
        forward_iterator<_iter, const _t&>, // t range must have forward iterator.
        same_type<_t, forward_iterator<_iter, const _t&>::value_type> // _t's type value and iterator must match.
    > requirements;

    // Use type_erasure::any to enforce the requirements onto ostream and iterator.
    typedef boost::type_erasure::any<requirements, _os&> ostream_type;
    typedef boost::type_erasure::any<requirements, _iter> iterator_type;
    // Declare the pure virtual function `do_print` that is defined in all real printers,
    // for example, see decor_printer below.
    virtual const abstract_printer& do_print(iterator_type first, iterator_type last, ostream_type os) const = 0;
}; // class abstract_printer


/*!
 separator_printer - writes the elements of a sequence
   separated by a fixed string.  For example, if
   the separator is ", " separator_printer produces
   a comma separated list.
*/

class separator_printer : public abstract_printer
{
public:
  explicit separator_printer(const std::string& sep = " ", const int n = 1) // Default single space.
    : separator_(sep), count_(n) {}

  separator_printer& count(const int n)
  { // 
    count_ = n;
    return *this; // Make chainable.
  }

  separator_printer& width(const int n)
  {
    width_ = n;
    return *this; // Make chainable.
  }
protected:
  virtual const separator_printer& do_print(
   iterator_type first, iterator_type last, ostream_type os = std::cout) const
  {
    if(first != last)
    {
      os << *first;
      ++first;
      for(; first != last; ++first)
      {
        int c = count_;
        while (c > 0)
        {
          os << separator_.c_str();
          c--;
        }
        os << *first;
      }
    }
    return *this; // Make chainable.
  }

  //virtual const separator_printer& do_set_count(const int count)
  //{
  //  count_ = count;
  //  return *this; // Make chainable.
  //}

  //virtual const separator_printer& do_set_width(const int width)
  //{
  //  width_ = width;
  //  return *this; // Make chainable.
  //}

private:
  std::string separator_;
  int count_;  // 
  int width_;
}; //  class separator_printer

//
//
//// Fully specialized for const std::pair<const int, double>&
////std::ostream& operator<< (std::ostream& os, const std::pair<const int, double>& p)
////{ /*! Output a pair of values.
////     \details For example: "1.23 , 3.45".
////   */
////  os << p.first << ", " << p.second;
////  return os;
////} // std::ostream& operator<<
//
//// But more useful is just providing the two template version:
//
//template <typename T1, typename T2>
//std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
//{ /*! Output a pair of values with space as delimiter.
//     \details For example: "1.23 3.45".
//   */
//  os << p.first << " " << p.second;
//  return os;
//} // std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
//
//// Or can use a version defined elsewhere, for example, in namespace my_detail?
//
////  using my_detail::operator<<;
//
//// Partial specialization of struct ostreamable for pair<const int, double>:
////template<class Os>
////struct ostreamable<Os, std::pair<const int, double> >
////{
////    static void apply(Os& out, const std::pair<const int, double>& arg)
////    {
////      out << arg;
////   }
////};
//
//// Partial specialization of struct ostreamable for pair<T1, T2>:
//template<class Os, typename T1, typename T2>
//struct ostreamable<Os, std::pair<T1, T2> >
//{
//    static void apply(Os& out, const std::pair<T1, T2>& arg)
//    {
//      out << arg;
//   }
//};


#endif // ifndef BOOST_TYPE_ERASURE_DECOR_PRINTERS
