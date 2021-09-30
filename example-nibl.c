#include <math.h>
#include <stdlib.h>
#include "nibl.h"

struct point {
  int32_t x, y;
  uint8_t r, g, b, a;
};

struct state {
  struct point *points;
  size_t count;
  size_t index;
};

void *
setup(struct Nibl *n)
{
  struct state *s = malloc(sizeof(*s));
  s->points = calloc(16000, sizeof(struct point));
  s->count = 0;
  s->index = 0;
  int i = 0, radius = 60, x = 0, y = 0;

  /* draw a circle in yellow */
  for (x = -radius; x <= radius; x++) {
    y = (int) (sqrt((radius*radius) - (x*x)) + .5);
    struct point p = {
      .x = (160 + x),
      .y = (120 + y),
      .r = 255,
      .g = 255,
      .b = 0,
      .a = 255
    };
    s->count++;
    s->points[i++] = p;
    s->count++;
    p.y = (120 - y);
    s->points[i++] = p;
    for (int yi = 120 - y; yi < 120 + y; yi++) {
      p.y = yi;
      s->points[i++] = p;
      s->count++;
    }
  }

  return (void *) s;
}

int32_t
draw(struct Nibl *n, void *userdata)
{
  struct state *s = userdata;
  nibl_background(n, 0, 0, 0);
  for (int i = 0; i < s->count; i++) {
    nibl_setfg(n, s->points[i].r, s->points[i].g, s->points[i].b, s->points[i].a);
    nibl_point(n, s->points[i].x, s->points[i].y);
  }

  return 0;
}

int
main(int argc, char **argv)
{
  struct Nibl *n = nibl_new(setup, draw);
  nibl_start(n);
  return 0;
}
