#ifndef 	SET_HPP
# define 	SET_HPP

# include <memory>
# include <algorithm>
# include <functional>
# include "../utils/utils.hpp"
# include "../utils/tree.hpp"

namespace ft
{
	template <class Key, class Compare = std::less <Key>, 
										class Allocator = std::allocator <Key> >
	class set
	{
		public :
			/* typedefs ---------------------------------------------------------- */
			typedef Key 													key_type;
			typedef Key 													value_type;
			typedef std::size_t												size_type;
			typedef std::ptrdiff_t											difference_type;
			typedef Compare													key_compare;
			typedef Compare													value_compare;
			typedef Allocator												allocator_type;
			typedef value_type & 											reference;
			typedef const value_type & 										const_reference;
			typedef typename Allocator::pointer 							pointer;
			typedef typename Allocator::const_pointer						const_pointer;
			typedef Node<value_type>										node;
			typedef node * 													node_pointer;
			typedef rbtree<value_type, value_compare, allocator_type>		rbtree;
			/* iterators --------------------------------------------------------- */
			typedef tree_iter<node_pointer, pointer>						iterator;
			typedef tree_iter<node_pointer, const_pointer>					const_iterator;											
			typedef reverse_iter<iterator>									reverse_iterator;
			typedef reverse_iter<const_iterator>							const_reverse_iterator;
			
			/* constructors ------------------------------------------------------ */
			set()
			:	__comp(key_compare()),
				__alloc(allocator_type()),
				__tree(__comp, __alloc)
			{}

			explicit set(	const key_compare & comp, 
							const allocator_type & alloc = allocator_type())
			:	__comp(comp),
				__alloc(alloc),
				__tree(comp, alloc)
			{}

			template <class InputIt>
			set(InputIt first, InputIt last, const key_compare & comp = key_compare(), 
											 const allocator_type & alloc = allocator_type())
			:	__comp(comp),
				__alloc(alloc),
				__tree(comp, alloc)
			{
				__tree.insert(first, last);
			}

			set(const set & other)
			:	__comp(other.__comp),
				__alloc(other.__alloc),
				__tree(other.__tree)
			{}
			
			set & operator=(const set & rhs)
			{
				if (this != &rhs)
				{
					__tree = rhs.__tree;
					__comp = rhs.__comp;
					__alloc = rhs.__alloc;
				}
				return *this;
			}

			~set()
			{}

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

			/* modifiers ----------------------------------------------------- */
			void clear()
			{
				__tree.clear();
			}

			ft::pair<iterator, bool> insert(const value_type & value)
			{
				ft::pair<iterator, bool> res = __tree.insert(value);
				return res;
			}

			iterator insert(iterator hint, const value_type & value)
			{
				return __tree.insert(hint, value);
			}

			template <class InputIt>
			void insert(InputIt first, InputIt last)
			{
				__tree.insert(first, last);
			}

			void erase(iterator pos)
			{
				__tree.erase(pos);
			}

			size_type erase(const key_type & x)
			{
				return __tree.erase(x);
			}

			void erase(iterator first, iterator last)
			{
				__tree.erase(first, last);
			}

			void swap(set & rhs)
			{
				std::swap(this->__tree, rhs.__tree);
				std::swap(this->__alloc, rhs.__alloc);
				std::swap(this->__comp, rhs.__comp);
			}

			/* lookup -------------------------------------------------------- */
			iterator find(const key_type & key)
			{
				return __tree.find(key);
			}

			const_iterator find(const key_type & x) const
			{
				return __tree.find(x);
			}

			size_type count(const key_type & key) const
			{
				return (find(key) == end()) ? 0 : 1;
			}

			iterator lower_bound(const key_type & key)
			{
				return __tree.lower_bound(key);
			}
			const_iterator lower_bound(const key_type & key) const
			{
				return __tree.lower_bound(key);
			}
			
			iterator       upper_bound(const key_type & key)
			{
				return __tree.upper_bound(key);
			}
			
			const_iterator upper_bound(const key_type & key) const
			{
				return __tree.upper_bound(key);
			}

			ft::pair<iterator, iterator> equal_range(const key_type & key)
			{
				return __tree.equal_range(key);
			}

			ft::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
			{
				return __tree.equal_range(key);
			}
			/* observers ----------------------------------------------------- */
			key_compare key_comp() const
			{
				return this->__comp;
			}
			
		protected :
			key_compare 	__comp;
			allocator_type 	__alloc;
			rbtree 			__tree;
	};

	template <class Key, class Compare, class Allocator>
	bool operator==(const set<Key, Compare, Allocator> & x,
					const set<Key, Compare, Allocator> & y)
	{
		return (x.size() == y.size()) 
				&& ft::equal(x.begin(), x.end(), y.begin());
	}

	template <class Key, class Compare, class Allocator>
	bool operator!=(	const set<Key, Compare, Allocator> & x,
						const set<Key, Compare, Allocator> & y)
	{
		return !(x == y);
	}

	template <class Key, class Compare, class Allocator>
	bool operator<(	const set<Key, Compare, Allocator> & x,
					const set<Key, Compare, Allocator> & y)
	{
		return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
	}

	template <class Key, class Compare, class Allocator>
	bool operator> (	const set<Key, Compare, Allocator> & x,
						const set<Key, Compare, Allocator> & y)
	{
		return y < x;
	}

	template <class Key, class Compare, class Allocator>
	bool operator>=(	const set<Key, Compare, Allocator> & x,
						const set<Key, Compare, Allocator> & y)
	{
		return !(x < y);
	}
	template <class Key, class Compare, class Allocator>
	bool operator<=(	const set<Key, Compare, Allocator> & x,
						const set<Key, Compare, Allocator> & y)
	{
		return !(x > y);
	}

	template <class Key, class Compare, class Allocator>
	void swap(	ft::set<Key, Compare, Allocator> & lhs,
           		ft::set<Key, Compare, Allocator> & rhs)
	{
		lhs.swap(rhs);
	}
}


#endif