library(meanr)

# valgrind reports false positives, so set nthreads to 1
test = score("", nthreads=1)
truth = 
data.frame(
  positive = 0L,
  negative = 0L,
  score = 0L,
  wc = 0L
)

stopifnot(identical(test, truth))
