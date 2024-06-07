
#include <math.h> // for: log, exp
// NOTE: double log(double) performs natural log only: it might as well be called ln

double nTauOfPcnt01(double pcnt01) {
  // note that log is ln
  return log( -1/(pcnt01 - 1) );
}

double pcnt01OfnTau(double ntau) {
  // note that exp is e^(its_argument)
  return 1 - exp(-ntau);
}