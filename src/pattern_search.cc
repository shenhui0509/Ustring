#include <alogrithm> // std::max
#include "pattern_search.h"

namespace ustr{
/* the threshold for trigger bm
 * TODO : determine the threshold by benchmark
 */
constexpr size_t threshold = 4096;

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

}//namespace ustr
