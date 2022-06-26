#ifndef 	TREE_HPP
# define 	TREE_HPP

# include <memory>
# include <functional>
# include <algorithm>
# include "utils.hpp"
# include "pair.hpp"
# include "node.hpp"
# include "bidirectional_iterator.hpp"

namespace ft
{	
	/* RB Tree class --------------------------------------------------------- */
	template <class T, class Compare = std::less<T>, class Allocator = std::allocator<T> >
	class rbtree
	{
		public :

			typedef T											value_type;
			typedef Node<value_type>							node;
			typedef Compare										compare;
			typedef Allocator									allocator_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef typename allocator_type::template rebind<node>::other node__allocator;
			typedef size_t										size_type;
			typedef std::ptrdiff_t								difference_type;

			/* Iterators ----------------------------------------------------- */
			typedef tree_iter<node *, pointer>			iterator;
			typedef tree_iter<node *, const_pointer>	const_iterator;
			typedef reverse_iter<iterator>				reverse_iterator;
			typedef reverse_iter<const_iterator>		const_reverse_iterator;

			explicit rbtree();
			explicit rbtree(const compare & comp = compare(), const allocator_type & alloc = allocator_type());
			rbtree(const rbtree &other);
			rbtree &operator=(const rbtree &other);
			~rbtree();

		protected :

			compare				__compare;
			node__allocator 	__alloc;
			size_type 			__size;
			node				*__root;
			node 				*__nil;
		
		public : 

			void clear();

			/* Iterarors ----------------------------------------------------- */
			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;
			reverse_iterator rbegin();
			const_reverse_iterator rbegin() const;
			reverse_iterator rend();
			const_reverse_iterator rend() const;

			/* Capacity ------------------------------------------------------ */
			bool empty() const;
			size_type size() const;
			size_type max_size() const;

			/* Modifiers ----------------------------------------------------- */
			void swap(rbtree & rhs);
			/* Insertion ----------------------------------------------------- */
			ft::pair<iterator, bool> insert(const value_type & x);
			iterator insert(iterator hint, const value_type & value);
			template <class InputIterator>
          	void insert(InputIterator first, InputIterator last);
			
			/* Erase --------------------------------------------------------- */
			void erase(iterator position);
			size_type erase(const T& x);
			void erase(iterator first, iterator last);

			ft::pair<iterator, iterator> equal_range(const T & key);
			ft::pair<const_iterator, const_iterator> equal_range(const T & key) const;
			iterator       lower_bound(const T& key);
			const_iterator lower_bound(const T& key) const;
			iterator       upper_bound(const T& key);
			const_iterator upper_bound(const T& key) const;

			iterator find(const value_type & key);
			const_iterator find(const value_type & key) const;

		protected :

			void __clear(node *curr);
			void __cleanNilNode(void);
			void __createNilNode(void);
			void __clone(node *&curr, node *parent, node *other);
			node *__findNode(const value_type & value) const;
			node *__createNode(const value_type & value = value_type());

			/* Insertion ----------------------------------------------------- */
			ft::pair<node *, bool> __insert_system(const value_type & x, node * subtreeRoot);
			void __tree_insert_fixup(node *elem);

			/* Erase --------------------------------------------------------- */
			void __erase(node *elem);
			void __transplant(node *u, node *v);
			void __remove_node(node *elem);
			void __tree_erase_fixup(node *elem);

			/* Rotation ------------------------------------------------------ */
			void ___leftRotate(node *elem);
			void ___rightRotate(node *elem);

			/* Min & Max ----------------------------------------------------- */
			node *__findMin(node *elem) const;
			node *__findMax(node *elem) const;

			void __visualize(int tabs, node *root)
			{
				std::string color = root->__is_red == false ? "\x1b[30;4m" : "\x1b[31;1m" ;
				for (int i = 0; i < tabs; i++)
					std::cout << '\t';
				std::cout << color << root->_value << "\x1b[30m" << std::endl;
				if (root->__left)
					__visualize(tabs - 1, root->__left);
				if (root->__right)
					__visualize(tabs + 1, root->__right);
			}
	};


	/* Coplien's form -------------------------------------------------------- */

	template <class T, class Compare, class Allocator>
	rbtree<T, Compare, Allocator>::rbtree()
	: 	__compare(),
		__alloc(),
		__size(0),
		__root(NULL)
	{
		__createNilNode();
	}

	template <class T, class Compare, class Allocator>
	rbtree<T, Compare, Allocator>::rbtree(const typename rbtree<T, Compare, Allocator>::compare & comp, 
		const typename rbtree<T, Compare, Allocator>::allocator_type & alloc)
	: 	__compare(comp),
		__alloc(alloc),
		__size(0),
		__root(NULL)
	{
		__createNilNode();
	}

	template <class T, class Compare, class Allocator>
	rbtree<T, Compare, Allocator>::rbtree(const rbtree &other)
	: 	__compare(other.__compare),
		__alloc(other.__alloc),
		__size(other.__size),
		__root(NULL),
		__nil(NULL)
	{
		__createNilNode();
		__clone(this->__root, NULL, other.__root);
		if (__root)
		{
			this->__nil->__left = this->__root;
			this->__root->__parent = this->__nil;
		}
	}

	template <class T, class Compare, class Allocator>
	rbtree<T, Compare, Allocator> & rbtree<T, Compare, Allocator>::operator=(const rbtree &other)
	{
		node *tmp = NULL;

		if (this != &other)
		{
			__clone(tmp, NULL, other.__root);
			clear();
			__root = tmp;
			if (__root)
			{
				this->__nil->__left = this->__root;
				this->__root->__parent = this->__nil;
			}
			__size = other.__size;
			__alloc = other.__alloc;
			__compare = other.__compare;
		}
		return *this;
	}

	template <class T, class Compare, class Allocator>
	rbtree<T, Compare, Allocator>::~rbtree()
	{
		clear();
		__cleanNilNode();
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::clear()
	{
		__clear(this->__root);
		__root = NULL;
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__cleanNilNode(void)
	{
		if (this->__nil)
		{
			__alloc.destroy(this->__nil);
			__alloc.deallocate(this->__nil, 1);
		}
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__clear(typename rbtree<T, Compare, Allocator>::node *curr)
	{
		if (curr == NULL)
			return ;
		__clear(curr->__left);
		__clear(curr->__right);
		__remove_node(curr);
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__clone(typename rbtree<T, Compare, Allocator>::node *&curr, 
	typename rbtree<T, Compare, Allocator>::node *parent, typename rbtree<T, Compare, Allocator>::node *other)
	{
		if (!other)
			return ;
		curr = __createNode(other->_value);
		curr->__parent = parent;
		curr->__is_red = other->__is_red;
		__clone(curr->__right, curr, other->__right);
		__clone(curr->__left, curr, other->__left);
	}

	/* Equal Range ----------------------------------------------------------- */
	template <class T, class Compare, class Allocator>
	ft::pair<typename rbtree<T, Compare, Allocator>::iterator, typename rbtree<T, Compare, Allocator>::iterator> 
			rbtree<T, Compare, Allocator>::equal_range(const T & key)
	{
		return ft::make_pair(lower_bound(key), upper_bound(key));
	}

	template <class T, class Compare, class Allocator>
	ft::pair<typename rbtree<T, Compare, Allocator>::const_iterator, typename rbtree<T, Compare, Allocator>::const_iterator> 
			rbtree<T, Compare, Allocator>::equal_range(const T & key) const
	{
		return ft::make_pair(lower_bound(key), upper_bound(key));
	}

	/* Capacity -------------------------------------------------------------- */

	template <class T, class Compare, class Allocator>
	bool rbtree<T, Compare, Allocator>::empty() const
	{
		return __size == 0;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::size_type rbtree<T, Compare, Allocator>::size() const
	{
		return __size;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::size_type rbtree<T, Compare, Allocator>::max_size() const
	{
		return __alloc.max_size();
	}

	/* Iterators ------------------------------------------------------------- */
	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::iterator rbtree<T, Compare, Allocator>::begin()
	{
		rbtree<T, Compare, Allocator>::iterator it;

		if (!__root)
			it = this->__nil;
		else
			it = this->__findMin(__root);
		return it;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::const_iterator rbtree<T, Compare, Allocator>::begin() const
	{
		rbtree<T, Compare, Allocator>::const_iterator it;
		
		if (!__root)
			it = this->__nil;
		else
			it = this->__findMin(__root);
		return it;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::iterator rbtree<T, Compare, Allocator>::end()
	{
		rbtree<T, Compare, Allocator>::iterator it(__nil);

		return it;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::const_iterator rbtree<T, Compare, Allocator>::end() const
	{
		rbtree<T, Compare, Allocator>::const_iterator it(__nil);

		return it;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::reverse_iterator rbtree<T, Compare, Allocator>::rbegin()
	{
		return reverse_iterator(this->end());
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::const_reverse_iterator rbtree<T, Compare, Allocator>::rbegin() const
	{
		return const_reverse_iterator(this->end());
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::reverse_iterator rbtree<T, Compare, Allocator>::rend()
	{
		return reverse_iterator(this->begin());
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::const_reverse_iterator rbtree<T, Compare, Allocator>::rend() const
	{
		return const_reverse_iterator(this->begin());
	}

	/* Private --------------------------------------------------------------- */
	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::node *rbtree<T, Compare, Allocator>::__createNode(
					const typename rbtree<T, Compare, Allocator>::value_type & value)
	{
		node *newNode = __alloc.allocate(1); 	/* size of one node */
		__alloc.construct(newNode, value);
		return (newNode);
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__createNilNode(void)
	{
		this->__nil = __alloc.allocate(1);
		this->__nil->__is_red = false;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::node 
		*rbtree<T, Compare, Allocator>::__findMin(typename rbtree<T, Compare, Allocator>::node *elem) const
	{
		node *min = elem;

		while (min && min->__left && min->__left != __nil)
			min = min->__left;
		return min;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::node 
		*rbtree<T, Compare, Allocator>::__findMax(typename rbtree<T, Compare, Allocator>::node *elem) const
	{
		node *max = elem;
		while (max && max->__right && max->__right != __nil)
			max = max->__right;
		return max;
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::node *
			rbtree<T, Compare, Allocator>::__findNode(const typename rbtree<T, Compare, Allocator>::value_type & value) const 
	{
		node *tmp = __root;

		while (tmp)
		{
			if (__compare(tmp->_value, value))
				tmp = tmp->__right;
			else if (__compare(value, tmp->_value))
				tmp = tmp->__left;
			else
			{
				return tmp;
			}
		}
		return NULL;
	}

	/* Modifiers ------------------------------------------------------------- */
	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::swap(rbtree & rhs)
	{
		std::swap(__compare, rhs.__compare);
		std::swap(__alloc, rhs.__alloc);
		std::swap(__size, rhs.__size);
		std::swap(__root, rhs.__root);
		std::swap(__nil, rhs.__nil);
	}

	/* Insert ---------------------------------------------------------------- */
	/* 
		O(log n) time complexity 
		- Insert node similarly to that in a BST
		- Assign the red color to the new node
	*/

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::iterator 
	rbtree<T, Compare, Allocator>::insert(	typename rbtree<T, Compare, Allocator>::iterator hint,
											const typename rbtree<T, Compare, Allocator>::value_type& x)
	{
		iterator 				tmp(hint);
		ft::pair<node *, bool> 	res;

		if (hint.base() && hint != this->end() 
			&& this->__compare(*hint, x) && this->__compare(x, *(tmp++)))
		{
			res = this->__insert_system(x, hint.base());
			return iterator(res.first);
		}
		else
			res = this->__insert_system(x, __root);
		return iterator(res.first);
	}

	template <class T, class Compare, class Allocator>
	template <class InputIterator>
    void rbtree<T, Compare, Allocator>::insert(InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
		{
			insert(*first);
		}
	}

	template <class T, class Compare, class Allocator>
	ft::pair<typename rbtree<T, Compare, Allocator>::iterator, bool> 
	rbtree<T, Compare, Allocator>::insert(const typename rbtree<T, Compare, Allocator>::value_type & x)
	{
		ft::pair<node *, bool> res = this->__insert_system(x, this->__root);
		return ft::make_pair(iterator(res.first), res.second);
	}

	/* Insert helper --------------------------------------------------------- */
	template <class T, class Compare, class Allocator>
	ft::pair<typename rbtree<T, Compare, Allocator>::node *, bool> 
	rbtree<T, Compare, Allocator>::__insert_system(	const typename rbtree<T, Compare, Allocator>::value_type & value, 
													typename rbtree<T, Compare, Allocator>::node * subtreeRoot)
	{
		if (__root == NULL)
		{
			__root = this->__createNode(value);
			__root->__is_red = false;
			__size++;
			this->__nil->__left = this->__root;
			this->__root->__parent = this->__nil;
			return ft::make_pair(this->__root, true);
		}
		
		this->__nil->__left = NULL;
		this->__root->__parent = NULL;

		node *a = subtreeRoot;
		node *b = NULL;
		node *newNode = this->__createNode(value); 		/* red by default */
		__size++;
		while (a)
		{
			b = a;
			if (__compare(newNode->_value, a->_value)) 	/* comparator, second template argument */
				a = a->__left;
			else if (__compare(a->_value, newNode->_value))
				a = a->__right;
			else
			{
				__remove_node(newNode);
				this->__nil->__left = this->__root;
				this->__root->__parent = this->__nil;
				return ft::make_pair(a, false);
			}
		}
		newNode->__parent = b;
		if (!b)
			subtreeRoot = newNode;
		else if (__compare(newNode->_value, b->_value))
			b->__left = newNode;
		else 
			b->__right = newNode;
		__tree_insert_fixup(newNode);
		this->__nil->__left = this->__root;
		this->__root->__parent = this->__nil;
		return ft::make_pair(newNode, true);
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__tree_insert_fixup(typename rbtree<T, Compare, Allocator>::node *elem)
	{
		node *uncle = NULL;

		while (elem->__parent && elem->__parent->__is_red) 			/* check double red */
		{
			if (elem->__parent == elem->__parent->__parent->__left) /* parent of new node is a left child */
			{
				uncle = elem->__parent->__parent->__right; 		/* uncle of a new node */
				if (uncle && uncle->__is_red)					/* red uncle */
				{
					elem->__parent->__is_red = false;
					uncle->__is_red = false;
					elem->__parent->__parent->__is_red = true;
					elem = elem->__parent->__parent;
				}
				else
				{
					if (elem == elem->__parent->__right)
					{
						elem = elem->__parent;
						this->___leftRotate(elem);
					}
					elem->__parent->__is_red = false;
					elem->__parent->__parent->__is_red = true;
					this->___rightRotate(elem->__parent->__parent); 		/* do rotation in any case */
				}
			}
			else
			{
				uncle = elem->__parent->__parent->__left;
				if (uncle && uncle->__is_red)
				{											
					elem->__parent->__is_red = false;
					uncle->__is_red = false;
					elem->__parent->__parent->__is_red = true;
					elem = elem->__parent->__parent;
				}
				else
				{
					if (elem == elem->__parent->__left)
					{
						elem = elem->__parent;
						this->___rightRotate(elem);
					}
					elem->__parent->__is_red = false;
					elem->__parent->__parent->__is_red = true;
					this->___leftRotate(elem->__parent->__parent);
				}
			}
		}
		this->__root->__is_red = false;
	}

	/* Rotation  ------------------------------------------------------------- */
	/* O(1) time complexity */
	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::___leftRotate(typename rbtree<T, Compare, Allocator>::node *a)
	{
		node *b;

		/* Initialize b */
		b = a->__right;
		/* Left b subtree becomes a right subtree */
		a->__right = b->__left;
		/* Establish parent-child relationship */
		if (b->__left)
			b->__left->__parent = a;
		b->__parent = a->__parent;
		if (a == __root)
			__root = b;
		else if (a == a->__parent->__left)
			a->__parent->__left = b;
		else 
			a->__parent->__right = b;
		b->__left = a;
		a->__parent = b;
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::___rightRotate(typename rbtree<T, Compare, Allocator>::node *a)
	{
		node *b;

		b = a->__left;
		a->__left = b->__right;
		if (b->__right)
			b->__right->__parent = a;
		b->__parent = a->__parent;
		if (a == __root)
			__root = b;
		else if (a == a->__parent->__right)
			a->__parent->__right = b;
		else
			a->__parent->__left = b;
		b->__right = a;
		a->__parent = b;
	}

	/* Lower & Upper bound --------------------------------------------------- */
	/* returns an iterator pointing to the next smallest number just greater than or equal to that number */
	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::iterator 
				rbtree<T, Compare, Allocator>::lower_bound(const T & key)
	{
		node *tmp = this->__root;
		node *res = this->__nil;

		while (tmp)
		{
			if (__compare(tmp->_value, key))
				tmp = tmp->__right;
			else
			{
				res = tmp;
				tmp = tmp->__left;
			}
		}
		return iterator(res);
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::const_iterator 
				rbtree<T, Compare, Allocator>::lower_bound(const T & key) const
	{
		node *tmp = this->__root;
		node *res = this->__nil;

		while (tmp)
		{
			if (__compare(tmp->_value, key))
				tmp = tmp->__right;
			else
			{
				res = tmp;
				tmp = tmp->__left;
			}
		}
		return const_iterator(res);
	}
	/* returns an iterator pointing to the first element in the range [first, last) that is greater than value */
	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::iterator 
				rbtree<T, Compare, Allocator>::upper_bound(const T & key)
	{
		node *tmp = this->__root;
		node *res = this->__nil;

		while (tmp)
		{
			if (__compare(key, tmp->_value))
			{
				res = tmp;
				tmp = tmp->__left;
			}
			else
				tmp = tmp->__right;
		}
		return iterator(res);
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::const_iterator 
				rbtree<T, Compare, Allocator>::upper_bound(const T & key) const
	{
		node *tmp = this->__root;
		node *res = this->__nil;

		while (tmp)
		{
			if (__compare(key, tmp->_value))
			{
				res = tmp;
				tmp = tmp->__left;
			}
			else
				tmp = tmp->__right;
		}
		return const_iterator(res);
	}

	/* Erase ----------------------------------------------------------------- */
	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__erase(typename rbtree<T, Compare, Allocator>::node *elem)
	{
		node *tmp;
		node *tmpTwo;
		bool trueColor = elem->__is_red;
		tmp = elem;
		
		if (!elem->__left)				 			/* a node to delete has only left or no child */
		{
			tmpTwo = elem->__right;
			__transplant(elem, elem->__right);
			__remove_node(elem);
		}
		else if (!elem->__right) 					/* a node to delete has only right or no child */
		{
			tmpTwo = elem->__left;
			__transplant(elem, elem->__left);
			__remove_node(elem);
		}
		else										/* a node to delete has two children */
		{
			tmp = __findMin(elem->__right);
			trueColor = tmp->__is_red;
			tmpTwo = tmp->__right;
			if (tmp->__parent && tmp->__parent != elem)
			{
				__transplant(tmp, tmp->__right);
				tmp->__right = elem->__right;
				tmp->__right->__parent = tmp;
			}
			__transplant(elem, tmp);
			tmp->__left = elem->__left;
			tmp->__left->__parent = tmp;
			tmp->__is_red = elem->__is_red;
			__remove_node(elem);
		}
		if (trueColor == false && tmpTwo)
			__tree_erase_fixup(tmpTwo);
		if (this->__root)
		{
			this->__nil->__left = this->__root;
			this->__root->__parent = this->__nil;
		}
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__tree_erase_fixup(typename 
							rbtree<T, Compare, Allocator>::node *elem)
	{
		node *sibling = NULL;
		while (elem != this->__root && !elem->__is_red)
		{
			if (elem && elem == elem->__parent->__left)	
			{
				sibling = elem->__parent->__right;
				if (sibling && sibling->__is_red)	/* case 1 */
				{
					sibling->__is_red = false;
					elem->__parent->__is_red = true;
					___leftRotate(elem->__parent);
					sibling = elem->__parent->__right;
				}
				if (sibling && sibling->__left && sibling->__right && 
					!sibling->__left->__is_red && !sibling->__right->__is_red)	/* case 2 */
				{
					sibling->__is_red = true;
					elem = elem->__parent;
				}	
				else
				{
					if (sibling && sibling->__right && !sibling->__right->__is_red)
					{
						sibling->__left->__is_red = false;
						sibling->__is_red = true;
						___rightRotate(sibling);
						sibling = elem->__parent->__right;
					}
					if (sibling)
						sibling->__is_red = elem->__parent->__is_red; /* case 4 */
					elem->__parent->__is_red = false;
					if (sibling && sibling->__right)
						sibling->__right->__is_red = false;
					___leftRotate(elem->__parent);
					elem = this->__root;
				}
			}
			else if (elem == elem->__parent->__right)
			{
				sibling = elem->__parent->__left;
				if (sibling && sibling->__is_red)
				{
					sibling->__is_red = false;
					elem->__parent->__is_red = true;
					___rightRotate(elem->__parent);
					sibling = elem->__parent->__left;
				}
				if (sibling && sibling->__left && sibling->__right && 
					!sibling->__left->__is_red && !sibling->__right->__is_red)
				{
					sibling->__is_red = true;
					elem = elem->__parent;
					sibling = elem->__parent->__left;
				}
				else
				{
					if (sibling && sibling->__left && !sibling->__left->__is_red)
					{
						sibling->__right->__is_red = false;
						sibling->__is_red = true;
						___leftRotate(sibling);
						sibling = elem->__parent->__left;
					}
					if (sibling)
						sibling->__is_red = elem->__parent->__is_red;
					elem->__parent->__is_red = false;
					if (sibling && sibling->__left)
					{
						sibling->__left->__is_red = false;
					}
					___rightRotate(elem->__parent);
					elem = this->__root;
				}
			}
		}	
		elem->__is_red = false;
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__transplant(	typename rbtree<T, Compare, Allocator>::node *toDel, 
														typename rbtree<T, Compare, Allocator>::node *toReplace)
	{
		if (toDel->__parent == this->__nil)
			this->__root = toReplace;
		else if (toDel == toDel->__parent->__left)
			toDel->__parent->__left = toReplace;
		else
			toDel->__parent->__right = toReplace;
		if (toReplace)
			toReplace->__parent = toDel->__parent;
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::__remove_node(typename rbtree<T, Compare, Allocator>::node * elem)
	{
		this->__alloc.destroy(elem);
		this->__alloc.deallocate(elem, 1);
		this->__size--;
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::erase(typename rbtree<T, Compare, Allocator>::iterator pos)
	{
		erase(*pos);
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::size_type rbtree<T, Compare, Allocator>::erase(const T & value)
	{
		node *toErase = __findNode(value);

		if (toErase)
		{
			__erase(toErase);
			return 1;
		}
		return 0;
	}

	template <class T, class Compare, class Allocator>
	void rbtree<T, Compare, Allocator>::erase(	typename rbtree<T, Compare, Allocator>::iterator first, 
												typename rbtree<T, Compare, Allocator>::iterator last)
	{
		while (first != last)
			erase(first++);
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::iterator rbtree<T, Compare, Allocator>::find(const 
		typename rbtree<T, Compare, Allocator>::value_type & key)
	{
		node * tmp = __findNode(key);
	
		if (tmp)
			return iterator(__findNode(key));
		return iterator(this->end());
	}

	template <class T, class Compare, class Allocator>
	typename rbtree<T, Compare, Allocator>::const_iterator rbtree<T, Compare, Allocator>::find(const 
		typename rbtree<T, Compare, Allocator>::value_type & key) const
	{
		node * tmp = __findNode(key);
	
		if (tmp)
			return const_iterator(__findNode(key));
		return const_iterator(this->end());
	}

} // end of namespace ft

#endif