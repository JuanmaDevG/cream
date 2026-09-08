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
  const char *name;
  size_t bools_count, datavecs_count, kwhosts_count, subcommands_count,
      anon_args_count;
  struct cream_bool *bools;
  struct cream_datavec *datavecs;
  struct cream_kwhost *kwhosts;
  struct cream_subcommand **subcommands;
  const char **anonymous_args;
  char data[];
};

typedef struct cream_subcommand cream_result;

// Config flags
#define CREAM_DISABLE_USAGE_MESSAGE 0x01
#define CREAM_CUSTOM_USAGE_MESSAGE 0x02
#define CREAM_DISABLE_ERROR_MESSAGE 0x04
#define CREAM_CUSTOM_ERROR_MESSAGE 0x08
#define CREAM_EXIT_ON_ERROR 0x10

#define CREAM_SHUT_UP 0x05
#define CREAM_SHUT_UP_AND_EXIT 0x15

struct cream_config {
  uint8_t flags;
  char *error_msg;
  char *usage_msg;
};

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
  size_t bools_capacity, datavecs_capacity, kwhosts_capacity,
      subcommands_capacity, anon_args_capacity;
};

struct _cream_runtime_binding {
  const struct cream_option *opt;
  struct _cream_subcommand_metadata metadata;
};

struct _cream_context {
  size_t subcommands;
  size_t arg_opts;
};

// =======================
// = Internal code logic =
// =======================

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
                                   const _cream_context *ctx) {
  // Current context block
  (*rtbs)->metadata.anon_args_capacity = 10;
  (*rtbs)->metadata.bools_capacity = 0;
  (*rtbs)->metadata.datavecs_capacity = 0;
  (*rtbs)->metadata.kwhosts_capacity = 0;
  (*rtbs)->metadata.subcommands_capacity = 0;
  (*rtbs)->metadata.cur_bool = 0;
  (*rtbs)->metadata.cur_datavec = 0;
  (*rtbs)->metadata.cur_kwhost = 0;
  (*rtbs)->metadata.cur_subcommand = 0;
  (*rtbs)->metadata.cur_anon = 0;

  for (const struct cream_option *o = opts; o->text != NULL; o++) {
    switch (o->flags & CREAM_FLAGS_TYPE) {
    case CREAM_TYPE_BOOLEAN:
      ((*rtbs)->metadata.bools_capacity)++;
      break;
    case CREAM_TYPE_DATAVEC:
      ((*rtbs)->metadata.datavecs_capacity)++;
      break;
    case CREAM_TYPE_KEYWORD_HOST:
      ((*rtbs)->metadata.kwhosts_capacity)++;
      break;
    case CREAM_TYPE_SUBCOMMAND:
      ((*rtbs)->metadata.subcommands_capacity)++;
      break;
    }
  }
  (*rtbs)->metadata.size =
      sizeof(struct cream_subcommand) +
      (sizeof(struct cream_bool) * (*rtbs)->metadata.bools_capacity) +
      (sizeof(struct cream_datavec) * (*rtbs)->metadata.datavecs_capacity) +
      (sizeof(struct cream_kwhost) * (*rtbs)->metadata.kwhosts_capacity) +
      (sizeof(char *) * (*rtbs)->metadata.anon_args_capacity);

  for (const struct cream_option *o = opts; o->text != NULL; o++) {
    if (o->flags & CREAM_TYPE_SUBCOMMAND) {
      (*rtbs)++;
      (*rtbs)->opt = o;
      __cream_fill_runtime_bindings(rtbs, o->info.subcommand.child_opts, ctx);
    }
  }
}

void _cream_fill_runtime_bindings(_cream_runtime_binding *rtbs,
                                  const cream_option *opts,
                                  const _cream_context *ctx) {
  rtbs[0].opt = NULL;
  _cream_runtime_binding *cur_rtb = rtbs;
  __cream_fill_runtime_bindings(&rtbs, opts, ctx);
}

struct _cream_runtime_binding *
_cream_get_runtime_bindings(const struct cream_option *opts,
                            _cream_context *ctx) {
  struct _cream_runtime_binding *rtbs;
  ctx->subcommands = _cream_count_subcommands(opts);

  rtbs = (struct _cream_runtime_binding *)malloc(
      sizeof(struct _cream_runtime_binding) * ctx->subcommands);
  if (!rtbs)
    return NULL;

  _cream_fill_runtime_bindings(rtbs, opts, ctx);
  return rtbs;
}

struct _cream_runtime_binding *
_cream_find_rtb(const struct _cream_runtime_binding *rtbs,
                const cream_option *opt, const struct _cream_context *ctx) {
  for (int i = 0; i < ctx->subcommands; i++) {
    if (opt == rtbs[i].opt)
      return (struct _cream_runtime_binding *)rtbs + i;
  }
  return NULL;
}

struct cream_subcommand *
_cream_alloc_subcommand(const struct _cream_runtime_binding *rtb) {
  struct cream_subcommand *sc =
      (struct cream_subcommand *)malloc(rtb->metadata.size);
  if (!sc) {
    return NULL;
  }

  if (!rtb->opt)
    sc->name = NULL;
  else
    sc->name = rtb->opt->text;

  sc->bools_count = 0;
  sc->datavecs_count = 0;
  sc->kwhosts_count = 0;
  sc->subcommands_count = rtb->metadata.subcommands_capacity;
  sc->anon_args_count = 0;

  sc->bools = (struct cream_bool *)sc->data;
  sc->datavecs =
      (struct cream_datavec *)(sc->bools + rtb->metadata.bools_capacity);
  sc->kwhosts =
      (struct cream_kwhost *)(sc->datavecs + rtb->metadata.datavecs_capacity);
  sc->anonymous_args =
      (const char **)(sc->kwhosts + rtb->metadata.kwhosts_capacity);

  if (sc->subcommands_count == 0) {
    sc->subcommands = NULL;
  } else
    sc->subcommands = (struct cream_subcommand **)calloc(
        1, sc->subcommands_count * sizeof(struct cream_subcommand **));

  return sc;
}

void _cream_destroy_subcommand(struct cream_subcommand *sc) {
  if (!sc)
    return;

  for (int i = 0; i < sc->subcommands_count; i++) {
    _cream_destroy_subcommand(sc->subcommands[i]);
  }
  free(sc->subcommands);
  free(sc);
}

struct cream_subcommand *
_cream_get_result_structure(const struct cream_option *opts,
                            const struct _cream_runtime_binding *cur_rtb,
                            const struct _cream_runtime_binding *rtbs,
                            const struct _cream_context *ctx) {

  struct cream_subcommand *sc = _cream_alloc_subcommand(cur_rtb);
  if (!sc)
    return NULL;

  int fill_index = 0;
  for (const cream_option *o = opts; o->text != NULL; o++) {
    if (o->flags & CREAM_TYPE_SUBCOMMAND) {
      const struct _cream_runtime_binding *rtb = _cream_find_rtb(rtbs, o, ctx);
      sc->subcommands[fill_index] = _cream_get_result_structure(
          o->info.subcommand.child_opts, rtb, rtbs, ctx);
      if (!sc->subcommands[fill_index]) {
        _cream_destroy_subcommand(sc);
        return NULL;
      }
      fill_index++;
    }
  }
  return sc;
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

// TODO: change the whole function
bool _cream_guarantee_mem(struct cream_subcommand **cur_sc,
                          struct _cream_runtime_binding *rtb, const void *attr,
                          size_t *capacity, const size_t cur_item,
                          const size_t item_size) {

  if (cur_item < *capacity) {
    return true;
  }
  struct cream_subcommand *alloc_sc;

  off_t offset = *capacity / 2;
  if (!offset)
    offset = 1;
  rtb->metadata.size += offset;
  alloc_sc = (struct cream_subcommand *)realloc(*cur_sc, rtb->metadata.size);

  if (!alloc_sc)
    return false;
  *cur_sc = alloc_sc;
  uint8_t *dest = (uint8_t *)attr + (offset * item_size);
  memmove(dest, attr, ((uint8_t *)(*cur_sc)->data + rtb->metadata.size) - dest);
  *capacity += offset;

  (*cur_sc)->bools = (struct cream_bool *)(*cur_sc)->data;
  (*cur_sc)->datavecs =
      (struct cream_datavec *)((*cur_sc)->bools + rtb->metadata.bools_capacity);
  (*cur_sc)->kwhosts = (struct cream_kwhost *)((*cur_sc)->datavecs +
                                               rtb->metadata.datavecs_capacity);
  (*cur_sc)->anonymous_args =
      (const char **)((*cur_sc)->kwhosts + rtb->metadata.kwhosts_capacity);

  return true;
}

bool _cream_check_duplicate(const struct cream_subcommand *cur_sc,
                            const cream_option *opt) {
  switch (opt->flags & CREAM_FLAGS_TYPE) {
  case CREAM_TYPE_BOOLEAN:
    for (int i = 0; i < cur_sc->bools_count; i++) {
      if (strcmp(cur_sc->bools[i].opt, opt->text) == 0) {
        return true;
      }
    }
    break;
  case CREAM_TYPE_DATAVEC:
    for (int i = 0; i < cur_sc->datavecs_count; i++) {
      if (strcmp(cur_sc->datavecs[i].opt, opt->text) == 0) {
        return true;
      }
    }
    break;
  case CREAM_TYPE_KEYWORD_HOST:
    for (int i = 0; i < cur_sc->kwhosts_count; i++) {
      if (strcmp(cur_sc->kwhosts[i].opt, opt->text) == 0) {
        return true;
      }
    }
    break;
  }

  return false;
}

void _cream_register_opt(struct cream_subcommand **cur_sc,
                         struct _cream_runtime_binding **rtb,
                         const struct _cream_runtime_binding *rtbs,
                         const struct cream_option *opt,
                         const struct cream_option *opts, const char **argv,
                         int *iter, const int argc,
                         const struct _cream_context *ctx) {

  if (opt->flags & CREAM_ARG_DENY_DUPLICATES) {
    if (_cream_check_duplicate(*cur_sc, opt)) {
      // TODO: _cream_raise_err the arg was duplicated
    }
  }

  switch (opt->flags & CREAM_FLAGS_TYPE) {
  case CREAM_TYPE_BOOLEAN:
    _cream_guarantee_mem(cur_sc, *rtb, &(*cur_sc)->bools,
                         &(*cur_sc)->bools_count, (*rtb)->metadata.cur_bool,
                         sizeof(cream_bool));
    (*cur_sc)->bools[(*rtb)->metadata.cur_bool].opt = opt->text;
    (*rtb)->metadata.cur_bool++;
    break;

  case CREAM_TYPE_DATAVEC:
    _cream_guarantee_mem(cur_sc, *rtb, &(*cur_sc)->datavecs,
                         &(*cur_sc)->datavecs_count,
                         (*rtb)->metadata.cur_datavec, sizeof(cream_datavec));
    (*cur_sc)->datavecs[(*rtb)->metadata.cur_datavec].opt = opt->text;

    (*cur_sc)->datavecs[(*rtb)->metadata.cur_datavec].data = argv + 1;
    (*cur_sc)->datavecs[(*rtb)->metadata.cur_datavec].size = 0;
    (*iter)++; // Ignore option name

    while (argv + *iter < argv + argc && !_cream_find_opt(argv[*iter], opts)) {
      (*cur_sc)->datavecs[(*rtb)->metadata.cur_datavec].size++;
      if ((*cur_sc)->datavecs[(*rtb)->metadata.cur_datavec].size >
          opt->info.datavec.max_elems) {
        // TODO: _cream_register_err
      }
      (*iter)++;
    }
    if ((*cur_sc)->datavecs[(*rtb)->metadata.cur_datavec].size <
        opt->info.datavec.min_elems) {
      // TODO: _cream_register_err
    }
    break;

  case CREAM_TYPE_KEYWORD_HOST:
    _cream_guarantee_mem(cur_sc, *rtb, &(*cur_sc)->kwhosts,
                         &(*cur_sc)->kwhosts_count, (*rtb)->metadata.cur_kwhost,
                         sizeof(cream_kwhost));
    (*cur_sc)->kwhosts[(*rtb)->metadata.cur_kwhost].opt = opt->text;

    if (opt->flags & CREAM_KWTYPE_EMBEDDED) {
      for (int i = 0; opt->text[i] != '\0' && argv[(*iter)][i] != '\0'; i++) {
        if (opt->text[i] != argv[(*iter)][i]) {
          (*cur_sc)->kwhosts[(*rtb)->metadata.cur_kwhost].keyword =
              argv[(*iter)] + i;
          break;
        }
      }
    } else if (opt->flags & CREAM_KWTYPE_EQUALOP) {
      for (int i = 0; argv[(*iter)][i] != '\0'; i++) {
        if (argv[(*iter)][i] == '=') {
          (*cur_sc)->kwhosts[(*rtb)->metadata.cur_kwhost].keyword =
              argv[(*iter)] + i + 1;
          break;
        }
      }
    } else if (opt->flags & CREAM_KWTYPE_SEPARATE) {
      (*iter)++;
      (*cur_sc)->kwhosts[(*rtb)->metadata.cur_kwhost].keyword = NULL;
      if (*iter >= argc)
        ; // TODO: _cream_register_err
      for (const char **p = opt->info.kwhost.values; *p != NULL; p++) {
        if (strcmp(*p, argv[*iter]) == 0) {
          (*cur_sc)->kwhosts[(*rtb)->metadata.cur_kwhost].keyword = *p;
          break;
        }
      }
      if ((*cur_sc)->kwhosts[(*rtb)->metadata.cur_kwhost].keyword == NULL) {
        // TODO: _cream_register_err bad keyword
      }
    }
    break;

  case CREAM_TYPE_SUBCOMMAND:
    // TODO: change the current subcommand and rtb
    *rtb = _cream_find_rtb(rtbs, opt, ctx);
    const cream_subcommand *new_sc = NULL;
    for (int i = 0; i < (*cur_sc)->subcommands_count; i++) {
      if (strcmp((*rtb)->opt->text, (*cur_sc)->subcommands[i]->name) == 0) {
        new_sc = (*cur_sc)->subcommands[i];
        break;
      }
    }
    if (!new_sc) {
      // TODO: _cream_raise_err
    }
    return; // Recursive subcommands have no generic checks
  }
}

// ==================
// = User functions =
// ==================

cream_result *cream_parse(const int argc, const char *argv[],
                          const struct cream_option *opts) {
  struct _cream_context ctx = {0, 0};
  for (const struct cream_option *o = opts; o->text != NULL; o++)
    ctx.arg_opts++;
  struct _cream_runtime_binding *rtbs = _cream_get_runtime_bindings(opts, &ctx);
  struct cream_subcommand *result = _cream_alloc_subcommand(rtbs);

  struct cream_subcommand *cur_sc = result;
  struct _cream_runtime_binding *rtb = rtbs;
  for (int i = 0; i < argc; i++) {
    if (!cur_sc) {
      _cream_destroy_subcommand(result);
      return NULL;
    }

    cream_option *found_opt = _cream_find_opt(argv[i], opts);
    if (!found_opt) {
      _cream_guarantee_mem(&cur_sc, rtb, cur_sc->anonymous_args,
                           &rtb->metadata.anon_args_capacity,
                           rtb->metadata.cur_anon, sizeof(char **));
      cur_sc->anonymous_args[rtb->metadata.cur_anon] = argv[i];
      rtb->metadata.cur_anon++;
      continue;
    }
    _cream_register_opt(&cur_sc, &rtb, rtbs, found_opt, opts, argv, &i, argc,
                        &ctx);
  }

  return result;
}

union cream_argtype cream_find(const char *opt_name) {
  // TODO: finds by name, does not produce errors
}

void cream_free(cream_result *sc) { _cream_destroy_subcommand(sc); }

#endif // CREAMLIB_H
