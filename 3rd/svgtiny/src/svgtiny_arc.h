#ifndef SVGTINY_ARC_H
#define SVGTINY_ARC_H

#include "tkc/rect.h"

/*Adapt from https://github.com/mozilla/newtab-dev/blob/master/dom/svg/nsSVGPathDataParser.cpp begin:*/
typedef struct _arc_info_t {
  uint32_t seg_index;
  uint32_t num_segs;
  float rx;
  float ry;
  float sin_phi;
  float cos_phi;
  pointf_t c;
  float delta;
  pointf_t from;
  float t;
  float theta;
} arc_info_t;

static float calc_vector_angle(float ux, float uy, float vx, float vy) {
  float ta = atan2(uy, ux);
  float tb = atan2(vy, vx);
  if (tb >= ta) return tb - ta;

  return 2 * M_PI - (ta - tb);
}

static ret_t arc_info_init(arc_info_t* info, pointf_t from, pointf_t to, pointf_t radii,
                           float angle, bool_t largeArcFlag, bool_t sweepFlag) {
  const float radPerDeg = M_PI / 180.0f;
  info->seg_index = 0;

  if ((from.x == to.x && from.y == to.y) || (radii.x == 0.0f || radii.y == 0.0f)) {
    info->num_segs = 0;
    return RET_OK;
  }
  /*
  // Convert to center parameterization as shown in
  // http://www.w3.org/TR/SVG/implnote.html
  */
  info->rx = fabs(radii.x);
  info->ry = fabs(radii.y);

  info->sin_phi = sin(angle * radPerDeg);
  info->cos_phi = cos(angle * radPerDeg);

  float x1dash = info->cos_phi * (from.x - to.x) / 2.0f + info->sin_phi * (from.y - to.y) / 2.0f;
  float y1dash = -info->sin_phi * (from.x - to.x) / 2.0f + info->cos_phi * (from.y - to.y) / 2.0f;

  float root;
  float numerator = info->rx * info->rx * info->ry * info->ry -
                    info->rx * info->rx * y1dash * y1dash - info->ry * info->ry * x1dash * x1dash;

  if (numerator < 0.0f) {
    /*
    //  If info->rx , info->ry and are such that there is no solution (basically,
    //  the ellipse is not big enough to reach from 'from' to 'to'
    //  then the ellipse is scaled up uniformly until there is
    //  exactly one solution (until the ellipse is just big enough).

    // -> find factor s, such that numerator' with info->rx'=s*info->rx and
    //    info->ry'=s*info->ry becomes 0 :
    */
    float s = sqrt(1.0f - numerator / (info->rx * info->rx * info->ry * info->ry));

    info->rx *= s;
    info->ry *= s;
    root = 0.0f;

  } else {
    root = (largeArcFlag == sweepFlag ? -1.0f : 1.0f) *
           sqrt(numerator /
                (info->rx * info->rx * y1dash * y1dash + info->ry * info->ry * x1dash * x1dash));
  }

  float cxdash = root * info->rx * y1dash / info->ry;
  float cydash = -root * info->ry * x1dash / info->rx;

  info->c.x = info->cos_phi * cxdash - info->sin_phi * cydash + (from.x + to.x) / 2.0f;
  info->c.y = info->sin_phi * cxdash + info->cos_phi * cydash + (from.y + to.y) / 2.0f;
  info->theta =
      calc_vector_angle(1.0f, 0.0f, (x1dash - cxdash) / info->rx, (y1dash - cydash) / info->ry);
  float dtheta = calc_vector_angle((x1dash - cxdash) / info->rx, (y1dash - cydash) / info->ry,
                                   (-x1dash - cxdash) / info->rx, (-y1dash - cydash) / info->ry);
  if (!sweepFlag && dtheta > 0)
    dtheta -= 2.0f * M_PI;
  else if (sweepFlag && dtheta < 0)
    dtheta += 2.0f * M_PI;

  /* Convert into cubic bezier segments <= 90deg */
  info->num_segs = (int32_t)(ceil(fabs(dtheta / (M_PI / 2.0f))));
  info->delta = dtheta / info->num_segs;
  info->t =
      8.0f / 3.0f * sin(info->delta / 4.0f) * sin(info->delta / 4.0f) / sin(info->delta / 2.0f);

  info->from = from;

  return RET_OK;
}

bool_t arc_info_next(arc_info_t* info, pointf_t* cp1, pointf_t* cp2, pointf_t* to) {
  if (info->seg_index == info->num_segs) {
    return FALSE;
  }

  float cosTheta1 = cos(info->theta);
  float sinTheta1 = sin(info->theta);
  float theta2 = info->theta + info->delta;
  float cosTheta2 = cos(theta2);
  float sinTheta2 = sin(theta2);

  /* a) calculate endpoint of the segment:*/
  to->x = info->cos_phi * info->rx * cosTheta2 - info->sin_phi * info->ry * sinTheta2 + info->c.x;
  to->y = info->sin_phi * info->rx * cosTheta2 + info->cos_phi * info->ry * sinTheta2 + info->c.y;

  /* b) calculate gradients at start/end points of segment:*/
  cp1->x = info->from.x +
           info->t * (-info->cos_phi * info->rx * sinTheta1 - info->sin_phi * info->ry * cosTheta1);
  cp1->y = info->from.y +
           info->t * (-info->sin_phi * info->rx * sinTheta1 + info->cos_phi * info->ry * cosTheta1);

  cp2->x = to->x +
           info->t * (info->cos_phi * info->rx * sinTheta2 + info->sin_phi * info->ry * cosTheta2);
  cp2->y = to->y +
           info->t * (info->sin_phi * info->rx * sinTheta2 - info->cos_phi * info->ry * cosTheta2);

  /* do next segment*/
  info->theta = theta2;
  info->from = *to;
  ++info->seg_index;

  return TRUE;
}
/*Adapt from https://github.com/mozilla/newtab-dev/blob/master/dom/svg/nsSVGPathDataParser.cpp end*/

#endif
