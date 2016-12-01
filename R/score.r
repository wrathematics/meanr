#' score
#' 
#' Computes the sentiment score (simple average) of each string
#' in a vector of strings (independently).
#' 
#' @details
#' The function uses the Hu and Liu sentiment dictionary (same
#' as everybody else) available here:
#' https://www.cs.uic.edu/~liub/FBS/sentiment-analysis.html
#' 
#' @param s
#' A character vector (vector of strings).
#' 
#' @references
#' Hu, M., & Liu, B. (2004). Mining opinion features in customer
#' reviews. National Conference on Artificial Intelligence.
#' 
#' @export
score <- function(s) .Call(R_score, s)
