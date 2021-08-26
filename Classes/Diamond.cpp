//
// Created by zeyuz on 2021/6/13.
//

#include "Diamond.h"

USING_NS_CC;

Diamond::Diamond() {
    setTag(-1);
    removeable = true;
    value = 2000;
    weight = 1;
}

Diamond *Diamond::create(const char *filename) {
    auto *sprite = new Diamond();
    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        Vec2 points[3] = {
                Vec2(8, 26), Vec2(35, 26), Vec2(22, 5)
        };
        auto physicsBody = PhysicsBody::createPolygon(points, 3, PhysicsMaterial(1.0f, 0.0f, 0.0f), Vec2(-20, -15));
        physicsBody->setDynamic(false);
        physicsBody->setContactTestBitmask(1);
        physicsBody->setGroup(-1);
        sprite->setPhysicsBody(physicsBody);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
