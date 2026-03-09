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

typedef struct cream_arg cream_arg;

typedef struct {
  unsigned short _arg_length;
  unsigned char* _data;
  bool _mandatory;
} _cream_arg;

typedef struct {
  _cream_arg* parent;
  _cream_arg* child;
} cream_relationship;

static size_t _cream_args_cap = 0;
static struct _cream_arg* _cream_arg_block = NULL;
static struct _cream_arg* _cream_next_arg = NULL;
static const struct _cream_arg* _cream_arg_block_limit = NULL;

static size_t _cream_txt_size = 0;
static unsigned char* _cream_txt_block = NULL;
static unsigned char* _cream_next_string = NULL;
static const unsigned char* _cream_txt_block_limit = NULL;

static size_t _cream_rel_count = 0;
static cream_relationship* _cream_rel_table = NULL;


bool _cream_check_argtables(const size_t _new_text_size = 0)
{
  if(_cream_next_arg == _cream_arg_block_limit)
  {
    _cream_args_cap += CREAM_DEFAULT_SLOTS;
    _cream_arg_block = realloc(_cream_arg_block, _cream_args_cap);
    //TODO: recalculate all pointers
  }
  if(_cream_next_string == _cream_txt_block_limit
      || (_cream_txt_block_limit - _cream_next_string) < _new_text_size)
  {
  }
  return true;
}

bool _cream_check_reltables()
{
}


void cream_init()
{
  _cream_args_cap = CREAM_DEFAULT_SLOTS;
  _cream_arg_block = (struct cream_arg*)calloc(sizeof(cream_arg) * CREAM_DEFAULT_SLOTS);
  _cream_next_arg = _cream_arg_block;
  _cream_arg_block_limit = _cream_arg_block + _cream_args_cap;
  _cream_txt_size = CREAM_DEFAULT_TXT_LENGTH;
  _cream_txt_block = (unsigned char*)malloc(CREAM_DEFAULT_TXT_LENGTH);
  _cream_next_string = _cream_txt_block;
  _cream_txt_block_limit = _cream_txt_block + _cream_txt_size;
}

bool cream_make_arg(const char *const _cream_new_arg)
{
  if(!_cream_arg_block)
    return false;

  size_t arg_len = strnlen(_cream_new_arg, CREAM_MAX_ARG_LENGTH);
  cream_check_argtables(arg_len);
  _cream_next_arg->_arg_length = arg_len;
}


//TODO: first version of this will manage children with simple allocations
//  Later may be more improvements after measuring
void cream_make_subarg(const char *const _cream_parent, const char *const _cream_child)
{
}


bool cream_write(const char *const out_filename = "cream.args")
{
}

#endif
