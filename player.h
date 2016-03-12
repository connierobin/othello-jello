#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include "intmove.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Side s;
    Side s2;
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
private:
	Board *board;
	Move *doBasicMove(Move *opponentsMove, int msLeft);
	Move *doHeuristicMove(Move *opponentsMove, int msLeft);
	Move *doABMove(Move *opponentsMove, int msLeft);
	IntMove *abBoard(Board *tester, int alpha, int beta, Side side, Side otherSide, int depth);
};

#endif
