#ifndef USTR_RE_STATE_H_
#define USTR_RE_STATE_H_

namespace ustr{
  namespace re{
    /* The characters can be represents by range rather than a single char,
     * for e.g., we usea uint16_t(since the w_char is size of 16 bits) to represent the range,
     * Without range based char representation, the expression [a-z] will generate 26 edges for each single characters,
     * of course you can use a multimap, however it is not the most efficient way,
     * The range based char representation is to address this problem.
     * [a-z] can be represented in range [97-122](uint16_t low = 'a'; uint16_t high = 'z';)
     */
     
    class CharClass{
    };
  };
};
#endif
