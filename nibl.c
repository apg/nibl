#include <stdlib.h>
#include "nibl.h"
/* don't reset mouse, since state is  */
#define RESET_NIBL(n) do { n->stopped = 0; n->t = 0; \
    n->fgr = n->fgg = n->fgb = n->fga = 0; n->bgr = n->bgg = n->bgb = n->bga = 0; \
    memset(n->keyboard, 0, sizeof(n->keyboard)); } while (0);
#define KEY_BUFFER_SIZE 32
struct nibl_key { SDL_Keycode code; int32_t counter; };
struct Nibl {
  void *(*setup)(struct Nibl *n);
  int32_t (*draw)(struct Nibl *n, void *userdata);
  SDL_Window *window; SDL_Renderer *renderer;
  uint8_t fgr; uint8_t fgg; uint8_t fgb; uint8_t fga;   uint8_t bgr; uint8_t bgg; uint8_t bgb; uint8_t bga; /* color state */
  uint64_t t; uint8_t stopped; uint8_t fps; /* epoch, stopped */
  int32_t mx; int32_t my; int32_t mxr; int32_t myr; int32_t mpressed; /* mouse x, y, relx, rely, pressed state */
  struct nibl_key keyboard[KEY_BUFFER_SIZE];
  void *userdata; /* provided by setup, passed to draw */
};
static void panic(char *msg) {
  fprintf(stderr, msg);
  exit(1);
}
static int32_t processEvents(struct Nibl *n) {
  SDL_Event event; int32_t err; uint32_t framestart; uint32_t frameticks; int keyplace;
  n->mx = n->my = n->mxr = n->myr = n->mpressed = 0; /* reset event state */
  framestart = SDL_GetTicks();
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      for (int i = 0; i < KEY_BUFFER_SIZE; i++) {
        if (n->keyboard[i].code == 0) { keyplace = i; }
        if (n->keyboard[i].code == event.key.keysym.sym) { keyplace = i; break; }
      }
      n->keyboard[keyplace].code = event.key.keysym.sym; n->keyboard[keyplace].counter++;
    } else if (event.type == SDL_KEYUP) {
      for (int i = 0; i < KEY_BUFFER_SIZE; i++) {
        if (n->keyboard[i].code == event.key.keysym.sym) {
          n->keyboard[i].code = n->keyboard[i].counter = 0; break;
        }
      }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
      n->mpressed |= SDL_BUTTON(event.button.button) & 0x7;
      n->mx = event.motion.x; n->my = event.motion.y;
    } else if (event.type == SDL_MOUSEBUTTONUP) {
      n->mpressed &= ~(SDL_BUTTON(event.button.button)) & 0x7;
      n->mx = event.motion.x; n->my = event.motion.y;
    } else if (event.type == SDL_MOUSEMOTION) {
      n->mx = event.motion.x; n->my = event.motion.y; n->mxr = event.motion.xrel; n->myr = event.motion.yrel;
    } else if (event.type == SDL_QUIT) {
      exit(0);
    }
    if (SDL_PeepEvents(&event, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)) {
      if (event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONUP) { break; } /* notice fast up/down events */
    }
  }
  if (n->t == 0) { /* run setup on first frame */
    n->userdata = n->setup(n);
  }
  SDL_RenderClear(n->renderer);
  err = n->draw(n, n->userdata);
  SDL_RenderPresent(n->renderer);
  n->t++; frameticks = SDL_GetTicks() - framestart;
  int32_t delay = (1000 / NIBL_FPS) - frameticks;
  if (delay > 0) { SDL_Delay((uint32_t)delay); }
  return err;
}
struct Nibl *nibl_new(void *(*setup)(struct Nibl *), int32_t (*draw)(struct Nibl *, void *)) {
  struct Nibl *n = malloc(sizeof(*n));
  if (n == NULL) { panic("unable to allocate Nibl context"); }
  RESET_NIBL(n);
  n->renderer = NULL;
  n->setup = setup;
  n->draw = draw;
  return n;
}
void nibl_destroy(struct Nibl *n) {
  if (n) { free(n); }
}
void nibl_start(struct Nibl *n) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) { panic("unable to initialize SDL\n"); }
  /* get a window. get everything setup. */
  n->window = SDL_CreateWindow("Nibl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, NIBL_WIDTH, NIBL_HEIGHT, SDL_WINDOW_SHOWN);
  if (n->window == NULL) { panic("unable to create window\n"); }
  n->renderer = SDL_CreateRenderer(n->window, -1, 0);
  if (n->renderer == NULL) { panic("unable to get renderer\n"); }
  RESET_NIBL(n);
  n->fps = NIBL_FPS;
  while (!processEvents(n));
}
void nibl_stop(struct Nibl *n) {
  SDL_DestroyRenderer(n->renderer);
  SDL_DestroyWindow(n->window);
  SDL_Quit();
}
int32_t nibl_mouse(struct Nibl *n, int32_t *x, int32_t *y, int32_t *xrel, int32_t *yrel) {
  if (x != NULL) { *x = n->mx; }
  if (y != NULL) { *y = n->my; }
  if (xrel != NULL) { *xrel = n->mxr; }
  if (yrel != NULL) { *yrel = n->myr; }
  return n->mpressed;
}
int32_t nibl_keyboard(struct Nibl *n, SDL_Keycode code) {
  for (int i = 0; i < KEY_BUFFER_SIZE; i++) {
    if (n->keyboard[i].code == code) { return n->keyboard[i].counter; }
  }
  return 0;
}
void nibl_setfg(struct Nibl *n, uint8_t r, uint8_t b, uint8_t g, uint8_t a) { n->fgr = r; n->fgg = g; n->fgb = b; n->fga = a; }
void nibl_setbg(struct Nibl *n, uint8_t r, uint8_t b, uint8_t g, uint8_t a) { n->bgr = r; n->bgg = g; n->bgb = b; n->bga = a; }
void nibl_background(struct Nibl *n, uint8_t r, uint8_t g, uint8_t b) {
  SDL_SetRenderDrawColor(n->renderer, r, g, b, 255);   SDL_RenderClear(n->renderer);
}
void nibl_line(struct Nibl *n, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  SDL_SetRenderDrawColor(n->renderer, n->fgr, n->fgg, n->fgb, n->fga);
  SDL_RenderDrawLine(n->renderer, x1, y1, x2, y2);
}
void nibl_triangle(struct Nibl *n, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3) {
  SDL_Point pts[3] = { { x1, y1 }, { x2, y2 }, { x3, y3 } };
  SDL_SetRenderDrawColor(n->renderer, n->fgr, n->fgg, n->fgb, n->fga);
  SDL_RenderDrawLines(n->renderer, pts, 3);
}
void nibl_box(struct Nibl *n, int32_t x, int32_t y, int32_t h, int32_t w, int32_t fill) {
  SDL_Rect r = { .x = x, .y = y, .w = w, .h = h };
  if (fill) {
    SDL_SetRenderDrawColor(n->renderer, n->bgr, n->bgg, n->bgb, n->bga);
    SDL_RenderFillRect(n->renderer, &r);
  }
  SDL_SetRenderDrawColor(n->renderer, n->fgr, n->fgg, n->fgb, n->fga);
  SDL_RenderDrawRect(n->renderer, &r);
}
void nibl_point(struct Nibl *n, int32_t x, int32_t y) {
  SDL_SetRenderDrawColor(n->renderer, n->fgr, n->fgg, n->fgb, n->fga);
  SDL_Rect r = { .x = x, .y = y, .w = NIBL_ZOOM, .h = NIBL_ZOOM };
  SDL_RenderDrawRect(n->renderer, &r);
}
/* Licensed under MIT: https://robey.lag.net/2010/01/23/tiny-monospace-font.html */
static uint32_t nibl_font[127] = {
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x44404, 0xaa, 0xaeaea, 0x6c6c4, 0x82482, 0xccea6, 0x44, 0x24442, 0x84448, 0xa4a, 0x4e4, 0x48, 0xe, 0x4, 0x22488, 0x6aaac, 0x4c444, 0xc248e, 0xc242c, 0xaae22, 0xe8c2c, 0x68eae, 0xe2488, 0xeaeae, 0xeae2c, 0x404, 0x4048, 0x24842, 0xe0e, 0x84248, 0xe2404, 0x4ae86, 0x4aeaa, 0xcacac, 0x68886, 0xcaaac, 0xe8e8e, 0xe8e88, 0x68ea6, 0xaaeaa, 0xe444e, 0x222a4, 0xaacaa, 0x8888e, 0xaeeaa, 0xaeeea, 0x4aaa4, 0xcac88, 0x4aae6, 0xcaeca, 0x6842c, 0xe4444, 0xaaaa6, 0xaaa44, 0xaaeea, 0xaa4aa, 0xaa444, 0xe248e, 0xe888e, 0x842, 0xe222e, 0x4a, 0xe, 0x84, 0xc6ae, 0x8caac, 0x6886, 0x26aa6, 0x6ac6, 0x24e44, 0x6ae24, 0x8caaa, 0x40444, 0x2022a4, 0x8acca, 0xc444e, 0xeeea, 0xcaaa, 0x4aa4, 0xcaac8, 0x6aa62, 0x6888, 0x6c6c, 0x4e446, 0xaaa6, 0xaae4, 0xaeee, 0xa44a, 0xaa624, 0xe6ce, 0x64846, 0x44044, 0xc424c, 0x6c
};
static void nibl_drawchar(struct Nibl *n, char c, int32_t x, int32_t y) {
  uint32_t fd = nibl_font[(int32_t)c]; int count = 0; int xo = 0; int yo = 0;
  SDL_Rect rects[24] = {0};
  for (int i = 23; i >= 0; i--) { /* backgrounds from bottom right corner */
    if (fd & 1) {
      xo = i % 4; yo = i / 4;
      rects[count] = (SDL_Rect) { .x = (x + (xo*NIBL_ZOOM)), .y = (y + (yo*NIBL_ZOOM)), .w = (NIBL_ZOOM), .h = (NIBL_ZOOM) };
      count++;
    }
    fd >>= 1;
  }
  if (count > 0) { SDL_SetRenderDrawColor(n->renderer, n->fgr, n->fgg, n->fgb, n->fga); SDL_RenderFillRects(n->renderer, rects, count); }
}
void nibl_text(struct Nibl *n, char *s, int32_t x, int32_t y)
{
  int32_t cx = x;
  while (*s) {
    nibl_drawchar(n, *s, cx, y); cx += (4 * NIBL_ZOOM) + NIBL_ZOOM; s++;
  }
}
