#ifndef 	ENABLE_IF_HPP
# define 	ENABLE_IF_HPP

namespace ft
{
/* -------------------------------- enable if ---------------------------------*/
template<bool B, class T = void>
struct enable_if 
{};
 
template<class T>
struct enable_if<true, T> 
{ 
	typedef T type;
};

template <class T>
struct is_iterator 
{
	private:

		template <class C>
		static typename C::iterator_category f(int a)
		{
			(void)a;
			return C::iterator_category();
		};
		
		template <class C>
		static double f(...)
		{
			return 0;
		};

	public:
		
		static const bool value = sizeof(f<T>(1)) != sizeof(double);
};

}

#endif