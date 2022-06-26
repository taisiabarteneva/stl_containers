#ifndef 	RANDOM_ACCESS_ITERATOR_HPP
# define	RANDOM_ACCESS_ITERATOR_HPP

# include "utils.hpp"

namespace ft
{

/* -------------------------- random access iterator ------------------------- */
template <class T>
class random_access_it : public ft::iterator<	typename ft::iterator_traits<T *>::iterator_category,
												typename ft::iterator_traits<T *>::value_type,
												typename ft::iterator_traits<T *>::difference_type,
												typename ft::iterator_traits<T *>::pointer,
												typename ft::iterator_traits<T *>::reference>
{
	public:

		typedef std::random_access_iterator_tag							iterator_category;	// std for unit tests
		typedef	typename ft::iterator_traits<T *>::value_type 			value_type;			// T
		typedef	typename ft::iterator_traits<T *>::difference_type 		difference_type;	// std::ptrdiff_t
		typedef typename ft::iterator_traits<T *>::pointer				pointer;			// T *
		typedef typename ft::iterator_traits<T *>::reference			reference;			// T &
		typedef typename ft::iterator_traits<T *>::pointer				iterator_type;		// T *
		typedef const reference											const_reference;	// const T &
		typedef	random_access_it<T>										self;

		random_access_it()
		: 	__iter(NULL) 
		{}

		random_access_it(iterator_type it)
		: 	__iter(it)
		{}

		random_access_it(const random_access_it & rhs)
		{
			*this = rhs;
		}
		random_access_it & operator=(const random_access_it & rhs)
		{
			if (this != &rhs)
				__iter = rhs.__iter;
			return *this;
		}

		~random_access_it()
		{}

		reference operator*() const
		{
			return *__iter;
		}

		pointer operator->() const
		{
			return __iter;
		}

		/* iterator addition, returns reference */
		self & operator+=(difference_type n)
		{
			// n > 0, n < 0, n == 0 
			__iter += n;
			return *this;
		}

		/* iterator addition, returns new object */
		self operator+(difference_type n) const
		{
			random_access_it tmp(__iter + n);
			return tmp;
		}

		/* iterator substruction, return reference */
		self & operator-=(difference_type n)
		{
			__iter += -n;
			return *this;
		}

		/* iterator substruction, returns new object */
		self operator-(difference_type n) const
		{
			random_access_it tmp(__iter - n);
			return tmp;
		}

		/* a - b */
		difference_type operator-(const random_access_it & rhs) const
		{
			return (__iter - rhs.__iter);
		}

		/* prefix */
		self & operator++()
		{
			++__iter;
			return *this;
		}
		
		self & operator--()
		{
			--__iter;
			return *this;
		}

		/* postfix */
		self operator++(int)
		{
			random_access_it tmp(*this);
			++__iter;
			return tmp;
		}
		self operator--(int)
		{
			random_access_it tmp(*this);
			--__iter;
			return tmp;
		}

		const_reference operator[](difference_type pos) const
		{
			return *(pos + __iter);
		}

		reference operator[](difference_type pos)
		{
			return *(pos + __iter);
		}
		
		bool operator!=(const random_access_it & rhs)
		{
			return __iter != rhs.__iter;
		}
		bool operator==(const random_access_it & rhs)
		{
			return __iter == rhs.__iter;
		}
		bool operator<(const random_access_it & rhs)
		{
			return __iter < rhs.__iter;
		}
		bool operator<=(const random_access_it & rhs)
		{
			return __iter <= rhs.__iter;
		}
		bool operator>(const random_access_it & rhs)
		{
			return __iter > rhs.__iter;
		}
		bool operator>=(const random_access_it & rhs)
		{
			return __iter >= rhs.__iter;
		}

	protected:
		iterator_type		__iter;
};

/* ---------------------- reverse random access iterator --------------------- */
template <class Iter>
class v_reverse_iter :	public ft::iterator<typename ft::iterator_traits<Iter>::iterator_category,
											typename ft::iterator_traits<Iter>::value_type,
											typename ft::iterator_traits<Iter>::difference_type,
											typename ft::iterator_traits<Iter>::pointer,
											typename ft::iterator_traits<Iter>::reference>
{
	public :
		typedef	Iter												iterator_type;
		typedef typename iterator_traits<Iter>::difference_type		difference_type;
		typedef typename iterator_traits<Iter>::pointer 			pointer;
		typedef typename iterator_traits<Iter>::reference 			reference;
		typedef v_reverse_iter<Iter>								self;
	
		v_reverse_iter()
		: __iter() {}

		v_reverse_iter(const v_reverse_iter & it)
		:	__iter(it.__iter)
		{}

		template <class It>
		v_reverse_iter(const v_reverse_iter<It> & it)
		: __iter(it.base())
		{}

		explicit v_reverse_iter(iterator_type __x)
		: __iter(__x)
		{}

		iterator_type base() const
		{
			return __iter;
		}

		reference operator*() const
		{
			Iter tmp = __iter;
			return *(--tmp);
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		self & operator++()
		{
			--__iter;
			return *this;
		}

		self operator++(int)
		{
			v_reverse_iter tmp = *this;
			--__iter;
			return tmp;
		}

		self & operator--()
		{
			++__iter;
			return *this;
		}

		self operator--(int)
		{
			v_reverse_iter tmp = *this;
			++__iter;
			return tmp;
		}

		self operator+(difference_type n) const
		{
			v_reverse_iter tmp(__iter - n);
			return tmp;
		}
		self operator-(difference_type n) const
		{
			v_reverse_iter tmp(__iter + n);
			return tmp;
		}
		self & operator+=(difference_type n)
		{
			__iter -= n;
			return *this;
		}
		self & operator-=(difference_type n)
		{
			__iter += n;
			return *this;
		}
		friend bool operator==(	const v_reverse_iter<Iter> & lhs,
								const v_reverse_iter<Iter> & rhs)
		{
			return lhs.base() == rhs.base();
		}

		friend bool operator!=(	const v_reverse_iter<Iter> & lhs,
								const v_reverse_iter<Iter> & rhs)
		{
			return lhs.base() != rhs.base();
		}

		protected :
			iterator_type __iter;
};

}

#endif