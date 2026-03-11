#ifndef CREAMLIB_CRAFT_H
#define CREAMLIB_CRAFT_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifndef CREAM_MAX_ARG_LENGTH
#define CREAM_MAX_ARG_LENGTH 256
#endif

#define CREAM_DEFAULT_SLOTS 20
#define CREAM_DEFAULT_TXT_LENGTH 4098

#define CREAM_ARG_NEEDS_DATA 0x01
#define CREAM_ARG_IS_MANDATORY 0x02
#define _CREAM_ARG_HAS_CHECKS 0x04

typedef struct {
  size_t count;
  void* base;
  void* next;
  void* limit;
} _cream_buf;

typedef struct cream_arg cream_arg;

typedef struct {
  unsigned short length;
  unsigned char flags;
  unsigned int text;
} _cream_arg;

typedef struct {
  unsigned int parent_idx;
  unsigned int child_idx;
} _cream_relationship;

typedef struct {
  unsigned short max_args;
  unsigned short min_args;
  const unsigned char* available_values; //NULL means anything
} _cream_runtime_check;


struct _cream_buf _cream_args = {0};
struct _cream_buf _cream_text = {0};
struct _cream_buf _cream_rels = {0};
struct _cream_buf _cream_checks = {0};


bool _cream_check_table(_cream_table* _tb, const size_t _min_available, const size_t _increment = 0, const bool _zalloc = false)
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


struct _cream_arg* _cream_find_arg(const unsigned char *const _arg)
{
  bool _found = false;
  for(_cream_arg* _p = (_cream_arg*)_cream_args.base; _p < _cream_args.limit; _p++)
  {
    const unsigned char *_txt = (const unsigned char*)_cream_text.base + _p->text;
    while(_txt < _cream_text.limit && *_txt == *_arg)
    {
      if(_p->length == (unsigned int)(_txt - ((unsigned char*)_cream_args.base + _p->length) - && *_arg == '\0') //TODO: check this
      {
        _found = true;
        break;
      }
      _txt++;
      _arg++;
    }
    if(_found) return _p;
  }
  return NULL;
}


bool cream_craft_arg(const char *const _cream_new_arg, const unsigned char _flags = 0x00)
{
  if(!_cream_new_arg) return false;
  size_t _arg_len = strnlen(_cream_new_arg, CREAM_MAX_ARG_LENGTH);
  if(!(_cream_check_table(&_cream_args, sizeof(_cream_arg), sizeof(_cream_arg) * CREAM_DEFAULT_SLOTS, true)
        && _cream_check_table(&_cream_text, _arg_len, CREAM_DEFAULT_TEXT_LENGTH)
        && _arg_len > 0))
  {
    return false;
  }
  _cream_arg* _p = (struct _cream_arg*)_cream_args.next;
  _p->length = _arg_len;
  _p->text = (unsigned int*)_cream_text->next - (unsigned int)_cream_text->base;
  _p->flags = _flags;
  memcpy(_p->text, _cream_new_arg, _arg_len);
  (char*)_cream_text->next += _arg_len;
  _p++;
  return true;
}


bool cream_craft_subarg(const char *const _parent, const char *const _child, const unsigned char _flags = 0x00)
{
  struct _cream_arg *_parent_info, *_child_info;
  _parent_info = _cream_find_arg(_parent);
  if(!_cream_arg) return false;
  _child_info = _cream_find_arg(_child);
  if(!_child_info)
  {
    if(!cream_craft_arg(_child, _flags)) return false;
    _child_info = _cream_find_arg(_child);
  }
  if(!_cream_check_table(&_cream_rels, sizeof(_cream_reloationship), sizeof(_cream_relationship) * CREAM_DEFAULT_SLOTS, true))
    return false;
  struct _cream_relationship* _r = (_cream_relationship*)_cream_rels->next;
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
