#ifndef		STACK_HPP
# define	STACK_HPP

# include "vector.hpp"

/* vector, list, deque should be compatible with stack */

namespace ft
{
	template <class T, class Container = ft::vector<T> >
	class stack
	{
		public:
			typedef	 		 Container					container_type;
			typedef typename Container::value_type		value_type;
			typedef typename Container::size_type		size_type;
			typedef typename Container::reference		reference;
			typedef typename Container::const_reference	const_reference;

			/* ------------------------ constructors ------------------------- */
			explicit stack(const container_type& cont = container_type())
			: __cont(cont)
			{}

			stack(const stack & other)
			: __cont(other.__cont)
			{}

			stack & operator=(const stack & other)
			{
				__cont = other.__cont;
				return *this;
			}
			~stack()
			{}

			/* ------------------------ member functions --------------------- */
			/* ------------------------ element access ----------------------- */
			reference top()
			{
				return __cont.back();
			}
			const_reference top() const
			{
				return __cont.back();
			}

			/* -------------------------- capacity ---------------------------- */
			bool empty() const
			{
				return __cont.empty();
			}
			size_type size() const
			{
				return __cont.size();
			}

			/* -------------------------- modifiers --------------------------- */
			void push(const value_type &value)
			{
				__cont.push_back(value);
			}
			void pop()
			{
				__cont.pop_back();
			}

			/* -------------------- non-member functions ---------------------- */
			template <class A, class Cont>
			friend bool operator==(const stack<A,Cont>& lhs, const stack<A,Cont>& rhs)
			{
				return lhs.__cont == rhs.__cont;
			}
			template <class A, class Cont>
			friend bool operator!=(const stack<A,Cont>& lhs, const stack<A,Cont>& rhs)
			{
				return lhs.__cont != rhs.__cont;
			}
			template <class A, class Cont>
			friend bool operator<(const stack<A,Cont>& lhs, const stack<A,Cont>& rhs)
			{
				return lhs.__cont < rhs.__cont;
			}
			template <class A, class Cont>
			friend bool operator<=(const stack<A,Cont>& lhs, const stack<A,Cont>& rhs)
			{
				return lhs.__cont <= rhs.__cont;
			}
			template <class A, class Cont>
			friend bool operator>(const stack<A,Cont>& lhs, const stack<A,Cont>& rhs)
			{
				return lhs.__cont > rhs.__cont;
			}
			template <class A, class Cont>
			friend bool operator>=(const stack<A,Cont>& lhs, const stack<A,Cont>& rhs)
			{
				return lhs.__cont >= rhs.__cont;
			}
	
		protected:
			container_type	__cont;
	};
}

#endif