// Copyright (C) 2017 by Drew Schmidt <wrathematics@gmail.com>
// 
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted.
// 
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE


#ifndef __REACTOR_CTESTS__
#define __REACTOR_CTESTS__


#include <Rinternals.h>
#include <math.h>


// Tests

static inline int is_na(SEXP x)
{
  switch (TYPEOF(x))
  {
    case LGLSXP:
      return LOGICAL(x)[0] == NA_LOGICAL;
    case INTSXP:
      return INTEGER(x)[0] == NA_INTEGER;
    case REALSXP:
      return ISNA(REAL(x)[0]);
    case STRSXP:
      return STRING_ELT(x, 0) == NA_STRING;
    
    default:
      return 0;
  }
}

static inline int is_badval(SEXP x)
{
  switch (TYPEOF(x))
  {
    case LGLSXP:
    case INTSXP:
    case STRSXP:
      return is_na(x);
    case REALSXP:
      return is_na(x) || ISNAN(REAL(x)[0]) || !R_FINITE(REAL(x)[0]);
    
    default:
      return 0;
  }
}

static inline int is_inty(SEXP x)
{
  switch (TYPEOF(x))
  {
    case INTSXP:
      return 1;
    case REALSXP:
      return fabs(REAL(x)[0] - (int) REAL(x)[0]) < 1e-10;
    
    default:
      return 0;
  }
}

static inline int is_annoying(SEXP x)
{
  return (LENGTH(x) != 1 || is_badval(x));
}

static inline int is_negative(SEXP x)
{
  switch (TYPEOF(x))
  {
    case INTSXP:
      return INTEGER(x)[0] < 0;
    case REALSXP:
      return REAL(x)[0] < 0;
    
    default:
      return 0;
  }
}

static inline int is_zero(SEXP x)
{
  switch (TYPEOF(x))
  {
    case INTSXP:
      return INTEGER(x)[0] == 0;
    case REALSXP:
      return REAL(x)[0] == 0.0;
    
    default:
      return 0;
  }
}



static inline int is_lgl(SEXP x)
{
  return TYPEOF(x) == LGLSXP;
}

static inline int is_str(SEXP x)
{
  return TYPEOF(x) == STRSXP;
}

static inline int is_num(SEXP x)
{
  switch (TYPEOF(x))
  {
    case INTSXP:
    case REALSXP:
      return 1;
    
    default:
      return 0;
  }
}



// 'Assertions'

#define CHECK_IS_MATRIX(...) \
  if (!isMatrix(REACTOR_FIRST(__VA_ARGS__))){ \
    REACTOR_ERRMSG("a matrix", __VA_ARGS__);}

#define CHECK_IS_LIST(...) \
  if (!isVectorList(REACTOR_FIRST(__VA_ARGS__))){ \
    REACTOR_ERRMSG("a list", __VA_ARGS__);}


#define CHECK_IS_FLAG(...) \
  if (!is_lgl(REACTOR_FIRST(__VA_ARGS__)) || is_annoying(REACTOR_FIRST(__VA_ARGS__))){ \
    REACTOR_ERRMSG("a flag (TRUE or FALSE)", __VA_ARGS__);}

#define CHECK_IS_STRING(...) \
  if (!is_str(REACTOR_FIRST(__VA_ARGS__)) || is_annoying(REACTOR_FIRST(__VA_ARGS__))){ \
    REACTOR_ERRMSG("a string (single non-NA character string)", __VA_ARGS__);}

#define CHECK_IS_STRINGS(...) \
  if (!is_str(REACTOR_FIRST(__VA_ARGS__)) || LENGTH(REACTOR_FIRST(__VA_ARGS__)) == 0){ \
    REACTOR_ERRMSG("a vector of strings", __VA_ARGS__);}



#define CHECK_IS_INT(...) \
  if (!is_inty(REACTOR_FIRST(__VA_ARGS__)) || is_annoying(REACTOR_FIRST(__VA_ARGS__))){ \
    REACTOR_ERRMSG("a positive integer", __VA_ARGS__);}

#define CHECK_IS_NATNUM(...) \
  if (!is_inty(REACTOR_FIRST(__VA_ARGS__)) || is_annoying(REACTOR_FIRST(__VA_ARGS__)) || is_negative(REACTOR_FIRST(__VA_ARGS__))){ \
    REACTOR_ERRMSG("a positive integer", __VA_ARGS__);}

#define CHECK_IS_POSINT(...) \
  if (!is_inty(REACTOR_FIRST(__VA_ARGS__)) || is_annoying(REACTOR_FIRST(__VA_ARGS__)) || is_negative(REACTOR_FIRST(__VA_ARGS__)) || is_zero(REACTOR_FIRST(__VA_ARGS__))){ \
    REACTOR_ERRMSG("a positive integer", __VA_ARGS__);}



// internals, pls ignore
#define REACTOR_FIRST_(first, ...) first
#define REACTOR_FIRST(...) REACTOR_FIRST_(__VA_ARGS__, 0)

#define __REACTOR_ERRMSG(type_explanation, x, argname, ...) error("argument '%s' must be %s", argname, type_explanation);
#define _REACTOR_ERRMSG(type_explanation, x, ...) __REACTOR_ERRMSG(type_explanation, __VA_ARGS__, #x, "")
#define REACTOR_ERRMSG(type_explanation, ...) _REACTOR_ERRMSG(type_explanation, __VA_ARGS__, "")


#endif
