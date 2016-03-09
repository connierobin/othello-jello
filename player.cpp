#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    
    board = new Board();
    s = side;
    if(s == WHITE)
    {
        s2 = BLACK;
    }
    else
    {
        s2 = WHITE;
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
     /*
      * add opponent's move to the board
      * check if there are any legal moves
      *     return NULL
      * go through board locations
      *     if it's a legal move
      *         make the move
      */

    board->doMove(opponentsMove, s2);
    if(board->hasMoves(s))
    {
        Move *m = new Move(0, 0);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                m->setX(i);
                m->setY(j);
                if(board->checkMove(m, s))
                {
                    board->doMove(m, s);
                    return m;
                }
            }
        }
    }

    return NULL;
}
