// NOTE: being non-static/inlined gives a measureable performance hit, but
// poshash.h and neghash.h have overlapping (non-identical) macros.

#include <stdbool.h>
#include "gperf/neghash.h"

bool is_neg_sentiment(const char *word, const int wordlen)
{
  const char *sw = in_neg_set(word, wordlen);
  return sw != NULL;
}
