#include "aopch.h"
#include "strings.h"

namespace strings
{
    char asciitolower(char in)
    {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    }

    std::string lowercase(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), [](char c) { return asciitolower(c); });
        return result;
    }

    std::string join(const std::string& separator, const std::vector<std::string>& arr)
    {
        std::string str = "";
        for (std::vector<std::string>::size_type i = 0; i < arr.size(); ++i)
        {
            str += arr[i];
            if (i < arr.size() - 1)
                str += separator;
        }

        return str;
    }

    std::string replace_all(const std::string str, const std::string& old_str, const std::string& new_str)
    {
        std::string replaced_str = str;

        while (true)
        {
            if (replaced_str.find(old_str) == std::string::npos)
                break;

            replaced_str.replace(replaced_str.find(old_str), old_str.length(), new_str);
        }

        return replaced_str;
    }

    std::string trim(const std::string& str, const std::string& trim_char)
    {
        // find the first non-trim_char character
        const std::string::size_type str_begin = str.find_first_not_of(trim_char);

        if (str_begin == std::string::npos)
            return str;

        // find the last non-trim_char character
        const std::string::size_type str_end = str.find_last_not_of(trim_char);
        const std::string::size_type str_range = str_end - str_begin + 1;

        // return the substring without leading and trailing trim_char
        return str.substr(str_begin, str_range);
    }

    // `first_n` - first 'n' chars of the str
    // `last_n` - last 'n' chars of the str
    std::string trim(const std::string& str, const std::size_t& first_n, const std::size_t& last_n)
    {
        return str.substr(first_n, str.size() - last_n);
    }

    bool is_empty(const std::string& str)
    {
        return all_of(str.begin(), str.end(), ::isspace) || str.empty();
    }

    // return true if any string from an iterable is present in target string
    // `strict` argument checks if any string from the iterable is equal to the target string
    bool any(const std::string& str, const std::vector<std::string>& iter, const bool& strict)
    {
        for (const std::string& i : iter)
        {
            if (strict && str == i)
                return true;

            else if (!strict && str.find(i) != std::string::npos)
                return true;
        }

        return false;
    }

    bool any(const int& str, const std::vector<int>& iter)
    {
        for (const int& i : iter)
        {
            if (str == i)
                return true;
        }

        return false;
    }

    bool in_array(const std::string& str, const std::vector<std::string>& iter)
    {
        return std::find(iter.begin(), iter.end(), str) != iter.end();
    }

    // return false if any `str` is found not to be present in the iter
    bool only(const std::string& str, const std::vector<std::string>& iter)
    {
        for (int i = 0; i < str.size(); i++)
        {
            if (!in_array(std::string(1, str[i]), iter))
                return false;
        }

        return true;
    }
}
