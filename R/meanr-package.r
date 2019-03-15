#' meanr: Sentiment Analysis Scorer
#' 
#' Sentiment analysis is a popular technique in text mining that
#' attempts to determine the emotional state of some text. We provide a new
#' implementation of a common method for computing sentiment, whereby words are
#' scored as positive or negative according to a dictionary lookup. Then the
#' sum of those scores is returned for the document. We use the Hu and Liu
#' sentiment dictionary (Hu and Liu, 2004) <doi:10.1145/1014052.1014073>
#' for determining sentiment. The scoring function is vectorized by document,
#' and scores for multiple documents are computed in parallel via OpenMP.
#' 
#' @name meanr-package
#' @docType package
#' @author Drew Schmidt \email{wrathematics AT gmail.com}
#' @keywords Package
NULL
