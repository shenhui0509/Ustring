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
  return boyer_moore_internal(text, pattern) != 0;
}
}//namespace ustr
