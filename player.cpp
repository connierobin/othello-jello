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
     
     return doABMove(opponentsMove, msLeft);
}

Move *Player::doABMove(Move *opponentsMove, int msLeft)
{
    board->doMove(opponentsMove, s2);
    Board *tester = board->copy();
    IntMove *result = abBoard(tester, -64, 64, s, s2, 0);
    delete tester;
    Move *myMove = result->getM();
    delete result;
    board->doMove(myMove, s);
    return myMove;
}

//IntMove *Player::abMove(Board *tester, Move *m, int alpha, int beta, Side side, Side otherSide, int depth)

IntMove *Player::abBoard(Board *tester, int alpha, int beta, Side side, Side otherSide, int depth)
{
    /*
     * if no moves
     *      return score(this board state) and null move
     *      the score of the current board state is our stones - their stones
     * for each move
     *      board->doMove
     *      score = -ab(board, -beta, -alpha, otherside, side)
     *      board->undoMove
     *      if score > alpha
     *          alpha = score
     *      if score >= beta
     *          break out of loop
     *          
     * return alpha
     */

     /*
      * idea to improve
      * store all possible moves in an array, iterate through that
      * instead of recreating it every time
      */

    Board *temp;

    IntMove *result;

    int score;
    Move *bestMove = new Move(0, 0);

    if(!tester->hasMoves(side) || depth > 8)
    {
        return new IntMove(tester->count(side) - tester->count(otherSide), NULL);
    }
    Move *m = new Move(0, 0);
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            // make the move
            m->setX(i);
            m->setY(j);

            // only work with valid moves
            if(tester->checkMove(m, side))
            {
                temp = tester->copy();
                temp->doMove(m, side);

                // get the result for this move by recursively calling this function
                result = abBoard(temp, -beta, -alpha, otherSide, side, depth + 1);
                score = -result->getX();
                delete result;
                delete temp;

                if(score > alpha)
                {
                    alpha = score;
                    bestMove->setX(m->getX());
                    bestMove->setY(m->getY());
                }
                if(score >= beta)
                {
                    i = j = 8;
                }
            }
        }
    }
    delete m;
    return new IntMove(alpha, bestMove);
}


Move *Player::doHeuristicMove(Move *opponentsMove, int msLeft)
{
    /*
      * add opponent's move to the board
      * go through board locations
      *     if it's a legal move
      *         calculate the score of the move
      *         if this is the best score so far, or the only move so far
      *             store the score and the move
      * if there were no legal moves
      *     return NULL
      * return the move with the best score
      */

    board->doMove(opponentsMove, s2);
    
    Move *m = new Move(0, 0);
    Move *bestm;
    int score, bestscore;
    bool hasmove = false;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            m->setX(i);
            m->setY(j);
            if(board->checkMove(m, s))
            {
                score = board->scoreMove(m, s);
                if(!hasmove)
                {
                    bestm = new Move(i, j);
                    bestscore = score;
                    hasmove = true;
                }
                else if(score > bestscore)
                {
                    bestm->setX(i);
                    bestm->setY(j);
                    bestscore = score;
                }
            }
        }
    }

    if(!hasmove)
    {
        return NULL;
    }
    board->doMove(bestm, s);
    return bestm;    
}

Move *Player::doBasicMove(Move *opponentsMove, int msLeft)
{
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
