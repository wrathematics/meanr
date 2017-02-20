/* Automatically generated. Do not edit by hand. */
  
  #include <R.h>
  #include <Rinternals.h>
  #include <R_ext/Rdynload.h>
  #include <stdlib.h>

extern SEXP R_meanr_nthreads();
extern SEXP R_score(SEXP s_, SEXP nthreads_);

static const R_CallMethodDef CallEntries[] = {
  {"R_meanr_nthreads", (DL_FUNC) &R_meanr_nthreads, 0},
  {"R_score", (DL_FUNC) &R_score, 2},
  {NULL, NULL, 0}
};
void R_init_meanr(DllInfo *dll)
                       {
                       R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
                       R_useDynamicSymbols(dll, FALSE);
                       }
