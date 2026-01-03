# Cellular Automata in C

Simple 'falling sands' simulation application, made in C99 and uses Raylib for rendering. It's meant as a pet project to learn C, and it can probably be used for educational purposes.

Don't expect 999-years-of-experience C code (:

## Features

This isn't currently finished, and i will work on it from time to time.

Here is the general plan for this project (many of which are already implemented):

- Has many unique cell types (sand, water, wood, fire... and more) that interact with eachother.
- Somewhat realistic-ish... (heat, fire spread, gravity, chemicals... not all of which are implemnted yet).
- UI.
- Should be as performant as reasonably possible.
- Should not depend on anything other than Raylib (third party wise).
- Easily configurable using a config file (currently config.h).

## Building

You can build this by using make:

`make`          // builds everything to 'build/'

`make run`      // builds and runs the debug build

`make debug`    // debug build

`make release`  // release build

For now you need to install raylib on your machine for it to work, i will add the lib to the project files soon.

This wasn't tested on windows, but it should be easy enough to build as it has only one .c file.

## Usage

You can refer to `config.h` for the controls, but generally you use:

- the number row to select cell types,
- left click for inserting cells,
- right click for deleting,
- mouse wheel to change brush size.

## Pull requests

I'm currently not accepting pull requests, this is a pet project and i want to work on it solo, you're free to fork though (refer to the License section beforehand).

## License

You can do whatever you want with this as long as it doesn't go against Islamic teachings, whose prohibitions include adding music (and other things).

