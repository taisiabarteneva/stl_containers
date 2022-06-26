#ifndef	IS_INTEGRAL_HPP
# define IS_INTEGRAL_HPP

# include <type_traits>

namespace ft
{

/* ------------------------------ is integral -------------------------------- */
/*  checks whether T is an integral type. Provides the member constant value which 
is equal to true, if T is the type bool, 
char, char16_t, char32_t, wchar_t, short, int, long, long long, or any implementation-defined 
extended integer types, including any signed, unsigned, and cv-qualified variants. Otherwise, 
value is equal to false. */

// template<typename T>
// struct is_integral
// {
// 	typedef T value_type;
//     static const bool value = false;

// 	bool operator() ()
// 	{ return value; }
	
// 	operator bool ()
// 	{ return value; }
// };

// template<>
// struct is_integral<bool>
// {
// 	static const bool value = true;
// 	bool operator() ()
// 	{ return value; }
	
// 	operator bool ()
// 	{ return value; }
// };

template <class T> 	 struct is_integral                     : public std::false_type {};
template <>          struct is_integral<bool>               : public std::true_type {};
template <>          struct is_integral<char>               : public std::true_type {};
template <>          struct is_integral<signed char>        : public std::true_type {};
template <>          struct is_integral<unsigned char>      : public std::true_type {};
template <>          struct is_integral<wchar_t>            : public std::true_type {};
template <>          struct is_integral<char16_t>           : public std::true_type {};
template <>          struct is_integral<char32_t>           : public std::true_type {};
template <>          struct is_integral<short>              : public std::true_type {};
template <>          struct is_integral<unsigned short>     : public std::true_type {};
template <>          struct is_integral<int>                : public std::true_type {};
template <>          struct is_integral<unsigned int>       : public std::true_type {};
template <>          struct is_integral<long>               : public std::true_type {};
template <>          struct is_integral<unsigned long>      : public std::true_type {};
template <>          struct is_integral<long long>          : public std::true_type {};
template <>          struct is_integral<unsigned long long> : public std::true_type {};
template <>          struct is_integral<__int128_t>         : public std::true_type {};
template <>          struct is_integral<__uint128_t>        : public std::true_type {};

}

#endif