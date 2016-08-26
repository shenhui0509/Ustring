#ifnedf USTR_CHAR_CLASS_H_
#define USTR_CHAR_CLASS_H_

#include "rune.h"
#include "ctor_utils.h"
namespace ustr{
namespace re{

class CharClass{
public:
  void Delete();
  
  typedef RuneRange* iterator;
  iterator begin() {return ranges_;}
  iterator end()   {return ranges_ + nranges_;}
  
  int size() {return n_runes_;}
  bool empty() {return n_runes_ == 0; }
  bool full()   { return n_runes_ == Runemax + 1;}
  bool FoldsASCII {return folds_ascii_;}
  bool Contains(Rune r);
  CharClass* Negate();
private:
  CharClass();
  ~CharClass();
  static CharClass* New(int maxranges);
  friend class CharClassBuilder;
  bool folds_ascii_;
  int nrunes_;
  RuneRange *ranges_;
  int nranges_;
  
  FORBID_COPY_AND_ASSIGN(CharClass);
};
}//namespace re
}//namespace ustr
#endif
