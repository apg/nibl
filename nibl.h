#ifndef NIBL_H_
#define NIBL_H_
#include <SDL.h>
#include <stdint.h>
#define NIBL_WIDTH 320
#define NIBL_HEIGHT 240
#define NIBL_ZOOM 2
#define NIBL_FPS 30
struct Nibl;
void nibl_setfg(struct Nibl *n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void nibl_setbg(struct Nibl *n, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void nibl_background(struct Nibl *n, uint8_t r, uint8_t g, uint8_t b);
void nibl_line(struct Nibl *n, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void nibl_triangle(struct Nibl *n, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);
void nibl_box(struct Nibl *n, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t fill);
void nibl_point(struct Nibl *n, int32_t x, int32_t y);
void nibl_text(struct Nibl *n, char *s, int32_t x, int32_t y);
struct Nibl *nibl_new(void *(*setup)(struct Nibl *n), int32_t (*draw)(struct Nibl *n, void *userdata));
void nibl_destroy(struct Nibl *n);
void nibl_start(struct Nibl *n);
void nibl_stop(struct Nibl *n);
int32_t nibl_mouse(struct Nibl *n, int32_t *x, int32_t *y, int32_t *xrel, int32_t *yrel);
int32_t nibl_keyboard(struct Nibl *n, SDL_Keycode key);
#endif
