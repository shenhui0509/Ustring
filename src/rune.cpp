#include <cstdarg>
#include <cstring>

#include "rune.h"

namespace ustr{
namespace re{

enum
{
  Bit1    = 7,
  Bitx    = 6,
  Bit2    = 5,
  Bit3    = 4,
  Bit4    = 3,
  Bit5    = 2,
  
  T1 = ((1 << (Bit1 + 1)) - 1) ^ 0xff, /* 0000 0000*/
  Tx = ((1 << (Bitx + 1)) - 1) ^ 0xff, /* 1000 0000*/
  T2 = ((1 << (Bit2 + 1)) - 1) ^ 0xff, /* 1100 0000*/
  T3 = ((1 << (Bit3 + 1)) - 1) ^ 0xff, /* 1110 0000*/
  T4 = ((1 << (Bit4 + 1)) - 1) ^ 0xff, /* 1111 0000*/
  T5 = ((1 << (Bit5 + 1)) - 1) ^ 0xff, /* 1111 1000*/
  
  Rune1	= (1<<(Bit1+0*Bitx))-1,		/* 0000 0000 0111 1111 */
	Rune2	= (1<<(Bit2+1*Bitx))-1,		/* 0000 0111 1111 1111 */
	Rune3	= (1<<(Bit3+2*Bitx))-1,		/* 1111 1111 1111 1111 */
	Rune4	= (1<<(Bit4+3*Bitx))-1,   /* 0001 1111 1111 1111 1111 1111*/
  
  Maskx = (1 << Bitx) - 1.
  Testx = Maskx ^ 0xff,
  Bad = Runeerror,
};

int char2rune(Rune *rune, const char *str)
{
  int c, c1, c2, c3;
  long l;
  
  c = *(unsigned char*)str;
  if(c < Tx){
    *rune = c;
    return 1;
  }
  
  c1 = *(unsigned char*)(str+1) ^ Tx;
  if(c1 & Textx)
    goto bad;
  if(c < T3){
    if(c < T2)
      goto bad;
    l = ((c << Bitx) | c1) & Rune2;
  }
}
}//namespace re
}//namespace ustr
