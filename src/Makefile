all: build run clean

build: game.cpp main.cpp
		g++ -g main.cpp game.cpp bitboard.cpp pieces.cpp moves.cpp -o game

run: build game
		./game

clean:
		rm -f game