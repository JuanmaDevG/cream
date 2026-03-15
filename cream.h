#ifndef CREAMLIB_H
#define CREAMLIB_H

#include <stdbool.h>
#include <stdlib.h>


// Flags for struct cream_arg_opt

// 0b00000011 for type info
#define CREAM_TYPE_BOOLEAN              0x00
#define CREAM_TYPE_DATAVEC              0x01  /* -f file.txt file2.txt */
#define CREAM_TYPE_SUBCOMMAND           0x02  /* appname subcommand --subcommand-arg... */
#define CREAM_TYPE_KEYWORD_HOST         0x03  /* -f <unroll-loops|fast-math|...> */

// 0b00001100 use with KEYWORD_HOST only (otherwise ignored)
#define CREAM_KWTYPE_SEPARATE           0x04  /* -f fast-math */
#define CREAM_KWTYPE_EMBEDDED           0x08  /* -ffast-math */
#define CREAM_KWTYPE_EQUALOP            0x0c  /* -f=fast-math */

// 0b11110000 general options
#define CREAM_ARG_IS_MANDATORY          0x10  /* alert error system if argument not present */
#define CREAM_ARG_DENY_DUPLICATES       0x20  /* alert error system if duplicate found */


struct _cream_info_datavec {
  unsigned short max_elems;
  unsigned short min_elems;
};

struct _cream_info_subcommand {
  struct cream_arg_opt *children;   // NULL ending
};

struct _cream_info_keyword_host {
  const unsigned char* values; // separated by ; (and nullchar at the end)
};

union cream_type_info {
  struct _cream_info_datavec datavec;
  struct _cream_info_subcommand subcommand;
  struct _cream_info_keyword_host kwhost;
};

struct cream_bool {
  const char *opt;
};

struct cream_datavec {
  unsigned short size;
  const char *opt;
  const char *data[];
};

struct cream_kwhost {
  const char *opt;
  const char *keyword;
};

struct cream_subcommand {
  const char **anonymous_args;
  struct cream_bool *bools;
  struct cream_datavec *datavecs;
  struct cream_subcommand *subcommands;
  struct cream_kwhost *kwhosts;
};

union cream_argtype {
  struct cream_bool boolean;
  struct cream_datavec datavec;
  struct cream_kwhost kwhost;
  struct cream_subcommand subcommand;
};

struct cream_arg_opt {
  unsigned char flags;
  const unsigned char *text;
  union cream_type_info info;
  void (run_oncheck*)(const union cream_argtype);
};

#define cream_no_arg {.text = NULL}
struct cream_result {
  const char **anonymous_args;
  struct cream_bool *bools;
  struct cream_datavec *datavecs;
  struct cream_subcommand *subcommands;
  struct cream_kwhost *kwhosts;
  const unsigned char data[];
};


cream_result* cream_parse(const int argc, const char *argv[], const cream_arg_opt *_arg_options)
{
  size_t _r_size, _r_avail;
  //TODO: alloc the cream_result and monitor byte placement and available
}


void cream_free(cream_result* _r)
{
  free(_r); // May change
}


#endif // CREAMLIB_H
