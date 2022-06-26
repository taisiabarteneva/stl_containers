#ifndef 	BIDIRECTIONAL_ITER_HPP
# define 	BIDIRECTIONAL_ITER_HPP

# include "utils.hpp"

namespace ft
{	
	template <class Iter, class T>
	class tree_iter : public iterator<	typename iterator_traits<Iter>::iterator_category,
										typename iterator_traits<Iter>::value_type,
										typename iterator_traits<Iter>::difference_type,
										typename iterator_traits<Iter>::pointer,
										typename iterator_traits<Iter>::reference>
	{
		public :
			/* typedef ------------------------------------------------------- */
			typedef Iter												iterator_type;
			typedef typename iterator_traits<T>::reference 				reference;
			typedef typename iterator_traits<T>::pointer				pointer;
			typedef typename iterator_traits<T>::difference_type		difference_type;
			typedef tree_iter<Iter, T>									self;

		private :
			iterator_type __it;

			iterator_type findMin(iterator_type currElem)
			{
				while (currElem && currElem->__left)
					currElem = currElem->__left;
				return currElem;
			}

			iterator_type findMax(iterator_type currElem)
			{
				while (currElem && currElem->__right)
					currElem = currElem->__right;
				return currElem;
			}

			iterator_type findPredessor(iterator_type elem)
			{
				iterator_type tmp = elem->__parent;
			
				if (elem->__left)
					return findMax(elem->__left);
				while (tmp && elem == tmp->__left)
				{
					elem = tmp;
					tmp = tmp->__parent;
				}
				return (tmp);
			}

			iterator_type findSuccessor(iterator_type elem)
			{
				iterator_type tmp = elem->__parent;
			
				if (elem->__right)
					return findMin(elem->__right);
				while (tmp && elem == tmp->__right)
				{
					elem = tmp;
					tmp = tmp->__parent;
				}
				return (tmp);
			}

		public :

			/* coplien ------------------------------------------------------- */
			tree_iter()
			: __it() {}

			tree_iter(iterator_type value)
			: __it(value) {}

			template <typename It, typename I>
			tree_iter(const tree_iter<It, I> & rhs)
			: __it(rhs.base()) {}

			template <typename It, typename I>		
			tree_iter &operator=(const tree_iter<It, I> & rhs)
			{
				if (this != &rhs)
					__it = rhs.base();
				return *this;
			}
			~tree_iter() {}

			/* public functions ---------------------------------------------- */
			iterator_type base() const
			{
				return __it;
			}

			reference operator*() const
			{
				return __it->_value;
			}
			pointer operator->() const
			{
				return &(__it->_value);
			}
			self operator++(int)
			{
				tree_iter tmp = *this;
				__it = findSuccessor(__it);
				return tmp;
			}
			self operator--(int)
			{
				tree_iter tmp = *this;
				__it = findPredessor(__it);
				return tmp;
			}
			self & operator++()
			{
				__it = findSuccessor(__it);
				return *this;
			}
			self & operator--()
			{
				__it = findPredessor(__it);
				return *this;
			}
			
			friend bool operator==(	const self & rhs, 
									const self & lhs)
			{
				return rhs.__it == lhs.__it;
			}

			friend bool operator!=(	const self & rhs, 
									const self & lhs)
			{
				return rhs.__it != lhs.__it;
			}
	};

	template <class Iter>
	class reverse_iter : 	public iterator<	typename iterator_traits<Iter>::iterator_category,
												typename iterator_traits<Iter>::value_type,
												typename iterator_traits<Iter>::difference_type,
												typename iterator_traits<Iter>::pointer,
												typename iterator_traits<Iter>::reference>
	{
		public :
			typedef	Iter										iterator_type;
			typedef std::ptrdiff_t								difference_type;
			typedef typename iterator_traits<Iter>::pointer 	pointer;
			typedef typename iterator_traits<Iter>::reference 	reference;
			typedef	reverse_iter<Iter>							self;
		
		protected :
			iterator_type __current;
		
		public :
			reverse_iter()
			: __current() {}

			reverse_iter(const reverse_iter & it)
			:	__current(it.__current)
			{}

			template <class It>
			reverse_iter(const reverse_iter<It> & it)
			: __current(it.base())
			{}

			/* this iterator will move in the opposite direction that x does */
			explicit reverse_iter(iterator_type __x)
			: __current(__x) {}

			iterator_type base() const
			{
				return __current;
			}

			reference operator*() const
			{
				Iter tmp = __current;
				return *(--tmp);
			}

			pointer operator->() const
			{
				return &(operator*());
			}

			self & operator++()
			{
				--__current;
				return *this;
			}

			self operator++(int)
			{
				reverse_iter tmp = *this;
				--__current;
				return tmp;
			}

			self & operator--()
			{
				++__current;
				return *this;
			}

			self operator--(int)
			{
				reverse_iter tmp = *this;
				++__current;
				return tmp;
			}

			friend bool operator==(	const reverse_iter<Iter> & lhs,
									const reverse_iter<Iter> & rhs)
			{
				return lhs.base() == rhs.base();
			}

			friend bool operator!=(	const reverse_iter<Iter> & lhs,
									const reverse_iter<Iter> & rhs)
			{
				return lhs.base() != rhs.base();
			}
	};
} 		// end of ft namespace

#endif