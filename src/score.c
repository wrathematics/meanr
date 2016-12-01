#include "RNACI.h"
#include "gperf/sentiment.h"

#define CHARPT(x,i) ((char*)CHAR(STRING_ELT(x,i)))

typedef struct sentwords sentwords_t;


SEXP R_score(SEXP s_)
{
  SEXP ret, ret_names;
  SEXP positive, negative, scores, nwords;
  size_t slen = 0;
  char *s = NULL;
  const int len = LENGTH(s_);
  
  if (TYPEOF(s_) != STRSXP)
    error("input must be a vector of strings");
  
  newRvec(positive, len, "int");
  newRvec(negative, len, "int");
  newRvec(scores, len, "dbl");
  newRvec(nwords, len, "int");
  
  for (int i=0; i<len; i++)
  {
    if (i%256 == 0)
      R_CheckUserInterrupt();
    
    char *in = CHARPT(s_, i);
    size_t inlen = strlen(in);
    
    if (inlen > slen)
    {
      // TODO realloc
      if (s) free(s);
      s = malloc(inlen * sizeof(*s));
      if (s == NULL)
        error("unable to allocate memory");
      
      slen = inlen;
    }
    
    
    memcpy(s, in, inlen*sizeof(*s));
    for (int j=0; j<inlen; j++)
    {
      if (ispunct(s[j]))
        s[j] = ' ';
      
      s[j] = tolower(s[j]);
    }
    
    
    int *pos = &(INTEGER(positive)[i]);
    int *neg = &(INTEGER(negative)[i]);
    double *sc = &(REAL(scores)[i]);
    int *nw = &(INTEGER(nwords)[i]);
    *pos = *neg = *sc = *nw = 0;
    
    int start = 0;
    int end;
    
    
    for (int j=0; j<=inlen; j++)
    {
      if (isspace(s[j]) || s[j] == '\0')
      {
        (*nw)++;
        end = j;
        s[end] = '\0'; // for gperf
        
        sentwords_t *sw = in_word_set(s+start, end-start);
        if (sw != NULL)
        {
          int score = sw->score;
          *(sc) += score;
          if (score > 0)
            (*pos)++;
          else
            (*neg)++;
        }
        
        j++;
        while (isspace(s[j]))
          j++;
        
        start = j;
      }
    }
    
    *sc /= *nw;
  }
  
  free(s);
  
  ret_names = make_list_names(4, "positive", "negative", "avg", "wc");
  ret = make_dataframe(RNULL, ret_names, 4, positive, negative, scores, nwords);
  
  R_END;
  return ret;
}
