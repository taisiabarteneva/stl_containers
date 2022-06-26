#ifndef		ITERATOR_TRAITS_HPP
# define 	ITERATOR_TRAITS_HPP

# include <iterator>

namespace ft
{

/* --------------------------- iterators hierarchy --------------------------- */
struct input_iterator_tag
{};
struct output_iterator_tag
{};
struct forward_iterator_tag       : public input_iterator_tag
{};
struct bidirectional_iterator_tag : public forward_iterator_tag
{};
struct random_access_iterator_tag : public bidirectional_iterator_tag
{};


/* ------------------------- iterator template class ------------------------- */
template <class Category, class T, class Distance = ptrdiff_t, 
										class Pointer = T*, class Reference = T&>
struct iterator
{
	/* the type pointed to by iterator */
    typedef T         value_type;
	/* distance between iterators */
    typedef Distance  difference_type;
	/* this type represents a pointer-to-value_type */
    typedef Pointer   pointer;
	/* this type represents a reference-to-value_type */
    typedef Reference reference;
	/* one of the iterator's tags */
    typedef Category  iterator_category;
};

/* ---------------------------- iterator traits ------------------------------ */
template <class Iterator>
struct iterator_traits
{
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
	typedef typename Iterator::iterator_category	iterator_category;
};

/* 
	the standard library provides partial specializations for pointer types T *, 
	which makes it possible to use all iterator-based algorithms with raw pointers 
*/
template <class T>
struct iterator_traits<T *>
{
	typedef std::ptrdiff_t 					difference_type;
	typedef T								value_type;
	typedef T *								pointer;
	typedef T &								reference; 
	typedef ft::random_access_iterator_tag	iterator_category;
};

template <class T>
struct iterator_traits<const T *>
{
	typedef std::ptrdiff_t 					difference_type;
	typedef const T							value_type;
	typedef const T *						pointer;
	typedef const T &						reference; 
	typedef ft::random_access_iterator_tag	iterator_category;
};

/* ------------------------ lexicographical compare -------------------------- */
/*  
	returns true if the range [first1,last1) compares lexicographically 
	less than the range [first2,last2)
*/

template < class InputIt1, class InputIt2 >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2)
{
	 while (first1 != last1)
  {
    if (first2 == last2 || *first2 < *first1) return false;
    else if (*first1 < *first2) return true;
    ++first1; ++first2;
  }
  return (first2 != last2);
}

template < class InputIt1, class InputIt2, class Compare >
bool lexicographical_compare( InputIt1 first1, InputIt1 last1,
                              InputIt2 first2, InputIt2 last2,
                              Compare comp)
{
	for (; first1 != last1 && first2 != last2; ++first1, (void) ++first2)
	{
		if (comp(*first1, *first2) == true)
			return true;
		if (comp(*first2, *first1) == true)
			return false;
	}
	return (first1 == last1) && (first2 != last2);
}
/* ---------------------------------- equal ---------------------------------- */
template < class InputIt1, class InputIt2 >
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
    for (; first1 != last1; ++first1, (void) ++first2) {
        if (*first1 != *first2) {
            return false;
        }
    }
    return true;
}

/* ---------------------------------- swap ----------------------------------- */

template <class T>
void swap(T & a, T & b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

/* ---------------------------------- distance ------------------------------- */
template <class It>
typename ft::iterator_traits<It>::difference_type 
			do_distance(It first, It last)
{
    typename ft::iterator_traits<It>::difference_type result = 0;
    while (first != last) 
	{
        ++first;
        ++result;
    }
    return result;
}

}

#endif