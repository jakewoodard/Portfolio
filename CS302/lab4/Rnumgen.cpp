#include <numeric>
#include <cstdlib>
#include <algorithm>
#include "Rnumgen.h"

void rnumgen::pdf(int seed, std::vector <int> &v) {
  F.resize(v.size());
  partial_sum(v.begin(), v.end(), F.begin());
  transform(F.begin(), F.end(), F.begin(),  
  bind2nd(std::divides<float>(), F.back()));
}

int rnumgen::rand() {
  const double randmax = RAND_MAX+1.0;
  const double p = (double)std::rand()/randmax;
  return upper_bound(F.begin(), F.end(), p) - F.begin();
}
