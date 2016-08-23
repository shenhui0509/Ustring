#ifndef USTR_MEM_POOL_TCC_
#define USTR_MEM_POOL_TCC_

namespace ustr
{
template<class T, size_t block_size>
inline typename MemoryPool<T, block_size>::size_type
MemoryPool<T, block_size>::pad_pointer(data_pointer p,  size_type align)
{
  uintptr_t result = reinterpret_cast<uintptr_t>(p);
  return ((align - result) % align);
}

template<class T, size_t block_size>
typename MemoryPool<T, block_size>::MemoryPool() noexcept :
  cur_block_(nullptr),
  cur_slot_(nullptr),
  last_slot_(nullptr),
  free_slot_(nullptr)
{}

template<class T, size_t block_size>
MemoryPool::~MemoryPool() noexcept
{
  slot_pointer curr = cur_block_;
  while(curr != nullptr){
    slot_pointer prev = curr->next;
    operator delete ( reinterpret_cast<void *>(curr) );
    curr = prev;
  }
}

template <class T, size_t block_size>
inline typename MemoryPool<T, block_size>::pointer
MemoryPool<T, block_size>::address(reference x) noexcept
{
  return &x;
}

template <class T, size_t block_size>
inline typename MemoryPool<T, block_size>::const_pointer
MemoryPool<T, block_size>::address(const_reference x) noexcept
{
  return &x;
}

template<class T, size_t block_size>
void MemoryPool<T,block_size>::allocate_block()
{ 
  data_pointer new_block =
      reinterpret_cast<data_pointer>(operator new(block_size));
  reinterpret_cast<slot_pointer>(new_block)->next = cur_block_;
  cur_block_ = reinterpret_cast<slot_pointer>(new_block);
  data_pointer body = new_block + sizeof(slot_pointer);
  size_type body_padding = pad_pointer(body, alignof(slot_type));
  cur_slot_ = reinterpret_cast<slot_pointer>(body + body_padding);
  last_slot_ = reinterpret_cast<slot_pointer>(new_block + block_size - sizeof(slot_type) + 1);
}

template<class T, size_t block_size>
inline typename MemoryPool<T, block_size>::pointer
MemoryPool<T, block_size>::allocate(size_type n, const pointer hint)
{
  if(n == 1 && free_slots != nullptr){
    pointer result = reinterpret_cast<pointer>(free_slots);
    free_slots = free_slots->next;
    reinterpret_cast<slot_type>(result)->next = nullptr;
    return result;
  }
  if(current_slot + n >= last_slot){
    allocate_block();
  }

  /* Link the block memory into list*/
  /* Convinience for free*/
  slot_pointer p = current_slot;
  for(size_type i = 0; i < n - 1; i++){
    (p + i)->next = (p + i + 1);
  }
  (p + n)->next = nullptr;

  pointer result = reinterpret_cast<pointer>(current_slot);
  current_slot += n;
  return result;
}

template<class T, size_t block_size>
inline void  MemoryPool<T,block_size>::deallocate(pointer ptr, size_type n)
{
  if(!ptr)
    return;
    
  slot_pointer head = reinterpret_cast<slot_pointer>(p);
  slot_pointer p = head;
  while(p->next)
    p = p->next;
    
  p->next = free_slot_;
  free_slot_ = head;
}

template<class T, size_t block_size>
inline typename MemoryPool<T, block_size>::size_type
MemoryPool<T, block_size>::max_size()
const noexcept
{
  size_type max_block = -1 / block_size;
  return (block_size - sizeof(data_pointer)) / sizeof(slot_type) * max_block;
}

template<typename T, size_type block_size>
template<typename U, class... Args>
inline void
MemoryPool<T, block_size>::construct(U *p, Args&&... args)
{
  new(p) U (std::forward<Args>(args)...);
}

template<class T, size_type block_size>
template<class U>
inline void
MemoryPool<T, block_size>::destroy(U *p)
{
  p->~U();
}

template<class T, size_t block_size>
template<class... Args>
inline typename MemoryPool<T,block_size>::pointer
MemoryPool<T,block_size>::new_element(Args&&... args)
{
  pointer p = allocate();
  construct<value_type>(p, std::forward<Args>(args)...);
  return p;
}

template<class T, size_t block_size>
inline void 
MemoryPool<T, block_size>::delete_element(pointer p)
{
  if(!p)
    return;
    
  p->~value_type();
  deallocate(p);
}

}//namespace ustr
#endif
