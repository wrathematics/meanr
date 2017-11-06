#include <RNACI.h>

#ifdef _OPENMP
#include <omp.h>
#endif

static inline int num_threads()
{
  int nth;
  
#ifdef _OPENMP
  #pragma omp parallel
  nth = omp_get_num_threads();
#else
  nth = 1;
#endif
  
  return nth;
}



SEXP R_meanr_nthreads()
{
  SEXP nth;
  newRvec(nth, 1, "int");
  
  INT(nth) = num_threads();
  
  unhideGC();
  return nth;
}
