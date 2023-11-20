# Battleship in C (minor) ++

An implementation of the Battleship game using C++ and the SFML library

### build

So far it has only been tested on Arch Linux.

It's made using C++ and SFML and built using the `make` utility, so on Arch you have to install the `sfml` and `make` packages (and `gcc` if you don't have it installed already).

### how to play

During the ship placement stage `left click` to place the ship down, `right click` to rotate it 90 degrees to the right.

During the actual game `left click` to shoot at a field. You only have one shot per round (for the moment).

### todo

- [ ] clean up code
- [ ] make Linux distro agnostic build
- [ ] make Windows build

- [ ] add better enemy AI
	- [ ] maybe difficulty levels?
- [ ] add replay function
- [ ] one shot per non-destroyed ship?