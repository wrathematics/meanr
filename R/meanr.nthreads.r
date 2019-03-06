#' meanr.nthreads
#' 
#' Returns the number of cores + hyperthreads on the system.  The function
#' respects the environment variable \code{OMP_NUM_THREADS}.
#' 
#' @return
#' An integer; the number of threads.
#' 
#' @useDynLib meanr R_meanr_nthreads
#' @export
meanr.nthreads = function()
{
  .Call(R_meanr_nthreads)
}
