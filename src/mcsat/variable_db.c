/*
 * The Yices SMT Solver. Copyright 2015 SRI International.
 *
 * This program may only be used subject to the noncommercial end user
 * license agreement which is downloadable along with this program.
 */

#if defined(CYGWIN) || defined(MINGW)
#ifndef __YICES_DLLSPEC__
#define __YICES_DLLSPEC__ __declspec(dllexport)
#endif
#endif

#include "mcsat/variable_db.h"

#include "io/term_printer.h"
#include "mcsat/tracing.h"

#include "yices.h"

void variable_db_construct(variable_db_t* var_db, term_table_t* terms, type_table_t* types, tracer_t* tracer) {
  var_db->terms = terms;
  var_db->types = types;
  var_db->tracer = tracer;
  init_ivector(&var_db->variable_to_term_map, 0);
  init_int_hmap(&var_db->term_to_variable_map, 0);
  init_pvector(&var_db->notify_new_variable, 0);
  init_ivector(&var_db->free_list, 0);

  // Create the null variable
  ivector_push(&var_db->variable_to_term_map, NULL_TERM);
  int_hmap_add(&var_db->term_to_variable_map, 0, variable_null);
}

void variable_db_destruct(variable_db_t* var_db) {
  delete_pvector(&var_db->notify_new_variable);
  delete_int_hmap(&var_db->term_to_variable_map);
  delete_ivector(&var_db->variable_to_term_map);
  delete_ivector(&var_db->free_list);
}

bool variable_db_has_variable(variable_db_t* var_db, term_t x) {
  assert(is_pos_term(x));
  int_hmap_pair_t* find;
  find = int_hmap_find(&var_db->term_to_variable_map, x);
  return find != NULL;
}

static void variable_db_notify_new_variable(variable_db_t* var_db, variable_t x) {
  uint32_t i;
  variable_db_new_variable_notify_t* to_notify;

  for (i = 0; i < var_db->notify_new_variable.size; ++ i) {
    to_notify = (variable_db_new_variable_notify_t*) var_db->notify_new_variable.data[i];
    to_notify->new_variable(to_notify, x);
  }
}

variable_t variable_db_get_variable(variable_db_t* var_db, term_t term) {
  int_hmap_pair_t* find;
  variable_t x;

  assert(is_pos_term(term));

  find = int_hmap_find(&var_db->term_to_variable_map, term);
  if (find != NULL) {
    return find->val;
  } else {

    // New id and forward map
    if (var_db->free_list.size > 0) {
      x = ivector_pop2(&var_db->free_list);
      assert(var_db->variable_to_term_map.data[x] == NULL_TERM);
      var_db->variable_to_term_map.data[x] = term;
    } else {
      x = var_db->variable_to_term_map.size;
      ivector_push(&var_db->variable_to_term_map, term);
    }

    // Back map
    assert(int_hmap_find(&var_db->term_to_variable_map, term) == NULL);
    int_hmap_add(&var_db->term_to_variable_map, term, x);
    // Notify
    variable_db_notify_new_variable(var_db, x);

    return x;
  }
}

static inline
void variable_db_remove_variable(variable_db_t* var_db, variable_t x) {
  term_t t;
  int_hmap_pair_t* find;

  t = var_db->variable_to_term_map.data[x];
  assert(t != NULL_TERM);

  if (trace_enabled(var_db->tracer, "mcsat::gc")) {
    trace_printf(var_db->tracer, "removing variable [%d]: ", x);
    trace_term_ln(var_db->tracer, var_db->terms, t);
  }

  var_db->variable_to_term_map.data[x] = NULL_TERM;
  find = int_hmap_find(&var_db->term_to_variable_map, t);
  assert(find != NULL);
  assert(find->val == x);
  int_hmap_erase(&var_db->term_to_variable_map, find);
  ivector_push(&var_db->free_list, x);

  assert(variable_db_get_variable_if_exists(var_db, t) == variable_null);
}

variable_t variable_db_get_variable_if_exists(variable_db_t* var_db, term_t term) {
  int_hmap_pair_t* find;

  assert(is_pos_term(term));

  find = int_hmap_find(&var_db->term_to_variable_map, term);
  if (find != NULL) {
    return find->val;
  } else {
    return variable_null;
  }
}

term_t variable_db_get_term(const variable_db_t* var_db, variable_t x) {
  assert(x >= 0 && x < var_db->variable_to_term_map.size);
  return var_db->variable_to_term_map.data[x];
}


void variable_db_add_new_variable_listener(variable_db_t* var_db, variable_db_new_variable_notify_t* listener) {
  pvector_push(&var_db->notify_new_variable, listener);
}

uint32_t variable_db_size(const variable_db_t* var_db) {
  // Deduct the freed ones and the null variable
  return var_db->variable_to_term_map.size - var_db->free_list.size - 1;
}

void variable_db_print_variable(const variable_db_t* var_db, variable_t x, FILE* out) {
  yices_pp_t printer;
  init_yices_pp(&printer, out, NULL, PP_HMODE, 0);
  pp_term_full(&printer, var_db->terms, variable_db_get_term(var_db, x));
  flush_pp(&printer.pp, false);
  delete_yices_pp(&printer, false);
}

void variable_db_print(const variable_db_t* var_db, FILE* out) {
  uint32_t i;
  term_t t;

  for (i = 0; i < var_db->variable_to_term_map.size; ++ i) {
    t = var_db->variable_to_term_map.data[i];
    if (t != NULL_TERM) {
      fprintf(out, "%d [%d] : ", i, t);
      variable_db_print_variable(var_db, i, out);
      fprintf(out, "\n");
    }
  }
}

bool variable_db_is_boolean(const variable_db_t* var_db, variable_t x) {
  return term_type_kind(var_db->terms, variable_db_get_term(var_db, x)) == BOOL_TYPE;
}

bool variable_db_is_real(const variable_db_t* var_db, variable_t x) {
  return term_type_kind(var_db->terms, variable_db_get_term(var_db, x)) == REAL_TYPE;
}

bool variable_db_is_int(const variable_db_t* var_db, variable_t x) {
  return term_type_kind(var_db->terms, variable_db_get_term(var_db, x)) == INT_TYPE;
}

type_kind_t variable_db_get_type_kind(const variable_db_t* var_db, variable_t x) {
  return term_type_kind(var_db->terms, variable_db_get_term(var_db, x));
}

void variable_db_get_subvariables(const variable_db_t* var_db, term_t term, int_mset_t* t_vars) {
  assert(false);
}

term_t variable_db_substitute_subvariable(const variable_db_t* var_db, term_t t, variable_t x, term_t subst) {

  // For now, just equality
  assert(term_kind(var_db->terms, t) == EQ_TERM);
  term_t x_term = variable_db_get_term(var_db, x);
  composite_term_t* eq = eq_term_desc(var_db->terms, t);
  term_t lhs = eq->arg[0];
  term_t rhs = eq->arg[1];
  if (lhs == x_term) {
    lhs = subst;
  }
  if (rhs == x_term) {
    rhs = subst;
  }
  term_t result = yices_eq(lhs, rhs);

  return result;
}

void variable_db_gc_sweep(variable_db_t* var_db, gc_info_t* gc_vars) {
  uint32_t i, to_collect;

  // How many left to reclaim
  assert(variable_db_size(var_db) >= gc_vars->marked.size);
  to_collect = variable_db_size(var_db) - gc_vars->marked.size;

  // Relocate (skip variable_null)
  for (i = 1; to_collect > 0 && i < var_db->variable_to_term_map.size; ++ i) {
    if (var_db->variable_to_term_map.data[i] != NULL_TERM) {
      if (gc_info_get_reloc(gc_vars, i) == variable_null) {
        // Reclaim
        variable_db_remove_variable(var_db, i);
      }
    }
  }

  gc_info_set_relocated(gc_vars);
}

void variable_db_set_tracer(variable_db_t* var_db, tracer_t* tracer) {
  var_db->tracer = tracer;
}

bool variable_db_is_variable(const variable_db_t* var_db, variable_t var, bool assert) {
  if (var == variable_null) {
    assert(!assert);
    return false;
  }
  if (var < 0) {
    assert(!assert);
    return false;
  }
  if (var >= var_db->variable_to_term_map.size) {
    assert(!assert);
    return false;
  }
  if (var_db->variable_to_term_map.data[var] == NULL_TERM) {
    assert(!assert);
    return false;
  }
  return true;
}