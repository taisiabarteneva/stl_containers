#ifndef 	NODE_HPP
# define 	NODE_HPP

namespace ft
{
	template <class T>
	struct Node
	{
		typedef T 			value_type;
		typedef Node * 		node_pointer;

		Node()
		{}
		explicit Node(	const value_type & value, bool is_red = true,
						node_pointer parent = NULL, node_pointer right = NULL, node_pointer left = NULL)
		:	_value(value), 		__is_red(is_red), 
			__parent(parent), 	__right(right), 
			__left(left)
		{}
		Node(const Node & rhs) 
		: 	_value(rhs._value), 	__is_red(rhs.__is_red), 
			__parent(rhs.__parent), __right(rhs.__right), 
			__left(rhs.__left)
		{}
		Node &operator=(const Node & rhs)
		{
			if (this != &rhs)
			{
				this->_value = rhs._value;
				this->__is_red = rhs.__is_red;
				this->parent = rhs.__parent;
				this->__right = rhs.__right;
				this->__left = rhs.__left;
			}
			return *this;
		}
		~Node() {}
	
		value_type 			_value;
		bool 				__is_red;
		node_pointer 		__parent;
		node_pointer 		__right;
		node_pointer 		__left;
	};
}

#endif