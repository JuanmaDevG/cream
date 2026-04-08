#ifndef CREAMLIB_H
#define CREAMLIB_H

#include <cstdlib>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Flags for struct cream_arg_opt

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

#define attach_result_pointer(prev_field, field, data_type, count)             \
  if (count > 0) {                                                             \
    _res->field = (data_type *)_res->prev_field;                               \
    _res->field##_end = _res->field + count;                                   \
  }

const char *cream_usage_message = NULL;

struct _cream_info_datavec {
  unsigned short max_elems;
  unsigned short min_elems;
};

struct _cream_info_subcommand {
  struct cream_arg_opt *children; // NULL ending
};

struct _cream_info_keyword_host {
  const unsigned char *values; // separated by ; (and nullchar at the end)
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
  unsigned char data[];
};

void _count_types(const cream_arg_opt *_opts, unsigned int *_bools,
                  unsigned int *_datavecs, unsigned int *_kwhosts,
                  unsigned int *_subcommands) {
  *_bools = *_datavecs = *_kwhosts = *_subcommands = 0;
  for (const cream_arg_opt *_i = _opts; _i->text != NULL; _i++) {
    switch (_i->flags & 0x03) {
    case CREAM_TYPE_BOOLEAN:
      (*_bools)++;
      break;
    case CREAM_TYPE_DATAVEC:
      (*_datavecs)++;
      break;
    case CREAM_TYPE_KEYWORD_HOST:
      (*_kwhosts)++;
      break;
    case CREAM_TYPE_SUBCOMMAND:
      (*_subcommands)++;
      break;
    }
  }
}

#define _CREAM_ARGCHECK_NO_COINCIDENCE ((const char *)NULL)
#define _CREAM_ARGCHECK_OK ((const char *)1)

const char *_arg_check(const char *_arg_cli, const cream_arg_opt *_opt) {
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
    else {
      if (skip_equals && _arg_cli[i] != '=') {
        fprintf(stderr, "\n%s\n", cream_usage_message);
        fprintf(stderr, "Argument option %s syntax is: %s=value\n", _opt->text,
                _opt->text);
        exit(1);
      }
      return _arg_cli + i + (skip_equals ? 1 : 0);
    }
  }
normal_loop:
  for (int i = 0; _arg_cli[i] != '\0' && _opt->text[i] != '\0'; i++) {
    if (_arg_cli[i] != _opt->text[i])
      return _CREAM_ARGCHECK_NO_COINCIDENCE;
  }
  return _CREAM_ARGCHECK_OK;
}

void _register_argument(const int _rem_argc, const char **_rem_argv,
                        const cream_arg_opt *_opt, cream_result *_res) {
  switch (_opt->flags & 0x03) {
  case CREAM_TYPE_BOOLEAN:
    break;
  case CREAM_TYPE_DATAVEC:
    break;
  case CREAM_TYPE_KEYWORD_HOST:
    break;
  case CREAM_TYPE_SUBCOMMAND:
    break;
  }
}

cream_result *cream_parse(const int argc, const char *argv[],
                          const cream_arg_opt *_arg_options) {
  unsigned int bools_count, datavecs_count, subcommands_count, kwhosts_count,
      anon_args_count;
  _count_types(_arg_options, &bools_count, &datavecs_count, &kwhosts_count,
               &subcommands_count);

  cream_result *_res = (cream_result *)malloc(
      sizeof(cream_result) + (sizeof(cream_bool) * bools_count) +
      (sizeof(cream_datavec) * datavecs_count) +
      (sizeof(cream_kwhost) * kwhosts_count) +
      (sizeof(cream_subcommand) * subcommands_count) +
      (sizeof(char *) * anon_args_count));

  if (!_res) {
    perror("There is not enough memory to start argument parsing");
    return NULL;
  }

  memset(_res, 0, sizeof(cream_result));
  attach_result_pointer(data, bools, cream_bool, bools_count);
  attach_result_pointer(bools_end, datavecs, cream_datavec, datavecs_count);
  attach_result_pointer(datavecs_end, kwhosts, cream_kwhost, kwhosts_count);
  attach_result_pointer(kwhosts_end, subcommands, cream_subcommand,
                        subcommands_count);
  attach_result_pointer(subcommands_end, anonymous_args, const char *,
                        anon_args_count);

  for (unsigned int _i = 0; _i < argc; _i++) {
    const char *cur_arg = argv[_i];
    bool is_anon = false;
    for (const cream_arg_opt *_opt = _arg_options; _opt->text != NULL; _opt++) {
      const char *delim = _arg_check(cur_arg, _opt);
      if (delim == _CREAM_ARGCHECK_NO_COINCIDENCE)
        continue;
      // Is an option, receive info
      _register_argument(argc - _i, argv + _i, _opt, _res);
      is_anon = true;
      break;
    }
    if (is_anon) {
      // TODO: register as anonymous argument
    }
  }

  return _res;
}

void cream_free(cream_result *_r) { free(_r); }

#endif // CREAMLIB_H
