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

  Maskx = (1 << Bitx) - 1,
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
  if(c1 & Testx)
    goto bad;
  if(c < T3){
    if(c < T2)
      goto bad;
    l = ((c << Bitx) | c1) & Rune2;
    if(l <= Rune1)
    	goto bad;
    return 2;
  }

  c2 = *(unsigned char*)(str+2) ^ Tx;
  if(c2 & Testx)
  	goto bad;
  if(c < T4){
  	l = (((( c << Bitx ) | c1) << Bitx) | c2) & Rune3;
  	if(l <= Rune2)
  		goto bad;
  	*rune = l;
  	return 3;
  }

  c3 = *(unsigned char*)(str + 3) ^ Tx;
  if(c3 & Testx)
  	goto bad;
  if(c < T5){
  	l = ((((((c << Bitx) | c1) << Bitx) | c2) << Bitx) | c3) & Rune4;
  	if (l <= Rune3)
			goto bad;
		*rune = l;
		return 4;
  }

bad:
	*rune = Bad;
	return 1;
}

int rune2char(char *str, const Rune *rune)
{
	unsigned long c;

	/* one character sequence
	 * 00000-0007f --> 00-7f
	 */
	 c = *rune;
	 if(c <= Rune1){
	 	str[0] = static_cast<char>(c);
	 	return 1;
	 }

	 /* 2 characters sequence
	  * 0080-07ff --> T2 Tx
	  */
	  if(c <= Rune2){
	  	str[0] = Tx | static_cast<char>(c >> 1 * Bitx);
	  	str[1] = Tx | (c & Maskx);
	  	return 2;
	  }

	  if(c > Runemax)
	  	c = Runeerror;

	  /* 3 characters sequence
	   * 0x800-FFFF --> T3 Tx Tx
	   */
	   if(c <= Rune3){
	   		str[0] = T3 | static_cast<char>(c >> 2 * Bitx);
	   		str[1] = Tx | ((c >> 1 * Bitx) & Maskx);
	   		str[2] = Tx | (c & Maskx);
	   		return 3;
	   }

	   /* 4 characters sequence
	    * 10000-1ffff -> T4 Tx Tx Tx
	    */
	    str[0] = T4 | static_cast<char>(c >> 3 * Bitx);
	    str[1] = Tx | ((c >> 2 * Bitx) & Maskx);
	    str[2] = Tx | ((c >> 1 * Bitx) & Maskx);
	    str[3] = Tx | (c & Maskx);
	    return 4;

}

int runelen(Rune rune)
{
	char str[10];
	return rune2char(str, &rune);
}

int fullrune(const char *str, int n)
{
	if(n > 0){
		int c = *(unsigned char*)str;
		if(c < Tx)
			return 1;
		if(n > 1){
			if(c < T3)
				return 1;
			if(n > 2){
				if(c < T4 || n > 3)
					return 1;
			}
		}
	}
	return 0;
}

int utflen(const char *s)
{
	int c;
	long n;
	Rune rune;
	n = 0;
	for(;;){
		c = *(unsigned char*)s;
		if(c < Runeself){
			if(c == 0)
				return n;
			s++;
		} else {
			s += char2rune(&rune, s);
		}
		n++;
	}
	return 0;
}

char* utfrune(const char *s, Rune c)
{
	long c1;
	Rune r;
	int n;

	if(c < Runesync)
		return strchr((char*)s, c);

	for(;;){
		c1 = *(unsigned char*)s;
		if(c1 < Runeself){
			if(c1 == 0)
				return 0;
			if(c1 == c)
				return (char*)s;
			s++;
			continue;
		}
		n = char2rune(&r, s);
		if(r == c)
			return (char*)s;
		s += n;
	}
	return 0;
}
}//namespace re
}//namespace ustr
