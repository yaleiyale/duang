//
// Created by zeyuz on 2021/6/13.
//

#include "Stone.h"

USING_NS_CC;

Stone::Stone() {
    setTag(-1);
    removeable = true;
    value = 50;
    weight = 3;
}

Stone *Stone::create(const char *filename) {
    Stone *sprite = new Stone();
    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(),
                                                  PhysicsMaterial(1.0f, 0.0f, 0.0f));
        physicsBody->setDynamic(false);
        physicsBody->setContactTestBitmask(1);
        physicsBody->setGroup(-1);
        sprite->setPhysicsBody(physicsBody);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
