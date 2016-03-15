#ifndef __INTMOVE_H__
#define __INTMOVE_H__

#include "common.h"

class IntMove {
   
public:
    int x;
    Move *m;
    IntMove(int x, Move *m) {
        this->x = x;
        this->m = m;      
    }
    ~IntMove() {}

    int getX() { return x; }
    Move *getM() { return m; }

    void setX(int x) { this->x = x; }
    void setM(Move *m) { this->m = m; }
};

#endif