#include "board.h"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

/* 
 * Returns an integer score representing how good the given move is for the given side. 
 * Score is based on the number of stones gained and the board location. 
 */
int Board::score(Move *m, Side s)
{
    /*
     * stones = how many stones flip
     * mult = multiplier based on location
     * 
     * Finding stones which flip:
     * Check each direction sequentially
     * counter = 0
     * nextX = x of next place to check
     * nextY = y of next place to check
     * while stone at nextX and nextY is the opposite color
     *      increase the counter
     * if the next stone is of our color (not blank or an edge)
     *      add the counter to the main counter
     */

    int stones = 0;
    int multiplier = 0;
    int counter = 0;
    int x = m->getX();
    int y = m->getY();
    Side s2;
    if(s == BLACK)
        s2 = WHITE;
    else
        s2 = BLACK;

    // first, check the up direction
    // this should count the number of stones in a row of the opposite
    // color, but should stop one short of the edge.
    x -= 1;
    while(x > 0 && get(s2, x, y))
    {
        counter++;
        x--;
    }
    if(counter > 0 && get(s, x, y))
        stones += counter;
    x = m->getX();
    counter = 0;

    // next, check the down direction
    x += 1;
    while(x < 7 && get(s2, x, y))
    {
        counter++;
        x++;
    }
    if(counter > 0 && get(s, x, y))
        stones += counter;
    x = m->getX();
    counter = 0;

    // next, check to the left
    y -= 1;
    while(y > 0 && get(s2, x, y))
    {
        counter++;
        y--;
    }
    if(counter > 0 && get(s, x, y))
        stones += counter;
    y = m->getY();
    counter = 0;

    // next, check to the right
    y += 1;
    while(y < 7 && get(s2, x, y))
    {
        counter++;
        y++;
    }
    if(counter > 0 && get(s, x, y))
        stones += counter;
    y = m->getY();
    counter = 0;

    /* Now we have a stones value. 
     * Next, we will determine a multiplier. 
     * The multiplier will be:
     * 4 for a corner
     * -3 for diagonally adjacent to a corner
     * -1 for adjacent to a corner
     * 2 for an edge
     * 1 otherwise
     */

    if(x == 0 || x == 7)
    {
        if(y == 0 || y == 7)    // corners
        {
            multiplier = 4;
        }
        else if(y == 1 || y == 6)   // adjacent to corner vertically
        {
            multiplier = -1;
        }
        else    // left or right edge
        {
            multiplier = 2;
        }
    }
    else if(x == 1 || x == 6)
    {
        if(y == 0 || y == 7)    // adjacent to corner horizontally
        {
            multiplier = -1;
        }
        if(y == 1 || y == 6)    // adjacent to corner diagonally
        {
            multiplier = -3;
        }
    }
    else if(y == 0 || y == 7)
    {
        multiplier = 2; // top or bottom edge
    }
    else
    {
        multiplier = 1; // everything else
    }
    //cerr << "stones: " << stones << endl;
    //cerr << "multiplier: " << multiplier << endl;
    return stones * multiplier;
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
