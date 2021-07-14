#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "pieces.cpp"
#include <limits.h>

#define DEPTH 3

class Game {

private:

	BitboardInteraction* b = new BitboardInteraction();

	Move move;

	PieceMover* mover = new PieceMover(b);

	//default
	int engineSide = NONE;
	int sideToMove = WHITE;
	bool firstMove = false;

public:

	void makeMove(Move move2) {
	    sideToMove = b->setMove(&move2, engineSide);
		std::string com = "move " + b->squares[move2.from] + b->squares[move2.to] + "\n";
		std::cout << com;
	}

	void startGame() {
		std::string s;

	    do {

	    	fflush(stdout);

	    	//read the command
	    	std::getline(std::cin, s);

	    	if(s.find("xboard") != std::string::npos) {
	    		std::cout << std::endl;
	    		continue;
	    	}

	    	if(s.find("protover") != std::string::npos) {
	    		std::cout << "feature usermove=1 sigint=0 sigterm=0 done=1" << std::endl;
	    		continue;
	    	}

	    	if(s.find("new") != std::string::npos) {
	    		engineSide = BLACK;
	    		continue;
	    	}

	    	if(s.find("go") != std::string::npos){
	    		makeMove(mover->negamax(b, engineSide, DEPTH, -INT_MAX, INT_MAX).second);
	    	}

	    	if(s.find("random") != std::string::npos)
	    		continue;

	    	if(s.find("accepted") != std::string::npos)
	    		continue;

	    	if(s.find("rejected") != std::string::npos)
	    		continue;

	    	if(s.find("time") != std::string::npos)
	    		continue;

	    	if(s.find("otim") != std::string::npos)
	    		continue;

	    	if(s.find("force") != std::string::npos) {
	    		engineSide = NONE;
	    		continue;
	    	}

	    	if(s.find("usermove") == 0) {
 
	    		Move* m = new Move();

	    		std::string to = "  ";
	    		std::string from = "  ";

	    		from[0] = s[9];
	    		from[1] = s[10];
	    		to[0] = s[11];
	    		to[1] = s[12];

	    		std::vector<std::string>::iterator it = std::find(b->squares.begin(), b->squares.end(), from);

	    		m->from = std::distance(b->squares.begin(), it);

	    		it = std::find(b->squares.begin(), b->squares.end(), to);

	    		m->to = std::distance(b->squares.begin(), it);

	    		sideToMove = b->setMove(m, !engineSide);
				
	    		if(engineSide == sideToMove) {
	    			makeMove(mover->negamax(b, engineSide, DEPTH, -INT_MAX, INT_MAX).second);
	    		}

	    		continue;
	    	}

	    	if(s.find("level") != std::string::npos)
	    		continue;

	    	if(s.find("post") != std::string::npos)
	    		continue;

	    	if(s.find("hard") != std::string::npos)
	    		continue;

	    	if(s.find("white") != std::string::npos) {
	    		engineSide = WHITE;
	    		continue;
	    	}

	    	if(s.find("black") != std::string::npos) {
	    		engineSide = BLACK;
	    		continue;
	    	}

	    } while(s.find("quit") == std::string::npos);
	}

	~Game() {
		delete b;
		delete mover;
	}
};