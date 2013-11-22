# Learn C the Hard Way Library #
* This is the library from [Learn C the Hard Way](http://c.learncodethehardway.org/book/)
* I have implemented many of the Extra-Credit and How to Improve it sections.
* Not yet complete as I am still working my way through the book.

## Changes from the book ##
* Has been made more robust with additional check() statements and more unit
  tests.
* Linked list operations `List_shift` and `List_unshift` have been renamed
  `List_fpop` and `List_fpush` for "front" pop and "front" push.
* Compiler flags have been changed slightly to allow for faster compiling
  (should not affect usage or post-compilation performance)
* Additional options for make:
  * `make devp` does `make dev` with `-O2` for testing performance.
  * `make remake` does `make clean` followed by `make`
  * `make remaked` does `make clean` followed by `make dev`
  * `make remakedp` does `make clean` followed by `make devp`

## Compilation ##
* Compile using `make`
  * The compile process is not very robust.
  * Some flags don't work with `gcc-4.8`, so I've changed `CC` to `gcc-4.2`
    installed using `brew install apple-gcc42`.
* The libs can be found as `liblcthw.a` and `liblcthw.so` in `./build`.

## License ##
* MIT License found in LICENSE
