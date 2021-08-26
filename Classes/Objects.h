//
// Created by zeyuz on 2021/6/13.
//

#ifndef DUANG_OBJECTS_H
#define DUANG_OBJECTS_H

#include "cocos2d.h"

class Objects : public cocos2d::Sprite {

protected:
    bool removeable;
    int value;
    float weight;
public:
    bool isRemoveable();

    virtual void leave();

    void setValue(int value);

    int getValue();

    void setWeight(float weight);

    float getWeight();


};


#endif //DUANG_OBJECTS_H
