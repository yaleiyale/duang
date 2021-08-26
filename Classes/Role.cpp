//
// Created by zeyuz on 2021/6/13.
//

#include "Role.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

Role::Role() {
    ring = Ring::create("ring.png");
    ring->setPosition(50, -15);
    ParticleSystemQuad *psd = ParticleSystemQuad::create("ring.plist");
    psd->setScale(0.4f);
    auto pos_psd = Vec2(ring->getContentSize().width / 2, ring->getContentSize().height / 2);
    psd->setPosition(pos_psd);
    ring->addChild(psd);
    this->addChild(ring);
    magic_now = Sprite::create();
    magic_now->setPosition(660, 550);
}

Role *Role::create(const char *filename) {
    Role *sprite = new Role();
    if (sprite->initWithFile(filename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void Role::heave(Vec2 pos_mouse) {
    if (!ring->getState()) {
        float y = pos_mouse.y - this->getPosition().y + 85;
        if (y < 0) {
            float speed = 500;
            float x = pos_mouse.x - this->getPosition().x;
            float vy = sqrt((speed * speed) / (x * x / y / y + 1)) * y / abs(y);
            float vx = abs(x / y * vy) * x / abs(x);
            auto body = ring->getPhysicsBody();
            body->setVelocity(Vec2(vx, vy));
            ring->setState(true);
            if (vx <= 0) {
                Vector<SpriteFrame *> frame_role;
                frame_role.pushBack(Sprite::create("role2.png")->getSpriteFrame());
                frame_role.pushBack(Sprite::create("role2.png")->getSpriteFrame());
                frame_role.pushBack(Sprite::create("role2.png")->getSpriteFrame());
                frame_role.pushBack(Sprite::create("role1.png")->getSpriteFrame());
                Animation *anim_role = Animation::createWithSpriteFrames(frame_role, 0.1);
                auto anim = Animate::create(anim_role);
                this->setScaleX(1);
                this->runAction(anim);
            } else if (vx > 0) {
                Vector<SpriteFrame *> frame_role;
                frame_role.pushBack(Sprite::create("role2.png")->getSpriteFrame());
                frame_role.pushBack(Sprite::create("role2.png")->getSpriteFrame());
                frame_role.pushBack(Sprite::create("role2.png")->getSpriteFrame());
                frame_role.pushBack(Sprite::create("role1.png")->getSpriteFrame());
                Animation *anim_role = Animation::createWithSpriteFrames(frame_role, 0.1);
                auto anim = Animate::create(anim_role);
                this->setScaleX(-1);
                this->runAction(anim);
            }
        }
    }
}

void Role::conjure(int i) {
    if (!magic.empty()) {
        auto myRing = this->getRing();
        if (myRing->getState() && myRing->getTag() == 1) {
            auto last_speed = myRing->getPhysicsBody()->getVelocity();
            auto last_body = myRing->getPhysicsBody();
            last_body->retain();
            auto new_physicsBody = PhysicsBody::createCircle(36.0f,
                                                             PhysicsMaterial(1.0f, 1.0f, 0.0f));
            new_physicsBody->setContactTestBitmask(1);
            switch (magic.back()) {
                case -1:
                    myRing->getPhysicsBody()->setVelocity(Vec2(0, -500));
                    break;
                case -2:
                    this->runAction(Sequence::create(CallFunc::create([=] {
                                                         myRing->setScale(2);
                                                         myRing->setPhysicsBody(new_physicsBody);
                                                         myRing->getPhysicsBody()->setVelocity(last_speed);
                                                     }),
                                                     DelayTime::create(2),
                                                     CallFunc::create([=] {
                                                         auto new_speed = myRing->getPhysicsBody()->getVelocity();
                                                         myRing->setScale(1);
                                                         myRing->setPhysicsBody(last_body);
                                                         if (myRing->getState() && myRing->getTag() == 1) {
                                                             myRing->getPhysicsBody()->setVelocity(new_speed);
                                                         } else {
                                                             myRing->getPhysicsBody()->setCollisionBitmask(0);
                                                             myRing->getPhysicsBody()->setVelocity(Vec2(0, 0));
                                                         }
                                                     }), NULL));
                    break;
                case -3:
                    this->runAction(Sequence::create(
                            CallFunc::create([=] {
                                myRing->getPhysicsBody()->setVelocity(last_speed * 2);
                            }),
                            DelayTime::create(2),
                            CallFunc::create([=] {
                                if (myRing->getTag() == 1) {
                                    auto new_speed = myRing->getPhysicsBody()->getVelocity();
                                    myRing->getPhysicsBody()->setVelocity(new_speed / 2);
                                }

                            }), NULL));
                    break;
                case -4:
                    myRing->transformValue(2500);
                    break;
                default:
                    break;
            }
            magic.erase(magic.end() - 1);
            if (!magic.empty()) {
                switch (magic.back()) {
                    case -1:
                        magic_now->setTexture("rush.png");
                        break;
                    case -2:
                        magic_now->setTexture("scoreup.png");
                        break;
                    case -3:
                        magic_now->setTexture("speedup.png");
                        break;
                    case -4:
                        magic_now->setTexture("stoneup.png");
                        break;
                    default:
                        magic_now->setTexture("null.png");
                        break;
                }
            } else {
                magic_now->setTexture("null.png");
            }

        }
    }
}

void Role::earnValue(int i) {
    if (i > 0) {
        score += i;
        auto last_score = atoi((((Text *) this->getParent()->getChildByName("score"))->getString()).c_str());
        ((Text *) this->getParent()->getChildByName("score"))->setString(std::to_string(last_score + i));
    } else if (i < 0) {
        magic.push_back(i);
        if (!magic.empty()) {
            switch (magic.back()) {
                case -1:
                    magic_now->setTexture("rush.png");
                    break;
                case -2:
                    magic_now->setTexture("scoreup.png");
                    break;
                case -3:
                    magic_now->setTexture("speedup.png");
                    break;
                case -4:
                    magic_now->setTexture("stoneup.png");
                    break;
                default:
                    magic_now->setTexture("null.png");
                    break;
            }
        }
    }
}

Ring *Role::getRing() {
    return ring;
}

std::vector<int> Role::getMagic() {
    return magic;
}

Sprite *Role::getNowMagic() {
    return magic_now;
}

int Role::getScore() {
    return score;
}

void Role::setMagicPos(Vec2 pos) {
    magic_now->setPosition(pos);
}

