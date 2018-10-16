#pragma once

#include "nanovg.h"
#include "agg_basics.h"

namespace agg {
class nanovg_vertex {
 public:
  class iterator;

 public:
  nanovg_vertex(NVGvertex* vertex, int n) {
    _n = n;
    _index = 0;
    _vertex = vertex;
  }

  void rewind(unsigned) {
    _index = 0;
  }

  unsigned vertex(double* x, double* y) {
    int index = _index++;
    NVGvertex* p = _vertex + index;

    if (index < _n) {
      *x = p->x;
      *y = p->y;

      if (index == 0) {
        return path_cmd_move_to;
      } else if (index == (_n - 1)) {
        if (p->x == _vertex[0].x && p->y == _vertex[0].y) {
          return (path_cmd_end_poly | path_flags_close);
        } else {
          return path_cmd_line_to;
        }
      } else {
        return path_cmd_line_to;
      }
    } else {
      return path_cmd_stop;
    }
  }

 private:
  int _n;
  int _index;
  NVGvertex* _vertex;
};

}  // namespace agg
