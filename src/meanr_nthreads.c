#include <RNACI.h>


#ifdef _OPENMP
int omp_get_num_threads();

static inline int num_threads()
{
  int nth = 0;
  
  #pragma omp parallel
  nth = omp_get_num_threads();
  
  return nth;
}
#endif



SEXP R_meanr_nthreads()
{
  SEXP nth;
  newRvec(nth, 1, "int");
  
#ifdef _OPENMP
  INT(nth) = num_threads();
#else
  INT(nth) = 1;
#endif
  
  unhideGC();
  return nth;
}
