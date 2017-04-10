# Jump 'n Bump

This is a game for the whole family. You play cute fluffy little bunnies and
hop on each other's heads.

At the beginning you are in the menu, where you have to let each active player
jump over the tree trunk to enter the play area, and then walk to the right.
You will then enter the arena. The aim is to jump on the other bunnies' heads…

Jump 'n Bump was originally a DOS game by Brainchild Design, which was open
sourced under the GPL license and ported to SDL, and then SDL2.

## Controls

The controls are keyboard layout-independent, which means that regardless of
the layout that you are using (e.g. AZERTY or Dvorak), they are located as if
it were QWERTY.

The controls on a **QWERTY** keyboard are:

- A, W, D to steer Dot
- ←, ↑, → to steer Jiffy
- J, I, L to steer Fizz
- 4, 8, 6 to steer Mijji (on the numeric pad)

You can also use joysticks (main axis and button 1 will be used).

- F10 toggles fullscreen
- ESC ends the game
- F12 closes the game and quits

## Extra levels

Additional levels are available on the website of
[Brainchild Design](http://www.brainchilddesign.com/games/jumpnbump/levels/levels1.html).
You can launch them with `jumpnbump -dat levelname.dat`.

## Networking

Jump 'n Bump can be played in multiplayer over the network. This can be done
with these commands:

Player 1: `jumpnbump -port 7777 -net 0 <ip_or_hostname_of_player2> <port_of_player2>`
Player 2: `jumpnbump -port 7777 -net 1 <ip_or_hostname_of_player1> <port_of_player1>`

You have to make sure that the port udp/7777 is open in your firewall.

You can add -net 2 and -net 3 for 3rd and 4th player, and make sure that all
the players are using the same `-dat level.dat`, if any.

## Compilation

To build Jump 'n Bump you will need the SDL2, SDL2_mixer and SDL2_net
development libraries, as well as BZip2 and Zlib.

You can then run `make` to build the engine, and `make install` (as root or
sudo) to install it system wide. You can impact the default installation paths
via the Make variables defined at the top of the `Makefile`.

## License

Jump 'n Bump is distributed under the GNU General Public License, version 2, or
(at your option) any later version (GPL-2.0+). See the AUTHORS file for
credits.
