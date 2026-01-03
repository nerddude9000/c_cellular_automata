# Cellular Automata simulation

Simple 'falling sands' simulation application, made in C99 and uses Raylib for rendering. It's meant as a pet project and can probably be used for educational purposes.

## Features

This isn't currently finished, and i will work on it from time to time, don't expect a full fledged simulation though.

Here is the 'philosophy' of this project:

- Configurable.
- Has many unique cell types (sand, water, wood, fire... and more) that interact with eachother.
- Somewhat realistic-ish... (heat, fire spread, gravity, chemicals... not all of which are implemnted yet).
- Should be as performant as reasonably possible.
- Should not depend on anything other than Raylib (third party wise).

## Building

You can build this by using make:

`make`

`make run`  // builds and runs the debug build

`make debug`

`make release`

For now you need to install raylib on your machine for it to work, i will add the lib to the project files soon.

This wasn't tested on windows, but it should be easy enough to build as it has only one .c file.


## License

You can do whatever you want with this as long as it doesn't go against Islamic Teachings, whose prohibitions include adding music.

