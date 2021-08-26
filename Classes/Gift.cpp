//
// Created by zeyuz on 2021/6/16.
//

#include "Gift.h"

USING_NS_CC;

Gift::Gift() {
    setTag(-1);
    removeable = true;
    value = -1;
    weight = 1;
}

Gift *Gift::create(const char *filename) {
    Gift *sprite = new Gift();
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


