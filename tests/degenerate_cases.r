library(meanr)

test = score("")
truth = 
data.frame(
  positive = 0L,
  negative = 0L,
  score = 0L,
  wc = 0L
)

stopifnot(identical(test, truth))
