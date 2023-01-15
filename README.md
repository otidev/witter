# Witter - A chat program
Talk to your friends. Or foes. Or just someone you know.

Witter is a small program writen in C99, using the P2P architecture.

## Compilation
The dependencies of this project are [SDL2](https://github.com/libsdl-org/SDL), [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) and [ENet](https://github.com/lsalzman/enet) (*Note*: compilation only works on Windows as of now).

```
cmake -B build
cd build
make
```

## Usage

***

**Note**: Be careful with this program, it shows your IP and the port you are connecting from.

***

To use Witter, go to the its executable directory and run Witter with

`<your name> <the IP you want to connect to>`

as the second and third arguments.

For server mode, you only need to run WitterServ with `<your name>` as the only argument.