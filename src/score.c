#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <R_ext/Utils.h>
#include "RNACI.h"

#include "gperf/poshash.h"
#include "gperf/neghash.h"

#define CHARPT(x,i) ((char*)CHAR(STRING_ELT(x,i)))
#define THROW_MEMERR() error("unable to allocate memory")
#define CHECKMALLOC(s) if (s == NULL) THROW_MEMERR()



static inline bool is_pos_sentiment(const char *word, const int wordlen)
{
  return in_pos_set(word, wordlen) != NULL;
}

static inline bool is_neg_sentiment(const char *word, const int wordlen)
{
  return in_neg_set(word, wordlen) != NULL;
}

static inline int get_sentiment_score(const char *word, const int wordlen)
{
  if (is_pos_sentiment(word, wordlen))
    return 1;
  else if (is_neg_sentiment(word, wordlen))
    return -1;
  else
    return 0;
}



static inline void check_interrupt_fun(void *ignored)
{
  R_CheckUserInterrupt();
}

static bool check_interrupt()
{
  return (R_ToplevelExec(check_interrupt_fun, NULL) == FALSE);
}



static inline size_t max_strlen(SEXP s_)
{
  size_t max = 0;
  for (int i=0; i<LENGTH(s_); i++)
  {
    char *s = CHARPT(s_, i);
    size_t tmp = strlen(s);
    if (tmp > max)
      max = tmp;
  }
  
  return max;
}



SEXP R_score(SEXP s_)
{
  SEXP ret, ret_names;
  SEXP positive, negative, scores, nwords;
  const int len = LENGTH(s_);
  size_t slen;
  char *s;
  
  if (TYPEOF(s_) != STRSXP)
    error("input must be a vector of strings");
  
  newRvec(positive, len, "int");
  newRvec(negative, len, "int");
  newRvec(scores, len, "dbl");
  newRvec(nwords, len, "int");
  
  slen = max_strlen(s_);
  s = malloc(slen * sizeof(*s));
  CHECKMALLOC(s);
  
  
  for (int i=0; i<len; i++)
  {
    if (i%512 == 0)
    {
      if (check_interrupt())
      {
        free(s);
        return R_NilValue;
      }
    }
    
    char *in = CHARPT(s_, i);
    size_t inlen = strlen(in) + 1;
    
    memcpy(s, in, inlen*sizeof(*s));
    
    for (int j=0; j<inlen; j++)
    {
      if (ispunct(s[j]))
        s[j] = ' ';
      
      s[j] = tolower(s[j]);
    }
    
    
    int *pos = INTEGER(positive) + i;
    int *neg = INTEGER(negative) + i;
    double *sc = REAL(scores) + i;
    int *nw = INTEGER(nwords) + i;
    *pos = *neg = *sc = *nw = 0;
    
    int start = 0;
    int end;
    
    
    for (int j=0; j<inlen; j++)
    {
      if (isspace(s[j]) || s[j] == '\0')
      {
        (*nw)++;
        end = j;
        s[end] = '\0'; // for gperf
        
        int score = get_sentiment_score(s+start, end-start);
        *(sc) += score;
        if (score > 0)
          (*pos)++;
        else if (score < 0)
          (*neg)++;
        
        j++;
        while (isspace(s[j]))
          j++;
        
        start = j;
      }
    }
  }
  
  free(s);
  
  ret_names = make_list_names(4, "positive", "negative", "score", "wc");
  ret = make_dataframe(RNULL, ret_names, 4, positive, negative, scores, nwords);
  
  R_END;
  return ret;
}
