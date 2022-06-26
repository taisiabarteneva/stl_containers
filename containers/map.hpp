#ifndef 	MAP_HPP
# define 	MAP_HPP

# include <memory>
# include <functional>
# include <algorithm>
# include "../utils/utils.hpp"
# include "../utils/tree.hpp"

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>,
                       class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
		public :
		
			/* typedefs ------------------------------------------------------ */
			typedef Key										key_type;
			typedef T										mapped_type;
			typedef pair<const Key, T>						value_type;
			typedef Compare									key_compare;
			typedef Allocator								allocator_type;
			typedef typename Allocator::reference			reference;
			typedef typename Allocator::const_reference		const_reference;
			typedef typename Allocator::pointer				value_pointer; 	/* ft::pair * */
			typedef typename Allocator::const_pointer		const_value_pointer;
			typedef	std::ptrdiff_t							difference_type;
			typedef	size_t									size_type;

			class value_compare
			{
				friend class map;
				protected:
					Compare comp;
					value_compare(Compare c) : comp(c) {}
				public:
					bool operator()(const value_type & x, const value_type & y) const 
					{
						return comp(x.first, y.first);
					} 
			};

			typedef rbtree<value_type, value_compare, allocator_type>	rbtree;

			/* iterators ----------------------------------------------------- */
			typedef Node<value_type>									node;
			typedef node *												node_pointer;
			typedef const node *										const_node_pointer;
			typedef tree_iter<node_pointer, value_pointer>				iterator;
			typedef tree_iter<node_pointer, const_value_pointer>		const_iterator;
			typedef reverse_iter<iterator>								reverse_iterator;
			typedef reverse_iter<const_iterator>						const_reverse_iterator;

			/* constructors ---------------------------------------------------*/
			map()
			:	__alloc(allocator_type()),
				__comp(key_compare()),
				__tree(__comp, __alloc)
			{}

			explicit map(const key_compare & comp, const allocator_type& alloc = allocator_type())
			: 	__alloc(alloc),
				__comp(comp),
				__tree(comp, alloc)
			{}

			template <class InputIt>
			map(InputIt first, InputIt last, 	const key_compare& comp = key_compare(),
												const allocator_type& alloc = allocator_type())
			: 	__alloc(alloc),
				__comp(comp),
				__tree(comp, alloc)
			{
				__tree.insert(first, last);
			}

        	map(const map & rhs)
			: 	__alloc(rhs.__alloc),
				__comp(rhs.__comp),
				__tree(rhs.__tree)
			{}

       		map & operator=(const map & rhs)
			{
				if (this != &rhs)
				{
					__tree = rhs.__tree;
					__comp = rhs.__comp;
					__alloc = rhs.__alloc;
				}
				return *this;
			}

       		~map() {};

			allocator_type get_allocator() const
			{
				return __alloc;
			}

			/* iterators ----------------------------------------------------- */
			iterator begin()
			{
				return __tree.begin();
			}
			const_iterator begin() const
			{
				return __tree.begin();
			}
			iterator end()
			{
				return __tree.end();
			}
			const_iterator end() const
			{
				return __tree.end();
			}
			reverse_iterator rbegin()
			{
				return __tree.rbegin();
			}
			const_reverse_iterator rbegin() const
			{
				return __tree.rbegin();
			}
			reverse_iterator rend()
			{
				return __tree.rend();
			}
			const_reverse_iterator rend() const
			{
				return __tree.rend();
			}

			/* capacity ------------------------------------------------------ */
			bool empty() const
			{
				return __tree.empty();
			}
			size_type size() const
			{
				return __tree.size();
			}
			size_type max_size() const
			{
				return __tree.max_size();
			}

			/* element access ------------------------------------------------ */
			mapped_type & operator[](const key_type & x)
			{
				iterator found = this->find(x);
				if (found != this->end())
					return found->second;
				return this->insert(ft::make_pair(x, mapped_type())).first->second; /* if the key does not exist */
			}

			/* if no such element exists, an exception of type std::out_of_range is thrown */
			mapped_type & at(const key_type & key)
			{
				iterator found = this->find(key);
				if (found == this->end())
					throw std::out_of_range("no such element");
				return found->second;	
			}

			const mapped_type & at(const key_type & key) const
			{
				const_iterator found = this->find(key);
				if (found == this->end())
					throw std::out_of_range("no such element");
				return found->second;
			}

			/* modifiers ----------------------------------------------------- */
			ft::pair<iterator, bool> insert(const value_type & x)
			{
				ft::pair<iterator, bool> res = __tree.insert(x);
				return res;
			}
			
			iterator insert(iterator position, const value_type & x)
			{
				return __tree.insert(position, x);
			}

			template <class InputIterator>
			void insert(InputIterator first, InputIterator last)
			{
				__tree.insert(first, last);
			}

			void erase(iterator pos)
			{
				__tree.erase(pos);
			}

			size_type erase(const key_type & x)
			{
				return __tree.erase(ft::make_pair(x, mapped_type()));
			}

			void erase(iterator first, iterator last)
			{
				__tree.erase(first, last);
			}

			void swap(map<Key, T, Compare, Allocator> & rhs)
			{
				std::swap(this->__tree, rhs.__tree);
				std::swap(this->__alloc, rhs.__alloc);
				std::swap(this->__comp, rhs.__comp);
			}

			void clear()
			{
				__tree.clear();
			}

			/* observers ----------------------------------------------------- */
			/* returns the function object that compares the keys, which is a copy 
			of this container's constructor argument comp */
			key_compare key_comp() const
			{
				return this->__comp;
			}

			/* returns a function object that compares objects of type std::map::value_type 
			(key-value pairs) by using key_comp to compare the first components of the pairs */
			value_compare value_comp() const
			{
				return this->value_comp(key_compare());
			}

			/* map operations ------------------------------------------------ */
			iterator find(const key_type & x)
			{
				iterator it = __tree.find(ft::make_pair(x, mapped_type()));
				return it;
			}
			const_iterator find(const key_type & x) const
			{
				const_iterator it = __tree.find(ft::make_pair(x, mapped_type()));
				return it;
			}
			size_type count(const key_type & x) const
			{
				return (find(x) == end()) ? 0 : 1;
			}
			iterator       lower_bound(const key_type & x)
			{
				return __tree.lower_bound(ft::make_pair(x, mapped_type()));
			}
			const_iterator lower_bound(const key_type & x) const
			{
				return __tree.lower_bound(ft::make_pair(x, mapped_type()));
			}
			iterator       upper_bound(const key_type & x)
			{
				return __tree.upper_bound(ft::make_pair(x, mapped_type()));
			}
			const_iterator upper_bound(const key_type & x) const
			{
				return __tree.upper_bound(ft::make_pair(x, mapped_type()));
			}
			ft::pair<iterator, iterator> equal_range(const key_type & key)
			{
				return __tree.equal_range(ft::make_pair(key, mapped_type()));
			}
			ft::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
			{
				return __tree.equal_range(ft::make_pair(key, mapped_type()));
			}
		private :
			allocator_type 	__alloc;
			key_compare		__comp;	
			rbtree 			__tree;
	};

	/* 	
		Checks if the contents of lhs and rhs are equal, that is, they have the 
		same number of elements and each element in lhs compares equal with the element 
		in rhs at the same position 
	*/
	template <class Key, class T, class Compare, class Allocator>
	bool operator==(const map<Key, T, Compare, Allocator> & x,
					const map<Key, T, Compare, Allocator> & y)
	{
		return (x.size() == y.size()) 
						&& ft::equal(x.begin(), x.end(), y.begin());
	}

	template <class Key, class T, class Compare, class Allocator>
	bool operator!=(	const map<Key, T, Compare, Allocator>& x,
						const map<Key, T, Compare, Allocator>& y)
	{
		return !(x == y);
	}

	template <class Key, class T, class Compare, class Allocator>
	bool operator<(	const map<Key, T, Compare, Allocator>& x,
					const map<Key, T, Compare, Allocator>& y)
	{
		return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
	}

	template <class Key, class T, class Compare, class Allocator>
	bool operator> (	const map<Key, T, Compare, Allocator> & x,
						const map<Key, T, Compare, Allocator> & y)
	{
		return y < x;
	}

	template <class Key, class T, class Compare, class Allocator>
	bool operator>=(	const map<Key, T, Compare, Allocator> & x,
						const map<Key, T, Compare, Allocator> & y)
	{
		return !(x < y);
	}
	template <class Key, class T, class Compare, class Allocator>
	bool operator<=(	const map<Key, T, Compare, Allocator> & x,
						const map<Key, T, Compare, Allocator> & y)
	{
		return !(x > y);
	}

	template <class Key, class T, class Compare, class Allocator>
	void swap(	ft::map<Key, T, Compare, Allocator> & lhs,
           		ft::map<Key, T, Compare, Allocator> & rhs)
	{
		lhs.swap(rhs);
	}
}

#endif