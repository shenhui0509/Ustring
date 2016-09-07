#include <vector>
#include "utils/logging.h"
#include "char_class.h"

namespace ustr
{
namespace re
{

CharClass* CharClass::New(int maxranges)
{
  CharClass *cc;
  uint8_t *data = new uint8_t[sizeof *cc, maxranges * sizeof cc->ranges_[0]];
  cc = reinterpret_cast<CharClass*>(data);
  cc->ranges_ = reinterpret_cast<RuneRange*>(data + sizeof *cc);
  cc->nranges_ = 0;
  cc->folds_ascii_ = false;
  cc->nrunes_ = 0;
  return cc;
}

void CharClass::Delete()
{
  uint8_t *data = reinterpret_cast<uint8_t*>(this);
  delete[] data;
}

CharClass* CharClass::Negate()
{
  CharClass *cc = CharClass::New(nranges_ + 1);
  cc->folds_ascii_ = folds_ascii_;
  cc->nrunes_ = Runemax + 1 - nrunes_;
  int n = 0;
  int next_lo = 0;
  for(CharClass::iterator it = begin(); it != end(); ++it){
    if(it->lo == next_lo){
      next_lo = it->hi + 1;
    } else {
      cc->ranges_[n++] = RuneRange(next_lo, it->lo - 1);
      next_lo = it->hi + 1;
    }
  }
  if(next_lo <= Runemax){
    cc->ranges_[n++] = RuneRange(next_lo, Runemax);
  }
  cc->nranges_ = n;
  return cc;
}

/* *****************************CharClassBuilder**************** */

CharClassBuilder::CharClassBuilder() :
  upper_(0), lower_(0), nrunes_(0)
{}

bool CharClass::Contains(Rune r) const noexcept
{
  RuneRange *rr = ranges_;
  int n = nranges_;
  while(n > 0){
    int m = n >> 1;
    if(rr[m].hi < r){
      rr += m+1;
      n -= m+1;
    } else if(r < rr[m].lo){
      n = m;
    } else {
      return true;
    }
  }
  return false;
}

CharClass* CharClassBuilder::GetCharClass()
{
  CharClass *cc = CharClass::New(static_cast<int>(ranges_.size()));
  int n = 0;
  for(auto it = begin(); it != end(); ++it)
    cc->ranges_[n++] = *it;
  cc->nranges_ = n;
  DCHECK_LE(n, static_cast<int>(ranges_.size()));
  cc->nrunes_ = nrunes_;
  cc->folds_ascii_ = FoldsASCII();
  return cc;
}

bool CharClassBuilder::contains(Rune r)
{
  return ranges_.find(RuneRange(r,r)) != end();
}

bool CharClassBuilder::FoldsASCII()
{
  return ((upper_ ^ lower_) & AlphaMask) == 0;
}

CharClassBuilder* CharClassBuilder::copy()
{
  CharClassBuilder *ccb = new CharClassBuilder;
  for(auto it = begin(); it != end(); ++it)
    ccb->ranges_.insert(RuneRange(it->lo, it->hi));
  ccb->upper_ = upper_;
  ccb->lower_ = lower_;
  ccb->nrunes_ = nrunes_;
  return ccb;
}

void CharClassBuilder::RemoveAbove(Rune r)
{
  if(r >= Runemax)
    return;

  if(r < 'z'){
    if(r < 'a')
      lower_ = 0;
    else
      lower_ &= AlphaMask >> ('z' - r);
  }

  if(r < 'Z'){
    if(r < 'A')
      upper_ = 0;
    else
      upper_ &= AlphaMask >> ('Z' - r);
  }

  for(;;){
    auto it = ranges_.find(RuneRange(r+1, Runemax));
    if(it == end())
      break;
    RuneRange rr = *it;
    ranges_.erase(it);
    nrunes_ -= rr.hi - rr.lo + 1;
    if(rr.lo <= r){
      rr.hi = r;
      ranges_.insert(rr);
      nrunes_+= rr.hi - rr.lo + 1;
    }
  }
}

bool CharClassBuilder::AddRange(Rune lo, Rune hi)
{
  if(hi < lo)
    return false;

  if(lo <= 'z' && hi >= 'A'){
    Rune lo1 = std::max<Rune>(lo, 'A');
    Rune hi1 = std::min<Rune>(hi, 'Z');
    if(lo1 <= hi1)
      upper_ |= ((1 << (hi1 - lo1 + 1)) - 1) << (lo1 - 'A');
    lo1 = std::max<Rune>(lo, 'a');
    hi1 = std::min<Rune>(hi, 'z');
    if(lo1 <= hi1)
      lower_ |= ((1 << (hi1 - lo1 + 1)) - 1) << (lo1 - 'a');
  }

  {
    auto it = ranges_.find(RuneRange(lo, lo));
    if(it != end() && it->lo <= lo && it->hi >= hi)
      return false;
  }

  if(lo > 0){
    auto it = ranges_.find(RuneRange(lo-1, lo-1));
    if(it != end()){
      lo = it->lo;
      if(it->hi > hi)
        hi = it->hi;
      nrunes_ -= it->hi - it->lo + 1;
      ranges_.erase(it);
    }
  }

  if(hi < Runemax){
    auto it = ranges_.find(RuneRange(hi + 1, hi + 1));
    if(it != end()){
      hi = it->hi;
      nrunes_ -= it->hi - it->lo + 1;
      ranges_.erase(it);
    }
  }
  for(;;){
    auto it = ranges_.find(RuneRange(lo, hi));
    if(it == end())
      break;
    nrunes_ -= it->hi - it->lo + 1;
    ranges_.erase(it);
  }

  nrunes_ += hi - lo + 1;
  ranges_.insert(RuneRange(lo, hi));
  return true;
}

void CharClassBuilder::AddCharClass(CharClassBuilder *cc)
{
  for(auto it = cc->begin(); it != cc->end(); ++it)
    AddRange(it->lo, it->hi);
}

void CharClassBuilder::Negate()
{
  std::vector<RuneRange> v;
  v.reserve(ranges_.size() + 1);

  auto it = begin();
  if(it == end()){
    v.push_back(RuneRange(0, Runemax));
  } else {
    int next_lo = 0;
    if(it->lo == 0){
      next_lo = it->hi + 1;
      ++it;
    }
    for(; it != end(); ++it){
      v.push_back(RuneRange(next_lo, it->lo - 1));
      next_lo = it->hi + 1;
    }
    if(next_lo <= Runemax){
      v.push_back(RuneRange(next_lo, Runemax));
    }
  }

  ranges_.clear();
  for(auto r : v)
    ranges_.insert(r);
  upper_ = AlphaMask & ~upper_;
  lower_ = AlphaMask & ~lower_;
  nrunes_ = Runemax + 1 - nrunes_;
}


}//namespace re
}//namespace ustr
