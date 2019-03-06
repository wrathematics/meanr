#' score
#' 
#' Computes the sentiment score, the sum of the total number of positive and
#' negative scored words.  The scorer is vectorized so that it will return one
#' row per input text, and each 
#' 
#' Preprocessing is largely unnecessary.  For example, the scorer ignores
#' case and punctuation.  That said, preprocessing probably won't hurt.
#' 
#' @details
#' The scorer uses OpenMP
#' 
#' The function uses the Hu and Liu sentiment dictionary (same as everybody
#' else) available here:
#' https://www.cs.uic.edu/~liub/FBS/sentiment-analysis.html
#' 
#' @param s
#' A string or vector of strings.
#' @param nthreads
#' Number of threads to use. By default it will use the total number of
#' cores + hyperthreads.
#' 
#' @return
#' A dataframe, consisting of columns "positive", "negative", "score", and "wc".
#' With the exception of "score", these are counts; that is, "positive" is the
#' number of positive sentiment words, "negative" is the number of negative
#' sentiment words, and "wc" is the wordcount (total number of words).
#' 
#' @examples
#' \dontrun{
#' library(meanr)
#' s1 = "Abundance abundant accessable."
#' s2 = "Banana apple orange."
#' s3 = "Abnormal abolish abominable."
#' s = c(s1, s2, s3)
#' 
#' # as separate 'documents'
#' score(s)
#' 
#' # as one document
#' score(paste0(s, collapse=" "))
#' }
#' 
#' @references
#' Hu, M., & Liu, B. (2004). Mining opinion features in customer
#' reviews. National Conference on Artificial Intelligence.
#' 
#' @useDynLib meanr R_score
#' @export
score = function(s, nthreads=meanr.nthreads())
{
  .Call(R_score, s, nthreads)
}
