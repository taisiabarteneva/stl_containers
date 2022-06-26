#ifndef		VECTOR_HPP
# define	VECTOR_HPP

# include <memory> 				/* std::allocator */
# include <algorithm> 			/* std::swap */
# include "../utils/utils.hpp"
# include "../utils/enable_if.hpp"
# include "../utils/random_access_iterator.hpp"

namespace ft
{

template<
    class T,
    class Allocator = std::allocator<T>
>
class vector
{
	public:
		/* --------------------------- member types -------------------------- */
		typedef T 											value_type;
		typedef Allocator 									allocator_type;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference 	const_reference;
		typedef typename allocator_type::size_type			size_type; 				/* typedef size_t size_type */
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer 		const_pointer;
		typedef typename allocator_type::difference_type	difference_type; 		/* typedef ptrdiff_t difference_type */
		
		/* ---------------------------- iterators ---------------------------- */
		typedef random_access_it<T>						iterator;
		typedef random_access_it<const T>				const_iterator;
		typedef v_reverse_iter<iterator>				reverse_iterator;
		typedef v_reverse_iter<const_iterator>			const_reverse_iterator;
		/* ---------------------------- constructors ------------------------- */
		vector();
		explicit vector(const allocator_type & alloc);
		explicit vector(size_type count, 	const_reference value = value_type(),
											const allocator_type & alloc = allocator_type());
		template <class InputIt>
		vector(typename enable_if<is_iterator<InputIt>::value, InputIt>::type first, 
							InputIt last, const allocator_type & alloc = allocator_type());
		vector(const vector & other);
		vector &operator=(const vector & other);
		void assign(size_type n, const_reference value);
		
		template <class InputIt>
		void assign(typename enable_if<is_iterator<InputIt>::value, InputIt>::type first, 
					InputIt last);

		/* ---------------------------- destructors -------------------------- */
		~vector();

		/* ---------------------------- iterators ---------------------------- */
		iterator 				begin();
		const_iterator 			begin() const;
		iterator 				end();
		const_iterator 			end() 	const;

		/* ------------------------ reverse iterators ------------------------ */
		reverse_iterator 		rbegin();
		const_reverse_iterator 	rbegin() 	const;
		reverse_iterator 		rend();
		const_reverse_iterator 	rend() 		const;

		/* ------------------------- element access -------------------------- */
		reference at(size_type pos);
		const_reference at(size_type pos) const;
		reference operator[](size_type pos);
		const_reference operator[](size_type pos) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;

		/* ---------------------------- capacity ----------------------------- */
		bool empty() const;
		size_type size() const;
		size_type max_size() const;
		void resize(size_type count, value_type value = T());
		size_type capacity() const;
		void reserve(size_type n);

		/* ----------------------------- modifiers ---------------------------- */
		void clear();
		iterator insert(iterator pos, const value_type & value);
		void insert(iterator pos, size_type count, const value_type & value);

		template < class InputIt >
		void insert(iterator pos, typename enable_if<is_iterator<InputIt>::value, 
									InputIt>::type first, InputIt last);
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		void push_back(const value_type & value);
		void pop_back();
		void swap(vector<T, allocator_type> & rhs);

		/* ----------------------------- allocator ---------------------------- */
		allocator_type get_allocator() const;

		private:
			allocator_type 	__alloc; 		/* allocator */
			size_t			__size;
			size_type 		__capacity; 	/* the total number of elements that the vector can hold without requiring reallocation */
			pointer			__arr; 			/* pointer to the first element */
};

template <class T, class Alloc>
vector<T, Alloc>::vector()
:	__alloc(allocator_type()),
	__size(0),
	__capacity(__size),
	__arr(NULL)
{}

template <class T, class Alloc>
vector<T, Alloc>::vector(const allocator_type & alloc)
: 	__alloc(alloc),
	__size(0),
	__capacity(0),
	__arr(NULL)
{}

template <class T, class Alloc>
vector<T, Alloc>::vector(	size_type count,
							const_reference value,
							const allocator_type& alloc)
: 	__alloc(alloc),
	__size(count),
	__capacity(count)
{
	size_type	i;

	__arr = __alloc.allocate(__capacity);
	/* strong exception guarantee */
	try
	{
		for (size_type i = 0; i < __size; ++i)
			__alloc.construct(&__arr[i], value);   
	}
	catch (...)
	{
		for (; i > 0; --i)
			__alloc.destroy(&__arr[i - 1]);
		__alloc.deallocate(__arr, __capacity);
		throw  ;
	}	
}

template <class T, class Alloc>
template <class InputIt>
vector<T, Alloc>::vector(	typename enable_if<is_iterator<InputIt>::value, 
											InputIt>::type first, 
											InputIt last, 
											const allocator_type & alloc)
:   __alloc(alloc)
{
	size_type 	i;
	InputIt 	tmp(first);

	__size = 0;
	while (*tmp != *last)
	{
		tmp++;
		__size++;
	}
	__capacity = __size;
	__arr = __alloc.allocate(__capacity);
	try
	{
		for (i = 0; *first != *last; ++i, first++)
			__alloc.construct(&__arr[i], *first);
	}
	catch	(...)
	{
		for (; i > 0; --i)
			__alloc.destroy(&__arr[i - 1]);
		__alloc.deallocate(__arr, __capacity);
		throw  ;
	}
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector &other)
:	__alloc(other.__alloc),
	__size(other.__size),
	__capacity(other.__capacity)
{
	if (__capacity)
		__arr = __alloc.allocate(__capacity);
	size_type i;
	/* strong exception guarantee */
	try
	{
		for (i = 0; i < __size; ++i)
			__alloc.construct(&__arr[i], other.__arr[i]);
	}
	catch(...)
	{
		for (; i > 0; --i)
			__alloc.destroy(&__arr[i - 1]);
		__alloc.deallocate(__arr, __capacity);
		throw  ;
	}
}

template <class T, class Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &other)
{
	if (this == &other)
		return *this;

	T *tmp;
	size_type i;

	/* may throw std::bad__alloc and exit */
	tmp = __alloc.allocate(other.__capacity); 
	/* strong exception guarantee */
	try
	{
		for (i = 0; i < other.__size; ++i)
			__alloc.construct(&tmp[i], other.__arr[i]);
	} 
	catch (...)
	{
		for (; i > 0; --i)
			__alloc.destroy(&tmp[i - 1]);
		__alloc.deallocate(tmp, other.__capacity);
		throw  ;
	}
	for (size_type i = 0; i < __size; ++i)
		__alloc.destroy(&__arr[i]);
	if (__capacity)
		__alloc.deallocate(__arr, __capacity);
	__size = other.__size;
	__capacity = other.__capacity;
	__arr = tmp;
	return *this;
}

template <class T, class Alloc>
vector<T, Alloc>::~vector()
{
	clear();
	if (__capacity)
		__alloc.deallocate(__arr, __capacity);
}

template <class T, class Alloc>
void vector<T, Alloc>::assign(size_type n, const_reference value)
{
	T 			*tmp;
	size_type 	i;

	if (n > __capacity)
		tmp = __alloc.allocate(n);
	try
	{
		/* strong exception guarantee */
		for (i = 0; i < n; ++i)
			__alloc.construct(&tmp[i], value);
	}
	catch	(...)
	{
		for (; i > 0; --i)
			__alloc.destroy(&tmp[i - 1]);
		__alloc.deallocate(tmp, n);
		throw ;
	}
	for (i = 0; i < __size; ++i)
		__alloc.destroy(&__arr[i]);
	__alloc.deallocate(__arr, __capacity);
	__size = n;
	if (n > __capacity)
		__capacity = __size;
	__arr = tmp;
}

template <class T, class Alloc> 
template <class InputIt>
void vector<T, Alloc>::assign(typename enable_if<is_iterator<InputIt>::value, InputIt>::type first, 
						InputIt last)
{
	size_type n = 0;
	InputIt t(first);
	T 			*tmp;
	size_type 	i;

	while (t != last)
	{
		t++;
		n++;
	}

	if (n > __capacity)
		tmp = __alloc.allocate(n);
	else
		tmp = __alloc.allocate(__capacity);
	try
	{
		for (i = 0; i < n; ++i, first++)
			__alloc.construct(&tmp[i], *first);
	}
	catch	(...)
	{
		for (; i > 0; --i)
			__alloc.destroy(&tmp[i - 1]);
		__alloc.deallocate(tmp, n);
		throw  ;
	}
	for (i = 0; i < __size; ++i)
		__alloc.destroy(&__arr[i]);
	__alloc.deallocate(__arr, __capacity);
	__size = n;
	if (n > __capacity)
		__capacity = __size;
	__arr = tmp;
}

/* --------------------------------------------------------------------------- */
template <class T, class Alloc> 
typename vector<T, Alloc>::iterator vector<T, Alloc>::begin()
{
	return iterator(__arr);
}

template <class T, class Alloc> 
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::begin() const
{
	return const_iterator(__arr);
}

template <class T, class Alloc> 
typename vector<T, Alloc>::iterator vector<T, Alloc>::end()
{
	return iterator(__arr + __size);
}

template <class T, class Alloc> 
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::end() const
{
	return const_iterator(__arr + __size);
}

/* --------------------------------------------------------------------------- */
template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator  vector<T, Alloc>::rbegin()
{
	return reverse_iterator(this->end());
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator 	vector<T, Alloc>::rbegin() const
{
	return const_reverse_iterator(this->end());
}

template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator 	vector<T, Alloc>::rend()
{
	return reverse_iterator(this->begin());
}

template <class T, class Alloc> 
typename vector<T, Alloc>::const_reverse_iterator 	vector<T, Alloc>::rend() const
{
	return const_reverse_iterator(this->begin());
}

/* --------------------------------------------------------------------------- */
template <class T, class Alloc>
typename vector<T, Alloc>::reference 
		vector<T, Alloc>::at(typename vector<T, Alloc>::size_type pos)
{
	if (!(pos < __size))
		throw std::out_of_range("Index out of range");
	return __arr[pos];
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference 
		vector<T, Alloc>::at(typename vector<T, Alloc>::size_type pos) const
{
	if (!(pos < __size))
		throw std::out_of_range("Index out of range");
	return __arr[pos];
}

/* accessing a nonexistent element through operator[] is undefined behavior */
template <class T, class Alloc>
typename vector<T, Alloc>::reference 
		vector<T, Alloc>::operator[](typename vector<T, Alloc>::size_type pos)
{
	if (!(pos < __size))
		throw std::out_of_range("Index out of range");
	return __arr[pos];
}

template <class T, class Alloc> 
typename vector<T, Alloc>::const_reference 
		vector<T, Alloc>::operator[](typename vector<T, Alloc>::size_type pos) const
{
	if (!(pos < __size))
		throw std::out_of_range("Index out of range");
	return __arr[pos];
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::front()
{
	return __arr[0];
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::front() const
{
	return __arr[0];
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::back()
{
	return __arr[__size - 1];
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::back() const
{
	return __arr[__size - 1];
}

/* --------------------------------------------------------------------------- */
template <class T, class Alloc> 
bool vector<T, Alloc>::empty() const
{
	return __size == 0;
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const
{
	return end() - begin();
}

/* returns the maximum number of elements the container is able to hold due to system */
template <class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::max_size() const
{
	return __alloc.max_size();
}

template <class T, class Alloc>
void vector<T, Alloc>::resize(typename vector<T, Alloc>::size_type count, T value)
{
	size_type tmp;
	size_type i;

	if (__size > count)
	{
		for (i = count; i < __size; ++i)
			__alloc.destroy(&__arr[i]);
		__size = count;
	}
	else
	{
		try
		{
			tmp = __size;
			for (i = __size; i < count; ++i)
				push_back(value);
		}
		catch	(...)
		{
			for (; i > __size; --i)
				__alloc.destroy(&__arr[i - 1]);
			__size = tmp;
			throw ;
		}
	}
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() const
{
	return __capacity;
}

/* increases the capacity of the vector to a value that's greater or equal to new_cap 
if new_cap is greater than the current capacity(), new storage is allocated, otherwise the function does nothing */
template <class T, class Alloc>
void vector<T, Alloc>::reserve(typename vector<T, Alloc>::size_type n)
{
	if (n > this->max_size())
		throw std::length_error("Unable to allocate");
	if (n <= __capacity)
		return ;

	T			*tmp;
	size_type 	i;

	tmp = __alloc.allocate(n);
	try
	{
		for (i = 0; i < __size; ++i)
			__alloc.construct(&tmp[i], __arr[i]);
	}
	catch	(...)
	{
		for (; i > 0; --i)
			__alloc.destroy(&tmp[i - 1]);
		__alloc.deallocate(tmp, n);
		throw  ;
	}
	
	for (size_type i = 0; i < __size; ++i)
		__alloc.destroy(&__arr[i]);
	__alloc.deallocate(__arr, __capacity);
	
	__capacity = n;
	__arr = tmp;
}

/* --------------------------------------------------------------------------- */

/* Erases all elements from the container. After this call, size() returns zero */
template <class T, class Alloc>
void vector<T, Alloc>::clear()
{
	for (unsigned int i = 0; i < __size; ++i)
		__alloc.destroy(&__arr[i]);
	__size = 0;
}

/* 
	inserts value before pos
	causes reallocation if the new size() is greater than the old capacity(). 
	if the new size() is greater than capacity(), all iterators and references are invalidated. 
	otherwise, only the iterators and references before the insertion point remain valid. 
	the past-the-end iterator is also invalidated
*/
template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(typename vector<T, Alloc>::iterator pos, 
								const typename vector<T, Alloc>::value_type & value)
{
	size_type 	i, new_cap;
	T 			*new__arr;

	size_type idx = pos - begin();
	if (__size + 1 > __capacity)
	{
		new_cap = __capacity * 2;
		new__arr = __alloc.allocate(new_cap);
		for (i = 0; i < idx; ++i)
		{
			try
			{
				__alloc.construct(&new__arr[i], __arr[i]);
			}
			catch (...)
			{
				for (; i > 0; --i)
					__alloc.destroy(&new__arr[i]);
				__alloc.deallocate(new__arr, new_cap);
				throw ;
			}
			__alloc.destroy(&__arr[i]);
		}
		try
		{
			__alloc.construct(&new__arr[i], value);
		}
		catch (...)
		{
			throw ;
		}
		for (; i < __size; i++)
		{
			try
			{
				__alloc.construct(&new__arr[i + 1], __arr[i]);
			}
			catch (...)
			{
				for (; i > 0; --i)
					__alloc.destroy(&new__arr[i - 1]);
				__alloc.deallocate(new__arr, new_cap);
			}
			__alloc.destroy(&__arr[i]);
		}
		__alloc.deallocate(__arr, __capacity);
		__capacity *= 2;
		__size++;
		__arr = new__arr;
	}
	else
	{
		size_type x =__size;
		for (; x != idx; x--)
		{
			try
			{
				__alloc.construct(&__arr[x], __arr[x - 1]);
			}
			catch (...)
			{
				for (; x < __size; ++x)
					__alloc.destroy(&__arr[x]);
				throw ;
			}
			__alloc.destroy(&__arr[x - 1]);
		}
		try
		{
			__alloc.construct(&__arr[x], value);
		}
		catch (...)
		{
			throw ;
		}
		__size++;
	}
	return __arr + idx;
}

/* inserts count copies of the value before pos */
template <class T, class Alloc>
void vector<T, Alloc>::insert(	typename vector<T, Alloc>::iterator pos, 
								typename vector<T, Alloc>::size_type count, 
								const typename vector<T, Alloc>::value_type& value)
{
	if (count == 0)
		return ;

	T 			*tmp;
	size_type 	new_cap, idx, tmp_idx;
	size_type 	i, j;
	idx = pos - begin();
	
	if (__size + count > __capacity)
	{
		new_cap = __capacity * 2 >= __size + count ? __capacity * 2 : __size + count;

		tmp = __alloc.allocate(new_cap);
		try
		{
			for (i = 0; i < idx; ++i)
				__alloc.construct(&tmp[i], __arr[i]);
		}
		catch (...)
		{
			for (; i > 0; --i)
				__alloc.destroy(&tmp[i - 1]);
			__alloc.deallocate(tmp, new_cap);
			throw ;
		}
		try
		{
			for (j = 0; j < count; ++j, ++i)
				__alloc.construct(&tmp[i], value);
		}
		catch (...)
		{
			for (; i > 0; --i)
				__alloc.destroy(&tmp[i - 1]);
			__alloc.deallocate(tmp, new_cap);
			throw ;
		}
		try
		{
			for (; idx < __size; ++idx, ++i)
				__alloc.construct(&tmp[i], __arr[idx]);
		}
		catch (...)
		{
			for (; i > 0; i--)
				__alloc.destroy(&tmp[i - 1]);
			__alloc.deallocate(tmp, new_cap);
			throw ;
		}
		for (i = 0; i < __size; ++i)
			__alloc.destroy(&__arr[i]);
		__alloc.deallocate(__arr, __capacity);
		__capacity = new_cap;
		__size += count;
		__arr = tmp;
		return ;
	}
	else
	{
		try
		{
			for (i = size() - 1; i >= idx; --i) 
				__alloc.construct(&__arr[i + count], __arr[i]);
		}
		catch (...)
		{
			for (; idx < i; ++idx)
				__alloc.destroy(&__arr[idx]);
			throw ;
		}
		try 
		{
			tmp_idx = idx;
			for (i = 0; i < count; ++i, ++tmp_idx) 
				__alloc.construct(&__arr[tmp_idx], value);
		} 
		catch (...)
		{
			for (; tmp_idx > idx; --tmp_idx)
				__alloc.destroy(&__arr[tmp_idx - 1]);
			throw ;
		}
		__size += count;
	}
}

template <class T, class Alloc>
template <class InputIt>
void vector<T, Alloc>::insert(vector<T, Alloc>::iterator pos,
						typename enable_if<is_iterator<InputIt>::value, InputIt>::type first,
						InputIt last)
{
	size_type count = ft::do_distance(first, last);
	size_type new_cap, idx, i, tmp_idx;
	T *tmp;

	idx = pos - begin();
	tmp_idx = idx;
	if (__size + count > __capacity)
	{
		new_cap = __capacity * 2 >= __size + count ? __capacity * 2 : __size + count;
		tmp = __alloc.allocate(new_cap);
		try
		{
			for (i = 0; i < idx; ++i)
				__alloc.construct(&tmp[i], __arr[i]);
		}
		catch (...)
		{
			for (; i > 0; --i)
				__alloc.destroy(&tmp[i - 1]);
			__alloc.deallocate(tmp, new_cap);
			throw ;
		}
		try
		{
			for (; first < last; ++first, ++i)
				__alloc.construct(&tmp[i], *first);
		}
		catch (...)
		{
			for (; i > 0; --i)
				__alloc.destroy(&tmp[i - 1]);
			__alloc.deallocate(tmp, new_cap);
			throw ;
		}
		try
		{
			for (; idx < __size; ++idx, ++i)
				__alloc.construct(&tmp[i], __arr[idx]);
		}
		catch (...)
		{
			for (; i > 0; --i)
				__alloc.destroy(&tmp[i - 1]);
			__alloc.deallocate(tmp, new_cap);
			throw ;
		}
		for (i = 0; i < __size; ++i)
			__alloc.destroy(&__arr[i]);
		__alloc.deallocate(__arr, __capacity);
		__capacity = new_cap;
		__size += count;
		__arr = tmp;
	}
	else
	{

		for (i = size() - 1; i >= idx; --i)
		{
			try
			{
				__alloc.construct(&__arr[i + count], __arr[i]);
			}
			catch (...) /* handle any type of exception */
			{
				for (; idx < i; ++idx)
					__alloc.destroy(&__arr[idx]);
				throw ;
			}
			__alloc.destroy(&__arr[i]);
		}
		try 
		{
			for (i = 0; i < count; ++i, ++tmp_idx, first++) 
				__alloc.construct(&__arr[tmp_idx], *first);
		} 
		catch (...)
		{
			for (; tmp_idx > idx; --tmp_idx)
				__alloc.destroy(&__arr[tmp_idx - 1]);
			throw ;
		}
		__size += count;
	}
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(typename vector<T, Alloc>::iterator pos)
{
	size_type idx = pos - begin();

	for (size_type i = idx; i < __size - 1; ++i)
	{
		__alloc.destroy(&__arr[i]);
		try
		{
			__alloc.construct(&__arr[i], __arr[i + 1]);
		}
		catch	(...)
		{
			for (size_type start = idx; start < i; ++start)
				__alloc.destroy(&__arr[start]);
			throw ;
		}
	}
	__alloc.destroy(&__arr[__size - 1]);
	__size -= 1;
	return iterator(__arr + idx);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator 
				vector<T, Alloc>::erase(	typename vector<T, Alloc>::iterator first, 
											typename vector<T, Alloc>::iterator last)
{
	size_type idx = first - begin();
	size_type count = last - first;
	size_type tmp = idx;

	for (size_type i = 0; i < count; ++i)
		__alloc.destroy(&__arr[tmp + i]);
	__size -= count;
	for (; tmp < __size; ++tmp)
	{
		// basic exception guarantee
		try
		{
			__alloc.construct(&__arr[tmp], __arr[tmp + count]);
		}
		catch	(...)
		{
			for (; tmp >= idx; --tmp)
				__alloc.destroy(&__arr[tmp]);
			for (; last != end(); *last++)
				__alloc.destroy(&__arr[*last]);
			throw ;
		}
		
		__alloc.destroy(&__arr[tmp + count]);
	}
	return iterator(__arr + idx);
}

template <class T, class Alloc>
void vector<T, Alloc>::push_back(const typename vector<T, Alloc>::value_type & value)
{
	T 			*tmp;
	size_type 	new_cap;
	size_type 	i;

	if (__capacity == 0)
	{
		__arr = __alloc.allocate(1);
		__capacity = 1;
	}
	if (__capacity < __size + 1)
	{
		new_cap = __capacity * 2;
		tmp = __alloc.allocate(new_cap);
		try
		{
			for (i = 0; i < __size; ++i)
				__alloc.construct(&tmp[i], __arr[i]);
			__alloc.construct(&tmp[i], value);
		}
		catch	(...)
		{	
			for (; i > 0; --i)
				__alloc.destroy(&tmp[i - 1]);
			__alloc.deallocate(tmp, new_cap);
			throw  ;
		}
		for (i = 0; i < __size; ++i)
			__alloc.destroy(&__arr[i]);
		__alloc.deallocate(__arr, __capacity);
		
		__size += 1;
		__capacity = new_cap;
		__arr = tmp;
	}
	else
	{
		try
		{
			__alloc.construct(&__arr[__size], value);
		}
		catch	(...)
		{
			throw  ;
		}
		__size += 1;
	}
}

template <class T, class Alloc>
void vector<T, Alloc>::pop_back()
{
	__alloc.destroy(&__arr[__size]);
	__size -= 1; 
}

template <class T, class Alloc>
void vector<T, Alloc>::swap(vector<T, allocator_type> & rhs)
{
	ft::swap(__size, rhs.__size);
	ft::swap(__capacity, rhs.__capacity);
	ft::swap(__arr, rhs.__arr);
}

/* --------------------------------------------------------------------------- */
/* returns the allocator associated with the container */
template <class T, class Alloc>
typename vector<T, Alloc>::allocator_type vector<T, Alloc>::get_allocator() const
{
	return __alloc;
}

/* non-member functions ------------------------------------------------------ */
template <class value_type, class allocator_type>
bool operator== (	const vector<value_type, allocator_type> & lhs, 
					const vector<value_type, allocator_type> & rhs)
{
	return (lhs.size() == rhs.size()) 
			&& ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class value_type, class allocator_type>
bool operator!= (	const vector<value_type, allocator_type> & lhs, 
					const vector<value_type, allocator_type> & rhs)
{
	return !(lhs == rhs);
}

template <class value_type, class allocator_type>
bool operator< (	const vector<value_type, allocator_type> & lhs, 
					const vector<value_type, allocator_type> & rhs)
{
	return ft::lexicographical_compare(	lhs.begin(), lhs.end(), 
										rhs.begin(), rhs.end());
}

template <class value_type, class allocator_type>
bool operator<= (	const vector<value_type, allocator_type> & lhs, 
					const vector<value_type, allocator_type> & rhs)
{
	return !(lhs > rhs);
}

template <class value_type, class allocator_type>
bool operator> (	const vector<value_type, allocator_type> & lhs, 
					const vector<value_type, allocator_type> & rhs)
{
	return rhs < lhs;
}

template <class value_type, class allocator_type>
bool operator>= (	const vector<value_type, allocator_type> & lhs, 
					const vector<value_type, allocator_type> & rhs)
{
	return !(lhs < rhs);
}

} // end of ft namespace

namespace std
{

template <class value_type, class allocator_type>
void swap(	ft::vector<value_type, allocator_type> & lhs, 
			ft::vector<value_type, allocator_type> & rhs)
{
	lhs.swap(rhs);
}

} // end of std namespace

#endif