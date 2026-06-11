#ifndef CREAMLIB_H
#define CREAMLIB_H

#include <cstdlib>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Flags for struct cream_option

#define CREAM_FLAGS_TYPE 0x03
#define CREAM_FLAGS_KWTYPE 0x0c
#define CREAM_FLAGS_GENERIC 0xf0

// 0b00000011 for type info
#define CREAM_TYPE_BOOLEAN 0x00
#define CREAM_TYPE_DATAVEC 0x01      /* -f file.txt file2.txt */
#define CREAM_TYPE_SUBCOMMAND 0x02   /* appname subcommand --subcommand-arg...*/
#define CREAM_TYPE_KEYWORD_HOST 0x03 /* -f <unroll-loops|fast-math|...> */

// 0b00001100 use with KEYWORD_HOST only (otherwise ignored)
#define CREAM_KWTYPE_SEPARATE 0x04 /* -f fast-math */
#define CREAM_KWTYPE_EMBEDDED 0x08 /* -ffast-math */
#define CREAM_KWTYPE_EQUALOP 0x0c  /* -f=fast-math */

// 0b11110000 general options
#define CREAM_ARG_IS_MANDATORY 0x10    /* alert error sys if not set */
#define CREAM_ARG_DENY_DUPLICATES 0x20 /* alert error sys if duplicate */

#define _cream_stack_rtdat_ptr(prev_field, field, data_type, count)            \
  if (count > 0) {                                                             \
    result->field = (data_type *)result->prev_field;                           \
    result->field##_end = result->field + count;                               \
  }

const char *cream_usage_message = NULL;

struct _cream_info_datavec {
  unsigned short max_elems;
  unsigned short min_elems;
};

struct _cream_info_subcommand {
  struct cream_option *children; // NULL ending
};

struct _cream_info_keyword_host {
  const char **values;
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

struct cream_option {
  unsigned char flags;
  const char *text;
  union cream_type_info info;
  void (*run_oncheck)(const union cream_argtype);
};

#define cream_no_arg {.text = NULL}

struct cream_result {
  const char **anonymous_args;
  const char **anonymous_args_end;
  struct cream_bool *bools;
  struct cream_bool *bools_end;
  struct cream_datavec *datavecs;
  struct cream_datavec *datavecs_end;
  struct cream_kwhost *kwhosts;
  struct cream_kwhost *kwhosts_end;
  struct cream_subcommand *subcommands;
  struct cream_subcommand *subcommands_end;
  char data[];
};

struct _cream_runtime_data {
  size_t result_size;
  cream_bool *cur_bool;
  cream_datavec *cur_datavec;
  cream_kwhost *cur_kwhost;
  cream_subcommand *cur_subcommand;
  const char **cur_anon;
  size_t bools, datavecs, kwhosts, subcommands, anon_args;
};

struct _cream_runtime_data
_cream_get_runtime_data(const struct cream_option *opts) {
  struct _cream_runtime_data rtdat;
  rtdat.anon_args = 10;
  rtdat.bools = rtdat.datavecs = rtdat.kwhosts = rtdat.subcommands = 0;
  for (const struct cream_option *_i = opts; _i->text != NULL; _i++) {
    switch (_i->flags & CREAM_FLAGS_TYPE) {
    case CREAM_TYPE_BOOLEAN:
      (rtdat.bools)++;
      break;
    case CREAM_TYPE_DATAVEC:
      (rtdat.datavecs)++;
      break;
    case CREAM_TYPE_KEYWORD_HOST:
      (rtdat.kwhosts)++;
      break;
    case CREAM_TYPE_SUBCOMMAND:
      (rtdat.subcommands)++;
      break;
    }
  }

  rtdat.result_size = sizeof(cream_result) +
                      (sizeof(cream_bool) * rtdat.bools) +
                      (sizeof(cream_datavec) * rtdat.datavecs) +
                      (sizeof(cream_kwhost) * rtdat.kwhosts) +
                      (sizeof(cream_subcommand) * rtdat.subcommands) +
                      (sizeof(char *) * rtdat.anon_args);
  return rtdat;
}

struct cream_result *_cream_setup_result(const cream_option *opts,
                                         _cream_runtime_data *rtdat) {
  cream_result *result = (cream_result *)calloc(1, rtdat->result_size);

  if (!result) {
    perror("There is not enough memory to start argument parsing");
    return NULL;
  }

  memset(result, 0, sizeof(cream_result));
  _cream_stack_rtdat_ptr(data, bools, cream_bool, rtdat->bools);
  _cream_stack_rtdat_ptr(bools_end, datavecs, cream_datavec, rtdat->datavecs);
  _cream_stack_rtdat_ptr(datavecs_end, kwhosts, cream_kwhost, rtdat->kwhosts);
  _cream_stack_rtdat_ptr(kwhosts_end, subcommands, cream_subcommand,
                         rtdat->subcommands);
  _cream_stack_rtdat_ptr(subcommands_end, anonymous_args, const char *,
                         rtdat->anon_args);

  rtdat->cur_bool = result->bools;
  rtdat->cur_datavec = result->datavecs;
  rtdat->cur_kwhost = result->kwhosts;
  rtdat->cur_subcommand = result->subcommands;
  rtdat->cur_anon = result->anonymous_args;

  return result;
}

// CRAP code (just as reference)
// ================================================================================================================
// ================================================================================================================
#define _CREAM_ARGCHECK_NO_COINCIDENCE ((const char *)NULL)
#define _CREAM_ARGCHECK_OK ((const char *)1)

const char *_arg_check(const char *_arg_cli, const cream_option *_opt) {
  if ((_opt->flags & CREAM_FLAGS_TYPE) == CREAM_TYPE_KEYWORD_HOST) {
    bool skip_equals = false;
    if ((_opt->flags & CREAM_FLAGS_KWTYPE) == CREAM_KWTYPE_EQUALOP) {
      skip_equals = true;
    } else if ((_opt->flags & CREAM_FLAGS_KWTYPE) != CREAM_KWTYPE_EMBEDDED) {
      goto normal_loop;
    }

    unsigned int i = 0;
    while (!(_arg_cli[i] == '\0' || _opt->text[i] == '\0')) {
      if (_opt->text[i] != _arg_cli[i])
        return _CREAM_ARGCHECK_NO_COINCIDENCE;
      i++;
    }
    if (_opt->text[i] != '\0' && _arg_cli[i] == '\0')
      return _CREAM_ARGCHECK_NO_COINCIDENCE;

    if (skip_equals && _arg_cli[i] != '=') {
      fprintf(stderr, "\n%s\n", cream_usage_message);
      fprintf(stderr, "Argument option %s syntax is: %s=value\n", _opt->text,
              _opt->text);
      exit(1);
    }
    return _arg_cli + i + (skip_equals ? 1 : 0);
  }
normal_loop:
  for (int i = 0; _arg_cli[i] != '\0' && _opt->text[i] != '\0'; i++) {
    if (_arg_cli[i] != _opt->text[i])
      return _CREAM_ARGCHECK_NO_COINCIDENCE;
  }
  return _CREAM_ARGCHECK_OK;
}
// ================================================================================================================
// ================================================================================================================

struct cream_option *_cream_find_opt(const char *arg,
                                     const struct cream_option *opts) {
  cream_option *result = NULL;

  // TODO: search logic

  return result;
}

void _cream_guarantee_space(struct cream_result **result,
                            const struct cream_option *opts,
                            struct _cream_runtime_data *rtdat,
                            const void *const start, const void *const finish,
                            size_t *update_counter, const size_t new_elems,
                            const size_t elem_size) {
  if ((unsigned char *)finish - (unsigned char *)start >=
      new_elems * elem_size) {
    return;
  }

  off_t offset = new_elems * elem_size;
  rtdat->result_size += offset;
  *result = (struct cream_result *)realloc(*result, rtdat->result_size);
  // TODO: recopy all from finish to start
  // do not memcpy, copy byte by byte because forward copy could break the
  // following elements
}

cream_result *cream_parse(const int argc, const char *argv[],
                          const struct cream_option *opts) {
  struct _cream_runtime_data rtdat = _cream_get_runtime_data(opts);
  struct cream_result *result = _cream_setup_result(opts, &rtdat);

  for (unsigned int i = 0; i < argc; i++) {
    cream_option *found_opt = _cream_find_opt(argv[i], opts);
    if (!found_opt) {
      _cream_guarantee_space(&result, opts, &rtdat, result->anonymous_args,
                             result->anonymous_args_end, &rtdat.anon_args, 1,
                             sizeof(const char **));
      rtdat.cur_anon = argv + i;
      rtdat.cur_anon++;
      continue;
    }
  }
  return result;
}

void cream_free(cream_result *_r) { free(_r); }

#endif // CREAMLIB_H
