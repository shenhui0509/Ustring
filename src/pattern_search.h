#ifndef PATTERN_SEARCH_H_
#define PATTERN_SEARCH_H_

#include <string>
#include <array>
#include "utils/ctor_util.h"
/*
 * This is for naive string search,
 * given a 'text' and 'pattern', determine whether the 'pattern' is in the 'text'
 * nearly equal to std::string::find
 * The implement of std::string::find is naive BruteForce algorithm,
 * it's expect time complexity is O(MN), when 'M' and 'N' is small, it is ok to use stl
 * and it is of no extra space need
 *
 * However, if the time complexity is sensitive, while space complexity is not,
 * We can use some more efficient algorithms, i.e. KMP or BoyerMoore, which are O(M+N)
 */
namespace ustr{

constexpr int char_num = 256;

using std::string;
using std::array;
using size_type = string::size_type;

class PatternSearcher
{
public:
  static bool Match(const string &text, const string &pattern) noexcept;
  static size_t Count(const string &text, const string &pattern) noexcept;
  static size_t Find(const string &text, const string &pattern) noexcept;
  static size_t Rfind(const string &text, const string &pattern) noexcept;
private:
  static size_t boyer_moore_internal(const string &text, const string &pattern);
  static array<size_t, char_num> bad_chars;
  FORBID_COPY_AND_ASSIGN(PatternSearcher);
};
}//namespace ustr


#endif /* end of include guard: PATTERN_SEARCH_H_ */
