#pragma once

#include "stroke.h"

namespace agge {
namespace caps {
class butt : public stroke::cap {
 public:
  virtual void calc(points& output, real_t w, const point_r& v0, real_t d, const point_r& v1) const;
};
}  // namespace caps

namespace joins {
class bevel : public stroke::join {
 public:
  virtual void calc(points& output, real_t w, const point_r& v0, real_t d01, const point_r& v1,
                    real_t d12, const point_r& v2) const;
};
}  // namespace joins
}  // namespace agge
