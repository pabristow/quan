// Boost.TypeErasure decor printer
//
// Copyright 2011 Steven Watanabe
// Copyright 2012 Paul A. Bristow
//
// Distributed under the Boost Software License Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// from \boost-sandbox\type_erasure\libs\type_erasure\example\print_sequence.cpp

#ifndef BOOST_TYPE_ERASURE_PRINTER
#define BOOST_TYPE_ERASURE_PRINTER

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/tuple.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>

#include<iostream>
#include <iomanip>

using namespace boost::type_erasure;

// Placeholders used by the abstract printer during specification of requirements.
struct _t : boost::type_erasure::placeholder {};
struct _iter : boost::type_erasure::placeholder {};
struct _os : boost::type_erasure::placeholder {};
// This is more readable than using the 'out of the box' _a, ... _g placeholders.
// See placeholder.hpp.

template<class T, class U = _self>
struct base_and_derived
{ // Template parameter for concept_interface.
  // and in list of requirements in abstract printer.
  // base_and_derived<std::ios_base, _os>, // _os must derive from std::ios_base.
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
};

// Fully specialized for const std::pair<const int, double>&
//std::ostream& operator<< (std::ostream& os, const std::pair<const int, double>& p)
//{ /*! Output a pair of values.
//     \details For example: "1.23 , 3.45".
//   */
//  os << p.first << ", " << p.second;
//  return os;
//} // std::ostream& operator<<

// But more useful is just providing the two template version:

template <typename T1, typename T2>
std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
{ /*! Output a pair of values with space as delimiter.
     \details For example: "1.23 3.45".
   */
  os << p.first << " " << p.second;
  return os;
} // std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)

// Or can use a version defined elsewhere, for example, in namespace my_detail?

//  using my_detail::operator<<;

// Partial specialization of struct ostreamable for pair<const int, double>:
//template<class Os>
//struct ostreamable<Os, std::pair<const int, double> >
//{
//    static void apply(Os& out, const std::pair<const int, double>& arg)
//    {
//      out << arg;
//   }
//};

// Partial specialization of struct ostreamable for pair<T1, T2>:
template<class Os, typename T1, typename T2>
struct ostreamable<Os, std::pair<T1, T2> >
{
    static void apply(Os& out, const std::pair<T1, T2>& arg)
    {
      out << arg;
   }
};

} // namespace type_erasure
} // namespace boost

/*! An abstract sequence printer - a 'template' that is inherited
 to implement the examples of actual printers defined below.
 */
 
template< class CharT = char, class Traits = std::char_traits<char> >
class abstract_printer
{
public:
    //! \tparam Range must be a Forward Range whose elements can be printed to `ostream os`.
    //! \param r Container to be printed, for example, a C array, std::vector, std::list ...
    //! \param os  `std::ostream` for printing, for example std::cout.
 
    template<class Range, class CharT, class Traits>
    void print(const Range& r, std::basic_ostream<CharT, Traits>& os = std::cout) const
    {
        // Capture the print arguments.
        // Range iterators.
        typename boost::range_iterator<const Range>::type
            first(boost::begin(r)),
            last(boost::end(r));
        // Assemble requirements into tuple: range pair of iterators and ostream.
        tuple<requirements, _iter, _iter, _os&> args(first, last, os);
        // and forward to the real implementation.
        do_print(get<0>(args), get<1>(args), get<2>(args));
        // get<0>() is first, get<1>() is last,  get<2>() is ostream.
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
    // Declare the pure virtual function `do_print` that is defined in all real printers.
    virtual void do_print(iterator_type first, iterator_type last, ostream_type os) const = 0;
}; // class abstract_printer

/*!
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
  \param term string to terminate the last row
*/
class decor_printer : public abstract_printer<>
{
public:
    explicit decor_printer(
      std::size_t num_columns = 0,
      std::size_t wid = 0,
      const std::string& pre = "\n",
      const std::string& sep = " ",
      const std::string& suf = "\n",
      const std::string& term = "\n")
      :
    cols(num_columns), width(wid), prefix(pre), separator(sep), terminator(term), suffix(suf)
    { /*! Constructor.\n
       Usage: for example: ``3, 10, "double testd[] = {\n    ", ", ", "\n    ", "\n  };\n"``
       3 columns with width 10, prefix "double testd[] = {\n    ", separator string comma space,
       suffix (newline at the end of a column),
       and a terminator string "\n  };\n".\n
       Defaults are provided for all parameters, so can contruct: `my_default_printer decor_printer;`\n
       that places all items on one line or row with space between items, and a final newline.
      */
    }
protected:
  // Print all items in rows.
    virtual void do_print(iterator_type first, iterator_type last, ostream_type os) const
    {
        os << prefix.c_str();
        std::size_t count = 0;
        for(; first != last; ++first)
        {
            if (width > 0)
            {
              static_cast<std::ios_base&>(os).width(width);
            }
            os << *first;
            boost::type_erasure::any<requirements, _iter> temp = first;
            ++temp;
            if(temp != last)
            {
              os << separator.c_str();
            }
            ++count;
            if((cols != 0) && (count % cols == 0))
            { // Last column.
                os << suffix.c_str(); // Usually newline at end of a row.
            }
        }
        os << terminator.c_str(); // Perhaps useful to have a terminating newline?
    }
private:
    std::size_t cols; // Set by constructor, number of columns (default 10).
    std::size_t width; // Set by constructor, optional column width (default zero).
    std::string prefix; // Set by constructor, for example, "{ ".
    std::string separator; // Set by constructor, for example, ", ".
    std::string suffix; // Set by constructor, for example "\n".
    std::string terminator; // Set by constructor, for example, "}\n".
}; // class decor_column_printer

#endif // ifndef BOOST_TYPE_ERASURE_PRINTER
