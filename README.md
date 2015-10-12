# Jump n Bump

This is a game for the whole family. You are cute fluffy little bunnies and hop on each other's heads.

It was a DOS game which had then be ported to SDL, this is an SDL2 port.

At the beginning you are in the menu, where you have to jump over the tree trunk if you want to play and walk right.
Then you'll enter the arena. The aim is to jump on the other bunnies' heads…

The keys are layout-independent, which means whatever layout you use they are located as if it were a qwerty.  
The controls on a **QWERTY** keyboard are:

- A,W,D to steer Dot
- ←,↑,→ to steer Jiffy
- J,I,L to steer Fizz
- 4,8,6 to steer Mijji (on the numeric pad)

You can also use joysticks (main axe and button 1 will be used).

- F10 toggles fullscreen
- ESC end the game
- F12 close the game and quits

For compilation you will need SDL2, SDL2-mixer and SDL2-net

You can find more levels at http://www.brainchilddesign.com/games/jumpnbump/levels/levels1.html  
which you can start with: jumpnbump -dat levelname.dat

Network play works like this:  
Player 1: jumpnbump -port 7777 -net 0 ip_or_hostname_of_player2 port_of_player2  
Player 2: jumpnbump -port 7777 -net 1 ip_or_hostname_of_player1 port_of_player1  
just take port 7777 for all ports and hope it's open and you aren't firewalled (it's all udp!).
You can add -net 3 and -net 4 for 3rd and 4th player, and be sure all the players use the same -dat level.dat
(the scoreboards is different on each player and strange things like that)

*For the original README look at README.old*

Thanks to:

- Brainchild Design for the original game from 1998.
- Chuck Mason (sabrewulf) for the original Linux port.
- Florian Schulze for the SDL port.
- ben for making patches to run it on mac os x.
- ivo for doing debian packages.
- Ryan C. Gordon.
