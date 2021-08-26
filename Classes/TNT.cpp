//
// Created by zeyuz on 2021/6/13.
//
#include "AudioEngine.h"
#include "TNT.h"

USING_NS_CC;

TNT::TNT() {
    setTag(2);
    removeable = true;
    value = 2;
    weight = 1;
}

void TNT::leave() {
    if (!destoryed) {
        AudioEngine::play2d("music_tnt.mp3");
        destoryed = true;
        ParticleExplosion *pex = ParticleExplosion::create();
        pex->setPosition(this->getPosition());
        pex->setTexture(Director::getInstance()->getTextureCache()->addImage("tta.png"));
        pex->setTotalParticles(100);
        pex->setLife(1);
        pex->setLifeVar(0.1);
        pex->setStartSize(50);
        pex->setStartSizeVar(50);
        pex->setEmissionRate(1000);
        pex->setStartColor(Color4F(1, 0.6, 0.5, 1));
        pex->setStartColorVar(Color4F(0, 0, 0, 0));
        pex->setEndColor(Color4F(0.4, 0.2, 0.2, 1));
        pex->setEndColorVar(Color4F(0, 0, 0, 0.2));
        pex->setEndSize(50);
        pex->setEndSizeVar(50);
        pex->setStartSpin(0);
        pex->setStartSpinVar(10);
        pex->setEndSpin(0);
        pex->setEndSpinVar(10);
        pex->retain();
        this->retain();
        this->runAction(Sequence::create(
                CallFunc::create([=] {
                    this->getParent()->addChild(pex);
                }),
                CallFunc::create([=] {
                    auto physicsBody = PhysicsBody::createBox(this->getContentSize() * 3,
                                                              PhysicsMaterial(1.0f, 0.0f, 0.0f));
                    physicsBody->setDynamic(false);
                    physicsBody->setContactTestBitmask(1);
                    // physicsBody->setGroup(-1);
                    this->setPhysicsBody(physicsBody);
                }), DelayTime::create(1), CallFunc::create([=] {
                    this->removeFromParent();
                }), NULL));
    }


}

TNT *TNT::create(const char *filename) {
    TNT *sprite = new TNT();
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

