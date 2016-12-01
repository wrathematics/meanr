#include <ctype.h>
#include "RNACI.h"

int get_sentiment_score(const char *word, const int wordlen);


#define CHARPT(x,i) ((char*)CHAR(STRING_ELT(x,i)))

#define THROW_MEMERR() error("unable to allocate memory")
#define CHECKMALLOC(s) if (s == NULL) THROW_MEMERR()


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
  
  // first guess
  s = malloc(1024 * sizeof(*s));
  CHECKMALLOC(s);
  
  for (int i=0; i<len; i++)
  {
    if (i%256 == 0)
      R_CheckUserInterrupt();
    
    char *in = CHARPT(s_, i);
    size_t inlen = strlen(in);
    
    if (inlen > slen)
    {
      void *realloc_ptr;
      realloc_ptr = realloc(s, inlen * sizeof(*s));
      if (realloc_ptr == NULL)
      {
        free(s);
        THROW_MEMERR();
      }
      
      s = (char*)realloc_ptr;
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
    
    *sc /= *nw;
  }
  
  free(s);
  
  ret_names = make_list_names(4, "positive", "negative", "avg", "wc");
  ret = make_dataframe(RNULL, ret_names, 4, positive, negative, scores, nwords);
  
  R_END;
  return ret;
}
