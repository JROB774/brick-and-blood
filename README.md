# Secret Santa Jam (???)

...

## Development

The game was developed in C using the **[SDL2](https://libsdl.org/)** library and compiled
using the **[TCC](https://bellard.org/tcc/)** compiler. A port for web browsers was also
developed using **[emscripten](https://github.com/emscripten-core/emscripten)**.

## Building

### Windows

Run the `buildwin32.bat` file to build the application. The script is setup to
require *TCC* in order to build, however setting up the project to build with
another C/C++ compiler would be relatively simple. All external dependencies
are provided with the repo.

### Web

Run the `buildweb.bat` file in order to build the application. This requires
*emscripten* to be setup in order for the build to work. All external dependencies
are provided with the repo.

## License

The project's code is available under the **[MIT License](https://github.com/JROB774/revivaljam/blob/master/LICENSE)**.
