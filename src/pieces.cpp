#include "bitboard.cpp"
#include <fstream>

#define WHITE 0
#define BLACK 1
#define NONE 2
#define ANALYZE 3
#define INVALID -1

enum Dir {
  NORTH,
  SOUTH,
  EAST,
  WEST,
  NORTH_EAST,
  NORTH_WEST,
  SOUTH_EAST,
  SOUTH_WEST
};

class PieceMover {

public:
	U64 rays[8][64];



	
	int col(int square) {
		return square % 8;
	}

	int row(int square) {
		return square / 8;
	}

	int bitscanForward(U64 board) {
		return board != 0ULL ? ( __builtin_ffsll(board) - 1 ) : -1;
	}

	int bitscanReverse(U64 board) {
		return board != 0ULL ?  (63 - __builtin_clzll(board)) : -1;
	}

	U64 eastN(U64 board, int n,BitboardInteraction* bitboard) {
		U64 newBoard = board;
		for (int i = 0; i < n; i++) {
			newBoard = ((newBoard << 1) & (~bitboard->fullFile[FILE_A]));
		}
		return newBoard;
	}
	
	U64 westN(U64 board, int n, BitboardInteraction* bitboard) {
		U64 newBoard = board;
		for (int i = 0; i < n; i++) {
			newBoard = ((newBoard >> 1) & (~bitboard->fullFile[FILE_H]));
		}
		return newBoard;
	}

	PieceMover(BitboardInteraction* bitboard) {
		for (int square = 0; square < 64; square++) {
			// North
			rays[NORTH][square] = 0x0101010101010100ULL << square;

			// South
			rays[SOUTH][square] = 0x0080808080808080ULL >> (63 - square);

			// East
			rays[EAST][square] = 2 * ((1ULL << (square | 7)) - (1ULL << square));

			// West
			rays[WEST][square] = (1ULL << square) - (1ULL << (square & 56));

			// North West
			rays[NORTH_WEST][square] = westN(0x102040810204000ULL, 7 - col(square), bitboard) << (row(square) * 8);

			// North East
			rays[NORTH_EAST][square] = eastN(0x8040201008040200ULL, col(square), bitboard) << (row(square) * 8);

			// South West
			rays[SOUTH_WEST][square] = westN(0x40201008040201ULL, 7 - col(square), bitboard) >> ((7 - row(square)) * 8);

			// South East
			rays[SOUTH_EAST][square] = eastN(0x2040810204080ULL, col(square),bitboard) >> ((7 - row(square)) * 8);
  		}
	}
	//verifies if a move is legal by seeing if it gets the king in check
	bool isLegal(BitboardInteraction bitboard, Move move, int color) {
		BitboardInteraction newState = bitboard;
		int hasMade = newState.setMove(&move, color);
		if(isCheck(&newState,color))
			return false;
		return true;
	}
	// if you are in check and you have no legal moves it results in a check mate
	bool isMate(BitboardInteraction *bitboard, int color) {
		return (isCheck(bitboard,color) && (getMoves(bitboard,color).size() == 0));
	}
	// verifies if King color is in check
	bool isCheck(BitboardInteraction* bitboard, int color) {
		// pt toate piesele adverse, daca atac_piese & rege != 0 e sah
		U64 opPawns = color == 1 ? bitboard->whitePawns : bitboard->blackPawns; 
		U64 opRooks = color == 1 ? bitboard->whiteRooks : bitboard->blackRooks;
		U64 opKnights= color == 1 ? bitboard->whiteKnights : bitboard->blackKnights;
		U64 opBishops = color == 1 ? bitboard->whiteBishops : bitboard->blackBishops;
		U64 opQueens = color == 1 ? bitboard->whiteQueen : bitboard->blackQueen;
		U64 ourKing = color == 0 ? bitboard->whiteKing : bitboard->blackKing;
		
		for (int i = 0; i < 64; i++) {
			if ((opPawns & (1ULL << i)) != 0) {
				U64 opPawnAttacks = color == 1 ? getWhitePawnMoves(1ULL << i, bitboard) : getBlackPawnMoves(1ULL << i, bitboard);
				if ((opPawnAttacks & ourKing) != 0) {
					return true;
				}
			}

			if ((opRooks & (1ULL << i)) != 0) {
				U64 opRooksAttacks = getRooksMoves(1ULL << i, bitboard, 1 - color);
				if ((opRooksAttacks & ourKing) != 0) {
					return true;
				}
			}
			
			if ((opKnights & (1ULL << i)) != 0) {
				U64 opKnightsAttacks = getKnightMoves(1ULL << i, bitboard, 1 - color);
				if ((opKnightsAttacks & ourKing) != 0) {
					return true;
				}
			}
			if ((opBishops & (1ULL << i)) != 0) {
				U64 opBishopsAttacks = getBishopMoves(1ULL << i, bitboard, 1 - color);
				if ((opBishopsAttacks & ourKing) != 0) {
					return true;
				}
			}
			if ((opQueens & (1ULL << i)) != 0) {
				U64 opQueensAttacks = getQueenMoves(1ULL << i, bitboard, 1 - color);
				if ((opQueensAttacks & ourKing) != 0) {
					return true;
				}
			}
			
		}

		return false;
	}

	int eval(BitboardInteraction* bitboard, int color) {
		// color is our color
		if (isCheck(bitboard, color)) {
			return -INT32_MAX;
		}

		int score = bitboard->getTotal(color) - bitboard->getTotal(1 - color);

		if (isCheck(bitboard, 1 - color)) {
			return score + 2;
		}
		
		return score;
	}

	std::pair<int, Move> negamax(BitboardInteraction* bitboard, int color, int depth, int alpha, 
								int beta) {
		// depth over
		if (depth == 0)
			return std::make_pair<int, Move>(eval(bitboard, color), {0, 0});

		Move bestMove;

		// for all moves
		for (auto& m : getMoves(bitboard, color)) {
			BitboardInteraction newState = *bitboard;
			int hasMade = newState.setMove(&m, color);
			int currentScore = -negamax(&newState, 1 - color, depth - 1, -beta, -alpha).first;
			if (currentScore >= alpha) {
				alpha = currentScore;
				bestMove = m;
			}

			if (alpha >= beta) {
				break;
			}
		}
		std::pair<int, Move> finalMove = std::make_pair(alpha, bestMove);

		return finalMove;
	}
	
	// functie care sa ia fiecare 1 din u64 si sa intoarca caracter
	std::vector<Move> getMoves(BitboardInteraction* bitboard, int color) {
		std::vector<Move> moves;
		U64 pawnsBitboard = color == 0 ? bitboard->whitePawns : bitboard->blackPawns; 
		U64 rooksBitboard = color == 0 ? bitboard->whiteRooks : bitboard->blackRooks;
		U64 knightsBitboard = color == 0 ? bitboard->whiteKnights : bitboard->blackKnights;
		U64 bishopsBitboard = color == 0 ? bitboard->whiteBishops : bitboard->blackBishops;
		U64 queensBitboard = color == 0 ? bitboard->whiteQueen : bitboard->blackQueen;
		U64 kingBitboard = color == 0 ? bitboard->whiteKing : bitboard->blackKing;

		// Generating all bitboards with each piece
		for (int i = 0; i <= 63; i++) {
			if ((pawnsBitboard & (1ULL << i)) != 0) {
				U64 potentialMoves = color == 0 ? getWhitePawnMoves(1ULL << i, bitboard) : getBlackPawnMoves(1ULL << i, bitboard);
				for ( int j = 0; j <= 63; j++ ) {
					if ((potentialMoves & (1ULL << j)) != 0) {
						Move move;
						move.from = i;
						move.to = j;
						if(isLegal(*bitboard,move,color))
							moves.push_back({i, j});	
						}
				}
					
			}

		if((rooksBitboard & (1ULL << i)) != 0) {
			U64 potentialMoves = getRooksMoves(1ULL << i, bitboard, color);
			for ( int j = 0; j <= 63; j++ ) {
				if ((potentialMoves & (1ULL << j)) != 0) {
					Move move;
					move.from = i;
					move.to = j;
					if(isLegal(*bitboard,move,color))
						moves.push_back({i, j});	
				}
			}
		}

		if((knightsBitboard & (1ULL << i)) != 0) {
			U64 potentialMoves = getKnightMoves(1ULL << i, bitboard, color);
			for ( int j = 0; j <= 63; j++ ) {
				if ((potentialMoves & (1ULL << j)) != 0) {
					Move move;
					move.from = i;
					move.to = j;
					if(isLegal(*bitboard,move,color))
						moves.push_back({i, j});	
				}
			}
		}

		if((bishopsBitboard & (1ULL << i)) != 0) {
			U64 potentialMoves = getBishopMoves(1ULL << i, bitboard, color);
			for ( int j = 0; j <= 63; j++ ) {
				if ((potentialMoves & (1ULL << j)) != 0) {
					Move move;
					move.from = i;
					move.to = j;
					if(isLegal(*bitboard,move,color))
						moves.push_back({i, j});	
				}
			}
		}

		if((queensBitboard & (1ULL << i)) != 0) {
			U64 potentialMoves = getQueenMoves(1ULL << i, bitboard, color);
			for ( int j = 0; j <= 63; j++ ) {
				if ((potentialMoves & (1ULL << j)) != 0) {
					Move move;
					move.from = i;
					move.to = j;
					if(isLegal(*bitboard,move,color))
						moves.push_back({i, j});
				}
			}
		}

			if((kingBitboard & (1ULL << i)) != 0) {
				U64 potentialMoves = getKingMoves(kingBitboard, bitboard, color);
				for ( int j = 0; j <= 63; j++ ) {
					if ((potentialMoves & (1ULL << j)) != 0) {
						Move move;
						move.from = i;
						move.to = j;
						if(isLegal(*bitboard,move,color))
							moves.push_back({i, j});	
					}
				}
			}
		}

		return moves;		
	}
	
	//returneaza bitboard cu toate miscarile pt pionu pawn
	//noi o folosim sa luam toate miscarile pt toti pionii, si noi ar trebui sa decidem cea mai buna miscare pt pionu pawn
	U64 getWhitePawnMoves(U64 pawn, BitboardInteraction* bitboard) {

		//moves a white pawn, a step forward
		U64 whiteOneStep = (pawn << 8) & ~(bitboard->allPieces);

		//moves a white pawn two steps forward
		U64 whiteTwoSteps = 
		((whiteOneStep & bitboard->fullRank[RANK_3]) << 8) & ~(bitboard->allPieces);

		U64 whitePawnLeftAttack = (pawn & bitboard->clearFile[FILE_A]) << 7;
		U64 whitePawnRightAttack = (pawn & bitboard->clearFile[FILE_H]) << 9;

		U64 whitePawnAttacks = whitePawnLeftAttack | whitePawnRightAttack;

		U64 whitePawnValidAttacks = whitePawnAttacks & bitboard->blackPieces;

		return whiteOneStep | whiteTwoSteps | whitePawnValidAttacks;
	}

	U64 getBlackPawnMoves(U64 pawn, BitboardInteraction* bitboard) {

		//moves a black pawn, a step forward
		U64 blackOneStep = (pawn >> 8) & ~(bitboard->allPieces);

		//moves a black pawn two steps forward
		U64 blackTwoSteps = 
		((blackOneStep & bitboard->fullRank[RANK_6]) >> 8) & ~(bitboard->allPieces);

		U64 blackPawnLeftAttack = (pawn & bitboard->clearFile[FILE_A]) >> 7;
		U64 blackPawnRightAttack = (pawn & bitboard->clearFile[FILE_H]) >> 9;

		U64 blackPawnAttacks = blackPawnLeftAttack | blackPawnRightAttack;

		U64 blackPawnValidAttacks = blackPawnAttacks & bitboard->whitePieces;

		return blackOneStep | blackTwoSteps | blackPawnValidAttacks;
	}

	U64 getKnightMoves(U64 knight, BitboardInteraction* bitboard, int color) {
		U64 spot_1_clip = bitboard->clearFile[FILE_A] & bitboard->clearFile[FILE_B];
		U64 spot_2_clip = bitboard->clearFile[FILE_A];
		U64 spot_3_clip = bitboard->clearFile[FILE_H];
		U64 spot_4_clip = bitboard->clearFile[FILE_H] & bitboard->clearFile[FILE_G];

		U64 spot_5_clip = bitboard->clearFile[FILE_H] & bitboard->clearFile[FILE_G];
		U64 spot_6_clip = bitboard->clearFile[FILE_H];
		U64 spot_7_clip = bitboard->clearFile[FILE_A];
		U64 spot_8_clip = bitboard->clearFile[FILE_A] & bitboard->clearFile[FILE_B];

		U64 spot_1 = (knight & spot_1_clip) << 6;
		U64 spot_2 = (knight & spot_2_clip) << 15;
		U64 spot_3 = (knight & spot_3_clip) << 17;
		U64 spot_4 = (knight & spot_4_clip) << 10;

		U64 spot_5 = (knight & spot_5_clip) >> 6;
		U64 spot_6 = (knight & spot_6_clip) >> 15;
		U64 spot_7 = (knight & spot_7_clip) >> 17;
		U64 spot_8 = (knight & spot_8_clip) >> 10;

		U64 knightValid = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |
						spot_7 | spot_8;
		
		return color == 0 ? (knightValid & ~bitboard->whitePieces) : (knightValid & ~bitboard->blackPieces);
	}

	U64 getKingMoves(U64 king, BitboardInteraction* bitboard, int color){

		U64 king_clip_file_h = king & bitboard->clearFile[FILE_H]; 
		U64 king_clip_file_a = king & bitboard->clearFile[FILE_A]; 

		U64 spot_1 = king_clip_file_h << 7; 
		U64 spot_2 = king << 8; 
		U64 spot_3 = king_clip_file_h << 9; 
		U64 spot_4 = king_clip_file_h << 1; 

		U64 spot_5 = king_clip_file_a >> 7; 
		U64 spot_6 = king >> 8; 
		U64 spot_7 = king_clip_file_a >> 9; 
		U64 spot_8 = king_clip_file_a >> 1; 

		U64 king_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |
							spot_7 | spot_8; 

		return  color == 0 ? (king_moves & ~bitboard->whitePieces) : (king_moves & ~bitboard->blackPieces); 
	}
	  

	U64 getBishopMoves(U64 bishop, BitboardInteraction* bitboard, int color) {
		U64 myPieces = color == 0 ? bitboard->whitePieces : bitboard->blackPieces;
		U64 blockers = bitboard->allPieces;
		U64 attacks = 0ULL;
		int square = bitscanForward(bishop);
		// North West
		attacks |= rays[NORTH_WEST][square];
		if (rays[NORTH_WEST][square] & blockers) {
			int blockerIndex = bitscanForward(rays[NORTH_WEST][square] & blockers);
			attacks &= ~rays[NORTH_WEST][blockerIndex];
		}

		// North East
		attacks |= rays[NORTH_EAST][square];
		if (rays[NORTH_EAST][square] & blockers) {
			int blockerIndex = bitscanForward(rays[NORTH_EAST][square] & blockers);
			attacks &= ~rays[NORTH_EAST][blockerIndex];
		}

		// South East
		attacks |= rays[SOUTH_EAST][square];
		if (rays[SOUTH_EAST][square] & blockers) {
			int blockerIndex = bitscanReverse(rays[SOUTH_EAST][square] & blockers);
			attacks &= ~rays[SOUTH_EAST][blockerIndex];
		}

		// South West
		attacks |= rays[SOUTH_WEST][square];
		if (rays[SOUTH_WEST][square] & blockers) {
			int blockerIndex = bitscanReverse(rays[SOUTH_WEST][square] & blockers);
			attacks &= ~rays[SOUTH_WEST][blockerIndex];
		}

		attacks &= ~myPieces;
		return attacks;
	}
	//north, south, east and west.
		U64 getRooksMoves(U64 rook, BitboardInteraction* bitboard, int color) {
		U64 myPieces = color == 0 ? bitboard->whitePieces : bitboard->blackPieces;
		U64 blockers = bitboard->allPieces;
		U64 attacks = 0ULL;
		int square = bitscanForward(rook);

		attacks |= rays[NORTH][square];
		if (rays[NORTH][square] & blockers) {
			int blockerIndex = bitscanForward(rays[NORTH][square] & blockers);
			attacks &= ~rays[NORTH][blockerIndex];
		}

		attacks |= rays[EAST][square];
		if (rays[EAST][square] & blockers) {
			int blockerIndex = bitscanForward(rays[EAST][square] & blockers);
			attacks &= ~rays[EAST][blockerIndex];
		}

		attacks |= rays[WEST][square];
		if (rays[WEST][square] & blockers) {
			int blockerIndex = bitscanReverse(rays[WEST][square] & blockers);
			attacks &= ~rays[WEST][blockerIndex];
		}

		attacks |= rays[SOUTH][square];
		if (rays[SOUTH][square] & blockers) {
			int blockerIndex = bitscanReverse(rays[SOUTH][square] & blockers);
			attacks &= ~rays[SOUTH][blockerIndex];
		}
		attacks &= ~myPieces;
		return attacks;
	}
	
	U64 getQueenMoves(U64 queen, BitboardInteraction* bitboard, int color) {
		return getRooksMoves(queen,bitboard,color) | getBishopMoves(queen,bitboard,color);
	}
};