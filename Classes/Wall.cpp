//
// Created by zeyuz on 2021/6/13.
//

#include "Wall.h"
#include "GameScene.h"

USING_NS_CC;

Wall::Wall() {
    value = 0;
    removeable = false;
}

Wall *Wall::create(const char *filename) {
    Wall *sprite = new Wall();
    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        auto physicsBody = PhysicsBody::createEdgeBox(sprite->getContentSize(),
                                                      PhysicsMaterial(1.0f, 1.0f, 0.0f));
        sprite->setPhysicsBody(physicsBody);
        physicsBody->setContactTestBitmask(1);
        physicsBody->setGroup(-1);
        physicsBody->setDynamic(false);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

Wall *Wall::create() {
    Wall *sprite = new Wall();
    if (sprite->init()) {
        sprite->autorelease();
        auto physicsBody = PhysicsBody::createEdgeBox(Size(960, 490),
                                                      PhysicsMaterial(1.0f, 1.0f, 0.0f));
        sprite->setPhysicsBody(physicsBody);
        physicsBody->setContactTestBitmask(1);
        physicsBody->setGroup(-1);
        physicsBody->setDynamic(false);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
