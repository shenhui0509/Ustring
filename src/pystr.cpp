#include <algorithm>
#include <cctype>

#include "pystr.h"

namespace ustr{
inline std::string capitalize (const std::string &str)
{
    if (str.empty())
        return "";
    auto result = str;
    result[0] = std::toupper (result[0]);
    std::transform (std::begin (result)+1, std::end (result), std::begin (result)+1,
    [] (unsigned char c) {
        return std::tolower (c);
    });
    return result;
}

inline std::string center (const std::string &str, const size_type width, const char fillchar)
{
    if (width <= str.length()) {
        std::string result (str);
        return result;
    }
    std::string result (width, fillchar);
    auto start = std::begin (result);
    start += (width - str.length()) >> 1;
    std::copy (std::begin (str), std::end (str), start);
    return result;

}

inline std::string ljust(const std::string &str, size_type width, const char fillchar)
{
    if (width <= str.length()) {
        std::string result (str);
        return result;
    }
    std::string result(width, fillchar);
    std::copy(std::begin(str), std::end(str), std::begin(result));
    return result;
}

inline std::string rjust(const std::string &str, size_type width, const char fillchar)
{
    if (width <= str.length()) {
        std::string result(str);
        return result;
    }
    std::string result(width, fillchar);
    std::copy(std::begin(str), std::end(str), std::end(result) - str.length());
    return result;
}

inline std::string expandtabs(const std::string &str, size_type tabsize)
{
    std::string result(str);
    size_type i = 0, len = str.length();
    int j = 0, offset = 0;
    for (; i < len; ++i) {
        if (str[i] == '\t') {
            if (tabsize > 0) {
                int fillsize = tabsize - (j % tabsize);
                j += fillsize;
                result.replace(i + offset, 1, std::string(fillsize, ' '));
            } else {
                result.replace(i + offset, 1, "");
                offset -= 1;
            }
        } else {
            ++j;
            if (str[i] == '\n' || str[i] == '\r')
                j = 0;
        }
    }
    return result;
}

inline std::string lower(const std::string &str)
{
    std::string result(str.length(), '\0');
    transform(std::begin(str), std::end(str), result.begin(),
    [](unsigned char c) {
        return ::tolower(c);
    });
    return result;
}

inline std::string upper(const std::string &str)
{
    std::string result(str.length(), '\0');
    std::transform(std::begin(str), std::end(str), std::begin(result),
    [](unsigned char c) {
        return ::toupper(c);
    });
    return result;
}

inline std::string swapcase(const std::string &str)
{
    std::string result(str);
    std::transform(std::begin(result), std::end(result),
                   std::begin(result),
    [](unsigned char c) {
        return ::isupper(c) ? ::tolower(c) :
               ((::islower(c) ? ::toupper(c) : c));
    });
    return result;
}

inline std::string lstrip(const std::string &str, const std::string &chars)
{
    std::string result(str);
    size_type start = result.find_first_not_of(chars);
    result.erase(0, start);
    return result;
}

inline std::string rstrip(const std::string &str, const std::string &chars)
{
    std::string result(str);
    size_type end = result.find_last_not_of(chars);
    if (end == result.length())
        return result;
    result.erase(end + 1);
    return result;
}

inline std::string strip(const std::string &str, const std::string &chars)
{
    auto start = str.find_first_not_of(chars);
    auto end = str.find_last_not_of(chars);
    std::string result(str.substr(start, end - start + 1));
    return result;
}

inline std::string replace(const std::string &str, const std::string &old, const std::string &_new, size_type count)
{
    size_type i = 0;
    std::string result(str);
    while (i < str.length() && count--) {
        i = result.find(old, i);
        if (i == npos)
            return result;
        result.replace(i, old.length(), _new);
    }
    return result;
}

inline std::string zfill(const std::string &str, size_type width)
{
    if (width <= str.length())
        return str;
    std::string result(width, '0');
    if (str[0] == '+' || str[0] == '-') {
        result[0] = str[0];
        std::copy(std::begin(str) + 1, std::end(str),
                  std::end(result) - str.length() + 1);
    } else {
        std::copy(std::begin(str), std::end(str),
                  std::end(result) - str.length());
    }
    return result;
}

inline bool isalnum(const std::string &str)
{
    return std::all_of(std::begin(str), std::end(str),
    [](unsigned char c) {
        return ::isalnum(c);
    });
}

inline bool isalpha(const std::string &str)
{
    return std::all_of(std::begin(str), std::end(str),
    [](unsigned char c) {
        return ::isalpha(c);
    });
}

inline bool isdigit(const std::string &str)
{
    return std::all_of(std::begin(str), std::end(str),
    [](unsigned char c) {
        return ::isdigit(c);
    });
}

inline bool islower(const std::string &str)
{
    return std::all_of(std::begin(str), std::end(str),
    [](unsigned char c) {
        return ::islower(c);
    });
}

inline bool isupper(const std::string &str)
{
    return std::all_of(std::begin(str), std::end(str),
    [](unsigned char c) {
        return ::isupper(c);
    });
}

inline bool isspace(const std::string &str)
{
    return std::all_of(std::begin(str), std::end(str),
    [](unsigned char c) {
        return ::isspace(c);
    });
}

inline bool istitle(const std::string &str)
{
    char prev_char = ' ';
    return  std::all_of(std::begin(str), std::end(str),
                        [&prev_char](unsigned char c)
    -> bool {
        if (!::isalpha(prev_char)) {
            prev_char = c;
            return ::isupper(c);
        } else {
            prev_char = c;
            return (::islower(c) || !::isalpha(c));
        }
    });
}

inline bool endswith(const std::string &str, const std::vector<std::string> &suffix,
                     size_type start, size_type end)
{
    auto sub_str = str.substr(start, end == npos ? npos : (end - start));
    return std::any_of(std::begin(suffix), std::end(suffix),
    [&sub_str](const std::string& _suffix) {
        if (_suffix.length() > sub_str.length())
            return false;
        return sub_str.rfind(_suffix) == (sub_str.length() - _suffix.length());
    });
}

inline bool startswith(const std::string &str, const std::vector<std::string> &prefix,
                       size_type start, size_type end)
{
    auto sub_str = str.substr(start, end == npos ? npos : (end-start));
    return std::any_of(std::begin(prefix), std::end(prefix),
    [&sub_str](const std::string& _prefix) {
        if (_prefix.length() > sub_str.length())
            return false;
        return sub_str.find(_prefix) == 0;
    });
}

inline size_type find(const std::string &str, const std::string &pattern, size_type start, size_type end)
{
    auto substr = str.substr(start, end == npos ? npos : (end - start));
    auto index = substr.find(pattern);
    return index == npos ? npos : (start + index);
}

inline size_type rfind(const std::string &str, const std::string &pattern, size_type start, size_type end)
{
    auto substr = str.substr(start, end == npos ? npos : (end - start));
    auto index = substr.rfind(pattern);
    return index == npos ? npos : (start + index);
}

inline size_type count(const std::string &str, const std::string &pattern, size_type start, size_type end)
{
    auto substr = str.substr(start, end == npos ? npos : (end - start));
    size_type count = 0;
    size_type position = 0;
    while (1) {
        position = substr.find(pattern, position);
        if (position == npos)
            break;
        ++count;
        position += pattern.length();
    }
    return count;
}

inline std::string join(const std::string &str, const std::vector<std::string> &iterable)
{
    std::string result;
    std::for_each(std::begin(iterable), std::end(iterable) - 1,
    [&result, &str](const std::string element) {
        result.append(element + str);
    });
    result.append(*iterable.rbegin());
    return result;
}

inline std::tuple<std::string, std::string, std::string>
partition(const std::string &str, const std::string &par)
{
    auto index = str.find(par);
    if (index == npos)
        return std::make_tuple(str, "", "");
    return std::make_tuple(str.substr(0, index), par, str.substr(index + par.length()));
}

inline std::tuple<std::string, std::string, std::string>
rpartition(const std::string &str, const std::string &par)
{
    auto index = str.rfind(par);
    if (index == npos)
        return std::make_tuple("", "", str);
    return std::make_tuple(str.substr(0, index), par, str.substr(index + par.length()));
}

inline std::vector<std::string>
split(const std::string &str, const std::string &seperator, int maxsplit)
{
    std::vector<std::string> result;
    size_type pos = 0, prev_pos = 0;
    auto sep_len = seperator.length();
    while (maxsplit != 1) {
        prev_pos = pos;
        pos = str.find(seperator, pos);
        if (pos == npos)
            break;
        result.push_back(str.substr(prev_pos, pos - prev_pos));
        pos += sep_len;
        --maxsplit;
    }
    if (pos != npos)
        result.push_back(str.substr(pos, npos));
    return result;
}

inline std::vector<std::string>
rsplit(const std::string &str, const std::string &seperator, int maxsplit)
{
    std::vector<std::string> result;
    size_type pos = npos, prev_pos = 0;
    auto sep_len = seperator.length();
    while (maxsplit != 0) {
        prev_pos = pos;
        pos = rfind(str, seperator, 0, pos);
        if (pos == npos) {
            result.push_back(str.substr(0, prev_pos));
            break;
        }
        result.push_back(str.substr(pos + sep_len, prev_pos == npos ? npos : (prev_pos - pos - sep_len)));
        --maxsplit;
    }
    if (pos != npos)
        result.push_back(str.substr(0, pos));
    return result;
}

inline std::vector<std::string>
splitlines(const std::string &str, bool keepends)
{
    std::vector<std::string> result;
    size_type i = 0, j = 0, eol, len = str.length();
    for (; i < len; ++i) {
        while (i < len && str[i] != '\n' && str[i] != '\r') ++i;
        eol = i;
        if (i < len) {
            if (str[i] == '\r' && i + 1 < len && str[i+1] == '\n')
                i += 2;
            else
                ++i;
            if (keepends)
                eol = i;
        }
        result.push_back(str.substr(j, i));
        j = i;
    }
    if(j < len)
      result.push_back(str.substr(j, len - j));
    return result;
}

}//namespace ustr
