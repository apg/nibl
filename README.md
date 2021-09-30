# nibl: A tiny bite of a graphics engine

Nibl is a *minimal* graphics framework, composed of a handful of
primitive functions, suitable for primitive games and toys. It
leverages [LibSDL](https://libsdl.org) using as few dependencies as
possible, with the hope of making it easy to embed into whatever
programming environment you fancy.

Really, it's a cleanroom implementation of the rendering aspects of
antirez's old [load81](https://github.com/antirez/load81) project,
with some new constraints.

## Goals

1. Trivially usable from other languages
2. Performant enough that it's not burdensome to use
3. "Retro" enough that it can get away with all the constraints it has
4. Only depends on _core_ LibSDL, and libc
5. Never exceeds 1,024 lines of "formatted" C code\**
6. Primitively "feature complete"

> **NOTE: This project got picked up again because I said "I bet I can
> write an easy to embed 'load81' in < 200 lines of code." The initial
> "complete" version was about 180 lines of poorly formatted C.

## API

* See `nibl.h` for now, and checkout `example-font.c` for setup.


## Copyright and Acknowledgements

This was originally antirez's
[idea](https://github.com/antirez/load81), almost 10 years
ago. [load81](https://github.com/antirez/load81) was released under a
2-Clause BSD License, in March 2012.

The font, [Tom
Thumb](https://robey.lag.net/2010/01/23/tiny-monospace-font.html), has
been used under the terms of the MIT License, and is copyright Robey
Pointer, 2010, building on the work of Brian J. Swetland.

Finally, this distribution is released under the 2 Clause BSD License,
and any appropriate copyrights not subject to the above, are henceforth
copyright 2021, Andrew Gwozdziewycz. See the [COPYING](./COPYING) file
for more information.

Copyright (c) 2021, Andrew Gwozdziewycz. All rights reserved.

