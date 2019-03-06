#' meanr: Sentiment Analysis Scorer
#' 
#' Sentiment analysis is a popular technique in text mining. Roughly
#' speaking, the technique is an attempt to determine the overall emotional
#' attitude of a piece of text (i.e., positive or negative). We provide a new
#' implementation of a common method for computing sentiment, whereby words are
#' scored as positive or negative according to a "dictionary", and then an
#' sum of those scores for the document is produced. We use the 'Hu' and 'Liu'
#' sentiment dictionary for determining sentiment. The scoring function is
#' 'vectorized' by document, and scores for multiple documents are computed in
#' parallel via 'OpenMP'.
#' 
#' @name meanr-package
#' @docType package
#' @author Drew Schmidt \email{wrathematics AT gmail.com}
#' @keywords Package
NULL
