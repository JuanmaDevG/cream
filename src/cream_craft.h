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


typedef struct cream_arg {
  unsigned short _arg_length;
  cream_arg* _children;
  unsigned char* _data;
} cream_arg;

static size_t _cream_argc = 0;
static struct cream_arg* _cream_arg_block = NULL;
static struct cream_arg* _cream_next_arg = NULL;
static const struct cream_arg* _cream_arg_block_limit = NULL;
static size_t _cream_txt_size = 0;
static unsigned char* _cream_txt_block = NULL;
static unsigned char* _cream_next_string = NULL;
static const unsigned char* _cream_txt_block_limit = NULL;


void cream_init()
{
  _cream_argc = CREAM_DEFAULT_SLOTS;
  _cream_arg_block = (struct cream_arg*)calloc(sizeof(cream_arg) * CREAM_DEFAULT_SLOTS);
  _cream_next_arg = _cream_arg_block;
  _cream_arg_block_limit = _cream_arg_block + _cream_argc;
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
  if(_cream_next_arg == _cream_arg_block_limit)
  {
    //resize
  }
  if(_cream_next_string == _cream_txt_block_limit)
  {
    //resize
  }
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
