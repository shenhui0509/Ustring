#include <alogrithm> // std::max
#include <vector>
#include "pattern_search.h"

namespace ustr{
/* the threshold for trigger bm
 * TODO : determine the threshold by benchmark
 */
constexpr size_t threshold = 4096;
using std::vector;


bool PatternSearcher::Match(const string &text, const string &pattern)
{
  auto m = text.size(), n = pattern.size();
  if(m * n <= threshold)
    return text.find(pattern) != string::npos;
  return boyer_moore_internal(text, pattern, true) != string::npos;
}

size_t PatternSearcher::Count(const string &text, const string &pattern)
{
  return boyer_moore_internal(text, pattern, false);
}

size_t PatternSearcher::Find(const string &text, const string &pattern)
{
  auto m = text.size(), n = pattern.size();
  if(m * n <= threshold)
    return text.find(pattern);
  return boyer_moore_internal(text, pattern, true);
}

size_t PatternSearcher::RFind(const string &text, const string &pattern)
{
  auto m = text.size(), n = pattern.size();
  if(m * n <= threshold)
    return text.rfind(pattern);
  string r_text(text.rbegin(), text.rend()), r_pattern(pattern.rbegin(), pattern.rend());
  return m - boyer_moore_internal(r_text, r_pattern, true) - 1;
}

static array<int, ASCII_NUM> compile_bad_char(const string &pattern)
{
  array<int, ASCII_NUM> bad_char;
  bad_char.fill(-1);
  auto len = pattern.size();
  for(size_t i = 0; i < len; ++i){
    bad_char[static_cast<int>(pattern[i])] = i;
  }
  return bad_char;
}

static vector<int> suffix(const string &pattern)
{
  auto len = pattern.length();
  vector<int> suffix_(len, 0);
  suffix_[len - 1] = len;

  int f = 0, i = 0, g = len - 1;
  for(i = len - 2; i >= 0; --i){
    if(i > g && suffix_[i + len - 1 - f] < i - g)
      suffix_[i] = suffix_[i + len - 1 - f];
    else{
      if(i < g)
        g = i;
      f = i;
      while(g >= 0 && pattern[g] == pattern[g + len - 1 -f])
        --g;
      suffix_[i] = f - g;
    }
  }
  return suffix_;
}

static vector<int> compile_good_suffix(const string &pattern)
{
  auto suffix_ = suffix(pattern);
  auto len = pattern.size();
  vector<int> good_suffix(len, 0);
  int i = 0, j = 0;
  std::fill(good_suffix.begin(), good_suffix.end(), len);

  for(i = len - 1; i >= 0; --i){
    if(suffix_[i] == i + 1){
      for(; j < len - 1 - i; j++){
        if(good_suffix[j] == len)
          good_suffix[j] = len - 1 - i;
      }
    }
  }

  for(i = 0; i <= len - 2; ++i){
    good_suffix[len - 1 - suffix_[i]] = len - 1 - i;
  }
  return good_suffix;
}

size_t boyer_moore_internal(const string &text, const string &pattern, bool match)
{
    auto bad_char = compile_bad_char(pattern);
  auto good_suffix = compile_good_suffix(pattern);

  auto n = text.length(), m = pattern.length();
  size_t s = 0;
  size_t result = 0;
  while(s <= (n - m)){
    int j = m - 1;
    while(j >= 0 && pattern[j] == text[s + j])
      --j;
    if(j < 0){
      if(match)
        return s;
      ++result;
      s += good_suffix[0];
    } else {
      s += std::max(good_suffix[j], j - bad_char[static_cast<int>(text[s+j])]);
    }
  }
  return match ? string::npos : result;
}
}//namespace ustr
