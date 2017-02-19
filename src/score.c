#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#include <R_ext/Utils.h>
#include "RNACI.h"

#include "safeomp.h"

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

static inline int8_t get_sentiment_score(const char *word, const int wordlen)
{
  if (is_pos_sentiment(word, wordlen))
    return 1;
  else if (is_neg_sentiment(word, wordlen))
    return -1;
  else
    return 0;
}



static inline size_t max_strlen(SEXP s_, const int len)
{
  size_t maxlen = 0;
  
#ifdef OMP_VER_4
  #pragma omp parallel for simd schedule(static,1024) if(len>OMP_MIN_SIZE) reduction(max:maxlen)
#else
  #pragma omp parallel for schedule(static,1024) if(len>OMP_MIN_SIZE)
#endif
  for (int i=0; i<len; i++)
  {
    char *s = CHARPT(s_, i);
    size_t tmp = strlen(s) + 1;
    #ifndef OMP_VER_4
    #pragma omp critical
    #endif
    if (tmp > maxlen)
      maxlen = tmp;
  }
  
  return maxlen;
}


int omp_get_thread_num(void);


SEXP R_score(SEXP s_)
{
  SEXP ret, ret_names;
  SEXP positive, negative, scores, nwords;
  const int len = LENGTH(s_);
  
  if (TYPEOF(s_) != STRSXP)
    error("input must be a vector of strings");
  
  newRvec(positive, len, "int");
  newRvec(negative, len, "int");
  newRvec(scores, len, "int");
  newRvec(nwords, len, "int");
  
  const size_t slen = max_strlen(s_, len);
  
  int8_t check = 0;
  
  #pragma omp parallel shared(check)
  {
    char *s = NULL;
    // NOTE uncomment to simulate oom failure
    // if (omp_get_thread_num() != 1)
    s = malloc(slen * sizeof(*s));
    
    #pragma omp atomic// update
    check += (s == NULL);
    
    
    if (!check)
    {
      #pragma omp for
      for (int i=0; i<len; i++)
      {
        char *in = CHARPT(s_, i);
        size_t inlen = strlen(in) + 1;
        
        memcpy(s, in, inlen*sizeof(*s));
        
        SAFE_SIMD
        for (int j=0; j<inlen; j++)
        {
          if (ispunct(s[j]))
          s[j] = ' ';
          
          s[j] = tolower(s[j]);
        }
        
        
        int *const pos = INTEGER(positive) + i;
        int *const neg = INTEGER(negative) + i;
        int *const sc = INTEGER(scores) + i;
        int *const nw = INTEGER(nwords) + i;
        *pos = *neg = *sc = *nw = 0;
        
        uint32_t start = 0;
        uint32_t end;
        
        
        for (uint32_t j=0; j<inlen; j++)
        {
          if (isspace(s[j]) || s[j] == '\0')
          {
            (*nw)++;
            end = j;
            s[end] = '\0'; // for gperf
            
            int8_t score = get_sentiment_score(s+start, end-start);
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
    }
    
    free(s);
  }
  
  if (check)
    THROW_MEMERR();
  
  ret_names = make_list_names(4, "positive", "negative", "score", "wc");
  ret = make_dataframe(RNULL, ret_names, 4, positive, negative, scores, nwords);
  
  R_END;
  return ret;
}
