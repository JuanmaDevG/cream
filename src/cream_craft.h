#ifndef CREAMLIB_CRAFT_H
#define CREAMLIB_CRAFT_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifndef CREAM_MAX_ARG_LENGTH
#define CREAM_MAX_ARG_LENGTH 256
#endif

#ifndef CREAM_DEFAULT_ARG_SLOTS
#define CREAM_DEFAULT_ARG_SLOTS 20
#endif

#ifndef CREAM_DEFAULT_TXT_BUF
#define CREAM_DEFAULT_TXT_BUF 4098
#endif


#define CREAM_ARG_NOFLAGS           0x00
#define CREAM_ARG_NEEDS_DATA        0x01
#define CREAM_ARG_IS_MANDATORY      0x02
#define CREAM_ARG_DENY_DUPLICATES   0x04
#define CREAM_ARG_ALLOW_EQUAL_OP    0x08   //-f=file.txt (the equalop value is not stored as data, it prevents cream from throwing an error)
#define CREAM_ARG_ALLOW_KEYWORD     0x10   //-ffile.txt (the keyword is not stored as data, is within the arg and can be an enum value)
#define CREAM_ARG_STACKABLE         0x20   //-czvf TODO: probably replace with arg groups (specific arguments that can be grouped into an option discarding first char)
#define CREAM_ARG_EXECUTES_ROUTINE  0x80   // void fun_ptr(struct receivedinfoaboutarg)


typedef struct {
  size_t count;
  void* base;
  void* next;
  void* limit;
} _cream_buf;

typedef struct cream_arg cream_arg;

typedef struct {
  unsigned short length;
  unsigned int text;
} cream_arg;

typedef struct {
  unsigned short parent_idx;
  unsigned short child_idx;
} cream_relationship;

typedef struct {
  unsigned char flags;
  unsigned short max_data;
  unsigned short min_data;
  unsigned short arg_id;
  const unsigned char* enum_values;
} cream_runtime_check;


struct _cream_buf _cream_args = {0};
struct _cream_buf _cream_text = {0};
struct _cream_buf _cream_rels = {0};
struct _cream_buf _cream_runtime_checks = {0};


bool _cream_craft_check_table(_cream_table* _tb, const size_t _min_available, const size_t _increment, const bool _zalloc)
{
  if((size_t)((char*)_tb->limit - (char*)_tb->next) >= _min_available)
    return true;

  off_t _next_idx = (char*)_tb->next - (char*)_tb->base;
  _tb->count += (_increment > 0 ? _increment : _min_available);
  _tb->base = realloc(_tb->base, _tb->count);
  if(!_tb->base) return false;
  _tb->next = ((char*)_tb->base) + _next_idx;
  _tb->limit = ((char*)_tb->base) + _tb->count;
  if(_zalloc)
      memset(_tb->next, 0, (unsigned char*)_tb->limit - (unsigned char*)_tb->next);
  return true;
}

#define CREAM_ID_NOT_FOUND 0xffff

unsigned short _cream_craft_find_arg(const unsigned char *const _arg)
{
  bool _found = false;
  unsigned short _id=0;
  for(cream_arg* _p = (cream_arg*)_cream_args.base; _p < _cream_args.limit && _p->length > 0; _p++, _id++)
  {
    const unsigned char *_txt = (const unsigned char*)_cream_text.base + _p->text;
    const unsigned char *const _txt_base = _txt;
    while(_txt < _cream_text.limit && *_txt == *_arg)
    {
      if(_txt == _txt_base + _p->length && *_arg == '\0')
      {
        _found = true;
        break;
      }
      _txt++;
      _arg++;
    }
    if(_found) return _id;
  }
  return CREAM_ID_ERROR;
}


//TODO: may delete the a lot of code because of allowing external definitions instead of allocating memory
void cream_craft_args(const cream_arg* _args)
{
  //TODO: register them to a pointer
}

void cream_craft_relationships(const cream_relationship* _rels)
{
}


void cream_craft_runtime_checks(const cream_runtime_check* _rchecks)
{
}

bool cream_craft_arg(const char *const _cream_new_arg)
{
  if(!_cream_new_arg) return false;
  size_t _arg_len = strnlen(_cream_new_arg, CREAM_MAX_ARG_LENGTH);
  if(!(_cream_craft_check_table(&_cream_args, sizeof(cream_arg), sizeof(struct cream_arg) * CREAM_DEFAULT_ARG_SLOTS, true)
        && _cream_craft_check_table(&_cream_text, _arg_len, CREAM_DEFAULT_TEXT_LENGTH, false)
        && _arg_len > 0))
  {
    return false;
  }
  cream_arg* _p = (struct cream_arg*)_cream_args.next;
  _p->length = _arg_len;
  _p->text = (unsigned char*)_cream_text->next - (unsigned char*)_cream_text->base;
  memcpy(_p->text, _cream_new_arg, _arg_len);
  (char*)_cream_text->next += _arg_len;
  _p++;
  return true;
}


bool cream_craft_subarg(const char *const _parent, const char *const _child)
{
  struct cream_arg *_parent_info, *_child_info;
  _parent_info = _cream_craft_find_arg(_parent);
  if(cream_arg == CREAM_ID_NOT_FOUND) return false;
  _child_info = _cream_craft_find_arg(_child);
  if(_child_info == CREAM_ID_NOT_FOUND)
  {
    if(!cream_craft_arg(_child)) return false;
    _child_info = _cream_craft_find_arg(_child);
  }
  if(!_cream_craft_check_table(
        &_cream_rels,
        sizeof(_cream_reloationship),
        sizeof(cream_relationship) * CREAM_DEFAULT_ARG_SLOTS, true))
  {
    return false;
  }
  struct cream_relationship* _r = (cream_relationship*)_cream_rels->next;
  _r->parent = _parent_info;
  _r->child = _child_info;
  _r++;
  _cream_rels.next = _r;
  return true;
}


bool cream_craft_runtime_check(const char *const _arg) //More param with future available checks
{
  return true;
}


bool cream_export(const char *const out_filename = "cream.args")
{
}

//struct sometype* cream_compact() instead of writing to file (for a compile-time executable), compact to an in-memory readable format for cream

#endif // CREAMLIB_CRAFT_H
