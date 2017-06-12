# meanr

* **Version:** 0.1-1
* **Status:** [![Build Status](https://travis-ci.org/wrathematics/meanr.png)](https://travis-ci.org/wrathematics/meanr)
* **License:** [BSD 2-Clause](http://opensource.org/licenses/BSD-2-Clause)
* **Author:** Drew Schmidt


**meanr** is an R package performing basic sentiment analysis.  Its main main method, `score()`, computes sentiment as a simple sum of the counts of positive (+1) and negative (-1) sentiment words in a piece of text.  More sophisticated techniques are available to R, for example in the **qdap** package's `polarity()` function.  This package uses [the Hu and Liu sentiment dictionary](https://www.cs.uic.edu/~liub/FBS/sentiment-analysis.html), same as everybody else.

**meanr** is significantly faster than everything else I tried (which was actually the motivation for its creation), but I don't claim to have tried everything.  I believe the package is quite fast.  However, the method is merely a dictionary lookup, so it ignores word context like in more sophisticated methods.  On the other hand, the more sophisticated tools are very slow.  If you have a large volume of text, I believe there is value in getting a "first glance" at the data, and **meanr** allows you to do this very quickly.



## Installation

 You can install the stable version from CRAN using the usual `install.packages()`:

```r
install.packages("meanr")
``` 

The development version is maintained on GitHub, and can easily be installed by any of the packages that offer installations from GitHub:

```r
### Pick your preference
devtools::install_github("wrathematics/meanr")
ghit::install_github("wrathematics/meanr")
remotes::install_github("wrathematics/meanr")
```



## Example Usage

I have a dataset that, for legal reasons, I can not describe, much less provide.  You can think of it like a collection of tweets (they are not tweets).  But take my word for it that it's real, English language text.  The data is in the form of a vector of strings, which we'll call `x`.

```r
x = readRDS("x.rds")

length(x)
## [1] 655760

sum(nchar(x))
## [1] 162663972

library(meanr)
system.time(s <- score(x))
##  user  system elapsed 
## 1.072   0.000   0.285 

head(s)
##   positive negative score  wc
## 1        2        0     2  32
## 2        5        0     5  29
## 3        4        2     2  67
## 4       12        3     9 203
## 5        8        2     6 101
## 6        4        3     1  99
```



## How It Works

The `score()` function receives a vector of strings, and operates on each one as follows:

1. The maximum string length is found, and a buffer of that size is allocated.
2. The string is copied to the buffer.
3. All punctuation is removed. All characters are converted to lowercase.
4. Score sentiment:
    - Tokenize words as collections of chars separated by a space.
    - Check if the word is positive; if not, check if it is negative; if not, then it's assumed to be neutral.  Each check is a lookup up in one of two tables of Hu and Liu's dictionaries.
    - If the word is in the table, get its value from the hash table (positive words have value 1, negative words -1) and update the various counts.  Otherwise, the word is "neutral" (score of 0).

This is all done in four passes of each string; each pass corresponds to each of the enumerated items above.  The hash tables uses perfect hash functions generated by gperf.
