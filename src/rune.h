#ifndef USTR_RUNE_H_
#define USTR_RUNE_H_

#include <cstdint>

namespace ustr {
  namespace re{
    /* for wide char representation */

    typedef signed int Rune; /* 4 byte */

    struct RuneRange {
      RuneRange() : lo(0), hi(0) {}
      RuneRange(int l, int h) : lo(l), hi(h) {}
      Rune lo;
      Rune hi;
    };

    struct RuneRangeLess
    {
      bool operator()(const RuneRange &a, const RuneRange &b)
      {
        return a.hi < b.lo;
      }
    };
    enum {
      UTFMax = 4, /* maximum bytes per rune*/
      Runesync = 0x80, /* cannot represent of a UTF sequence */
      Runeself = 0x80, /* rune and the utf sequence are the same */
      Runeerror = 0xfffd, /* decoding error in utf */
      Runemax = 0x10ffff /* max rune value, now Unicode 4.0 are 21bits wide */
    };

    int rune2char(char *s, const Rune *r);
    int char2run(Rune *r, const char *s);
    int fullrune(const char *s, int n);
    int utflen(const char *s);
    char* utfrune(const char*, Rune);
  }//namespace re
}//namespace ustr
#endif
