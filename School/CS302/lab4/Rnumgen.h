#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <cstdlib>
#include <vector>
class rnumgen {
  public:
    rnumgen(int seed, std::vector<int> &vec) { srand(seed); }

    void pdf(int seed, std::vector<int> &v);
    int rand();

  private:
     std::vector<int> F;
};

#endif
