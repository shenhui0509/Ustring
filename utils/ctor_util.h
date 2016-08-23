#ifndef USTR_CTOR_UTIL_H_
#define USTR_CTOR_UTIL_H_

#define FORBID_COPY_AND_ASSIGN(type)    \
private:                                \
  type(const type&) = delete;           \
  type& operator=(const type&) = delete
  
#define DEFAULT_COPY_AND_ASSIGN(type)   \
public:                                 \
  type(const type&) = default;          \
  type& operator=(const type&) = default 
  
#define DEFAULT_MOVE_AND_MOVEASSIGN     \
public:                                 \
  type(type&&) = default;               \
  type& operator=(type&&) = default
  
#endif
