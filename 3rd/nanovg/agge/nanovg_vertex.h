#pragma once

#include "agge/types.h"
#include "nanovg.h"

namespace agge {
class nanovg_vertex {
 public:
  class iterator;

 public:
  nanovg_vertex(NVGvertex* vertex, int n);

  iterator iterate() const;

 private:
  int _n;
  NVGvertex* _vertex;
};

class nanovg_vertex::iterator {
 public:
  iterator(NVGvertex* vertex, int n);

  int vertex(real_t* x, real_t* y);

 private:
  int _n;
  int _index;
  NVGvertex* _vertex;
};
}  // namespace agge
