#' meanr: Basic Sentiment Analysis Scorer
#' 
#' A popular technique in text analysis today is sentiment
#' analysis, or trying to determine the overall emotional
#' attitude of a piece of text (positive or negative).
#' We provide a new, basic implementation of a common
#' method for computing sentiment, whereby words are scored
#' as positive or negative according to a "dictionary", and
#' then an average of those scores for the document is produced.
#' The package uses the Hu and Liu sentiment dictionary for
#' assigning sentiment.
#' 
#' @useDynLib meanr, R_score
#' 
#' @name meanr-package
#' @docType package
#' @author Drew Schmidt \email{wrathematics AT gmail.com}
#' @keywords Package
NULL
