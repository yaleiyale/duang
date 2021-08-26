//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_RING_H
#define DUANG_RING_H

#include "cocos2d.h"

class Ring : public cocos2d::Sprite {
    bool flying;
public:
    Ring();

    static Ring *create(const char *filename);

    void move(float weight);

    void transformValue(int value);

    void setState(bool);

    bool getState() const;
};


#endif //DUANG_RING_H
