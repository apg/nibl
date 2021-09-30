#include <SDL.h>
#include <stdint.h>
#include "nibl.h"

static int
randn(int n)
{
  return rand() % n;
}

void *
setup(struct Nibl *n)
{
  nibl_setbg(n, 0, 0, 0, 255);
  nibl_setfg(n, 255, 255, 255, 255);
  return NULL;
}

int32_t
draw(struct Nibl *n, void *userdata)
{
  int32_t click, x, y, xr, yr, xoff = 0, yoff = 0;
  (void)userdata;
  click = nibl_mouse(n, &x, &y, &xr, &yr);
  if (click) {
    nibl_setfg(n, randn(256), randn(256), randn(256), 255);
    printf("Click: (%d, %d, %d, %d)\n", x, y, xr, yr);
  }

  if (nibl_keyboard(n, SDLK_a)) {
    xoff += randn(30) - 15;
    yoff += randn(30) - 15;
  }

  nibl_background(n, 255, 255, 255);
  nibl_text(n, "abcdefghijklmnopqrstuvwxyz", xoff+10, yoff+(10*4));
  nibl_text(n, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", xoff+10, yoff+(20*4));
  nibl_text(n, "0123456789!@#$%^&*()-_=+", xoff+10, yoff+(30*4));
  nibl_text(n, "[{]}\\|;:'\",<.>/?", xoff+10, yoff+(40*4));

  nibl_line(n, 0, 0, 320, 240);
  nibl_box(n, 200, 200, 20, 20, 1);
  return 0;
}

int
main(int argc, char **argv)
{
  struct Nibl *n = nibl_new(setup, draw);
  nibl_start(n);
}
