#include <iostream>
#include <vector>
#include <algorithm>
#include "moves.cpp"

typedef unsigned long long U64;

#define RANK_1 0
#define RANK_2 1
#define RANK_3 2
#define RANK_4 3
#define RANK_5 4
#define RANK_6 5
#define RANK_7 6
#define RANK_8 7

#define FILE_A 0
#define FILE_B 1
#define FILE_C 2
#define FILE_D 3
#define FILE_E 4
#define FILE_F 5
#define FILE_G 6
#define FILE_H 7

class BitboardInteraction {

public:
	enum enumSquare {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

std::vector<std::string> squares = {
  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

int whitePawnScore[64] = {
			0,  0,  0,  0,  0,  0,  0,  0,
			50, 50, 50, 50, 50, 50, 50, 50,
			10, 10, 20, 30, 30, 20, 10, 10,
			5,  5, 10, 25, 25, 10,  5,  5,
			0,  0,  0, 20, 20,  0,  0,  0,
			5, -5,-10,  0,  0,-10, -5,  5,
			5, 10, 10,-20,-20, 10, 10,  5,
			0,  0,  0,  0,  0,  0,  0,  0
};
int blackPawnScore[64] = { 
	0, 0, 0, 0, 0, 0, 0, 0, 
	5, 10, 10, -20, -20, 10, 10, 5, 
	5, -5, -10, 0, 0, -10, -5, 5, 
	0, 0, 0, 20, 20, 0, 0, 0, 
	5, 5, 10, 25, 25, 10, 5, 5, 
	10, 10, 20, 30, 30, 20, 10, 10, 
	50, 50, 50, 50, 50, 50, 50, 50, 
	0, 0, 0, 0, 0, 0, 0, 0
};
int blackKnightScore[64] = { 
	-50, -40, -30, -30, -30, -30, -40, -50, 
	-40, -20, 0, 5, 5, 0, -20, -40, 
	-30, 5, 10, 15, 15, 10, 5, -30, 
	-30, 0, 15, 20, 20, 15, 0, -30, 
	-30, 5, 15, 20, 20, 15, 5, -30, 
	-30, 0, 10, 15, 15, 10, 0, -30, 
	-40, -20, 0, 0, 0, 0, -20, -40, 
	-50, -40, -30, -30, -30, -30, -40, -50

};
int whiteKnightScore[64] = {
			-50,-40,-30,-30,-30,-30,-40,-50,
			-40,-20,  0,  0,  0,  0,-20,-40,
			-30,  0, 10, 15, 15, 10,  0,-30,
			-30,  5, 15, 20, 20, 15,  5,-30,
			-30,  0, 15, 20, 20, 15,  0,-30,
			-30,  5, 10, 15, 15, 10,  5,-30,
			-40,-20,  0,  5,  5,  0,-20,-40,
			-50,-40,-30,-30,-30,-30,-40,-50
};
int blackBishopsScore[64] = { 
	-20, -10, -10, -10, -10, -10, -10, -20, 
	-10, 5, 0, 0, 0, 0, 5, -10, 
	-10, 10, 10, 10, 10, 10, 10, -10, 
	-10, 0, 10, 10, 10, 10, 0, -10, 
	-10, 5, 5, 10, 10, 5, 5, -10, 
	-10, 0, 5, 10, 10, 5, 0, -10, 
	-10, 0, 0, 0, 0, 0, 0, -10, 
	-20, -10, -10, -10, -10, -10, -10, -20

};
int whiteBishopsScore[64] = {
			-20,-10,-10,-10,-10,-10,-10,-20,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-10,  0,  5, 10, 10,  5,  0,-10,
			-10,  5,  5, 10, 10,  5,  5,-10,
			-10,  0, 10, 10, 10, 10,  0,-10,
			-10, 10, 10, 10, 10, 10, 10,-10,
			-10,  5,  0,  0,  0,  0,  5,-10,
			-20,-10,-10,-10,-10,-10,-10,-20
};
int blackRooksScore[64] = { 
	0, 0, 0, 5, 5, 0, 0, 0, 
	-5, 0, 0, 0, 0, 0, 0, -5, 
	-5, 0, 0, 0, 0, 0, 0, -5, 
	-5, 0, 0, 0, 0, 0, 0, -5, 
	-5, 0, 0, 0, 0, 0, 0, -5, 
	-5, 0, 0, 0, 0, 0, 0, -5, 
	5, 10, 10, 10, 10, 10, 10, 5, 
	0, 0, 0, 0, 0, 0, 0, 0
	
};
int whiteRooksScore[64] = {
			0,  0,  0,  0,  0,  0,  0,  0,
			5, 10, 10, 10, 10, 10, 10,  5,
			-5,  0,  0,  0,  0,  0,  0, -5,
			-5,  0,  0,  0,  0,  0,  0, -5,
			-5,  0,  0,  0,  0,  0,  0, -5,
			-5,  0,  0,  0,  0,  0,  0, -5,
			-5,  0,  0,  0,  0,  0,  0, -5,
			0,  0,  0,  5,  5,  0,  0,  0
};
int blackQueenScore[64] = { 
	-20, -10, -10, -5, -5, -10, -10, -20, 
	-10, 0, 0, 0, 0, 5, 0, -10, 
	-10, 0, 5, 5, 5, 5, 5, -10, 
	-5, 0, 5, 5, 5, 5, 0, 0, 
	-5, 0, 5, 5, 5, 5, 0, -5, 
	-10, 0, 5, 5, 5, 5, 0, -10, 
	-10, 0, 0, 0, 0, 0, 0, -10, 
	-20, -10, -10, -5, -5, -10, -10, -20

};
int whiteQueenScore[64] = {
			-20,-10,-10, -5, -5,-10,-10,-20,
			-10,  0,  0,  0,  0,  0,  0,-10,
			-10,  0,  5,  5,  5,  5,  0,-10,
			-5,  0,  5,  5,  5,  5,  0, -5,
			0,  0,  5,  5,  5,  5,  0, -5,
			-10,  5,  5,  5,  5,  5,  0,-10,
			-10,  0,  5,  0,  0,  0,  0,-10,
			-20,-10,-10, -5, -5,-10,-10,-20
};
int blackKingScore[64] = { 
	20, 30, 10, 0, 0, 10, 30, 20, 
	20, 20, 0, 0, 0, 0, 20, 20, 
	-10, -20, -20, -20, -20, -20, -20, -10, 
	-20, -30, -30, -40, -40, -30, -30, -20, 
	-30, -40, -40, -50, -50, -40, -40, -30, 
	-30, -40, -40, -50, -50, -40, -40, -30, 
	-30, -40, -40, -50, -50, -40, -40, -30, 
	-30, -40, -40, -50, -50, -40, -40, -30

};

int whiteKingScore[64] = {
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-30,-40,-40,-50,-50,-40,-40,-30,
		-20,-30,-30,-40,-40,-30,-30,-20,
		-10,-20,-20,-20,-20,-20,-20,-10,
		20, 20,  0,  0,  0,  0, 20, 20,
		20, 30, 10,  0,  0, 10, 30, 20
};
	void updateBitboards() {

		whitePieces = whitePawns | whiteRooks | whiteKnights | 
					  whiteBishops | whiteKing | whiteQueen;

		blackPieces = blackPawns | blackRooks | blackKnights | 
					  blackBishops | blackKing | blackQueen;

		allPieces = whitePieces | blackPieces;
	}

	U64 updateBitboard(U64* old, Move* move) {

		U64 new_bitboard = *old;

		//clear from bit
		new_bitboard ^= (1ULL << move->from);

		//set to bit
		new_bitboard ^= (1ULL << move->to);

		return new_bitboard;
	}
	// face move pt culoarea color
	// returneaza next color
	void deletePiece(U64 piece) {
		if((whitePawns & piece))
			whitePawns ^= piece;

		if((piece & whiteRooks))
			whiteRooks ^= piece;

		if((piece & whiteBishops))
			whiteBishops ^= piece;

		if((piece & whiteKnights))
			whiteKnights ^= piece;

		if((piece & whiteQueen))
			whiteQueen ^= piece;

		if((piece & blackPawns))
			blackPawns ^= piece;
			
		if((piece & blackRooks))
			blackRooks ^= piece;

		if((piece & blackBishops))
			blackBishops ^= piece;

		if((piece & blackKnights))
			blackKnights ^= piece;

		if((piece & blackQueen))
			blackQueen ^= piece;
	}

	int setMove(Move* move, int color) {

		U64 piece = (1ULL << move->from);
		// daca move->to este piesa adversa clear bit in piesa adversa
		U64 deletedPiece = (1ULL << move->to);
		
		// sterge piesa capturata
		deletePiece(deletedPiece);

		if((whitePawns & piece)){
			whitePawns = updateBitboard(&whitePawns, move);
		}

		if((piece & whiteRooks))
			whiteRooks = updateBitboard(&whiteRooks, move);

		if((piece & whiteBishops))
			whiteBishops = updateBitboard(&whiteBishops, move);

		if((piece & whiteKing))
			whiteKing = updateBitboard(&whiteKing, move);

		if((piece & whiteKnights))
			whiteKnights = updateBitboard(&whiteKnights, move);

		if((piece & whiteQueen))
			whiteQueen = updateBitboard(&whiteQueen, move);

		if((piece & blackPawns))
			blackPawns = updateBitboard(&blackPawns, move);

		if((piece & blackRooks))
			blackRooks = updateBitboard(&blackRooks, move);

		if((piece & blackBishops))
			blackBishops = updateBitboard(&blackBishops, move);

		if((piece & blackKing))
			blackKing = updateBitboard(&blackKing, move);

		if((piece & blackKnights))
			blackKnights = updateBitboard(&blackKnights, move);

		if((piece & blackQueen))
			blackQueen = updateBitboard(&blackQueen, move);
			
		if((whitePawns & 0xff00000000000000)) {
			whitePawns ^= deletedPiece;
			whiteQueen ^= deletedPiece;
		}
		if((blackPawns & 0xff)){
			blackPawns ^= deletedPiece;
			blackQueen ^= deletedPiece;
		}
		updateBitboards();
		return 1 - color;
	}

	int getTotal(int color) {
		int scorePawn = 100;
		int scoreKnight = 320;
		int scoreBishop = 330;
		int scoreRook = 500;
		int scoreQueen = 900;
		U64 pawnsBitboard = color == 0 ? whitePawns : blackPawns; 
		U64 rooksBitboard = color == 0 ? whiteRooks : blackRooks;
		U64 knightsBitboard = color == 0 ? whiteKnights : blackKnights;
		U64 bishopsBitboard = color == 0 ? whiteBishops : blackBishops;
		U64 queensBitboard = color == 0 ? whiteQueen : blackQueen;
		U64 kingBitboard = color == 0 ? whiteKing : blackKing;
		int total = 0;
		for (int i = 0; i <= 63; i++) {
			if((pawnsBitboard & (1ULL << i)) != 0){
				total += (scorePawn + (color == 0 ? whitePawnScore[i] : blackPawnScore[i]));
			}
			if((rooksBitboard & (1ULL << i)) != 0){
				total += (scoreRook + (color == 0 ? whiteRooksScore[i] : blackRooksScore[i]));
				//total += scoreRook;
			}
			if((knightsBitboard & (1ULL << i)) != 0){
				total += (scoreKnight + (color == 0 ? whiteKnightScore[i] : blackKnightScore[i]));
				//total += scoreKnight;
			}
			if((bishopsBitboard & (1ULL << i)) != 0){
				total += (scoreBishop + (color == 0 ? whiteBishopsScore[i] : blackBishopsScore[i]));
				//total += scoreBishop;
			}
			if((queensBitboard & (1ULL << i)) != 0){
				total += (scoreQueen + (color == 0 ? whiteQueenScore[i] : blackQueenScore[i]));
				//total += scoreQueen;
			}
		}
		return total;
	}

	//white pawns
	U64 whitePawns = 0xff00;

	//white rooks
	U64 whiteRooks = 0x81;
	
	//white knights
	U64 whiteKnights = 0x42;

	//white bishops
	U64 whiteBishops = 0x24;

	//white king
	U64 whiteKing = 0x10;

	//white queen
	U64 whiteQueen = 0x8;

	//black pawns
	U64 blackPawns = 0xff000000000000;

	//black rooks
	U64 blackRooks = 0x8100000000000000;

	//black knights
	U64 blackKnights = 0x4200000000000000;

	//black bishops
	U64 blackBishops = 0x2400000000000000;

	//black king
	U64 blackKing = 0x1000000000000000;

	//black queen
	U64 blackQueen = 0x800000000000000;

	U64 whitePieces = 0xffff;

	U64 blackPieces = 0xffff000000000000;

	U64 allPieces = 0xffff00000000ffff;

	std::vector<U64> clearRank = {
		0xffffffffffffff00,
		0xffffffffffff00ff,
		0xffffffffff00ffff,
		0xffffffff00ffffff,
		0xffffff00ffffffff,
		0xffff00ffffffffff,
		0xff00ffffffffffff,
		0x00ffffffffffffff};

	std::vector<U64> fullRank = {
		0x00000000000000ff, 
		0x000000000000ff00,
		0x0000000000ff0000,
		0x00000000ff000000,
		0x000000ff00000000,
		0x0000ff0000000000,
		0x00ff000000000000,
		0xff00000000000000};

	std::vector<U64> clearFile = {
		0xfefefefefefefefe,
		0xfdfdfdfdfdfdfdfd,
		0xfbfbfbfbfbfbfbfb,
		0xf7f7f7f7f7f7f7f7,
		0xefefefefefefefef,
		0xdfdfdfdfdfdfdfdf,
		0xbfbfbfbfbfbfbfbf,
		0x7f7f7f7f7f7f7f7f};

	std::vector<U64> fullFile = {
		0x0101010101010101,
		0x0202020202020202,
		0x0404040404040404,
		0x0808080808080808,
		0x1010101010101010,
		0x2020202020202020,
		0x4040404040404040,
		0x8080808080808080};
};