// This has to be in its own file because including sentiment.h
// makes my linter go crazy on every save of the file.

#include "gperf/sentiment.h"

typedef struct sentwords sentwords_t;

int get_sentiment_score(const char *word, const int wordlen)
{
  int score = 0;
  sentwords_t *sw = in_word_set(word, wordlen);
  
  if (sw != NULL)
    score = sw->score;
  
  return score;
}
