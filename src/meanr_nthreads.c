#ifdef _OPENMP
#include <omp.h>
#endif

#include <RNACI.h>

#define MIN(a,b) ((a)<(b)?(a):(b))

static inline int num_threads(void)
{
  int n = 0;
  
#ifdef _OPENMP
  int nth, tl;
  #pragma omp parallel
  {
    nth = omp_get_num_threads();
    tl = omp_get_thread_limit();
  }
  
  n = MIN(nth, tl);
#else
  n = 1;
#endif
  
  return n;
}



SEXP R_meanr_nthreads(void)
{
  SEXP nth;
  newRvec(nth, 1, "int");
  
  INT(nth) = num_threads();
  
  unhideGC();
  return nth;
}
