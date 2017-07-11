library(meanr)

words = "Abundance abundant accessable. Banana apple orange. Abnormal abolish abominable."

# valgrind reports false positives, so set nthreads to 1
test <- score(words, nthreads=1)
stopifnot(identical(test$positive, 3L))
stopifnot(identical(test$negative, 3L))
stopifnot(all.equal(test$score, 0.0))
stopifnot(identical(test$wc, 9L))
