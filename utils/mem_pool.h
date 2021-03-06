#ifndef USTR_MEM_POOL_H_
#define USTR_MEM_POOL_H_

#include <climits>
#include <cstddef>
#include "ctor_util.h"

namespace ustr
{
/* memmory pool for small size obejects
 * especially for 'node' for the FSM(NFA/DFA)
 * 'T' : the type that the memory pool handles
 * 'block_size' : default size for initialize the pool and allocate extra blocks from system
 */

template<class T, size_t block_size>
class MemoryPool
{
public:
  typedef T         value_type;
  typedef T&        reference;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef const T&  const_reference;
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;

  MemoryPool() noexcept;
  DEFAULT_MOVE_AND_MOVEASSIGN(MemoryPool);
  ~MemoryPool() noexcept;

  template <class U> struct rebind { typedef MemoryPool<U, block_size> other; };

  pointer address(reference x) noexcept;
  const_pointer address(const_reference x) noexcept;

  pointer allocate(size_type n = 1, const_pointer hint = 0);
  void deallocate(pointer p, size_type n = 1);
  size_type max_size() noexcept;

  template<class U, class... Args>
  void construct(U *p, Args &&... args);
  template<class U>
  void destroy(U *p);

  template<class... Args>
  pointer new_element(Args&&... args);
  void delete_element(pointer p);

private:
  union slot{
    value_type element;
    slot *next;
  };
  typedef char *data_pointer;
  typedef slot slot_type;
  typedef slot* slot_pointer;

  slot_pointer cur_block_;
  slot_pointer cur_slot_;
  slot_pointer last_slot_;
  slot_pointer free_slot_;

  size_type pad_pointer(data_pointer p, size_type align) noexcept;
  void allocate_block();

  static_assert(block_size >= 2 * sizeof(slot_type), "need a larger block size!");
  FORBID_COPY_AND_ASSIGN(MemoryPool);
};
}//namespace ustr

#include "mem_pool.tcc"
#endif
