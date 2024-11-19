#pragma once

#include <ZC/Tools/ZC_cNumber.h>

#include <string>
#include <sstream>

class ZC_NumberToString
{
public:
    /*
    Convert number to string.

    Params:
    - number - number to convert.
    - precision - after dot numbers count. Have effect only for float, double, long double.
    - cut_after_dot_nulls - cuts unused nulls after dot.  Have effect only for float, double, long double.
    */
    template<ZC_cNumber TNum>
    static std::string ToStr(TNum number, i_zc precision = 2, bool cut_after_dot_nulls = false)
    {
        if constexpr (std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double>)
        {
            return DoubleToStrAndWStr<std::string, std::ostringstream>(number, precision, cut_after_dot_nulls);
        }
        else return std::to_string(number);
    }

    /*
    Convert number to string.

    Params:
    - number - number to convert.
    - precision - after dot numbers count. Have effect only for float, double, long double.
    - cut_after_dot_nulls - cuts unused nulls after dot.  Have effect only for float, double, long double.
    */
    template<ZC_cNumber TNum>
    static std::wstring ToWStr(TNum number, i_zc precision = 2, bool cut_after_dot_nulls = false)
    {
        if constexpr (std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double>)
        {
            return DoubleToStrAndWStr<std::wstring, std::wostringstream>(number, precision, cut_after_dot_nulls);
        }
        else return std::to_wstring(number);
    }

private:
    template <typename TStr, typename TStream>
    static TStr DoubleToStrAndWStr(d_zc number, i_zc precision, bool cut_after_dot_nulls)
    {
        TStr str_null;
        if constexpr (std::same_as<TStr, std::string>)		//	string
        {
            str_null = "0";
        }
        else	//	wstring
        {
            str_null = L"0";
        }

        TStream osstream;
        osstream.precision(precision);
        osstream << std::fixed << number;
        TStr str = osstream.str();

        bool erase_nulls = false;
        ul_zc not_null_front_length = str.size();
        if (cut_after_dot_nulls)	//  need to cut nulls after numbers behind the dot, if they are
        {
            not_null_front_length = str.find_last_not_of(str_null) + 1ull;	//	find first not null index from the end, and set it to substr count (+1)
            erase_nulls = not_null_front_length != str.size();
        }

        bool erase_minus = false;
        if (number <= 0.)						 //	str_null[0] take char null
            erase_minus = std::count(str.begin(), str.end(), str_null[0]) == (l_zc)str.size() - 2ll;	//	if count of symbols equal count of nulls minus 2(minus and dot), need erase minus

        if (erase_nulls && erase_minus) return str.substr(1ull, not_null_front_length - 1ull);	//	take string withought of minues and useles after dot nulls
        else if (erase_minus) return str.substr(1ull, str.size() - 1ull);		//	take string withought minus
        else if (erase_nulls) return str.substr(0ull, not_null_front_length);	//	take string withought useles after dot nulls
        else return str;
    }
};



// #include <ZC/ZC_Types.h>

// #include <string>
// #include <concepts>
// #include <cmath>

// template <typename T>
// concept ZC_cStrOrWStr = std::same_as<T, std::string> || std::same_as<T, std::wstring>;

// template <ZC_cStrOrWStr TStr>
// TStr ZC_DoubleToStrWithoughtEpsilone(d_zc number, ul_zc after_dot_count, bool cut_after_dot_nulls)
// {
// 	static TStr str_null_dot;
// 	static TStr str_minus;
// 	if constexpr (std::same_as<TStr, std::string>)
// 	{
// 		str_null_dot = "0.";
// 		str_minus = "-";
// 	}
// 	else
// 	{
// 		str_null_dot = L"0.";
// 		str_minus = L"-";
// 	}
	
// 	bool number_negative = number < 0.;
// 	if (number_negative) number *= -1.;

// 	d_zc decimal_to_left_number = number;
// 	ul_zc nulls_at_start = 0ull;
// 	for (ui_zc i = 0u; i < after_dot_count; ++i)
// 	{
// 		decimal_to_left_number *= 10.;
// 		if (l_zc(decimal_to_left_number) == 0ll) ++nulls_at_start;
// 	}

//     decimal_to_left_number = std::round(decimal_to_left_number);
//     ul_zc number_ulong = decimal_to_left_number;						//	HERE PROBLEM NEED USE DOUBLE OR OVERFLOW
	
// 	if (number_ulong == 0ull)	//	number is null
// 	{
// 		if (cut_after_dot_nulls) return str_null_dot;	//	don't need nulls after dot
// 	    TStr str_null(after_dot_count + 2ull, str_null_dot[0]);
// 	    str_null[1] = str_null_dot[1];	//	set dot
// 	    return str_null;
// 	}

//         //  to str/wstr
//     TStr number_str(nulls_at_start, str_null_dot[0]);	//	set with nulls at start
//     if constexpr (std::same_as<TStr, std::string>) number_str += std::to_string(number_ulong);
//     else number_str += std::to_wstring(number_ulong);

// 	ul_zc length_to_dot = number_str.size() - after_dot_count;
// 	length_to_dot == 0ull ? number_str.insert(0ull, str_null_dot) : number_str.insert(length_to_dot, &str_null_dot[1]);		//	insert dot or null dot if number in range (-1, 1)

// 	if (number_negative) number_str.insert(0ull, str_minus);

// 	if (cut_after_dot_nulls)	//  need to cut nulls after numbers behind the dot, if they are
// 	{
// 		ul_zc last_not_null_index = number_str.find_last_not_of(str_null_dot[0]) + 1ull;	//	find first not null index from the end, and set it to substr count (+1) 
// 			//	substr count not equal string size, need cut nulls at the end with substr
// 		if (last_not_null_index != number_str.size()) return number_str.substr(0ull, last_not_null_index);
// 	}
	
// 	return number_str;
// }