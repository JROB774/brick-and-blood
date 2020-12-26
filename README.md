# Secret Santa Jam (BRICK AND BLOOD)

### ***THIS PROJECT HAS NOW BEEN ARCHIVED!***

*The project has been archived to preserve its state at the end of 2020's
**[Secret Santa Jam](https://itch.io/jam/secret-santa)** contest.*

***Below is the original README for this repository.***

This project was made for 2020's **[Secret Santa Jam](https://itch.io/jam/secret-santa)**.
It is a simple 2D turn-based top-down survival game (more of a prototype than a fully fleshed out experience).
A download for the game can be found over on **[itch.io](https://jrob774.itch.io/brick-and-blood)**, the game
is also playable in browser on the same page.

## Development

The game was developed in C++17 using the **[SDL2](https://libsdl.org/)** library. The game
was ported to the web using **[emscripten](https://github.com/emscripten-core/emscripten)**.

## Building

### Windows

Run the `build/win32/build.bat` file in order to build the application.
The script supports Visual Studio. By commenting out certain lines in the file
`build/win32/utility/buildvars.bat` you can control which type of build to
compile (Debug/Release x86/x64). C++17 is required and all of the external
dependencies are provided with the repo.

### Web

Run the `build/web/build.bat` file in order to build the application. This
requires *emscripten* to be setup in order for the build to work. C++17 is
required and all of the external dependencies are provided with the repo.

## Attribution

The art assets are public domain assets from the **[1-Bit Asset Pack](https://kenney.nl/assets/bit-pack)** made by **[Kenney](https://kenney.nl/)**.

The game font is the pixel art font **[Weiholmir](https://justfredrik.itch.io/weiholmir)** made by **[JustFredrik](https://justfredrik.itch.io/)**.

The title font is the, free for personal use, font **[billieBob](https://www.1001fonts.com/billiebob-font.html)** made by **[JOEBOB graphics](https://www.joebobgraphics.com/)**.

## License

The project's code is available under the **[MIT License](https://github.com/JROB774/santajam/blob/master/LICENSE)**.
