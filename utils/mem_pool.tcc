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
typename MemoryPool<T, block_size>::slot_pointer
MemoryPool<T, block_size>::_cur_block = nullptr;

template<class T, size_t block_size>
typename MemoryPool<T, block_size>::slot_pointer
MemoryPool<T, block_size>::_cur_slot = nullptr;

template<class T, size_t block_size>
typename MemoryPool<T, block_size>::slot_pointer
MemoryPool<T, block_size>::_last_slot = nullptr;

template<class T, size_t block_size>
typename MemoryPool<T, block_size>::slot_pointer
MemoryPool<T, block_size>::_free_slot = nullptr;

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


}//namespace ustr
#endif
