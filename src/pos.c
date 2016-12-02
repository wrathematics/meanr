// NOTE: being non-static/inlined gives a measureable performance hit, but
// poshash.h and neghash.h have overlapping (non-identical) macros.

#include <stdbool.h>
#include "gperf/poshash.h"

bool is_pos_sentiment(const char *word, const int wordlen)
{
  const char *sw = in_pos_set(word, wordlen);
  return sw != NULL;
}
