// This has to be in its own file because including sentiment.h
// makes my linter go crazy on every save of the file.

#include <stdbool.h>

// #include "gperf/sentiment.h"
#include "gperf/poshash.h"
#include "gperf/neghash.h"


static inline bool is_pos(const char *word, const int wordlen)
{
  const char *sw = in_pos_set(word, wordlen);
  return sw != NULL;
}

static inline bool is_neg(const char *word, const int wordlen)
{
  const char *sw = in_neg_set(word, wordlen);
  return sw != NULL;
}

int get_sentiment_score(const char *word, const int wordlen)
{
  if (is_pos(word, wordlen))
    return 1;
  else if (is_neg(word, wordlen))
    return -1;
  else return 0;
}
