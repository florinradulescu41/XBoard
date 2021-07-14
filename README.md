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

The engine is composed of several files with code written in C ++, as
to be listed below:

- main.cpp contains only the main function, which starts and starts the game;

- game.cpp, where several command possibilities are declared during
execution and running. The Game class contains a do-while in which they are
constantly analyzed the possible commands that the engine can receive / process
(xboard, protover, new, black etc.); important variables are of type
BitboardInteraction, Move and PieceMover, which we will detail later;

- bitboard.cpp contains the implementation of the bitboard structure. defining
RANK and FILE variables constitute the globalization of line attributes and
rows of chessboard. Bitboards being a coding of all spaces
on the chessboard, have the shape of an unsigned long long (64 bits, 1 bit for
each square of the board). The Bitboard class has an enumeration that encodes mode
classic (columns a to h, rows 1 to 8) chessboard boxes;
the update function has the role of reloading the bitboards after executing a
mutari; it is used by setMove, which receives a move that
the previously identified piece can do it; parts and groups of parts
of the same type or color have the bitboards declared separately; the moves are
interpreted as checks and changes of bits (shifts) within
different bitboards; these are specified in the pieces.cpp file;
getTotal assigns the pieces on the board a score and returns the total that
the color given as a parameter holds it; the score will be used as the mode
calculation of the best moves, depending on the eval - more details at
the part about the algorithmic approach;

- moves.cpp, Move class only;

- pieces.cpp, contains the basic operations of chess as a game, designed for
can be interpreted by a computing machine; Dir is an enumeration of directions
on which the pieces can move (ex: north - advancing on the board; west - moving in
lateral etc.); The PieceMover class contains both return functions
current positions, as well as the calculation of future positions; each direction
it is in turn represented as a shift of the bits in the bitboard; function
getMoves has the role of generating, for a certain song, all possible moves
and to store these moves in a vector that will be analyzed later by
negamax; these generations are made separately in the particular functions of
getMoves (ex: getWhitePawnMoves, getKnightMoves etc.); each of these
Particular functions use a method of calculating their own positions
future / possible movement / attack depending on the specifics of the piece assigned in
the game of chess and its rules; for example, a horse can move anywhere
in the 8 houses formed by the L specific to its unique movement, to occupy
or capture an area on the board, regardless of the surrounding fish, but a madman or
one turn is limited to movements until they can hit another piece, without being able to
passes behind her; these generated move vectors will be used in a
analysis of the best solutions for a current state by the type algorithm
negamax;


3. Details about the algorithmic approach (algorithms used, complexities)

The algorithm chosen for this stage is an alpha-beta optimized negamax, which
analyzes the best moves creating hypothetical situations resulting in the case
in which both players play optimally, in a given depth; they are evaluated
using the evaluation function and the scores assigned to the pieces, as well as the scores
for various key positions of the parts. Some of their pieces or positions are
more valuable, so capturing them could mean a gain. Also a
The move that leads the opponent's king to chess is considered quite a move
good, while a move that brings one's own king into chess is unacceptable;
The score is represented by a difference between the current player's pieces and
those of the opponent, and the purpose of the algorithm is to make the engine
maximize your own score, this result bringing us closer to victory.
The best situations for checkmate chess will be considered and preferred, meanwhile
those that bring us closer to our own defeat are undesirable.

4. Sources of inspiration

The idea of ​​implementation based on bitboards was taken from the platform
chessprogramming and on Youtube. Additional information about chess and work
with bitboard were also taken from WinBoard Forum, Wikipedia, chessprogramming,
and so on. An online bitboard computer was also useful.
