//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_ROLE_H
#define DUANG_ROLE_H

#include "cocos2d.h"
#include "Ring.h"

USING_NS_CC;

class Role : public cocos2d::Sprite {
    int name = 0;
    int score = 0;
    std::vector<int> magic;
    Ring *ring;
    Sprite *magic_now;
public:
    Role();

    int getScore();

    static Role *create(const char *filename);

    void heave(Vec2 pos);

    void conjure(int choice);

    void earnValue(int value);

    Ring *getRing();

    std::vector<int> getMagic();

    Sprite *getNowMagic();

    void setMagicPos(Vec2 pos);
};


#endif //DUANG_ROLE_H
