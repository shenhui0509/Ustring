#ifndef PATTERN_SEARCH_H_
#define PATTERN_SEARCH_H_

#include <string>
#include <vector>

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

}//namespace ustr


#endif /* end of include guard: PATTERN_SEARCH_H_ */
