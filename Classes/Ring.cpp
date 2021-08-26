//
// Created by zeyuz on 2021/6/13.
//

#include "Ring.h"
#include "Role.h"

USING_NS_CC;

void Ring::move(float weight) {
    this->runAction(
            Sequence::create(MoveTo::create(0.5f * weight, Vec2(50, -15)),
                             CallFunc::create([=] {
                                 this->setState(false);
                                 this->setTag(1);
                                 this->getPhysicsBody()->setCollisionBitmask(0xFFFFFFFF);
                                 this->setColor(Color3B::WHITE);
                             }), NULL));
}

void Ring::transformValue(int i) {
    ((Role *) this->getParent())->earnValue(i);
}

Ring::Ring() {
    flying = false;
    this->setTag(1);
}

Ring *Ring::create(const char *filename) {
    Ring *sprite = new Ring();
    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        auto physicsBody = PhysicsBody::createCircle(18.0f,
                                                     PhysicsMaterial(1.0f, 1.0f, 0.0f));
        physicsBody->setContactTestBitmask(1);
        physicsBody->setGroup(-2);
        sprite->setPhysicsBody(physicsBody);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Ring::setState(bool i) {
    flying = i;
}

bool Ring::getState() const {
    return flying;
}
