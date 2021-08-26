//
// Created by zeyuz on 2021/6/13.
//

#include "Animal.h"
#include "string"

USING_NS_CC;
using namespace std;

Animal::Animal() {
    setTag(-1);
    removeable = true;
    value = 2001;
    weight = 1;
}

void Animal::act(const std::string &name) {
    Vector < SpriteFrame * > frame_r;
    Vector < SpriteFrame * > frame_l;
    frame_r.pushBack(Sprite::create(name + "_r1.png")->getSpriteFrame());
    frame_r.pushBack(Sprite::create(name + "_r2.png")->getSpriteFrame());
    frame_r.pushBack(Sprite::create(name + "_r3.png")->getSpriteFrame());
    frame_r.pushBack(Sprite::create(name + "_r4.png")->getSpriteFrame());
    Animation *anim_r = Animation::createWithSpriteFrames(frame_r, 0.1);
    auto move_r = Animate::create(anim_r);

    frame_l.pushBack(Sprite::create(name + "_l1.png")->getSpriteFrame());
    frame_l.pushBack(Sprite::create(name + "_l2.png")->getSpriteFrame());
    frame_l.pushBack(Sprite::create(name + "_l3.png")->getSpriteFrame());
    frame_l.pushBack(Sprite::create(name + "_l4.png")->getSpriteFrame());
    Animation *anim_l = Animation::createWithSpriteFrames(frame_l, 0.1);

    auto move_l = Animate::create(anim_l);
    auto mb = MoveBy::create(1.6, Vec2(400, 0));
    auto move_rl = Sequence::create(mb, mb->reverse(), NULL);
    auto anim_rl = Sequence::create(move_r, move_r->clone(), move_r->clone(), move_r->clone(), move_l, move_l->clone(),
                                    move_l->clone(), move_l->clone(), NULL);
    this->runAction(RepeatForever::create(move_rl));
    this->runAction(RepeatForever::create(anim_rl));
}

Animal *Animal::create(const char *filename) {
    auto *sprite = new Animal();
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
