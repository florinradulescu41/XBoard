# XBoard

1. Compilation instructions:

The program compiles the sources to the make command. The same command has the effect and
start the created executable, which starts running Xboard. From this point it is
made the opening of the Xboard interface, loading the engine and starting
the possibility to transmit its commands through the graphical interface (moves
made on the chessboard).

2. Details about the project structure:

The project uses bitboards as a basis, which are some encodings of
chessboard depending on its occupation with pieces, possible places in
which they can move or attack, based on bits (1 or 0).
The principles of this bitboard-based structure will be explained above
a paragraph, when we talk about the bitboard.cpp file.

The engine consists of several files with code written in C ++. The first of
they, main.cpp, contain only the main function, which starts the game.
This file uses game.cpp, where several possibilities are declared
command during execution and running. Game class contains a do-while
in which the possible commands that the engine is constantly analyzed
can receive them and process them (xboard, protover, new, black, etc.)

The bitboard.cpp file contains the implementation of the bitboard structure. defining
RANK and FILE variables constitute the globalization of line attributes and
rows of chessboard. Bitboards being a coding of all spaces
on the chessboard, have the shape of an unsigned long long (64 bits, 1 bit for
each square of the board). The Bitboard class has an enumeration that encodes mode
classic (columns from a to h, rows 1 to 8) tin boxes.
The update function has the role of reloading the bitboards after executing one
mutari. It is used by setMove, which receives a move
the previously identified piece can do it. Parts and groups of parts
of the same type or color have the bitboards declared separately. The moves are
interpreted as checks and changes of bits (shifts) within
different bitboards. These are made explicit in the pieces.cpp file.

3. Details about the algorithmic approach (algorithms used, complexities)

This is not the case at the moment. Enigine expects commands from xboard / user, le
interpret and answer. If you have to make a move, use it
bitboards to see what moves it can make. In the case of pawns, towards
for example, the engine checks the parts (pawns) that can be moved, the positions
their current positions and the positions they can hold. Finally,
choose a position and make a valid move. If he can't do it anymore
no valid move, it resigns and the game ends.

4. Sources of inspiration

The idea of ​​implementation based on bitboards was taken from the platform
chessprogramming and on Youtube. Additional information about chess and work
with bitboard were also taken from WinBoard Forum, Wikipedia, chessprogramming,
and so on
