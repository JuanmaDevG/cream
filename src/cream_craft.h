#ifndef CREAMLIB_CRAFT_H
#define CREAMLIB_CRAFT_H

#include <stdbool.h>

#ifndef CREAM_BUF_MAX_SIZE
#define CREAM_BUF_MAX_SIZE 4098
#endif


typedef struct cream_arg {
  unsigned short _arg_length;
  cream_arg* _children;
  size_t _data_size;
  unsigned char _data[];
} cream_arg;

static unsigned char _cream_buf[CREAM_BUF_MAX_SIZE];
static unsigned char* _cream_main_block;


//TODO: main function for shared object named "cream()" that loads the object
void cream_make_arg(const char *const _cream_new_arg)
{
  //TODO: use a big fixed size buffer
}


void cream_make_subarg(const char *const _cream_parent, const char *const _cream_child)
{
  //TODO: think about later about the data structure
}


bool cream_write(const char *const out_filename = "cream.args")
{
}

#endif
