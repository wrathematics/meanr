#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h> // for ispunct()

#include "include/reactor.h"
#include "include/RNACI.h"
#include "include/safeomp.h"

#include "hashtable/poshash.h"
#include "hashtable/neghash.h"

#define THROW_MEMERR() error("unable to allocate memory")
#define CHECKMALLOC(s) if (s == NULL) THROW_MEMERR()

#define FREE(ptr) if(ptr!=NULL) free(ptr)



// ----------------------------------------------------------------------------
// Hashtable interface
// ----------------------------------------------------------------------------

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



// ----------------------------------------------------------------------------
// Finds the necessary size of the temporary storage
// ----------------------------------------------------------------------------

#define SCHED_LEN 64

static inline size_t max_strlen(SEXP s_, const int len, const int nthreads)
{
  size_t maxlen = 0;
  
  // NOTE the reduction clause is deliberately missing from omp versions < 4,
  // OpenMP didn't include max reductions before then.
#ifdef OMP_VER_4
  #pragma omp parallel for simd num_threads(nthreads) schedule(static,SCHED_LEN) if(len>OMP_MIN_SIZE) reduction(max:maxlen) 
#else
  #pragma omp parallel for      num_threads(nthreads) schedule(static,SCHED_LEN) if(len>OMP_MIN_SIZE) // no reduction!
#endif
  for (int i=0; i<len; i++)
  {
    const char *const s = STR(s_, i);
    size_t tmp = strlen(s) + 1;
    
    #ifndef OMP_VER_4
    #pragma omp critical
    #endif
    if (tmp > maxlen)
      maxlen = tmp;
  }
  
  return maxlen;
}



// ----------------------------------------------------------------------------
// R interface
// ----------------------------------------------------------------------------

SEXP R_score(SEXP s_, SEXP nthreads_)
{
  SEXP ret, ret_names;
  SEXP positive, negative, scores, nwords;
  
  CHECK_IS_STRINGS(s_, "s");
  CHECK_IS_POSINT(nthreads_, "nthreads");
  
  const int len = LENGTH(s_);
  const int nthreads = asInteger(nthreads_);
  
  newRvec(positive, len, "int");
  newRvec(negative, len, "int");
  newRvec(scores, len, "int");
  newRvec(nwords, len, "int");
  
  const size_t slen = max_strlen(s_, len, nthreads);
  
  int8_t check = 0;
  
  #pragma omp parallel shared(check) num_threads(nthreads)
  {
    char *s = NULL;
    // NOTE uncomment to simulate oom failure
    // if (omp_get_thread_num() != 1)
    s = malloc(slen * sizeof(*s));
    
    // all threads tmp space malloc check
    #pragma omp atomic// update
    check += (s == NULL);
    
    
    // malloc succeeded
    if (!check)
    {
      #pragma omp for
      for (int i=0; i<len; i++)
      {
        char *in = STR(s_, i);
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
    
    FREE(s);
  }
  
  // malloc failed
  if (check)
    THROW_MEMERR();
  
  ret_names = make_list_names(4, "positive", "negative", "score", "wc");
  ret = make_dataframe(RNULL, ret_names, 4, positive, negative, scores, nwords);
  
  R_END;
  return ret;
}
