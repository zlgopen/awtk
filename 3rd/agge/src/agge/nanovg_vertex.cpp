#include <agge/nanovg_vertex.h>
#include <agge/path.h>

namespace agge {
nanovg_vertex::nanovg_vertex(NVGvertex* vertex, int n) {
  _vertex = vertex;
  _n = n;
}

nanovg_vertex::iterator nanovg_vertex::iterate() const {
  return iterator(_vertex, _n);
}

nanovg_vertex::iterator::iterator(NVGvertex* vertex, int n) : _n(n), _index(0), _vertex(vertex) {
}

int nanovg_vertex::iterator::vertex(real_t* x, real_t* y) {
  int index = _index++;
  NVGvertex* p = _vertex + index;

  if (index < _n) {
    *x = p->x;
    *y = p->y;

    if (index == 0) {
      return path_command_move_to;
    } else if (index == (_n - 1)) {
      if (p->x == _vertex[0].x && p->y == _vertex[0].y) {
        return (path_command_end_poly | path_flag_close);
      } else {
        return path_command_line_to;
      }
    } else {
      return path_command_line_to;
    }
  } else {
    return path_command_stop;
  }
}
}  // namespace agge
