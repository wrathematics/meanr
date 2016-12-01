library(meanr)

words = "Abundance abundant accessable. Banana apple orange. Abnormal abolish abominable."

test <- score(words)
stopifnot(identical(test$positive, 3L))
stopifnot(identical(test$negative, 3L))
stopifnot(all.equal(test$avg, 0.0))
stopifnot(identical(test$wc, 9L))
