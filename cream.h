#ifndef CREAMLIB_H
#define CREAMLIB_H

#include <cstdlib>
#include <memory.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
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

// TODO: error system

// ===================
// = Defined by user =
// ===================

struct cream_info_datavec {
  uint16_t max_elems;
  uint16_t min_elems;
};

struct cream_info_subcommand {
  struct cream_option *child_opts; // NULL ending
};

struct cream_info_keyword_host {
  const char **values;
};

union cream_type_info {
  struct cream_info_datavec datavec;
  struct cream_info_subcommand subcommand;
  struct cream_info_keyword_host kwhost;
};

struct cream_option {
  uint8_t flags;
  const char *text;
  union cream_type_info info;
  void (*run_oncheck)(const union cream_argtype);
};

#define cream_no_arg {.text = NULL}

// =====================
// = Returned by cream =
// =====================

struct cream_bool {
  const char *opt;
};

struct cream_datavec {
  uint16_t size;
  const char *opt;
  const char **data;
};

struct cream_kwhost {
  const char *opt;
  const char *keyword;
};

struct cream_subcommand {
  size_t bools_count, datavecs_count, kwhosts_count, subcommands_count,
      anon_args_count;
  struct cream_bool *bools;
  struct cream_datavec *datavecs;
  struct cream_kwhost *kwhosts;
  struct cream_subcommand *subcommands;
  const char **anonymous_args;
  char data[];
};

typedef struct cream_subcommand cream_result;

// =====================
// = Used in callbacks =
// =====================

union cream_argtype {
  struct cream_bool boolean;
  struct cream_datavec datavec;
  struct cream_kwhost kwhost;
  struct cream_subcommand subcommand;
};

struct _cream_subcommand_metadata {
  size_t size;
  size_t cur_bool, cur_datavec, cur_kwhost, cur_subcommand, cur_anon;
  size_t bools_typecount, datavecs_typecount, kwhosts_typecount,
      subcommands_typecount, anon_args_typecount;
};

struct _cream_runtime_binding {
  const struct cream_option *opt;
  struct _cream_subcommand_metadata metadata;
};

struct _cream_runtime_context {
  size_t subcommands;
  size_t arg_opts;
};

// ==============
// = Code Logic =
// ==============

size_t _cream_count_subcommands(const struct cream_option *opts) {
  if (!opts)
    return 0;

  size_t result = 1; // Current argument result is the main subcommand (appname)
  for (const struct cream_option *o = opts; o->text != NULL; o++) {
    if (o->flags & CREAM_TYPE_SUBCOMMAND) {
      result++;
      if (!o->info.subcommand.child_opts)
        continue;
      for (const struct cream_option *sub_o = o->info.subcommand.child_opts;
           sub_o->text != NULL; sub_o++) {
        result += _cream_count_subcommands(opts);
      }
    }
  }

  return result;
}

void __cream_fill_runtime_bindings(_cream_runtime_binding **rtbs,
                                   const cream_option *opts,
                                   const _cream_runtime_context *ctx) {
  // Current context block
  (*rtbs)->metadata.anon_args_typecount = 10;
  (*rtbs)->metadata.bools_typecount = 0;
  (*rtbs)->metadata.datavecs_typecount = 0;
  (*rtbs)->metadata.kwhosts_typecount = 0;
  (*rtbs)->metadata.subcommands_typecount = 0;
  (*rtbs)->metadata.cur_bool = 0;
  (*rtbs)->metadata.cur_datavec = 0;
  (*rtbs)->metadata.cur_kwhost = 0;
  (*rtbs)->metadata.cur_subcommand = 0;
  (*rtbs)->metadata.cur_anon = 0;

  for (int i = 0; i < ctx->arg_opts; i++) {
    switch (opts[i].flags & CREAM_FLAGS_TYPE) {
    case CREAM_TYPE_BOOLEAN:
      ((*rtbs)->metadata.bools_typecount)++;
      break;
    case CREAM_TYPE_DATAVEC:
      ((*rtbs)->metadata.datavecs_typecount)++;
      break;
    case CREAM_TYPE_KEYWORD_HOST:
      ((*rtbs)->metadata.kwhosts_typecount)++;
      break;
    case CREAM_TYPE_SUBCOMMAND:
      ((*rtbs)->metadata.subcommands_typecount)++;
      break;
    }
  }
  (*rtbs)->metadata.size =
      sizeof(struct cream_subcommand) +
      (sizeof(struct cream_bool) * (*rtbs)->metadata.bools_typecount) +
      (sizeof(struct cream_datavec) * (*rtbs)->metadata.datavecs_typecount) +
      (sizeof(struct cream_kwhost) * (*rtbs)->metadata.kwhosts_typecount) +
      (sizeof(char *) * (*rtbs)->metadata.anon_args_typecount);

  for (int i = 0; i < ctx->arg_opts; i++) {
    if (opts[i].flags & CREAM_TYPE_SUBCOMMAND) {
      (*rtbs)++;
      (*rtbs)->opt = &opts[i];
      __cream_fill_runtime_bindings(rtbs, opts[i].info.subcommand.child_opts,
                                    ctx);
    }
  }
}

void _cream_fill_runtime_bindings(_cream_runtime_binding *rtbs,
                                  const cream_option *opts,
                                  const _cream_runtime_context *ctx) {
  rtbs[0].opt = NULL;
  _cream_runtime_binding *cur_rtb = rtbs;
  __cream_fill_runtime_bindings(&rtbs, opts, ctx);
}

struct _cream_runtime_binding *
_cream_get_runtime_bindings(const struct cream_option *opts,
                            _cream_runtime_context *ctx) {
  struct _cream_runtime_binding *rtbs;
  ctx->subcommands = _cream_count_subcommands(opts);

  rtbs = (struct _cream_runtime_binding *)malloc(
      sizeof(struct _cream_runtime_binding) * ctx->subcommands);
  if (!rtbs)
    return NULL;

  _cream_fill_runtime_bindings(rtbs, opts, ctx);
  return rtbs;
}

struct cream_subcommand *
_cream_alloc_subcommands(const cream_option *opts,
                         struct _cream_runtime_binding *rtbs,
                         const struct _cream_runtime_context *ctx) {
  struct cream_subcommand *result =
      (struct cream_subcommand *)calloc(1, rtbs[0].metadata.size);

  if (!result) {
    perror("There is not enough memory to start argument parsing");
    return NULL;
  }

  result->bools = (struct cream_bool *)result->data;
  result->datavecs =
      (struct cream_datavec *)(result->bools + rtdat->bools_typecount);
  result->kwhosts =
      (struct cream_kwhost *)(result->datavecs + rtdat->datavecs_typecount);
  result->subcommands =
      (struct cream_subcommand *)(result->kwhosts + rtdat->kwhosts_typecount);
  result->anonymous_args =
      (const char **)(result->subcommands + rtdat->subcommands_typecount);

  // TODO: before subcommands alloc, multiple runtime datas

  return result;
}

struct cream_option *_cream_find_opt(const char *arg,
                                     const struct cream_option *opts) {
  struct cream_option *result = NULL;

  while (!result && opts->text != NULL) {
    if ((opts->flags & CREAM_KWTYPE_EQUALOP) |
        (opts->flags & CREAM_KWTYPE_EMBEDDED)) { // TODO: substitute for XOR
      result = (struct cream_option *)opts;
      for (int i = 0; opts->text[i] != '\0' && arg[i] != '\0'; i++) {
        if (opts->text[i] != arg[i] && opts->text[i] != '\0') {
          result = NULL;
        }
      }
    } else if (strcmp(opts->text, arg) == 0)
      result = (struct cream_option *)opts;
    opts++;
  }

  return result;
}

void _cream_guarantee_space(struct cream_subcommand **result,
                            const struct cream_option *opts,
                            struct _cream_subcommand_metadata *rtdat,
                            const void *const cur_elem,
                            const void *const buf_end, size_t *max_attrs,
                            const size_t new_elems, const size_t elem_size) {

  off_t offset = new_elems * elem_size;
  if ((char *)buf_end - (char *)cur_elem >= offset) {
    return;
  }

  *max_attrs +=
      (offset - (size_t)((char *)buf_end - (char *)cur_elem)) / elem_size;
  rtdat->size += offset;
  *result = (struct cream_subcommand *)realloc(*result, rtdat->size);

  char *writepoint = (*result)->data + rtdat->size - 1;
  const char *const limit = (const char *const)buf_end,
                    *readpoint = writepoint - offset;
  while (limit > readpoint) {
    *writepoint = *readpoint;
    readpoint--;
    writepoint--;
  }

  (*result)->bools = (struct cream_bool *)(*result)->data;
  (*result)->datavecs =
      (struct cream_datavec *)((*result)->bools + rtdat->bools_typecount);
  (*result)->kwhosts =
      (struct cream_kwhost *)((*result)->datavecs + rtdat->datavecs_typecount);
  (*result)->subcommands =
      (cream_subcommand *)((*result)->kwhosts + rtdat->kwhosts_typecount);
  (*result)->anonymous_args =
      (const char **)((*result)->subcommands + rtdat->subcommands_typecount);
}

void _cream_register_opt(struct cream_subcommand *result,
                         const cream_option *opt, cream_option *opts,
                         _cream_subcommand_metadata *rtdat, const char **argv,
                         int *iter, const int argc) {

  switch (opt->flags & CREAM_FLAGS_TYPE) {
  case CREAM_TYPE_BOOLEAN:
    _cream_guarantee_space(&result, opts, rtdat,
                           result->bools + rtdat->cur_bool,
                           result->bools + rtdat->bools_typecount,
                           &rtdat->bools_typecount, 1, sizeof(cream_bool));
    result->bools[rtdat->cur_bool].opt = opt->text;
    break;
  case CREAM_TYPE_DATAVEC:
    _cream_guarantee_space(
        &result, opts, rtdat, result->datavecs + rtdat->cur_datavec,
        result->datavecs + rtdat->datavecs_typecount,
        &rtdat->datavecs_typecount, 1, sizeof(cream_datavec));
    result->datavecs[rtdat->cur_datavec].opt = opt->text;

    result->datavecs[rtdat->cur_datavec].data = argv + 1;
    result->datavecs[rtdat->cur_datavec].size = 0;
    (*iter)++; // Ignore option name
    while (argv + *iter < argv + argc && !_cream_find_opt(argv[*iter], opts)) {
      result->datavecs[rtdat->cur_datavec].size++;
      if (result->datavecs[rtdat->cur_datavec].size >
          opt->info.datavec.max_elems) {
        // TODO: design the error system
      }
      (*iter)++;
    }
    if (result->datavecs[rtdat->cur_datavec].size <
        opt->info.datavec.min_elems) {
      // TODO: design the error system
    }
    break;
  case CREAM_TYPE_KEYWORD_HOST:
    _cream_guarantee_space(&result, opts, rtdat,
                           result->kwhosts + rtdat->cur_kwhost,
                           result->kwhosts + rtdat->kwhosts_typecount,
                           &rtdat->kwhosts_typecount, 1, sizeof(cream_kwhost));
    result->kwhosts[rtdat->cur_kwhost].opt = opt->text;

    if (opt->flags & CREAM_KWTYPE_EMBEDDED) {
      for (int i = 0; opt->text[i] != '\0' && argv[(*iter)][i] != '\0'; i++) {
        if (opt->text[i] != argv[(*iter)][i]) {
          result->kwhosts[rtdat->cur_kwhost].keyword = argv[(*iter)] + i;
          break;
        }
      }
    } else if (opt->flags & CREAM_KWTYPE_EQUALOP) {
      for (int i = 0; argv[(*iter)][i] != '\0'; i++) {
        if (argv[(*iter)][i] == '=') {
          result->kwhosts[rtdat->cur_kwhost].keyword = argv[(*iter)] + i + 1;
          break;
        }
      }
    } else if (opt->flags & CREAM_KWTYPE_SEPARATE) {
      (*iter)++;
      result->kwhosts[rtdat->cur_kwhost].keyword = NULL;
      for (const char **p = opt->info.kwhost.values; *p != NULL; p++) {
        if (strcmp(*p, argv[*iter]) == 0) {
          result->kwhosts[rtdat->cur_kwhost].keyword = *p;
          break;
        }
      }
      if (result->kwhosts[rtdat->cur_kwhost].keyword == NULL) {
        // TODO: error keyword host argument, incomplete argument
      }
    }
    break;
  case CREAM_TYPE_SUBCOMMAND:
    _cream_guarantee_space(
        &result, opts, rtdat, result->subcommands + rtdat->cur_subcommand,
        result->subcommands + rtdat->subcommands_typecount,
        &rtdat->subcommands_typecount, 1, sizeof(cream_subcommand));
    // TODO: allocate subcommands separately
    // TODO: subcommands should be pointers allocated in different buffers
    break;
  }

  // TODO: check generic flags
}

cream_result *cream_parse(const int argc, const char *argv[],
                          const struct cream_option *opts) {
  struct _cream_runtime_context ctx = {0, 0};
  for (const struct cream_option *o = opts; o->text != NULL; o++)
    ctx.arg_opts++;
  struct _cream_runtime_binding *rtbs = _cream_get_runtime_bindings(opts);
  struct _cream_subcommand_metadata rtdat = _cream_get_runtime_data(opts);
  struct cream_subcommand *result = _cream_alloc_subcommands(opts, rtdat);

  for (unsigned int i = 0; i < argc; i++) {
    cream_option *found_opt = _cream_find_opt(argv[i], opts);
    if (!found_opt) {
      _cream_guarantee_space(&result, opts, &rtdat,
                             result->anonymous_args + result->anon_args_count,
                             result->anonymous_args + rtdat.anon_args_typecount,
                             &rtdat.anon_args_typecount, 1, sizeof(char **));
      result->anonymous_args[rtdat.cur_anon] = argv[i];
      rtdat.cur_anon++;
      continue;
    }
    _cream_register_opt(result, found_opt, opts, &rtdat);
  }
  return result;
}

void cream_free(cream_result *_r) { /* TODO: free subcommands */ }

#endif // CREAMLIB_H
