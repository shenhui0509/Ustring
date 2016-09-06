#ifndef USTR_CHAR_CLASS_H_
#define USTR_CHAR_CLASS_H_
#include <set>
#include "rune.h"
#include "utils/ctor_util.h"

namespace ustr{
namespace re{

class CharClass{
public:
  void Delete();

  typedef RuneRange* iterator;
  iterator begin() {return ranges_;}
  iterator end()   {return ranges_ + nranges_;}

  int size() const noexcept {return nrunes_;}
  bool empty() const noexcept {return nrunes_ == 0; }
  bool full() const noexcept { return nrunes_ == Runemax + 1;}
  bool FoldsASCII() const noexcept {return folds_ascii_;}
  bool Contains(Rune r) const noexcept;
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

class CharClassBuilder
{
  public:
    CharClassBuilder();

    typedef std::set<RuneRange, RuneRangeLess> RuneRangeSet;
    typedef RuneRangeSet::iterator iterator;
    iterator begin() {return ranges_.begin();}
    iterator end() {return ranges_.end();}

    int size() const noexcept {return nrunes_;}
    bool empty() const noexcept {return nrunes_ == 0;}
    bool full() const noexcept {return nrunes_  == Runemax + 1;}

    bool contains(Rune r);
    bool FoldsASCII();
    bool AddRange(Rune lo, Rune hi);
    CharClassBuilder* copy();
    void AddCharClass(CharClassBuilder *ccb);
    void Negeate();
    void RemoveAbove(Rune r);

    CharClass *GetCharClass();
  private:
    static const uint32_t AlphaMask = (1 << 26) - 1;
    uint32_t upper_;
    uint32_t lower_;
    int nrunes_;
    RuneRangeSet ranges_;
    FORBID_COPY_AND_ASSIGN(CharClassBuilder);
};

}//namespace re
}//namespace ustr
#endif
