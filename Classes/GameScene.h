//
// Created by zeyuz on 2021/6/12.
//

#ifndef DUANG_GAMESCENE_H
#define DUANG_GAMESCENE_H


#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Scene {
    Size visibleSize;
    bool isSingle;
    int music_id;
    bool music_playing;
    int time = 30;
    bool end = false;
    int target;
    bool running;

    void createOfflineScene();

    void createOnlineScene();

    EventDispatcher *ed = Director::getInstance()->getEventDispatcher();
public:
    static cocos2d::Scene *createScene();

    int getTime() const;

    void setTarget(int);

    void setRunning(bool);

    bool getRunning();

    int getTarget() const;

    void setEnd(bool);

    bool getEnd() const;

    void setTime();

    bool init() override;

    void setMode(bool isSingle);

    bool hit(PhysicsContact &contact);

    void setMusic(int id, bool playing);

    CREATE_FUNC(GameScene);
};


#endif //DUANG_GAMESCENE_H
