#include <agge/math.h>

#include <math.h>

namespace agge {
float sqrt(float x) {
  return ::sqrtf(x);
}

double sqrt(double x) {
  return ::sqrt(x);
}

template <>
inline float limits<float>::resolution() {
  return 1e-6f;
}

template <>
inline double limits<double>::resolution() {
  return 1e-15;
}

const real_t distance_epsilon = limits<real_t>::resolution();
}  // namespace agge
